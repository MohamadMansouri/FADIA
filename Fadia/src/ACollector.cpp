/*
 * Collector.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: mmrota
 */




#include <string.h>
#include <omnetpp.h>
#include <algorithm>

#include "NetworkOwner.h"
#include "ACollector.h"
#include "Join_m.h"
#include "Attest_m.h"
#include "Update_m.h"

using namespace omnetpp;



Define_Module(ACollector);


void ACollector::initialize()
{
    initUID();
}

void ACollector::initUID()
{
    do
    {
        CId = (getBaseID<CID>() + getParentModule()->getIndex()) % MAXCID;
    } while(!CId);
}

void
ACollector::handleJoinReq(cMessage* msg)
{
    JoinReq* reqmsg = check_and_cast<JoinReq *>(msg);
    if((CID)reqmsg->getDestination() != CId)
    {
        logError("handleJoinReq: received message with wrong dest");
        return;
    }

    UID senderid = (UID) reqmsg->getSource();

    if(!checkMAC<JoinReq>(reqmsg))
    {
        logError("handleJoinReq: Invalid MAC");
    }
    jsessions.push_back(senderid);
    sendJoinResp(senderid, reqmsg->getBattery());
}

void
ACollector::sendJoinResp(UID target, double battery)
{
    JoinResp* msg = new JoinResp();
    msg->setKind(JNRP);
    msg->setSource(CId);
    msg->setDestination(target);
    msg->setVerify(chooseVerify(battery));
    msg->setProve(chooseProve(battery));
    msg->setMac(generateMAC(msg, channelKeys[target]));
    sendProver(target, msg);
}

void
ACollector::handleJoinAck(cMessage* msg)
{
    JoinAck* ackmsg = check_and_cast<JoinAck *>(msg);
    if((CID)ackmsg->getDestination() != CId)
    {
        logError("handleJoinReq: received message with wrong dest");
        return;
    }
    if(find(jsessions.begin(),jsessions.end(),(UID)ackmsg->getSource()) == jsessions.end())
    {
        logError("handleJoinReq: session is not open");
        return;
    }

    UID senderid = (UID) ackmsg->getSource();

    if(!checkMAC<JoinAck>(ackmsg))
    {
        logError("handleJoinReq: Invalid MAC");
    }
    statusTable[senderid] = make_pair(true, SimTime());
}

void
ACollector::handleUpReq(cMessage* msg)
{

}

void
ACollector::sendRevReq(UID comdev)
{

}

void
ACollector::sendSyncReq()
{

}

void
ACollector::handleSyncReq(cMessage* msg)
{

}



void
ACollector::logDebug(string m)
{
    EV_DEBUG << "Collector [Index=" << getParentModule()->getIndex()
             << ", CID=" << hex <<  CId << "]: " << dec<< m << endl;
}

void
ACollector::logInfo(string m)
{
    EV << "Collector [Index=" << getParentModule()->getIndex()
             << ", CID=" << hex <<  CId << "]: " << dec<< m << endl;
}

void
ACollector::logError(string m)
{
    ostringstream oss;
    oss << "Collector [Index=" << getParentModule()->getIndex()
        << ", CID=" << hex <<  CId << "]: " << dec<< m << endl;
    EV_ERROR << oss.str();
    getSimulation()->getActiveEnvir()->alert(oss.str().c_str());

}

int
ACollector::chooseProve(double p)
{
    return 1;
}

int
ACollector::chooseVerify(double p)
{
    return 2;
}
