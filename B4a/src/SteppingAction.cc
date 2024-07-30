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
//
/// \file B4/B4a/src/SteppingAction.cc
/// \brief Implementation of the B4a::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4AnalysisManager.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"

using namespace B4;

namespace B4a
{

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    SteppingAction::SteppingAction(const DetectorConstruction* detConstruction,
        EventAction* eventAction)
        : fDetConstruction(detConstruction),
        fEventAction(eventAction)
    {}

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void SteppingAction::UserSteppingAction(const G4Step* step)
    {
        // Collect energy and track length step by step

          // get volume of the current step
        auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

        // energy deposit
        auto edep = step->GetTotalEnergyDeposit();

        auto particle = step->GetTrack()->GetDefinition();

        G4int trackID = step->GetTrack()->GetTrackID();

        G4ThreeVector position = step->GetPostStepPoint()->GetPosition();

        // step length
        G4double stepLength = 0.;
        if (step->GetTrack()->GetDefinition()->GetPDGCharge() == 0. && particle->GetPDGEncoding() == 2112) {
            stepLength = step->GetStepLength();

            // Only consider neutrons

            if (volume == fDetConstruction->GetDetectorPhys()) {
                if (fInitialPositionMap.find(trackID) == fInitialPositionMap.end()) {
                    // If this track ID is not in the map, store the initial position
                    fInitialPositionMap[trackID] = step->GetPreStepPoint()->GetPosition();
                }

                // Track position to fill H2 histogram
                G4double x = step->GetPreStepPoint()->GetPosition().x();
                G4double y = step->GetPreStepPoint()->GetPosition().y();
                auto analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH2(0, x, y);
            }
            else if (fInitialPositionMap.find(trackID) != fInitialPositionMap.end()) {
                // If the particle exits the detector, calculate the effective range
                G4ThreeVector initialPosition = fInitialPositionMap[trackID];
                G4ThreeVector finalPosition = step->GetPreStepPoint()->GetPosition();
                G4double range = (finalPosition - initialPosition).mag();

                // Add the effective range to the event action
                fEventAction->AddEffectiveRange(range);

                // Remove the track ID from the map
                fInitialPositionMap.erase(trackID);
            }
        }

    }



    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

};
