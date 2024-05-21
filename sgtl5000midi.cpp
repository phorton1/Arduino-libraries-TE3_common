// sgtl5000midi.cpp
//
// Cross platform API to sgtl5000 MIDI CC numbers and parameters

#include "sgtl5000midi.h"


// extern
bool sgtl5000_writeOnlyCC(uint8_t cc)
{
	if (cc == SGTL_CC_DUMP ||
		cc == SGTL_CC_SET_DEFAULTS ||
		cc == SGTL_CC_LINEIN_LEVEL ||
		cc == SGTL_CC_DAC_VOLUME ||
		cc == SGTL_CC_LINEOUT_LEVEL ||
		cc == SGTL_CC_HP_VOLUME)
		return true;
	return false;
}


// extern
uint8_t sgtl5000_getCCMax(uint8_t cc)
{
	switch (cc)
	{
		case SGTL_CC_DUMP					: return 255;
		case SGTL_CC_SET_DEFAULTS			: return 255;
		case SGTL_CC_INPUT_SELECT			: return 1;
		case SGTL_CC_MIC_GAIN_				: return 3;
		case SGTL_CC_LINEIN_LEVEL			: return 15;
		case SGTL_CC_LINEIN_LEVEL_LEFT		: return 15;
		case SGTL_CC_LINEIN_LEVEL_RIGHT		: return 15;
		case SGTL_CC_DAC_VOLUME				: return 127;
		case SGTL_CC_DAC_VOLUME_LEFT		: return 127;
		case SGTL_CC_DAC_VOLUME_RIGHT		: return 127;
		case SGTL_CC_DAC_VOLUME_RAMP		: return 2;
		case SGTL_CC_LINEOUT_LEVEL			: return 31;
		case SGTL_CC_LINEOUT_LEVEL_LEFT		: return 31;
		case SGTL_CC_LINEOUT_LEVEL_RIGHT	: return 31;
		case SGTL_CC_HP_SELECT				: return 1;
		case SGTL_CC_HP_VOLUME				: return 127;
		case SGTL_CC_HP_VOLUME_LEFT			: return 127;
		case SGTL_CC_HP_VOLUME_RIGHT		: return 127;
		case SGTL_CC_MUTE_HP				: return 1;
		case SGTL_CC_MUTE_LINEOUT			: return 1;
		case SGTL_CC_ADC_HIGH_PASS			: return 2;
		case SGTL_CC_DAP_ENABLE				: return 2;
		case SGTL_CC_SURROUND_ENABLE		: return 2;
		case SGTL_CC_SURROUND_WIDTH			: return 7;
		case SGTL_CC_BASS_ENHANCE_ENABLE	: return 1;
		case SGTL_CC_BASS_CUTOFF_ENABLE		: return 1;
		case SGTL_CC_BASS_CUTOFF_FREQ		: return 6;
		case SGTL_CC_BASS_BOOST				: return 127;
		case SGTL_CC_BASS_VOLUME			: return 0x3f;
		case SGTL_CC_EQ_SELECT				: return 3;
		case SGTL_CC_EQ_BAND0				: return 0x5f;
		case SGTL_CC_EQ_BAND1				: return 0x5f;
		case SGTL_CC_EQ_BAND2				: return 0x5f;
		case SGTL_CC_EQ_BAND3				: return 0x5f;
		case SGTL_CC_EQ_BAND4				: return 0x5f;
	}
	return 255;
}


// extern
const char *sgtl5000_getCCName(uint8_t cc)
{
	switch (cc)
	{
		case SGTL_CC_DUMP					: return "DUMP_SGTL";
		case SGTL_CC_SET_DEFAULTS			: return "SET_DEFAULTS";
		case SGTL_CC_INPUT_SELECT			: return "INPUT_SELECT";
		case SGTL_CC_MIC_GAIN_				: return "MIC_GAIN";
		case SGTL_CC_LINEIN_LEVEL			: return "LINEIN_LEVEL";
		case SGTL_CC_LINEIN_LEVEL_LEFT		: return "LINEIN_LEVEL_LEFT";
		case SGTL_CC_LINEIN_LEVEL_RIGHT		: return "LINEIN_LEVEL_RIGHT";
		case SGTL_CC_DAC_VOLUME				: return "DAC_VOLUME";
		case SGTL_CC_DAC_VOLUME_LEFT		: return "DAC_VOLUME_LEFT";
		case SGTL_CC_DAC_VOLUME_RIGHT		: return "DAC_VOLUME_RIGHT";
		case SGTL_CC_DAC_VOLUME_RAMP		: return "DAC_VOLUME_RAMP";
		case SGTL_CC_LINEOUT_LEVEL			: return "LINEOUT_LEVEL";
		case SGTL_CC_LINEOUT_LEVEL_LEFT		: return "LINEOUT_LEVEL_LEFT";
		case SGTL_CC_LINEOUT_LEVEL_RIGHT	: return "LINEOUT_LEVEL_RIGHT";
		case SGTL_CC_HP_SELECT				: return "HP_SELECT";
		case SGTL_CC_HP_VOLUME				: return "HP_VOLUME";
		case SGTL_CC_HP_VOLUME_LEFT			: return "HP_VOLUME_LEFT";
		case SGTL_CC_HP_VOLUME_RIGHT		: return "HP_VOLUME_RIGHT";
		case SGTL_CC_MUTE_HP				: return "MUTE_HP";
		case SGTL_CC_MUTE_LINEOUT			: return "MUTE_LINEOUT";
		case SGTL_CC_ADC_HIGH_PASS			: return "ADC_HIGH_PASS";
		case SGTL_CC_DAP_ENABLE				: return "DAP_ENABLE";
		case SGTL_CC_SURROUND_ENABLE		: return "SURROUND_ENABLE";
		case SGTL_CC_SURROUND_WIDTH			: return "SURROUND_WIDTH";
		case SGTL_CC_BASS_ENHANCE_ENABLE	: return "BASS_ENHANCE_ENABLE";
		case SGTL_CC_BASS_CUTOFF_ENABLE		: return "BASS_CUTOFF_ENABLE";
		case SGTL_CC_BASS_CUTOFF_FREQ		: return "BASS_CUTOFF_FREQ";
		case SGTL_CC_BASS_BOOST				: return "BASS_BOOST";
		case SGTL_CC_BASS_VOLUME			: return "BASS_VOLUME";
		case SGTL_CC_EQ_SELECT				: return "EQ_SELECT";
		case SGTL_CC_EQ_BAND0				: return "EQ_BAND0";
		case SGTL_CC_EQ_BAND1				: return "EQ_BAND1";
		case SGTL_CC_EQ_BAND2				: return "EQ_BAND2";
		case SGTL_CC_EQ_BAND3				: return "EQ_BAND3";
		case SGTL_CC_EQ_BAND4				: return "EQ_BAND4";
	}
	return "UNKNOWN_SGTL_CC";
}





// end of sgtl5000.cpp


