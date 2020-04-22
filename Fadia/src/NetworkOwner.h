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
#define MAXTREEID INT32_MAX

typedef struct sha256_t
{
    unsigned char byte[32];
    // TODO: overload parameters and create constructors
} sha256_t;

typedef uint32_t mac_t;
typedef uint32_t cid_t;
typedef uint32_t uid_t;                                       // Unique ID of a device
typedef uint32_t keyid_t;                                     // ID of a key
typedef uint32_t keyv_t;                                       // A key (We suppose it is SHA256 for now... can be changed later)
typedef uint32_t treeid_t;                                    // Tree ID type
typedef std::map<keyid_t, keyv_t> keyrng_t;                        // A KeyRing, pair of Key ID and Key.

using namespace omnetpp;
using namespace std;

//fix public private members
class NetworkOwner
{

private:
    cRNG* rng;
    size_t KPsize;
    size_t KRsize;
    keyrng_t keyPool;

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

    keyrng_t getKeyRing();
};

#endif /* SRC_NETWORKOWNER_H_ */
