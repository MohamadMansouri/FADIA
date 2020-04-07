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

#include "../inet/src/inet/mobility/contract/IMobility.h"



using namespace omnetpp;

NetworkOwner ADevice::NO = NetworkOwner();
const int ADevice::baseID = 0x1;

void
ADevice::handleMessage(cMessage *msg)
{
    switch(msg->getKind())
    {
    case JOIN:
        logInfo("Joining the network");
        sendJoinReq();
        break;
    case MKTREE:
        logInfo("Creating a spanning tree");
        // sendCommitReq();
        break;
    case UP:
        logDebug("Sending Updates to the collector");
        sendUpReq();
        break;
    case REVOKE:
        logDebug("Received a revocation message");
        handleRevMsg(msg);
        break;
    case JNRQ:
    case JNRP:
    case JNAK: 
        logDebug("Received a join message");
        handleJoinMsg(msg);
        break;
    case CMRQ: 
    case CMRP: 
    case CMAK: 
        logDebug("Received a commitment message");
        handleCommitMsg(msg);
        break;
    case UNKOWN:
    default:
        logError("Received message of unknown type");
    }


}

void
ADevice::handleJoinMsg(cMessage* msg)
{
    switch(msg->getKind())
    {
    case JNRQ: 
        handleJoinReq(msg);
        break;
    case JNRP:
        handleJoinResp(msg);
        break;
    case JNAK:
        handleJoinAck(msg);
        break;
    }
}


void
ADevice::handleCommitMsg(cMessage* msg)
{
    switch(msg->getKind())
    {
    case CMRQ: 
        handleCommitReq(msg);
        break;
    case CMRP:
        handleCommitResp(msg);
        break;
    case CMAK:
        handleCommitAck(msg);
        break;
    }
}

void
ADevice::handleUpMsg(cMessage* msg)
{
    handleUpReq(msg);
}

void
ADevice::handleRevMsg(cMessage* msg)
{
    handleRevReq(msg);
}

void
ADevice::handleSyncMsg(cMessage* msg)
{
    handleSyncReq(msg);
}


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

    inet::IMobility* mobility = check_and_cast<inet::IMobility *> (getParentModule()->getSubmodule("mobility"));
    inet::Coord spos = mobility->getCurrentPosition();

    for(int i = 0; i < size; ++i)
    {

        if((int)i != getParentModule()->getIndex())
        {
            cModule* mod = getSystemModule()->getSubmodule("prover", i);

            inet::IMobility* mobility = check_and_cast<inet::IMobility *> (mod->getSubmodule("mobility"));
            inet::Coord dpos = mobility->getCurrentPosition();
            if(spos.sqrdist(dpos) <= range*range)
            {
                cMessage* msgd = msg->dup();
                sendDirect(msgd, uniform(0.2,0.2), 0, mod, "radioIn");
            }
        }
    }
}

int
ADevice::generateMAC(cMessage* msg, KEYID kid)
{
    return 0;
}


