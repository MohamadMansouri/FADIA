/*
 * NetworkOwner.cpp
 *
 *  Created on: Mar 27, 2020
 *      Author: mmrota
 */

#include "../devices/NetworkOwner.h"

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

    keyid_t keyid;
    do {
        keyid = (keyv_t) intrand(rng, MAXUID);
        if (keyid != NOID)
            keyPool[keyid] = (keyv_t) intrand(rng, MAXUID);
    }
    while (keyPool.size() != KPsize);
}


keyrng_t
NetworkOwner::getKeyRing()
{
    keyrng_t KeyRing;
    do
    {
        auto it = keyPool.begin();
        std::advance(it, intrand(rng, keyPool.size()));
        KeyRing.insert(*it);
    }
    while (KeyRing.size() != KRsize);

    return KeyRing;
}
