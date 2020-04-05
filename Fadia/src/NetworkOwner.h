/*
 * NetworkOwner.h
 *
 *  Created on: Mar 27, 2020
 *      Author: mmrota
 */

#ifndef SRC_NETWORKOWNER_H_
#define SRC_NETWORKOWNER_H_

#include <string.h>
#include <omnetpp.h>
#include <stdint.h>
#include <stddef.h>
#include <map>
#include <utility>
#include <vector>

#define NOID 0
#define MAXUID INT32_MAX
#define MAXCID INT32_MAX

typedef uint32_t CID;
typedef uint32_t UID;                                       // Unique ID of a device
typedef uint32_t KEYID;                                     // ID of a key
typedef uint32_t KEY;                                       // A key (We suppose it is SHA256 for now... can be changed later)
typedef std::map<KEYID, KEY> KEYRNG;                        // A KeyRing, pair of Key ID and Key.
typedef std::map<KEYID, KEY>::iterator KEYRNG_IT;           // Iterator for KEYRNG.

using namespace omnetpp;
using namespace std;

//fix public private members
class NetworkOwner
{

private:
    cRNG* rng;
    size_t KPsize;
    size_t KRsize;
    KEYRNG keyPool;

public:

    void generateKeyPool();

    bool initialized;
    NetworkOwner();
    NetworkOwner(cRNG* r, size_t ps, size_t rs)

    {
        KRsize = rs;
        KPsize = ps;
        rng = r;
        generateKeyPool();
    }

    KEYRNG getKeyRing();
};

#endif /* SRC_NETWORKOWNER_H_ */
