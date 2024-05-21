//-----------------------------------------------------------------------------
// sgtl5000Midi.h
//-----------------------------------------------------------------------------
// #defines and cross platform methods for controlling the SGTL5000 via midi
//
// This file contains #defines that are used in the actual SGTL5000 implementation
// for the enumerated parameters to SGTL5000 API methods.

#pragma once

#include <Arduino.h>

#define SGTL5000_CABLE 		0x0E
#define SGTL5000_CHANNEL	1
	// common to TE3 and TE3_hub
	
extern bool sgtl5000_writeOnlyCC(uint8_t cc);				// true if CC is writeOnly
extern const char *sgtl5000_getCCName(uint8_t cc);			// get the name of a CC
extern uint8_t sgtl5000_getCCMax(uint8_t cc);				// get the maximum value (range) of a CC
	// returns 255 for monadic commands


//----------------------------------------
// enumerated method paramters
//----------------------------------------

#define	SGTL_INPUT_LINEIN	0	// same as AUDIO_INPUT_LINEIN
#define SGTL_INPUT_MIC		1	// same as AUDIO_INPUT_MIC
	// params to setInput()

#define HEADPHONE_NORMAL	0	// normal mode
#define HEADPHONE_LINEIN	1	// bypass mode
	// param to setHeadphoneSelect()

#define DAC_VOLUME_RAMP_EXPONENTIAL 0
#define DAC_VOLUME_RAMP_LINEAR  	1
#define DAC_VOLUME_RAMP_DISABLE  	2
	// param to setDacVolumeRamp()

#define ADC_HIGH_PASS_ENABLE		0
#define ADC_HIGH_PASS_FREEZE		1
#define ADC_HIGH_PASS_DISABLE		2
	// param to setAdcHighPassFilter()

#define DAP_DISABLE					0
#define DAP_ENABLE_PRE				1
#define DAP_ENABLE_POST				2
	// param to setDapEnable()

#define SURROUND_DISABLED			0
#define SURROUND_MONO				1
#define SURROUND_STEREO				2
	// param to setSurroundEnable()

#define EQ_FLAT 					0
#define EQ_PARAM_EQ 				1
#define EQ_TONE_CONTROLS			2
#define EQ_GRAPHIC_EQ				3
	// param to setEqSelect()

#define EQ_BAND0					0 	// 115 Hz = TONE_CONTROLS(BASS)
#define EQ_BAND1					1 	// 330 Hz
#define EQ_BAND2					2 	// 990 Hz
#define EQ_BAND3					3 	// 3000 Hz
#define EQ_BAND4					4	// 9900 Hz = TONE_CONTROLS(TREBLE)
	// param to setEqBand

#define FILTER_LOPASS 				0
#define FILTER_HIPASS 				1
#define FILTER_BANDPASS 			2
#define FILTER_NOTCH 				3
#define FILTER_PARAEQ 				4
#define FILTER_LOSHELF 				5
#define FILTER_HISHELF 				6
	// params to PEQ method


//----------------------------------------
// CC Numbers
//----------------------------------------
// does not include AVC or PEQ
// Note that enable() does NOT set all registers, and that
// 		they will contain stale values on a soft-reboot,
// 	    and it generates a loud pop on a soft-reboot.
// For that reason my version of enable() detects a soft
//	    reboot and bails.
// For sanity, call setDefaults() after enable() to have
//		the thing in a known state.


#define SGTL_CC_BASE					30

// Values on my scales							//	MAX				RESET	ENABLE	setDefaults					notes
//																					x = reset to sgtl->enable defaults
#define SGTL_CC_DUMP					30		//  Write Only
#define SGTL_CC_SET_DEFAULTS			31		//	Write Only
#define SGTL_CC_INPUT_SELECT			32      //	1				1  		0  		x							enable() switches to line input
#define SGTL_CC_MIC_GAIN_				33      //	3               0       		1 = 20db
#define SGTL_CC_LINEIN_LEVEL			34		//	Write Only
#define SGTL_CC_LINEIN_LEVEL_LEFT		35      //	15              0       		7 = middle of range
#define SGTL_CC_LINEIN_LEVEL_RIGHT		36      //	15              0       		7
#define SGTL_CC_DAC_VOLUME				37		//	Write Only
#define SGTL_CC_DAC_VOLUME_LEFT			38      //	127             0       0		x							126=-63db; 127=muted; enable() explicitly re-sets the default value
#define SGTL_CC_DAC_VOLUME_RIGHT		39      //	127             0       0		x							126=-63db; 127=muted; enable() explicitly re-sets the default value
#define SGTL_CC_DAC_VOLUME_RAMP			40      //	2               1       0  		x							enable() enables exponential, not linear, ramping
#define SGTL_CC_LINEOUT_LEVEL			41		//	Write Only
#define SGTL_CC_LINEOUT_LEVEL_LEFT		42      //	31              27      2  		13 = middle of range, but	enable() explicitly sets LINE_OUT levels
#define SGTL_CC_LINEOUT_LEVEL_RIGHT		43      //	31              27      2		13 = upper of paul's 0..18	enable() explicitly sets LINE_OUT levels
#define SGTL_CC_HP_SELECT				44      //	1               0       0		x							enable() explicitly write CHIP_ANA_CTRL, including this bit
#define SGTL_CC_HP_VOLUME				45		//	Write Only
#define SGTL_CC_HP_VOLUME_LEFT			46      //	127             97      0		97							enable() currently turns the headphones all the way down
#define SGTL_CC_HP_VOLUME_RIGHT			47      //	127             97      0		97							enable() currently turns the headphones all the way down
#define SGTL_CC_MUTE_HP					48      //	1               0       1  		0							enable() explicitly write CHIP_ANA_CTRL, including this bit
#define SGTL_CC_MUTE_LINEOUT			49      //	1               0       0		0							enable() explicitly write CHIP_ANA_CTRL, including this bit
#define SGTL_CC_ADC_HIGH_PASS			50      //	2               0				x
#define SGTL_CC_DAP_ENABLE				51      //	2               0				x
#define SGTL_CC_SURROUND_ENABLE			52      //	2               0			    x
#define SGTL_CC_SURROUND_WIDTH			53      //	7               4               x
#define SGTL_CC_BASS_ENHANCE_ENABLE		54      //	1               0               x
#define SGTL_CC_BASS_CUTOFF_ENABLE		55      //	1               0               x
#define SGTL_CC_BASS_CUTOFF_FREQ		56      //	6               4               x
#define SGTL_CC_BASS_BOOST				57      //	127             96              x
#define SGTL_CC_BASS_VOLUME				58      //	63              58              x
#define SGTL_CC_EQ_SELECT				59      //	3               0               x
#define SGTL_CC_EQ_BAND0				60      //	95              15              x
#define SGTL_CC_EQ_BAND1				61      //	95              15              x
#define SGTL_CC_EQ_BAND2				62      //	95              15              x
#define SGTL_CC_EQ_BAND3				63      //	95              15              x
#define SGTL_CC_EQ_BAND4				64      //	95              15              x

// #define SGTL_CC_AVC					65		// unimpelmented/complicated 	 	DISABLED in setDefaults but AVC specific registers are currently not reset
// #define SGTL_CC_PEQ					66		// unimplemented/complicated 		DISABLED in setDefaults but PEQ specific registers are currently not reset

#define SGTL_CC_MAX						64


// end of sgtl5000midi.h