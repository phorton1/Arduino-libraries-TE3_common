// teCommon.cpp
//
// Cross platform API to TE_hub MIDI CC numbers and parameters

#include "teCommon.h"


#define TEHUB_CC_DUMP					30		// Write Only monadic command
#define TEHUB_CC_REBOOT					31		// Write Only monadic command
#define TEHUB_CC_RESET					32		// Write Only monadic command (unimplemented at this time)
#define TEHUB_CC_MIX_IN					33		// 0..127, 100=0db, default(0)
#define TEHUB_CC_MIX_USB				34		// 0..127, 100=0db, default(0)
#define TEHUB_CC_MIX_LOOP				35		// 0..127, 100=0db, default(100)
#define TEHUB_CC_MIX_AUX				36		// 0..127, 100=0db, default(0 or 80 if sine is output)



// extern
bool tehub_writeOnlyCC(uint8_t cc)
{
	if (cc == TEHUB_CC_DUMP	||
		cc == TEHUB_CC_REBOOT ||
		cc == TEHUB_CC_RESET)
		return true;
	return false;
}


// extern
uint8_t tehub_getCCMax(uint8_t cc)
{
	switch (cc)
	{
		case TEHUB_CC_DUMP		: return 255;
		case TEHUB_CC_REBOOT	: return 255;
		case TEHUB_CC_RESET		: return 255;
		case TEHUB_CC_MIX_IN	: return 127;
		case TEHUB_CC_MIX_USB	: return 127;
		case TEHUB_CC_MIX_LOOP	: return 127;
		case TEHUB_CC_MIX_AUX	: return 127;
	}
	return 255;
}


// extern
const char *tehub_getCCName(uint8_t cc)
{
	switch (cc)
	{
		case TEHUB_CC_DUMP		: return "DUMP_TEHUB";
		case TEHUB_CC_REBOOT	: return "REBOOT_TEHUB";
		case TEHUB_CC_RESET		: return "RESET_TEHUB";
		case TEHUB_CC_MIX_IN	: return "MIX_IN";
		case TEHUB_CC_MIX_USB	: return "MIX_USB";
		case TEHUB_CC_MIX_LOOP	: return "MIX_LOOP";
		case TEHUB_CC_MIX_AUX	: return "MIX_AUX";
	}
	return "UNKNOWN_TEHUB_CC";
}





// end of sgtl5000.cpp


