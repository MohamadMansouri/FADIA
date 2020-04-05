#ifndef ADEVICE_TMP_H
#define ADEVICE_TMP_H

template <class T>
bool
ADevice::checkMAC(T* msg)
{
    return msg->getMac() == 0;
}

template <typename T>
const T
ADevice::getBaseID()
{
    return (T)baseID;
}

#endif
