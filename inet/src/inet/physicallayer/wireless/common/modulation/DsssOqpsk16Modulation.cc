//
// Copyright (C) 2014 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/physicallayer/wireless/common/modulation/DsssOqpsk16Modulation.h"

namespace inet {

namespace physicallayer {

const DsssOqpsk16Modulation DsssOqpsk16Modulation::singleton;

DsssOqpsk16Modulation::DsssOqpsk16Modulation() :
    // TODO fill in the correct real and imaginary parts
    ApskModulationBase(new std::vector<ApskSymbol>({ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN),
                                                    ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN),
                                                    ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN),
                                                    ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN), ApskSymbol(NaN, NaN)}))
{
}

DsssOqpsk16Modulation::~DsssOqpsk16Modulation() {
    delete constellation;
}

double DsssOqpsk16Modulation::calculateBER(double snir, Hz bandwidth, bps bitrate) const
{
    // Taken from MiXiM 802.15.4 decider by Karl Wessel
    // Valid for IEEE 802.15.4 2.45 GHz OQPSK modulation
    // Following formula is defined in IEEE 802.15.4 standard, please check the
    // 2006 standard, page 268, section E.4.1.8 Bit error rate (BER)
    // calculations, formula 7). Here you can see that the factor of 20.0 is correct ;).

    // without these, the calculation does not make sense
    ASSERT(bandwidth >= MHz(2));
    ASSERT(bitrate == kbps(250));

    const double dSNRFct = 20.0 * snir;
    double dSumK = 0;
    int k = 2;

    /* following loop was optimized by using n_choose_k symmetries
       for (k=2; k <= 16; ++k) {
        dSumK += pow(-1.0, k) * n_choose_k(16, k) * exp(dSNRFct * (1.0 / k - 1.0));
       }
     */

    // n_choose_k(16, k) == n_choose_k(16, 16-k)
    for (; k < 8; k += 2) {
        // k will be 2, 4, 6 (symmetric values: 14, 12, 10)
        dSumK += math::n_choose_k(16, k) * (exp(dSNRFct * (1.0 / k - 1.0)) + exp(dSNRFct * (1.0 / (16 - k) - 1.0)));
    }

    // for k =  8 (which does not have a symmetric value)
    k = 8;
    dSumK += math::n_choose_k(16, k) * exp(dSNRFct * (1.0 / k - 1.0));
    for (k = 3; k < 8; k += 2) {
        // k will be 3, 5, 7 (symmetric values: 13, 11, 9)
        dSumK -= math::n_choose_k(16, k) * (exp(dSNRFct * (1.0 / k - 1.0)) + exp(dSNRFct * (1.0 / (16 - k) - 1.0)));
    }

    // for k = 15 (because of missing k=1 value)
    k = 15;
    dSumK -= math::n_choose_k(16, k) * exp(dSNRFct * (1.0 / k - 1.0));

    // for k = 16 (because of missing k=0 value)
    k = 16;
    dSumK += math::n_choose_k(16, k) * exp(dSNRFct * (1.0 / k - 1.0));
    double ber = (8.0 / 15) * (1.0 / 16) * dSumK;
    ASSERT(0.0 <= ber && ber <= 1.0);
    return ber;
}

double DsssOqpsk16Modulation::calculateSER(double snir, Hz bandwidth, bps bitrate) const
{
    return NAN;
}

} // namespace physicallayer

} // namespace inet

