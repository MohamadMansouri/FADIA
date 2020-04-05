/*
 * ADevice.cpp
 *
 *  Created on: Mar 19, 2020
 *      Author: mmrota
 */

#include <stdint.h>
#include <string.h>
#include <omnetpp.h>
#include <map>
#include <iterator>

#include "ADevice.h"
#include "ADevice_template.h"

using namespace omnetpp;

NetworkOwner ADevice::NO = NetworkOwner();
const int ADevice::baseID = 0x1;




void
ADevice::sendProver(UID uid, cMessage* msg)
{
    size_t indx = ( uid - getBaseID<UID>()) % MAXUID;
    cModule* mod = getSystemModule()->getSubmodule("prover", indx);
    cMessage* msgd = msg->dup();
    sendDirect(msgd, uniform(0.2,0.2), 0, mod, "radioIn");
}

void
ADevice::sendCollector(CID target, cMessage* msg)
{
    size_t indx = ( target - getBaseID<CID>()) % MAXUID;
    cModule* mod = getSystemModule()->getSubmodule("collector", indx);
    cMessage* msgd = msg->dup();
    sendDirect(msgd, 0.2, 0, mod, "radioIn");
}

void
ADevice::sendProverBroadcast(cMessage* msg)
{
    int size = getParentModule()->getVectorSize();
    assert(size);
    for(int i = 0; i < size; ++i)
    {
        if((int)i != getParentModule()->getIndex())
        {
            cModule* mod = getSystemModule()->getSubmodule("prover", i);
            cMessage* msgd = msg->dup();
            sendDirect(msgd, uniform(0.2,0.2), 0, mod, "radioIn");
        }
    }
}

int
ADevice::generateMAC(cMessage* msg, KEYID kid)
{
    return 0;
}


