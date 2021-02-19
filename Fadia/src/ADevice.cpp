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


using namespace omnetpp;

NetworkOwner ADevice::NO = NetworkOwner();
const int ADevice::baseID = 0x1;
size_t ADevice::countcrash = 0;

void
ADevice::handleMessage(cMessage *msg)
{
    if(crashed)
        return;
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
            case REVOKE:
                logInfo("Revoking a device");
                sendRevReq(0, vector<uid_t>());
                delete msg;
                break;
            case SYNC:
                logInfo("Synchronizating with other collectors");
                sendSyncReq();

            //JOIN
            case JNRQ:
                sendCollector<JoinReq>(msg);
                break;
            case JNRP:
                (void)sendProver<JoinResp>(msg);
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
                (void)sendProver<CommitResp>(msg);
                break;
            case CMAK: 
                (void)sendProver<CommitAck>(msg);
                break;

            case RVKRQ:
                sendProver<RevokeReq>(msg);
                break;
            case CMRPTO: 
                handleCommitRespTimeOut(msg);
                break;
            case CMAKTO: 
                handleCommitAckTimeOut(msg);
                break;    

            // UPDATE
            case UPRQ:
#ifdef WIRELESS
                if(isWithinRange<UpdateReq>(msg))
                    sendProver<UpdateReq>(msg);
                else
                {
                    logInfo("Parent not within range, sending to collector instead!");
                    UpdateReq* smsg = check_and_cast<UpdateReq *> (msg);
                    smsg->setKind(UPRQF);
                    smsg->setDestination(getBaseID<cid_t>());
                    sendCollector<UpdateReq>(smsg);
                }
#else
                sendProver<UpdateReq>(msg);
#endif

                break;
            case UPRQC:
            case UPRQF:
                sendCollector<UpdateReq>(msg);
                break;
            case UPTO:  
                handleUpdateTimeOut(msg);
                break;

            case TXDONE:
                handleTxDoneMsg(msg);
                break;
            case CBUSYMSG:
                handlePostponeDoneMsg(msg);
                break;
#ifdef ENERGY_TEST                
            case ENTXDONE:
            case ENRXDONE:
            case ENMAC:
                handleDoneEnMsg(msg);
                break;
#endif
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
        case UPRQF:
            updateGates<UpdateReq>(msg);
            handleUpMsg(msg);
            break;
        case RVKRQ:
            handleRevMsg(msg);
            break;
        case UNKOWN:
        default:
            logError("Received message of unknown type");
            return;
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
    cPacket* pkt = (cPacket*) msg;
    double bl = pkt->getByteLength();
    // if(checkRecordTime() && device != SERV)
    // {
    //     emit(rxsig, bl);
    // }    
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
ADevice::handleTxDoneMsg(cMessage* msg)
{
    txrxstat = DIDLE;
    if(msgqueue.size() > 0)
    {
        handleMessage(msgqueue.front());
        msgqueue.pop();
    }
}   

#ifdef ENERGY_TEST
void 
ADevice::handleDoneEnMsg(cMessage* msg)
{
    energy->updateResidualCapacity();
    if (status == FINISHED)
        energy->totalPowerConsumption = inet::units::values::mW(IDLE_CONSUMPTION);
    else
        energy->totalPowerConsumption = inet::units::values::mW(LISTENING_CONSUMPTION);

    if(msg->getKind() == ENRXDONE || msg->getKind() == ENMAC)
        delete msg;
    if((energy->getResidualEnergyCapacity()).get() <=0 )
    {
        emit(crashsig, simTime().dbl());
        crashed = true;
        logWarn("Device energy depleted!"); 
        if(++countcrash >= MAX_CRASH)
        {
            endSimulation();
        }
    }
}

#endif

void 
ADevice::handlePostponeDoneMsg(cMessage* msg)
{
    chanstat = CIDLE;
    if(msgqueue.size() > 0)
    {
        handleMessage(msgqueue.front());
        msgqueue.pop();
    }
}   

#ifdef WIRELESS
void
ADevice::sendProverBroadcast(cMessage* msg)
{

    if(txrxstat == TRANSMITING || chanstat == BUSY)
    {
        msgqueue.push(msg);
        return;
    }

    if(isChannelBusy(drange, -1))
    {
        chanstat = BUSY;
        msgqueue.push(msg);
        scheduleAt(simTime() + postponetime, cbusymsg);
        return;
    }

    int size = getParentModule()->getVectorSize();
    assert(size);

    inet::IMobility* mobility = check_and_cast<inet::IMobility *> (getParentModule()->getSubmodule("mobility"));
    inet::Coord spos = mobility->getCurrentPosition();
    bool t = false;
    
    cPacket* pkt = (cPacket*)msg;
    double bl = pkt->getByteLength();

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
                sendDirect(msgd, ndelay + bl / byterate , 0, mod, "radioIn");
                t = true;
            }
        }
    }
    if(t)
    {
        if(checkRecordTime() && device != SERV)
        {
            emit(txsig, bl);
        }
        txrxstat = TRANSMITING;
#ifdef ENERGY_TEST
        energy->updateResidualCapacity();
        energy->totalPowerConsumption = inet::units::values::mW(TRANSMITION_CONSUMPTION);
        scheduleAt(simTime() + bl / byterate, entxmsg);
#endif
        scheduleAt(simTime() + ndelay + bl / byterate, txmsg);
    }
}

