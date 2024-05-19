//----------------------------------------------
// teMIDI.h
//----------------------------------------------
// Common MIDI definitions for TE3, including
// used MIDI message "types" and the important
// msgUnion class.


#pragma once

#include <Arduino.h>

//--------------------------------------------
// Standard MIDI types as used in TE3/Looper
//--------------------------------------------

#define MIDI_TYPE_NOTE_OFF		0x08
#define MIDI_TYPE_NOTE_ON		0x09
#define MIDI_TYPE_CC			0x0B
#define MIDI_TYPE_PGM_CHG		0x0C


//----------------------
// msgUnion
//----------------------

#define MIDI_CABLE_NUM_MASK    	0xf0
	// mask for either b[0] or uint32_t

#define MIDI_OUTPUT_B2_FLAG  	0x80
    // set into the otherwise unused high order bit of the B2 byte


class msgUnion
	// An abstraction of a USB or Serial MIDI message.
	//
	//		A USB/Serial MIDI nmessage is essantially
	// 		a 3 byte 'standard MIDI 1.0' message
	//		prefixed with a byte that has the 'cable'
	//		number in the high nibble, and a redundant
	//		copy of the message 'type' in the low nibble.
	//
	// The uint8_t byte array is in the expected order.
	//
	//		[0] = 0xNT		N=cable number 0..15, T=redundant message type
	//		[1] = 0xTC		T=message type 0x0..0xF, C=1 based channel number 1-16
	//		[2] = 0xP1		P1 = 0x00..0x7f (0..127) 'parameter 1'
	//		[3] = 0xP2		P2 = 0x00..0x7f (0..127) 'parameter 2'
	//
	// The uint32_t is big endian, so when viewed as 32 bits, it
	// 		looks like this:  0x P2 P1 TC NT
	//
	// In standard MIDI the high order bit of P2 is generally unused.
	// 		This API allows it to be used (in memory) to store additional
	//		(routing information) where a message from an input cable
	//		can be marked for output, and a table can determine the
	//		output mapping (i.e. new cable and/or channel number) for
	//		the message.
	//
	// Note that we don't actually use or need the redundant type
	//		stored in b[0], and *could* use that nibble, for example,
	//		in the Serial MIDI protocol, for other stuff, like to pass
	//		a cable number excluding 0xA, 0xD, and 0x8 over our purpose
	//		built Serial MIDI protocol.
{
    public:

		msgUnion(uint32_t msg32)  { i = msg32; }

        msgUnion(uint8_t cable, uint32_t msg32)
        {
            i = (msg32 & ~MIDI_CABLE_NUM_MASK) | (cable << 4);
        }
        msgUnion(uint8_t cable, uint8_t type, uint8_t channel, uint8_t p1, uint8_t p2)
        {
            b[0] = type | (cable << 4);
            b[1] = (type<<4) | (channel - 1);
            b[2] = p1;
            b[3] = p2;
        }
		void setCable(uint8_t cable)
		{
            i = (i & ~MIDI_CABLE_NUM_MASK) | (cable << 4);
		}
        void setOutput()
        {
            b[2] |= MIDI_OUTPUT_B2_FLAG;
        }

		uint32_t forOutput()		const { return i & ~(MIDI_OUTPUT_B2_FLAG<<24); }
			// returns the uint32_t with the output flag bit removed
			// not needed if client does not use the output flag,
			// they can just send msgUnion.i

        inline uint8_t cable()		const { return (i & MIDI_CABLE_NUM_MASK) >> 4; }
        inline uint8_t type()   	const { return b[1] >> 4; }
        inline uint8_t channel()	const { return (b[1] & 0xf) + 1; }
        inline uint8_t param1() 	const { return b[2] & ~MIDI_OUTPUT_B2_FLAG; }
        inline uint8_t param2() 	const { return b[3]; }

        bool isOutput()				const { return b[2] & MIDI_OUTPUT_B2_FLAG; }
		bool isActiveSense()		const { return (i & 0xff0f) == 0xfe0f; }

	//------------------------------
	// public data
	//------------------------------

    union {
        uint32_t i;
        uint8_t b[4];
    };

};



//--------------------------------------------
// SERIAL MIDI NOTES
//--------------------------------------------
// Not to be confused with "Standard MIDI" as used in Arduino
// libraries, which is also a serial protocol.  This protocol
// is specific to my applications and uses standard Arduino
// (teensy and ESP32) hardware Serial ports.
//
// My Serial MIDI comes in two flavors: Pure or Embedded
// A "Pure" Serial MIDI message may carry a cable number
// in the high nibble of the 0th byte.
//
// An "Embedded" Serial MIDI message may be embedded in a
// serial stream that otherwise contains normal text
// (i.e. debugging output) and MUST have a high nibble
// of zero.  This allows Serial MIDI messages to be
// disambiguated (pulled out) of a text stream because
// we only use a limited subset of low nibbles (midi
// "types") that SPECIFICALLY DO NOT INCLUDE the common
// serial text bytes 0x0A, 0x0D, and 0x09 (carriage return,
// linefeed), and 0x09 (tab).
//
// Therefore the receiver can look at each incoming byte,
// any any time it sees 0x0?, where ? is not A,D, or 9,
// the receiver can "gobble" up the 4 byte Serial MIDI
// message from the stream while continuing to buffer
// and/or process the surrounding "normal" text.
//
// In practice I have found that the Serial port implmentations
// on the teensy and ESP32 receive the whole 4 byte serial MIDI
// message in one interrupt, and the 4 bytes bits are NEVER
// interspersed with other characters.




// end of teMIDI.h