//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//---------------------------------------------------------------------
//
// GaletEventActionMessenger.hh
//
// [Description]
//   UIcommand for Material construction from File
//
// [Histoy]
//   30 Aug, 2006, T.Aso and T.Kawaguchi
//   2013-07-03 T.Aso Add property command.
//   2014-01-09 T.Aso SystemOfUnits.
//
//---------------------------------------------------------------------
//
#include "GaletEventActionMessenger.hh"
#include "GaletEventAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tokenizer.hh"

GaletEventActionMessenger::
GaletEventActionMessenger(GaletEventAction* eventAction)
  :fEventAction(eventAction) {

  fDir = new G4UIdirectory("/My/eventaction/");
  fDir->SetGuidance("UI commands for eventAction");

  feneResoCmd = new G4UIcommand("/My/eventaction/eneResoParam",this);
  feneResoCmd->SetGuidance("Usage: /My/runaction/eneResoPram {par0} {par1} {par2}");
  feneResoCmd->SetGuidance("Define parameters for energy resolution.");
  feneResoCmd->SetGuidance(" {par0} + {par1}*sqrt{E} + {par2}*E  (keV)");
  G4UIparameter* param=0;
  param = new G4UIparameter("par0",'d',true);
  feneResoCmd->SetParameter(param);
  param = new G4UIparameter("par1",'d',true);
  feneResoCmd->SetParameter(param);
  param = new G4UIparameter("type",'d',true);
  feneResoCmd->SetParameter(param);
}

GaletEventActionMessenger::~GaletEventActionMessenger() {
  delete feneResoCmd;
  delete fDir;
}

void GaletEventActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if(command == feneResoCmd){
    G4Tokenizer next(newValue);
    G4double par0 = StoD(next());
    G4double par1 = StoD(next());
    G4double par2 = StoD(next());
    fEventAction->SetEneResoParam(par0,par1,par2);
  }
}

G4String GaletEventActionMessenger::GetCurrentValue(G4UIcommand*) {
  return G4String("I do not know,");
}

