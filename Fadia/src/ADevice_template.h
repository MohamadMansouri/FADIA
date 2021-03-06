#ifndef ADEVICE_TMP_H
#define ADEVICE_TMP_H

#include <omnetpp.h>
#include "../inet/src/inet/mobility/contract/IMobility.h"
#include "../inet/src/inet/power/storage/SimpleEpEnergyStorage.h"
#include "../inet/src/inet/common/Units.h"

template <class T>
bool
ADevice::checkMAC(T* msg, keyid_t kid)
{
    macdelays.push(macDelay<T>(msg));
    return msg->getMac() == 0;
}

template <typename T>
const T
ADevice::getBaseID()
{
    return (T)baseID;
}



template <typename T>
bool
ADevice::isWithinRange(cMessage* msg)
{

    T *smsg = check_and_cast<T *>(msg);
    uid_t target = (uid_t) smsg->getDestination();

    inet::IMobility* mobility = check_and_cast<inet::IMobility *> (getParentModule()->getSubmodule("mobility"));
    inet::Coord spos = mobility->getCurrentPosition();
    
    size_t indx = ( target - getBaseID<uid_t>()) % MAXUID;
    
    cModule* mod = getSystemModule()->getSubmodule("prover", indx);

    mobility = check_and_cast<inet::IMobility *> (mod->getSubmodule("mobility"));
    inet::Coord dpos = mobility->getCurrentPosition();
    if(spos.sqrdist(dpos) <= range*range)
        return true;
    return false;

}


template <typename T>
void
ADevice::sendProver(cMessage* msg)
{
    if(txrxstat == TRANSMITING || chanstat == BUSY)
    {
        msgqueue.push(msg);
        return;
    }

	T *smsg = check_and_cast<T *>(msg);
    uid_t target = (uid_t) smsg->getDestination();
    cPacket* pkt = (cPacket*)msg;
    double bl = pkt->getByteLength();

    if(device != SERV)
    {
        if(isChannelBusy(drange, -1))
        {
            chanstat = BUSY;
            msgqueue.push(msg);
            scheduleAt(simTime() + postponetime, cbusymsg);
            return;
        }
    }
    else
    {

        if(isChannelBusyServ(drange, -1, target))
        {
            chanstat = BUSY;
            msgqueue.push(msg);
            scheduleAt(simTime() + postponetime, cbusymsg);
            return;   
        }

    }


#ifdef WIRELESS
    inet::IMobility* mobility;
    inet::Coord spos;
    if(device != SERV)
    {
        mobility = check_and_cast<inet::IMobility *> (getParentModule()->getSubmodule("mobility"));
        spos = mobility->getCurrentPosition();
    }
    
    size_t indx = ( target - getBaseID<uid_t>()) % MAXUID;
    cModule* mod = getSystemModule()->getSubmodule("prover", indx);

    inet::Coord dpos;
    if(device != SERV)
    {
        mobility = check_and_cast<inet::IMobility *> (mod->getSubmodule("mobility"));
        dpos = mobility->getCurrentPosition();
    }
    if(device != SERV && (spos.sqrdist(dpos) <= range*range))
    {
        if(this->checkRecordTime() && device != SERV)
        {
            emit(txsig, bl);
        }
        sendDirect(msg, ndelay + bl / byterate , 0, mod, "radioIn");
    }
    else
    {
        sendDirect(msg, ndelay + bl / byterate , 0, mod, "radioIn");
    }
#else
    send(msg, "appio$o", deviceg[target]);
#endif
    txrxstat = TRANSMITING;
#ifdef ENERGY_TEST
    energy->updateResidualCapacity();
    energy->totalPowerConsumption = inet::units::values::mW(TRANSMITION_CONSUMPTION);
    scheduleAt(simTime() + bl / byterate, entxmsg);
#endif
    scheduleAt(simTime() + ndelay + bl / byterate, txmsg);
}

template <typename T>
void
ADevice::sendCollector(cMessage* msg)
{
    if(txrxstat == TRANSMITING || chanstat == BUSY)
    {
        msgqueue.push(msg);
        return;
    }

    if(isChannelBusy(drange, -1) || isCollectorBusy())
    {
        chanstat = BUSY;
        msgqueue.push(msg);
        scheduleAt(simTime() + postponetime, cbusymsg);
        return;
    }

	T *smsg = check_and_cast<T *>(msg);
    cid_t target = (cid_t) smsg->getDestination();
    cPacket* pkt = (cPacket*)msg;
    double bl = pkt->getByteLength();
#ifdef WIRELESS
    size_t indx = ( target - getBaseID<cid_t>()) % MAXUID;
    cModule* mod = getSystemModule()->getSubmodule("collector", indx);
    if(this->checkRecordTime())
    {
        emit(txsig, bl);
    }
    sendDirect(msg, ndelay + bl / byterate , 0, mod, "radioIn");
#else
    send(msg, "appio$o", deviceg[target]);
#endif
    txrxstat = TRANSMITING;
#ifdef ENERGY_TEST
    energy->updateResidualCapacity();
    energy->totalPowerConsumption = inet::units::values::mW(TRANSMITION_CONSUMPTION);
    scheduleAt(simTime() + bl / byterate, entxmsg);
#endif
    scheduleAt(simTime() + ndelay + bl / byterate, txmsg);
}

template <typename T>
void
ADevice::updateGates(cMessage* msg)
{
#ifndef WIRELESS

    T* rmsg = check_and_cast<T *>(msg);
    uid_t uid = rmsg->getSource();
    if (deviceg.find(uid) == deviceg.end())
    {
        int k = rmsg->getArrivalGate()->getIndex();
        deviceg[uid] = k;
    }

#endif
}


template <typename T>
double
ADevice::macDelay(T* msg)
{
    switch (device)
    {
        case SKY:
            return HMAC_DELAY_S(msg->getByteLength());
        case PI2:
            return HMAC_DELAY_P(msg->getByteLength());
        case SERV:
            return HMAC_DELAY_P(msg->getByteLength());
        case NA:
        default:
            return HMAC_DELAY(msg->getByteLength());
    }
}

#endif



