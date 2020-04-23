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

#include "Join_m.h"
#include "Update_m.h"
#include "Commit_m.h"

#include "../inet/src/inet/mobility/contract/IMobility.h"



using namespace omnetpp;

NetworkOwner ADevice::NO = NetworkOwner();
const int ADevice::baseID = 0x1;

void
ADevice::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        switch(msg->getKind())
        {
            case JOIN:
                logInfo("Joining the network");
                sendJoinReq();
                break;
            case MKTREE:
                logInfo("Creating a spanning tree");
                sendCommitReq(NOID);
                break;
            case CHECK: 
                logInfo("Checking the current software configuration");
                checkSoftConfig();
                break;
            case ATTEST: 
                logInfo("Entering a new attestation period");
                startAttestation();
                break;
            // case UPDATE:
            //     logInfo("Sending Updates to my parent");
            //     sendUpReq();
            //     break;
            case SYNC:
                logInfo("Synchronizating with other collectors");
                sendSyncReq();

            //JOIN
            case JNRQ:
                sendCollector<JoinReq>(msg);
                break;
            case JNRP:
                sendProver<JoinResp>(msg);
                break;
            case JNAK: 
                sendCollector<JoinAck>(msg);
                break;
            case JNRPTO: 
                handleJoinRespTimeOut(msg);
                break;
            case JNAKTO: 
                handleJoinAckTimeOut(msg);
                break;                
            
            //COMMIT
            case CMRQ: 
                sendProverBroadcast(msg);
                break;
            case CMRP: 
                sendProver<CommitResp>(msg);
                break;
            case CMAK: 
                sendProver<CommitAck>(msg);
                break;
            case CMRPTO: 
                handleCommitRespTimeOut(msg);
                break;
            case CMAKTO: 
                handleCommitAckTimeOut(msg);
                break;    

            // UPDATE
            case UPRQ:
                sendProver<UpdateReq>(msg);
                break;
            case UPRQC:
                sendCollector<UpdateReq>(msg);
                break;
            case UPTO:  
                handleUpdateTimeOut(msg);
                break;


            case UNKOWN:
            default:
                logError("Received message of unknown type");
                break;
        }
    }
    else
    {
        switch(msg->getKind())
        {
        case JNRQ:
            updateGates<JoinReq>(msg);
            handleJoinMsg(msg);
            break;
        case JNRP:
            updateGates<JoinResp>(msg);
            handleJoinMsg(msg);
            break;
        case JNAK:
            updateGates<JoinAck>(msg);
            handleJoinMsg(msg);
            break; 
        case CMRQ: 
            updateGates<CommitReq>(msg);
            handleCommitMsg(msg);
            break; 
        case CMRP: 
            updateGates<CommitResp>(msg);
            handleCommitMsg(msg);
            break; 
        case CMAK: 
            updateGates<CommitAck>(msg);
            handleCommitMsg(msg);
            break;
        case UPRQ:
        case UPRQC:
            updateGates<UpdateReq>(msg);
            handleUpMsg(msg);
            break;
        case UNKOWN:
        default:
            logError("Received message of unknown type");
            break;
        }
    }
}

void
ADevice::handleJoinMsg(cMessage* msg)
{
    switch(msg->getKind())
    {
    case JNRQ: 
        logDebug("Received a join Request");
        handleJoinReq(msg);
        break;
    case JNRP:
        logDebug("Received a join Response");
        handleJoinResp(msg);
        break;
    case JNAK:
        logDebug("Received a join Acknowledgment");
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
        logDebug("Received a commitment Request");
        handleCommitReq(msg);
        break;
    case CMRP:
        logDebug("Received a commitment Response");
        handleCommitResp(msg);
        break;
    case CMAK:
        logDebug("Received a commitment Acknowledgment");
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
ADevice::sendProverBroadcast(cMessage* msg)
{
    size_t gsize = gateSize("appio$o");
    while(gsize > 1)
    {
        if(getParentModule()->gate("gate$o", --gsize)->isConnected())
        {
            cMessage* msgd = msg->dup();
            send(msgd, "appio$o", gsize);
        }
    }
}

#if 0
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
                sendDirect(msgd, ndelay, 0, mod, "radioIn");
            }
        }
    }
}
#endif

int
ADevice::generateMAC(cMessage* msg, keyid_t kid)
{
    return 0;
}

