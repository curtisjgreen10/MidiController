/*
 * digFilter.h
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */

#ifndef INCLUDE_DIGFILTER_H_
#define INCLUDE_DIGFILTER_H_

#include <stdint.h>


#define FIR_FILTER_LENGTH 128

typedef struct
{
	float buf[FIR_FILTER_LENGTH];
	uint8_t bufIndex;
	float out;
} FirFilter;


namespace DigFilter
{
	void StartVoiceRecorder(FirFilter *fir, long seconds);
	void FirFilterInit(FirFilter *fir);
	float FirFilterUpdate(FirFilter *fir, float input);
}



#endif /* INCLUDE_DIGFILTER_H_ */
