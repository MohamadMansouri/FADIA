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

#include "common.h"

using namespace omnetpp;

static int seed = 0;

static KEYRNG keyPool;


const KEYRNG getKeyPool(cRNG* kidrng, cRNG* krng, size_t KPsize){

    if (keyPool.size() == KPsize)
        return keyPool;

    cUniform* poolIdGen = new cUniform(kidrng, 0, 1);
    cUniform* poolKeyGen = new cUniform(krng, 0, 1);
    KEYID keyid;
    do {
        keyid = (KEYID) (poolIdGen->draw() * MAXUID);
        if (keyid != NOID)
            keyPool[keyid] = (KEY) (poolKeyGen->draw() * MAXUID);
    }
    while (keyPool.size() != KPsize);

    return keyPool;
}


