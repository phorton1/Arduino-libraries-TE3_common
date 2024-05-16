//-------------------------------------------------------------------------
// SGTL5000.h
//-------------------------------------------------------------------------
// Audio Library for Teensy 3.X
// Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
// please see LICENSE.TXT.
//
// This API has been reworked for clarity and control by MIDI.
// It is envisioned that client code will have some way of persistently
// storing the SGTL5000 "configuration" and will initialize all gains,
// senstivities, filters, and DSP blocks for the given application.

#pragma once

#include <AudioStream.h>
#include "AudioControl.h"

#define SGTL5000_I2C_ADDR_CS_NORMAL		0x0A  // CTRL_ADR0_CS pin low (normal configuration)
#define SGTL5000_I2C_ADDR_CS_ALT		0x2A  // CTRL_ADR0_CS  pin high


#define HEADPHONE_NORMAL	0	// normal mode
#define HEADPHONE_LINEIN	1	// bypass mode
	// param to setHeadphoneSelect()

#define DAC_VOLUME_RAMP_NORMAL  	0
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
#define SURROUND_STERO				2
	// param to setSurroundEnable()


#define EQ_FLAT 					0
#define EQ_PARAM_EQ 				1
#define EQ_TONE_BASS_AND_TREBLE		2
#define EQ_GRAPHIC_EQ_5CH			3
	// param to setEqSelect()

#define EQ_BASS_BAND0				0 	// 115 Hz
#define EQ_BAND1					1 	// 330 Hz
#define EQ_BAND2					2 	// 990 Hz
#define EQ_BAND3					3 	// 3000 Hz
#define EQ_TREBLE_BAND4				4	// 9900 Hz
	// param to setEqBand

#define FILTER_LOPASS 				0
#define FILTER_HIPASS 				1
#define FILTER_BANDPASS 			2
#define FILTER_NOTCH 				3
#define FILTER_PARAEQ 				4
#define FILTER_LOSHELF 				5
#define FILTER_HISHELF 				6
	// params to PEQ method

// CC Numbers

#define SGTL_CC_SET_DEFAULT_GAINS		30		// Write Only
#define SGTL_CC_INPUT_SELECT			31
#define SGTL_CC_MIC_GAIN_				32
#define SGTL_CC_LINEIN_LEVEL			33		// Write Only
#define SGTL_CC_LINEIN_LEVEL_LEFT		30
#define SGTL_CC_LINEIN_LEVEL_RIGHT		30
#define SGTL_CC_DAC_VOLUME				30		// Write Only 
#define SGTL_CC_DAC_VOLUME_LEFT			30
#define SGTL_CC_DAC_VOLUME_LEFT			30
#define SGTL_CC_DAC_VOLUME_RAMP			30
#define SGTL_CC_LINEOUT_LEVEL			33		// Write Only
#define SGTL_CC_LINEOUT_LEVEL_LEFT		30
#define SGTL_CC_LINEOUT_LEVEL_RIGHT		30
#define SGTL_CC_HP_SELECT				30
#define SGTL_CC_HP_VOLUME				30		// Write Only
#define SGTL_CC_HP_VOLUME_LEFT			30
#define SGTL_CC_HP_VOLUME_RIGHT			30
#define SGTL_CC_MUTE_HP					30
#define SGTL_CC_MUTE_LINEOUT			30
#define SGTL_CC_ADC_HIGH_PASS			30



#define MAX_SGTL500_BYTE_REG			0x13B



