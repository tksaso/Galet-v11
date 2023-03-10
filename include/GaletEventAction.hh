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
// $Id: GaletEventAction.hh 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file GaletEventAction.hh
/// \brief Definition of the GaletEventAction class

#ifndef GaletEventAction_h
#define GaletEventAction_h 1

#include "G4UserEventAction.hh"
#include "GaletHit.hh"
#include "globals.hh"
#include "GaletEventActionMessenger.hh"

/// Event action class
///

class GaletEventAction : public G4UserEventAction
{
public:
  GaletEventAction();
  ~GaletEventAction() override;

  void  BeginOfEventAction(const G4Event* event) override;
  void    EndOfEventAction(const G4Event* event) override;

  void SetEneResoParam(G4double par0, G4double par1, G4double par2)
  { fEneResoPar[0] = par0; fEneResoPar[1] = par1; fEneResoPar[2] = par2; };
 
    
private:
  // methods
  GaletHitsCollection* GetHitsCollection(G4int hcID,
					 const G4Event* event) const;

  // data members                   
  G4int  fHCID = -1;
  G4double fEneResoPar[3]={0.907260935,0.054974692, -0.000336824}; // in keV

  GaletEventActionMessenger* fMessenger = nullptr;
  
};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
