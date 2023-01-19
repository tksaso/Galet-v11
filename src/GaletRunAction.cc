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
// $Id: GaletRunAction.cc 87359 2014-12-01 16:04:27Z gcosmo $
//
/// \file GaletRunAction.cc
/// \brief Implementation of the GaletRunAction class

#include "GaletRunAction.hh"
#include "GaletRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4TScoreHistFiller.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletRunAction::GaletRunAction()
  : G4UserRunAction()
{ 
  //
  fMessenger = new GaletRunActionMessenger(this);
  //
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(50);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in GaletAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  // Create directories 
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);
  //
  auto histFiller = new G4TScoreHistFiller<G4AnalysisManager>;
  histFiller->SetVerboseLevel(1);

  // Book histograms, ntuple
  //
  
  // Creating histograms
  analysisManager->CreateH1("H1_1","Edep (keV)", 2700, 0., 2700);

  // Creating ntuple
  //
  G4int ntupleId = analysisManager->CreateNtuple("Galet", "Galet");
  analysisManager->CreateNtupleIColumn(ntupleId,"evno");
  analysisManager->CreateNtupleFColumn(ntupleId,"de");
  analysisManager->CreateNtupleFColumn(ntupleId,"x");
  analysisManager->CreateNtupleFColumn(ntupleId,"y");
  analysisManager->CreateNtupleFColumn(ntupleId,"z");
  analysisManager->FinishNtuple();
  //G4cout << " Ntuple Id " << ntupleId<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletRunAction::~GaletRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  analysisManager->OpenFile(fFileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
