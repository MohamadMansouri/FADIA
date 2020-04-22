#ifndef ADEVICE_TMP_H
#define ADEVICE_TMP_H

template <class T>
bool
ADevice::checkMAC(T* msg, keyid_t kid)
{
    return msg->getMac() == 0;
}

template <typename T>
const T
ADevice::getBaseID()
{
    return (T)baseID;
}

template <typename T>
void
ADevice::sendProver(cMessage* msg)
{
	T *smsg = check_and_cast<T *>(msg);
    uid_t target = (uid_t) smsg->getDestination();
    size_t indx = ( target - getBaseID<uid_t>()) % MAXUID;
    cModule* mod = getSystemModule()->getSubmodule("prover", indx);
    // cMessage* msgd = smsg->dup();
    sendDirect(msg, ndelay, 0, mod, "radioIn");
}

template <typename T>
void
ADevice::sendCollector(cMessage* msg)
{
	T *smsg = check_and_cast<T *>(msg);
    cid_t target = (cid_t) smsg->getDestination();
    size_t indx = ( target - getBaseID<cid_t>()) % MAXUID;
    cModule* mod = getSystemModule()->getSubmodule("collector", indx);
    // cMessage* msgd = smsg->dup();
    sendDirect(msg, ndelay, 0, mod, "radioIn");
}

#endif
