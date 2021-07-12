/*
 * iirFilter.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: cjgree13
 */

#include "iirFilter.h"

void IirFilter::IirFilterInit(IirData *iir)
{
	iir->num = 2;

	for(int i=0; i<FILTER_ORDER; i++)
	{
		iir->regX[i] = 0.0;
		iir->regY[i] = 0.0;
		iir->regZ[i] = 0.0;
	}


	// section 1 coefficients
	iir->b0[0] = 1;
	iir->b1[0] = -1.999991085448208361086130935291294008493;
	iir->b2[0] = 1;
	iir->a0[0] = 1;
	iir->a1[0] = -1.921873699827897574721191631397232413292;
	iir->a2[0] = 0.92546310338379877968861819681478664279;

	// section 2 coefficients
	iir->b0[1] = 1;
	iir->b1[1] = -1.999949076868472674917143194761592894793;
	iir->b2[1] = 1;
	iir->a0[1] = 1;
	iir->a1[1] = -1.989964926837106240498087572632357478142;
	iir->a2[1] = 0.991043953543983113796400630235439166427;


}

void IirFilter::RunIIRFilter(IirData *iir, double *BufferIn, double *BufferOut, int length)
{
	double y;
	int j, k;

	for(j=0;j<length;j++)
	{
		iir->regX[0] = BufferIn[j] - iir->a1[0] * iir->regY[0] - iir->a2[0] * iir->regZ[0];
		y = iir->b0[0] * iir->regX[0] + iir->b1[0] * iir->regY[0] + iir->b2[0] * iir->regZ[0];

		iir->regZ[0] = iir->regY[0];
		iir->regY[0] = iir->regX[0];


		for(k=1; k<(iir->num); k++)
		{
			iir->regX[k] = BufferIn[j] - iir->a1[k] * iir->regY[k] - iir->a2[k] * iir->regZ[k];
			y = iir->b0[k] * iir->regX[k] + iir->b1[k] * iir->regY[k] + iir->b2[k] * iir->regZ[k];

			iir->regZ[k] = iir->regY[k];
			iir->regY[k] = iir->regX[k];
		}
		BufferOut[j] = y;
	}
}

