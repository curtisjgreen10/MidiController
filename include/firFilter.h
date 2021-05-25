/*
 * firFilter.h
 *
 *  Created on: Apr 13, 2021
 *      Author: cjgree13
 */
#ifndef INCLUDE_FIRFILTER_H_
#define INCLUDE_FIRFILTER_H_
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <gmp.h>
#include <gmpxx.h>

#define FIR_FILTER_LENGTH 513

typedef struct
{
	double buf[FIR_FILTER_LENGTH];
	uint16_t bufIndex;
	double out;
} FirData;

class FirFilter
{

private:

	void InitializeCoeffs();

public:
	//! \brief TBD.
	//!
	//! @return TBD.
	void FirFilterInit(FirData *fir);


	//! \brief TBD.
	//!
	//! @return TBD.
	double FirFilterUpdate(FirData *fir, double input);
};

#endif /* INCLUDE_FIRFILTER_H_ */
