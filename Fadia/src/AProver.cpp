/*
 * Collector.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: mmrota
 */


#include <string.h>
#include <omnetpp.h>
#include <map>
#include <cmath>

#include "AProver.h"








// The module class needs to be registered with OMNeT++
Define_Module(AProver);


void AProver::initialize()
{
    cRNG* kidrng = getRing(kidseed);
    cRNG* krng = getRing(keyseed);
    getKeyPool(kidrng, krng, KPsize);



}

void AProver::handleMessage(cMessage *msg)
{
}

