
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#include <stdlib.h>
#include <omnetpp.h>
#include <veins-vlc/application/simpleVlcApp/SimpleVlcApp.h>
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include <veins/modules/mobility/traci/TraCICommandInterface.h>

#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "veins/modules/mobility/traci/TraCIBuffer.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/mobility/traci/TraCIConnection.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "veins/modules/mobility/traci/ParBuffer.h"








using namespace veins;

Define_Module(veins::SimpleVlcApp);

SimpleVlcApp::SimpleVlcApp()
    : traciManager(NULL)
    , mobility(NULL)
    , annotations(NULL)
    ,traciVehicle(NULL)
{
}
 SimpleVlcApp::~SimpleVlcApp()
{
if (getParentModule()->getIndex()==0){
double speed =  traciVehicle->getSpeed();
if (getParentModule()->getIndex() == 0 && speed == 0) {
    auto dsrc = [this]() {
   getParentModule()->getDisplayString().setTagArg("i", 1, "red");

VlcMessage* vlcMsg = new VlcMessage();
vlcMsg->setAccessTechnology(DSRC);
send(vlcMsg, toLower);
   };
int vlcModule = BOTH_LIGHTS;
auto vlc = [this, vlcModule]() {
EV_INFO << "Sending VLC message of type: " << vlcModule << std::endl;
VlcMessage* vlcMsg = generateVlcMessage(VLC, vlcModule);
send(vlcMsg, toLower);
};
timerManager.create(veins::TimerSpecification(vlc).oneshotAt(SimTime(40, SIMTIME_S)));


}

std::string AltRoute =("53848937,54031153#1,54031153#2,16117212#2,16117212#3,16117212#4,16117212#5,16117212#6,16117212#7,567799791#0,567799791#1,10211064#0,10211064#1,10211064#2,-689762019");
if (getParentModule()->getIndex() == 1) {
    traciVehicle->changeRoute(AltRoute, 0);
}
if (getParentModule()->getIndex() == 2) {
    traciVehicle->changeRoute(AltRoute, 0);
}
if (getParentModule()->getIndex() == 3) {
    traciVehicle->changeRoute(AltRoute, 0);
}
}
}


void SimpleVlcApp::initialize(int stage)
{
    if (stage == 0) {
        toLower = findGate("lowerLayerOut");
        fromLower = findGate("lowerLayerIn");

        // Pointers to simulation modules
        traciManager = TraCIScenarioManagerAccess().get();
        ASSERT(traciManager);

        cModule* tmpMobility = getParentModule()->getSubmodule("veinsmobility");
        mobility = dynamic_cast<TraCIMobility*>(tmpMobility);
        ASSERT(mobility);

        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        sumoId = mobility->getExternalId();
        debug = par("debug").boolValue();
        byteLength = par("packetByteLength");
        transmissionPeriod = 1 / par("beaconingFrequency").doubleValue();
    }




    if (getParentModule()->getIndex()==0){
        double speed = mobility->TraCIMobility::getSpeed();
        EV_INFO <<"Car ID 0 is traveling at speed of :" << speed <<"m/s"<< std::endl;
        if (getParentModule()->getIndex() == 0 && mobility->TraCIMobility::accidentCount > 0) {
        auto dsrc = [this]() {
       getParentModule()->getDisplayString().setTagArg("i", 1, "red");

    VlcMessage* vlcMsg = new VlcMessage();
    vlcMsg->setAccessTechnology(VLC);
    send(vlcMsg, toLower);
       };
    int vlcModule = BOTH_LIGHTS;
    auto vlc = [this, vlcModule]() {
    EV_INFO << "Sending VLC message of type: " << vlcModule << std::endl;
    VlcMessage* vlcMsg = generateVlcMessage(VLC, vlcModule);
    send(vlcMsg, toLower);
    };
    timerManager.create(veins::TimerSpecification(vlc).oneshotAt(SimTime(40, SIMTIME_S)));


    }
}

}


void SimpleVlcApp::handleMessage(cMessage* msg)
{
    // To handle the timer
    if (timerManager.handleMessage(msg)) return;
    if (msg->isSelfMessage()) {
    }
    else {
        VlcMessage* vlcMsg = check_and_cast<VlcMessage*>(msg);
        int accessTech = vlcMsg->getAccessTechnology();
        switch (accessTech) {
        case DSRC: {
            EV_INFO << "DSRC message received!" << std::endl;
            delete vlcMsg;
            break;
        }
        case VLC: {
            EV_INFO << "VLC message received from: " << vlcMsg->getSourceNode() << std::endl;
            delete vlcMsg;

            break;
        }
        default:
            error("message neither from DSRC nor VLC");
            break;
        }
    }

}

VlcMessage* SimpleVlcApp::generateVlcMessage(int accessTechnology, int sendingModule)
{
    VlcMessage* vlcMsg = new VlcMessage();
    // OMNeT-specific
    vlcMsg->setName("NEHODA");
    // VlcMesage fields
    vlcMsg->setSourceNode(this->sumoId.c_str());
    vlcMsg->setDestinationNode("BROADCAST");
    vlcMsg->setAccessTechnology(accessTechnology);
    vlcMsg->setTransmissionModule(sendingModule);
    vlcMsg->setTimestamp();
    // Set application layer packet length
    vlcMsg->setByteLength(byteLength);

    return vlcMsg;

}
