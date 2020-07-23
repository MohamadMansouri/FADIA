#ifndef CONTSANTS_H_
#define CONTSANTS_H_


#define RUNTIME_TEST
// #define ENERGY_TEST
// #define REVOKE_TEST

#define PASTA 0
#define SALADS 1 
#define FADIA 2
#define PADS 3

#define SCENARIO PASTA
#define TREE 

#define REALLEN(_N_,_R_) _N_* _R_ / (1 + _R_)  
#define FAKELEN(_N_,_R_) _N_ / (1 + _R_)  

#define JRQ_SIZE(_RSIZE_) 40 + _RSIZE_ * 4
#define JRP_SIZE 48
#define CRQ_SIZE(_RSIZE_) 12 + _RSIZE_ * 4
#define CRP_SIZE 48
#define CAK_SIZE 44
#define URQ_SIZE(_RSIZE_, _MSIZE_) 44 + (_RSIZE_ * 4 * 2) +  (_RSIZE_ * 1/8)  + (32 * _MSIZE_)
#define RVK_SIZE(_KSIZE_) 32 + (_KSIZE_ * 4) 


#define SLOPE(X1,Y1,X2,Y2) (Y2 - Y1) / (X2 - X1)
#define INTERCEPT(X1,Y1,X2,Y2) (Y1*X2 - Y2*X1) / (X2 - X1)


#ifndef RUNTIME_TEST
#define WIRELESS
#endif // RUNTIME_TEST

#define MAXCHILDREN 20
#define NEIGHBOR_DISTANCE 1
#define DELTAH 600
#define MAX_CRASH 1000
#define MINGSIZE 1

#ifdef ENERGY_TEST
#define TRANSMITION_CONSUMPTION 58.5
#define RECEPTION_CONSUMPTION 65.4
#define LISTENING_CONSUMPTION 65.4
#define MCU_CONSUMPTION 5.4
#define IDLE_CONSUMPTION 0.16

#define CRITICAL_ENERGY 0.1
#define FULL_ENERGY 0.9
#define CHILDREN(_PER_) SLOPE(CRITICAL_ENERGY,0, FULL_ENERGY, MAXCHILDREN) * per + \
						 INTERCEPT(CRITICAL_ENERGY,0, FULL_ENERGY, MAXCHILDREN)

#define CRITICAL_ENERGYD 0
#define FULL_ENERGYD 1 
#define DELTAG(_PER_, _DELTAGMAX_) SLOPE(CRITICAL_ENERGYD,0, FULL_ENERGYD, _DELTAGMAX_) * per + \
						 INTERCEPT(CRITICAL_ENERGYD,0, FULL_ENERGYD, _DELTAGMAX_)						 
#endif //ENERGY_TEST


// #define FIRMWARE_SIZE 32 * 1024 														// firmware size 32 KB

#if SCENARIO == PASTA
#define NDELAY 0.0023																	// end-to-end delay 2.3 ms
#define PDELAY NDELAY / 4
#define BITRATE 12.51 * 1024 * 1024																		// bitrate not needed (configured in the channel)
#define BYTERATE BITRATE / 8
#define TIMEOUT_ACK 999																	// timeout for acknowledgement to be received 2 s
#define TIMEOUT_RSP 11																	// timeout for commitment response to be received 6 s
#define TIMEOUT_UP 999																	// timeout for commitment response to be received 6 s
#define HMAC_DELAY_S(_SIZE_) 1 * 0													// should never be used (raise an error)
#define HMAC_DELAY_P(_SIZE_) 1 * 0													// should never be used (raise an error)
#define HMAC_DELAY(_SIZE_) (0.000256944 * _SIZE_ + 0.037888889) * 0.001
#define FIRMWARE_CHECK_DELAY 13.171 * 0.001
#define FIRMWARE_CHECK_DELAY_S 1 / 0												// should never be used (raise an error)
#define FIRMWARE_CHECK_DELAY_P 1 / 0												// should never be used (raise an error)
#elif SCENARIO == PADS
#define NDELAY 0.001																	// end-to-end delay 15 ms
#define PDELAY NDELAY / 4
#define BITRATE 250 * 1024 																// bitrate 35 kbps
#define BYTERATE BITRATE / 8
#define TIMEOUT_ACK 999																	// timeout for acknowledgement to be received 2 s
#define TIMEOUT_RSP 111																	// timeout for commitment response to be received 6 s
#define TIMEOUT_UP 999																	// timeout for commitment response to be received 6 s
#define HMAC_DELAY_S(_SIZE_) 1 / 0													// should never be used (raise an error)
#define HMAC_DELAY_P(_SIZE_) 1 / 0													// should never be used (raise an error)
#define HMAC_DELAY(_SIZE_) 48 * 0.001
#define FIRMWARE_CHECK_DELAY 187 * 0.001
#define FIRMWARE_CHECK_DELAY_S 1 / 0												// should never be used (raise an error)
#define FIRMWARE_CHECK_DELAY_P 1 / 0												// should never be used (raise an error)
#elif SCENARIO == SALADS
#define NDELAY 0.015																	// end-to-end delay 15 ms
#define PDELAY NDELAY / 4
#define BITRATE 35 * 1024 																// bitrate 35 kbps
#define BYTERATE BITRATE / 8
#define TIMEOUT_ACK 2																	// timeout for acknowledgement to be received 2 s
#define TIMEOUT_RSP 6																	// timeout for commitment response to be received 6 s
#define TIMEOUT_UP 6																	// timeout for commitment response to be received 6 s
#define HMAC_DELAY_S(_SIZE_) 1 / 0													// should never be used (raise an error)
#define HMAC_DELAY_P(_SIZE_) 1 / 0													// should never be used (raise an error)
#define HMAC_DELAY(_SIZE_) (0.001210594 * _SIZE_ + 0.191260997) * 0.001
#define FIRMWARE_CHECK_DELAY 40.02 * 0.001
#define FIRMWARE_CHECK_DELAY_S 1 / 0												// should never be used (raise an error)
#define FIRMWARE_CHECK_DELAY_P 1 / 0												// should never be used (raise an error)
#elif SCENARIO == FADIA
#define NDELAY 0.030725																	// end-to-end delay 30.725 ms
#define PDELAY NDELAY / 4
#define BITRATE 25.2 * 1024 															// bitrate 25.2 kbps
#define BYTERATE BITRATE / 8
#define TIMEOUT_ACK 999																	// timeout for acknowledgement to be received 2 s
#define TIMEOUT_RSP 111																	// timeout for commitment response to be received 6 s
#define TIMEOUT_UP 999																	// timeout for commitment response to be received 6 s
#define HMAC_DELAY_S(_SIZE_) (0.237098 * _SIZE_ + 55.6929) * 0.001
#define HMAC_DELAY_P(_SIZE_) (0.000246304 * _SIZE_ + 0.0601183) * 0.001
#define HMAC_DELAY(_SIZE_) 1 / 0													// should never be used (raise an error)
#define FIRMWARE_CHECK_DELAY_S 1960 * 0.001												// 8 KB firmware 
#define FIRMWARE_CHECK_DELAY_P 8.079 * 0.001											// 32 KB firmware
#define FIRMWARE_CHECK_DELAY 1 / 0													// should never be used (raise an error)
#else 
#error No scenarion was specified!!!
#endif // SCENARIO

#endif // CONTSANTS_H_