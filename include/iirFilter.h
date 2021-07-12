/*
 * iirFilter.h
 *
 *  Created on: Apr 13, 2021
 *      Author: cjgree13
 */
#ifndef INCLUDE_IIRFILTER_H_
#define INCLUDE_IIRFILTER_H_

#define FILTER_ORDER 4


typedef struct
{

	double regX[FILTER_ORDER];
	double regY[FILTER_ORDER];
	double regZ[FILTER_ORDER];

	double a0[FILTER_ORDER];
	double a1[FILTER_ORDER];
	double a2[FILTER_ORDER];
	double b0[FILTER_ORDER];
	double b1[FILTER_ORDER];
	double b2[FILTER_ORDER];
	int num;
} IirData;


class IirFilter
{

private:

public:

	//! \brief TBD.
	//!
	//! @return TBD.
	void IirFilterInit(IirData *iir);

	//! \brief TBD.
	//!
	//! @return TBD.
	void RunIIRFilter(IirData *iir, double *BufferIn, double *BufferOut, int length);
};


#endif /* INCLUDE_IIRFILTER_H_ */
