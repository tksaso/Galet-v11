//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: GaletEventAction.cc 89037 2015-03-18 09:25:01Z gcosmo $
// 
/// \file GaletEventAction.cc
/// \brief Implementation of the GaletEventAction class

#include "GaletEventAction.hh"
#include "GaletSD.hh"
#include "GaletHit.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletEventAction::GaletEventAction()
 : G4UserEventAction()
{
  fMessenger = new GaletEventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletEventAction::~GaletEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletHitsCollection* 
GaletEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<GaletHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletEventAction::BeginOfEventAction(const G4Event* /*event*/)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletEventAction::EndOfEventAction(const G4Event* event)
{  
  // Print per event (modulo n)
  //
  //  UI:  /run/printProgress <n>
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     
  }  

  // Get hits collections IDs (only once)
  if ( fHCID == -1 ) {
    fHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("galetSD/GaletHitsCollection");
    G4cout << " fHCID = " << fHCID << G4endl;
  }

  // Skip following code by return if no hits collection
  //
  if ( fHCID < 0 ) return;

  // Get hits collections
  GaletHitsCollection* HC = GetHitsCollection(fHCID, event);

  if ( !HC ) return;

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // Loop over the hits
  //
  G4double totalE = 0.0;
  for ( G4int i = 0; i < (G4int)(HC->entries()) ; i++){
    GaletHit* Hit = (*HC)[i];
    G4double  de  = Hit->GetEnergyDeposit()/keV;
    //G4int     trackID  = Hit->GetTrackID();
    totalE += de;
    /*
    G4cout << "Event: "<<eventID
           <<" trackID: "<< trackID
           <<" EnergyDep(keV): " << de 
           <<" totalE(keV): " << totalE 
           <<G4endl;
    */
  }
  //
  // fill histograms
  if ( totalE > 0 ) {
    G4double SigmaE = (fEneResoPar[0]
                       + fEneResoPar[1]*std::sqrt(totalE)
                       + fEneResoPar[2]*totalE)/2.355;
    G4double SmearE = CLHEP::RandGauss::shoot(totalE, SigmaE );
    analysisManager->FillH1(1, SmearE);
  }
  //
  // Fill ntuple
  /*
  for ( G4int i = 0; i < HC->entries() ; i++){
    GaletHit* Hit = (*HC)[i];
    G4double  de  = Hit->GetEnergyDeposit()/keV;
    const G4ThreeVector& pos = Hit->GetHitPoint()/mm;
    G4double  x = pos.x();
    G4double  y = pos.y();
    G4double  z = pos.z();
    const G4String& process = Hit->GetCreatorProcessType();
    // fill ntuple
    analysisManager->FillNtupleIColumn(0, eventID);
    analysisManager->FillNtupleFColumn(1, de);
    analysisManager->FillNtupleFColumn(2, x);
    analysisManager->FillNtupleFColumn(3, y);
    analysisManager->FillNtupleFColumn(4, z);
    analysisManager->FillNtupleSColumn(5, process);
    analysisManager->AddNtupleRow();  
  } 
  */ 
  //
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
