/*
 * digFilter.h
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */

#ifndef INCLUDE_DIGFILTER_H_
#define INCLUDE_DIGFILTER_H_

#include <stdint.h>


#define FIR_FILTER_LENGTH 257
#define REG_SIZE 4 // filter order




typedef struct
{
	float buf[FIR_FILTER_LENGTH];
	uint16_t bufIndex;
	float out;
} FirFilter;

typedef struct
{
	double Reg0[REG_SIZE];
	double Reg1[REG_SIZE];
	double Reg2[REG_SIZE];
	double a0[REG_SIZE];
	double a1[REG_SIZE];
	double a2[REG_SIZE];
	double b0[REG_SIZE];
	double b1[REG_SIZE];
	double b2[REG_SIZE];
	int NumSections;
} IirFilter;


namespace DigFilter
{


	void StartVoiceRecorder(IirFilter *iir, long seconds);
	void FirFilterInit(FirFilter *fir);
	float FirFilterUpdate(FirFilter *fir, float input);

	void IirFilterInit(IirFilter *iir);
	void RunIIRBiquadForm2(IirFilter *iir, char *Input, char *Output, int NumSigPts);
	double SectCalcForm2double(IirFilter *iir, int k, double x);
	double SectCalcForm2(IirFilter *iir, int k, char x);
}



#endif /* INCLUDE_DIGFILTER_H_ */
