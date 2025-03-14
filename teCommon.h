//-------------------------------------------------------------------------
// teCommon.h
//-------------------------------------------------------------------------
// Defines and methods common to TE3_audio and TE3.

#pragma once

#include <Arduino.h>

// orthogonal API for getting CC descriptors
// Note that the port numbers are different between
// TE3, which has 8 midi ports, and TE3_audio which
// has only one input serial midi port.
//
// TE3_audio differentiates messages for the SGTL
// from those for itself by use of the following
// define.

#define AUDIO_CHANNEL		14
	// The SGTL_5000 uses channel 15

extern bool audio_writeOnlyCC(uint8_t cc);				// true if CC is writeOnly
extern const char *audio_getCCName(uint8_t cc);			// get the name of a CC
extern uint8_t audio_getCCMax(uint8_t cc);				// get the maximum value (range) of a CC
	// returns 255 for monadic commands


//----------------------------------------
// CC Numbers
//----------------------------------------

#define AUDIO_CC_BASE					30

#define AUDIO_CC_DUMP					30		// Write Only monadic command
#define AUDIO_CC_REBOOT					31		// Write Only monadic command
#define AUDIO_CC_RESET					32		// Write Only monadic command (unimplemented at this time)

// Mixer settings
// IN = monitor the SGTL5000 i2s IN, which is sent to the iPad via USB out
// USB = monitor the USB in that comes back from the iPad and sent to the rpI Looper
// LOOP = monitor what comes back from the rPi Looper which currently represents
//		the final output of the whole system (but that might change)
// AUX = unused

#define AUDIO_CC_MIX_IN					33		// 0..127, 100=0db, default(0)
#define AUDIO_CC_MIX_USB				34		// 0..127, 100=0db, default(0)
#define AUDIO_CC_MIX_LOOP				35		// 0..127, 100=0db, default(100)
#define AUDIO_CC_MIX_AUX				36		// 0..127, 100=0db, default(0 or 80 if sine is output)

#define AUDIO_CC_MAX					36


// end of teCommon.h
