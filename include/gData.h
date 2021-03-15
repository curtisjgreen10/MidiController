/*
 * gData.h
 *
 *  Created on: Mar 4, 2021
 *      Author: cjgree13
 */

#ifndef INCLUDE_MIDIGLOBALDATA_H_
#define INCLUDE_MIDIGLOBALDATA_H_

namespace MidiGlobalData
{
	extern bool micInput;
	extern double micInputvol;
	extern double drum1vol;
	extern double drum2vol;
	extern double drum3vol;
	extern double drum4vol;
	extern double cymbal1vol;
	extern double cymbal2vol;
	extern double cymbal3vol;
	extern double cymbal4vol;
	extern double drum1hits[64];
	extern int drum1numHits;
}

#endif /* INCLUDE_MIDIGLOBALDATA_H_ */
