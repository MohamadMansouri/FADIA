/*
 * common.h
 *
 *  Created on: Mar 19, 2020
 *      Author: mmrota
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <omnetpp.h>
#include <map>

using namespace omnetpp;

#define NOID 0
#define MAXUID INT32_MAX

typedef uint32_t UID;                                       // Unique ID of a device
typedef uint32_t KEYID;                                     // ID of a key
typedef uint32_t KEY;                                       // A key (We suppose it is SHA256 for now... can be changed later)
typedef std::map<KEYID, KEY> KEYRNG;                        // A KeyRing, pair of Key ID and Key.
typedef std::map<KEYID, KEY>::iterator KEYRNG_IT;           // Iterator for KEYRNG.



KEYRNG getKeyRing(cRNG* rng, size_t KPsize, size_t KRsize);
const UID getBaseID();
#endif /* COMMON_H_ */
