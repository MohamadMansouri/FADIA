/*
 * NetworkOwner.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: mmrota
 */

#include "NetworkOwner.h"

NetworkOwner::NetworkOwner()
{
    initialized = false;
}

//NetworkOwner::NetworkOwner(cRng* r, size_t ps,  size_t rs)
//{
//    KRsize = rs;
//    KPsize = ps;
//    rng = r;
//    generateKeyPool();
//}

void
NetworkOwner::generateKeyPool()
{

    if (keyPool.size() == KPsize)
        return;

    KEYID keyid;
    do {
        keyid = (KEYID) intrand(rng, MAXUID);
        if (keyid != NOID)
            keyPool[keyid] = (KEY) intrand(rng, MAXUID);
    }
    while (keyPool.size() != KPsize);
}


KEYRNG
NetworkOwner::getKeyRing()
{
    KEYRNG KeyRing;
    do
    {
        auto it = keyPool.begin();
        std::advance(it, intrand(rng, keyPool.size()));
        KeyRing.insert(*it);
    }
    while (KeyRing.size() != KRsize);

    return KeyRing;
}
