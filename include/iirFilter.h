/*
 * iirFilter.h
 *
 *  Created on: Apr 13, 2021
 *      Author: cjgree13
 */
#ifndef INCLUDE_IIRFILTER_H_
#define INCLUDE_IIRFILTER_H_

#define REG_SIZE 4 // filter order


typedef struct
{
	//form 1 registers
	double RegX1[REG_SIZE];
	double RegY1[REG_SIZE];
	double RegX2[REG_SIZE];
	double RegY2[REG_SIZE];

	//form 2 registers
	double Reg0[REG_SIZE];
	double Reg1[REG_SIZE];
	double Reg2[REG_SIZE];

	//coefficients for both forms
	double a0[REG_SIZE];
	double a1[REG_SIZE];
	double a2[REG_SIZE];
	double b0[REG_SIZE];
	double b1[REG_SIZE];
	double b2[REG_SIZE];
	int NumSections;
} IirData;


class IirFilter
{

private:

	//! \brief TBD.
	//!
	//! @return TBD.
	double SectCalcForm1(IirData *iir, int k, double x);

	//! \brief TBD.
	//!
	//! @return TBD.
	double SectCalcForm2(IirData *iir, int k, double x);

public:

	//! \brief TBD.
	//!
	//! @return TBD.
	void IirFilterInit(IirData *iir);

	//! \brief TBD.
	//!
	//! @return TBD.
	void RunIIRBiquadForm1(IirData *iir, double *Input, double *Output, int NumSigPts);

	//! \brief TBD.
	//!
	//! @return TBD.
	void RunIIRBiquadForm2(IirData *iir, double *Input, double *Output, int NumSigPts);
};


#endif /* INCLUDE_IIRFILTER_H_ */