#else

void
ADevice::sendProverBroadcast(cMessage* msg)
{
    if(txrxstat == TRANSMITING || chanstat == BUSY)
    {
        msgqueue.push(msg);
        return;
    }

    if(isChannelBusy(drange, -1))
    {
        chanstat = BUSY;
        msgqueue.push(msg);
        scheduleAt(simTime() + postponetime, cbusymsg);
        return;
    }

    size_t gsize = gateSize("appio$o");
    bool t = false;
    while(gsize > 0)
    {
        if(getParentModule()->gate("gate$o", --gsize)->isConnected())
        {

            cMessage* msgd = msg->dup();
            send(msgd, "appio$o", gsize);
            t = true;
        }
    }
    if(t)
    {
        txrxstat = TRANSMITING;
        scheduleAt(simTime() + ndelay, txmsg);
    }

}

#endif

int
ADevice::generateMAC(cMessage* msg, keyid_t kid)
{
    return 0;
}

bool 
ADevice::isChannelBusy()
{
    if(isTransmiting())
        return true;

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
                ADevice* adev = (ADevice*) mod->getSubmodule("proverapp");
                assert(adev);
                if(adev->isTransmiting())
                    return true;
            }
        }
    }
    return false;
}

int
ADevice::connectedGates()
{
    int c = 0;
    size_t gsize = gateSize("appio$o");
    while(gsize > 0)
    {
        cGate* pgate = getParentModule()->gate("gate$o", --gsize);
        if(pgate->isConnected())    
            c++;
    }
    return c;
}

bool 
ADevice::isChannelBusy(int far, int gid)
{
    if(far == -1 && gid == -1)
        return isChannelBusy();

    if(isTransmiting())
        return true;

    if(far == 0)
        return false;


    size_t gsize = gateSize("appio$o");
    while(gsize > 0)
    {
        cGate* pgate = getParentModule()->gate("gate$o", --gsize);
        if(pgate->isConnected() && pgate->getNextGate()->isVector() && pgate->getId() != gid)
        {
            ADevice* mod = dynamic_cast<ADevice *>(pgate->getNextGate()->getNextGate()->getOwnerModule());
            int gidx = pgate->getNextGate()->getIndex();
            int gateid = mod->getParentModule()->gate("gate$o", gidx)->getId();
            if(mod->isChannelBusy(far - 1, gateid))
                return true;
        }
    }

    return false;
}


double
ADevice::getMacDelays()
{
    double sum = 0.0;
    while(!macdelays.empty())
    {
        sum += macdelays.front();
        macdelays.pop();
    }
    return sum;
}


bool
ADevice::checkRecordTime()
{
    if(simTime().dbl() >= 0 && simTime().dbl() <= deltah * 6 )
        return true;
    return false;
}


double
ADevice::checkFirmwareDelay()
{
    switch (device)
    {
        case SKY:
            return (double) FIRMWARE_CHECK_DELAY_S;
        case PI2:
            return (double) FIRMWARE_CHECK_DELAY_P;
        case SERV:
            return 0.0;
        case NA:
        default:
            return (double) FIRMWARE_CHECK_DELAY;
    }
}