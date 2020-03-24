/*
 * common.cpp
 *
 *  Created on: Mar 19, 2020
 *      Author: mmrota
 */

#include <stdint.h>
#include <string.h>
#include <omnetpp.h>
#include <map>
#include <iterator>

#include "common.h"

using namespace omnetpp;

static KEYRNG keyPool;
static const UID baseID = 0xFFFF;

static const KEYRNG
getKeyPool(cRNG* rng, size_t KPsize)
{

    if (keyPool.size() == KPsize)
        return keyPool;

    KEYID keyid;
    do {
        keyid = (KEYID) intrand(rng, MAXUID);
        if (keyid != NOID)
            keyPool[keyid] = (KEY) intrand(rng, MAXUID);
    }
    while (keyPool.size() != KPsize);

    return keyPool;
}


KEYRNG
getKeyRing(cRNG* rng, size_t KPsize, size_t KRsize)
{
    const KEYRNG KeyPool = getKeyPool(rng, KPsize);
    KEYRNG KeyRing;
    do
    {
        auto it = KeyPool.begin();
        std::advance(it, intrand(rng, KeyPool.size()));
        KeyRing.insert(*it);
    }
    while (KeyRing.size() != KRsize);

    return KeyRing;

}


const UID
getBaseID()
{
    return baseID;
}
