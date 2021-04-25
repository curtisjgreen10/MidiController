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
	extern bool micInput;		/*!< true if mic is to be routed to output */
	extern double micInputvol;	/*!< current volume of microphone */
	extern double drum1vol;		/*!< current volume of drum 1 */
	extern double drum2vol;		/*!< current volume of drum 2 */
	extern double drum3vol;		/*!< current volume of drum 3 */
	extern double drum4vol;		/*!< current volume of drum 4 */
	extern double cymbal1vol;	/*!< current volume of cymbal 1 */
	extern double cymbal2vol;	/*!< current volume of cymbal 2 */
	extern double cymbal3vol;	/*!< current volume of cymbal 3 */
	extern double cymbal4vol;	/*!< current volume of cymbal 4 */
	extern double drum1hitTime;	/*!< time of drum 1 button press */
}

#endif /* INCLUDE_MIDIGLOBALDATA_H_ */
