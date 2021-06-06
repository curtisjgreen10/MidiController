/*
 * iirFilter.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: cjgree13
 */

#include "iirFilter.h"

void IirFilter::IirFilterInit(IirData *iir)
{
	iir->NumSections = 2;

	for(int i=0; i<REG_SIZE; i++) // Init the shift registers.
	{
		iir->Reg0[i] = 0.0;
		iir->Reg1[i] = 0.0;
		iir->Reg2[i] = 0.0;
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

void IirFilter::RunIIRBiquadForm1(IirData *iir, double *Input, double *Output, int NumSigPts)
{
	double y;
	int j, k;

	for(j=0; j<REG_SIZE; j++) // Init the shift registers.
	{
		iir->RegX1[j] = 0.0;
		iir->RegX2[j] = 0.0;
		iir->RegY1[j] = 0.0;
		iir->RegY2[j] = 0.0;
	}

	for(j=0; j<NumSigPts; j++)
	{
		y = SectCalcForm1(iir, 0, Input[j]);
		for(k=1; k<iir->NumSections; k++)
		{
			y = SectCalcForm1(iir, k, y);
		}
		Output[j] = y;
	}
}

double IirFilter::SectCalcForm1(IirData *iir, int k, double x)
{
	double y, CenterTap;

	CenterTap = x * iir->b0[k] + iir->b1[k] * iir->RegX1[k] + iir->b2[k] * iir->RegX2[k];
	y = iir->a0[k] * CenterTap - iir->a1[k] * iir->RegY1[k] - iir->a2[k] * iir->RegY2[k];

	iir->RegX2[k] = iir->RegX1[k];
	iir->RegX1[k] = x;
	iir->RegY2[k] = iir->RegY1[k];
	iir->RegY1[k] = y;

	return(y);
}

void IirFilter::RunIIRBiquadForm2(IirData *iir, double *Input, double *Output, int NumSigPts)
{
	double y;
	int j, k;

	for(j=0;j<NumSigPts;j++)
	{
		y = SectCalcForm2(iir, 0, Input[j]);

		for(k=1; k<(iir->NumSections); k++)
		{
			y = SectCalcForm2(iir, k, y);
		}
		Output[j] = y;
	}
}

double IirFilter::SectCalcForm2(IirData *iir, int k, double x)
{
	double y;

	iir->Reg0[k] = x - iir->a1[k] * iir->Reg1[k] - iir->a2[k] * iir->Reg2[k];
	y = iir->b0[k] * iir->Reg0[k] + iir->b1[k] * iir->Reg1[k] + iir->b2[k] * iir->Reg2[k];

	// Shift the register values
	iir->Reg2[k] = iir->Reg1[k];
	iir->Reg1[k] = iir->Reg0[k];

	return(y);
}