class SGTL5000 : public AudioControl
	// Client may call setDefaultGains() for a quick seetup of reasonable initial values.
	// Otherwise, client may call the the methods associated with the [bracketed] blocks.
	//
	//				 (bypass)
	//					+----------------------------------------------------------------------------------+
	//                  |                                                                                  |--> [HP_VOLUME] --> HP_OUT
	//   LINE_IN -------+--------+                                                                         |
	//                           |---> [LINEIN_LEVEL] --> ADC --> [SWITCH] --> [DAC_VOLUME] --> DAC ---+---+
	//   MIC ---> [MIC_GAIN] ----+      (ANALOG_GAIN)               |  ^                               |
	//												 			    v  |							   +------> [LINEOUT_VOL] --> LINE_OUT
	//                                                           [DSP BLOCK]
{
public:

	SGTL5000(void) : m_i2c_addr(SGTL5000_I2C_ADDR_CS_NORMAL)  {}
	void setAltAddress()  { m_i2c_addr = SGTL5000_I2C_ADDR_CS_ALT; }

	bool enable(void) override;
		// enable with the SGTL5000 as the master
	bool enable(const unsigned extMCLK, const uint32_t pllFreq = (4096.0l * AUDIO_SAMPLE_RATE_EXACT) );
		// enable setting the teensy as the Master with given glock settings
		// In either case, enable() sets the SGTL5000 to work well with the
		// teensy voltage levels.  As such it also sets various defaults
		// that may be modified by the API, including:
		// 		lineOutLevel(2) = "approx 1.3v p-p"  on my scale

	void loop();
		// after enable(), loop() is necessary if using the
		// TONE(2) or GEQ(3) eq setters, as the documentation
		// says that to avoid clicks these registers must not
		// be arbitrarily changed, but rather, only ramped up
		// in 0.5db (increments of 2 in the uint8_t values).
		
	// unimplmented orthogonal base class control API

	bool disable(void) override 		{ return false; }
	bool inputLevel(float n) override 	{ return false; }
	bool volume(float n) override 		{ return false; }
	bool inputSelect(int n) override 	{ return false; };

	// additional sensible API
	// these methods return false if they fail to write to the SGTL5000
	// there are separate accessors for "getting" register values.

	bool setDefaultGains();
		// Unmutes everything, turns off all DSP blocks and sets all
		// senstivities and gains to reasonable levels so that you are
		// likely to hear something for testing.
		//
		// micGain(3) 			= 40db (maximum)
		// lineInLevel(5) 		= 7.5db (1.33 Volts p-p measured)
		// headphoneVolume(90) 	= -3.5db

	//----------------------------------------------------
	// control API from left to right
	//----------------------------------------------------
	// These will be represented by incrementing Midi CC numbers,
	// and all will take uint8_t parameters in the range 0..127.

	bool setInput(uint8_t val);
		// Uses teeensy audio system constants.
		//		0 = AUDIO_INPUT_LINEIN
		//		1 = AUDIO_INPUT_MIC
		// Does not set or change any gains or other characteristics

	bool setMicGain(uint8_t val);	// 0..3
		// 0 = 0db
		// 1 = 20db
		// 2 = 30db
		// 3 = 40db

	bool setLineInLevel(uint8_t left, uint8_t right);	// 0..15
	bool setLineInLevel(uint8_t n)	 { return setLineInLevel(n, n); }
		// Sets the ANALOG_GAIN independent of the MIC_GAIN.
		// in 1.5db steps from 0 to 22.5db. See implementation
		// for table of measured p-p voltages. Note the register
		// supports a 6db attenuation bit to make it go from
		// -6.0db to 16.5db if needed.

	bool setDacVolume(uint8_t val);	// 0..127 INVERTED!
	bool setDacVolume(uint8_t left, uint8_t right);
		// ATTENUATE (turn down) the digital signal before the DAC
		// in 0.5db steps, from 0 to -63db, or mute it entirely.
		//
		// 		0 = 0 db (no attentuation)
		// 		1 = -0.5 db
		// 		2 = -1.0 db
		//		...
		//		125 = -62.5db
		//		126 = -63db
		//		127 = mute
		//
		// Note that to accomodate the 0..127 midi CC value range, we do not
		// access the full SGTL5000 range of -90db available for this register.
		// IMO it is unlikely anyone will ever want to turn the DAC down by more than 63db.
		// This is set to default of 0 on chip reset.
	bool setDacVolumeRamp(uint8_t val);	// 0..2
		// control ramping of dac
		// 0 = normal, 1=linear, 2=disabled

	unsigned short setLineOutLevel(uint8_t val);		// 0..18 (see note)
	unsigned short setLineOutLevel(uint8_t left, uint8_t right);
		// Sets the LINE_OUT volume in 0.5db steps
		//		from some arbitrary starting point.
		// This register is complicated.
		// At some point Paul measured the device and created a useful scale.
		// Paul's original uint8_t API had wacky allowed values of 13..31,
		//  	and was, I believe, "backwards" where higher numbers
		//      result in less p-p output voltage with 13=3.3V p-p
		//		and 31=1.66V p-p
		// I use his scale, but, I believe, with the "correct"" sense
		//		where bigger numbers result in bigger output p-p voltages.
		// On my scale, 0=1.66v p-p and 18=3.16v p-p
		// On my scale enable() defaults to lineOutLevel(2) == 1.29v p-p
		// Mutes at volume 0, unMutes if > 0 and muted

	bool setHeadphoneSelect(uint8_t val);
		// The headphone amplifier can be connected to the DAC (HEADPHONE_NORMAL)
		// or "bypass mode" (HEADPHONE_LINEIN) that routes the LINE_IN directly
		// to the headphone amp.

	bool setHeadphoneVolume(uint8_t val);	// 0..127;
	bool setHeadphoneVolume(uint8_t left, uint8_t right);
		// Adjusts the HEADPHONE amplifier from -51.5 db to +12db
		// in 0.5db steps. headPhoneVolume(97) = 0db
		// Mutes at volume 0, unMutes if > 0 and muted
		// Does not change the LineOut levels.

	bool setMuteHeadphone(uint8_t mute);
	bool setMuteLineOut(uint8_t mute);
		// These single bit modifiers do what they say.

	bool setAdcHighPassFilter(uint8_t val);	// 0..2
		// oddball API.
		// 		0 = ADC_HIGH_PASS_ENABLE
		// 		1 = ADC_HIGH_PASS_FREEZE
		// 		2 = ADC_HIGH_PASS_DISABLE
		// Disabling the ADC High Pass filter may give better bass response,
		// but allow DC noise in. Freezing it support, but weird, as it is an
		// ongoing automatic process.



	//------------------------------------------
	// DAP Blocks
	//------------------------------------------
	// all this stuff is for the DAP block and gruesomely complicated
	//
	// switch -> MIX --> AVC --> SURROUND --> BASS_ENHANCE --> TONE_CONTROL --> switch
	//           +6db    +12b                 +6db             +12db
	//
	// The MIX is generally disabled in this implementation.

	bool setDapEnable(uint8_t val);
		// Disable the DAP, or enable it pre or post I2S
		// 		0 = DAP_DISABLE
		// 		1 = DAP_ENABLE_PRE before I2S_OUT
		// 		2 = DAP_ENABLE_POST after I2S_IN
		// "It is good practice to mute the outputs before enabling or disabling the Audio Processor,
		// to avoid clicks or thumps."



	// prh - API below here needs work
	//
	// Above everything scrunched into 0..127 midi uint8_t params.
	// With AVC and PEQ we run into problems where there are full 16 bit registers,
	// 		and the old API uses floats extensively.
	// I may not be able to sensibly represent all blocks as midi parameterized devices.
	// I will probably need to figure out some common use cases that CAN be represented
	// 		by 7 bit midi values and implement those in terms of the below API's.

	bool setAutoVolumeEnable(uint8_t enable);
		// limiter/compressor stage
	bool setAutoVolumeControl(uint8_t maxGain, uint8_t lbiResponse, uint8_t hardLimit, float threshold, float attack, float decay);
		// Configures the auto volume control, which is implemented as a
		// compressor/expander or hard limiter. maxGain is the maximum gain that
		// can be applied for expanding, and can take one of three values:
		// 0 (0dB), 1 (6.0dB) and 2 (12dB). Values greater than 2 are treated as 2.
		// response controls the integration time for the compressor and can take four values:
		// 0 (0ms), 1 (25ms), 2 (50ms) or 3 (100ms).
		// Larger values average the volume over a longer time, allowing short-term peaks through.
		//
		// If hardLimit is 0, a 'soft knee' compressor is used to progressively compress louder
		// values which are near to or above the threashold (the louder they are,
		// the greater the compression). If it is 1, a hard compressor is used
		// (all values above the threashold are the same loudness). The threashold is
		// specified as a float in the range 0dBFS to -96dBFS, where -18dBFS is a typical value.
		// attack is a float controlling the rate of decrease in gain when the signal is over threashold,
		// in dB/s. decay controls how fast gain is restored once the level drops below threashold, again in dB/s.
		// It is typically set to a longer value than attack.


	// SURROUND
	bool setSurroundEnable(uint8_t enable);	// 0..2
		// 0 = disabled
		// 1 = mono
		// 2 = stereo
	bool setSurroundWidth(uint8_t width);	// 0..7
		// 0 to 7 (widest)


	// BASS_ENHANCE
	bool setEnableBassEnhance(uint8_t enable);				// 0..1
	bool setEnableBassEnhanceCutoff(uint8_t enable);		// 0..1
	bool setBassEnhanceCutoff(uint8_t freq);				// 0..6
		// 0 =  80Hz
		// 1 = 100Hz
		// 2 = 125Hz
		// 3 = 150Hz
		// 4 = 175Hz
		// 5 = 200Hz
		// 6 = 225Hz
	bool setBassEnhanceBoost(uint8_t val);					// 0..0x7f
		// sets amount of harmonics boost.
		// default = 0x60
	bool setBassEnhanceVolume(uint8_t val);					// 0..0x3f
		// set level up to +6dB
		// default = 0x3A (0db?)


	// TONE_CONTROL
	// PEQ(1) not really currently supported
	// For the other two, TONE(2) and GEQ(3) in order to avoid pops,
	// the changes are automated to occur in no more than 0.5db steps.
	// Hence, the SGTL has a loop() method that must be called to
	// handle this automation.

	bool setEqSelect(uint8_t n);	// 0..3
		// Selects the type of frequency control, 
		// FLAT_FREQUENCY (0) Equalizers and tone controls disabled, flat frequency response.
		// PARAMETRIC_EQUALIZER (1) Enables the 7-band parametric equalizer
			void eqFilter(uint8_t filterNum, int *filterParameters);
				// Configurs the parametric equalizer. The number of filters (1 to 7) is
				// specified along with a pointer to an array of filter coefficients.
				// The parametric equalizer is implemented using 7 cascaded, second order
				// bi-quad filters whose frequencies, gain, and Q may be freely configured,
				// but each filter can only be specified as a set of filter coefficients.
			unsigned short eqFilterCount(uint8_t n);
				// Enables zero or more of the already configured parametric filters.
			void calcBiquad(uint8_t filtertype, float fC, float dB_Gain, float Q, uint32_t quantization_unit, uint32_t fS, int *coef);
				// Helper method to build filter parameters
		// TONE_CONTROLS (2) Enables bass and treble tone controls
		// GRAPHIC_EQUALIZER (3) Enables the five-band graphic equalizer
			bool setEqBand(uint8_t band_num, uint8_t val);	// 0..95 (0x5F)
				// for ToneControl use 0 and 4
				// for GE use 0..4
				// Sets EQ band gain from -11.75db to +12db in 0.25db steps.
				// reset default is 47 (0x2f) = 0 db


	// Serial MIDI implementation

	bool dispatchCC(uint8_t cc, uint8_t val);
	uint8_t getCC(uint8_t cc);
		// uses #SGTL_CC_XXXX constants and 0..127 uint8_t parameters
		// all setters are defined
		// some getters are not defined and will return 0
	

protected:

	uint8_t m_i2c_addr;

	bool m_hp_muted;
	bool m_lineout_muted;
	uint16_t m_ana_ctrl;

	// automation variables
	// note that the user must handle

	uint8_t m_dap_enable;
	uint8_t m_eq_select;
	uint8_t m_band_value[5];
	uint8_t m_band_target[5];
	volatile uint8_t m_in_automation;
		// bitwise bands that need automation
		// with 1<<6 as a busy flag

	bool write(unsigned short reg_num, unsigned short val);
		// returns 0 on failure, 1 on success
	unsigned short read(unsigned short reg_num);
		// note that API cannot differentiate between
		// a read failure, and read of a register containing zero
	bool modify(unsigned short reg_num, unsigned short val, unsigned short mask);
		// returns 1 if the write() succeeds, or zero if it fails.
		// can fail to function properly and still return 1 due to
		// API limitation of read()

	// utilities

	void handleEqAutomation(uint8_t band_num);

};


//-----------------------------------------
// CC message dispatcher
//-----------------------------------------



