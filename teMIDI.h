//----------------------------------------------
// teMIDI.h
//----------------------------------------------
// Common MIDI definitions for TE3, including
// MIDI_TYPE_CC and the  and the important
// msgUnion class.

#pragma once

#include <Arduino.h>

//--------------------------------------------
// Standard MIDI types as used in TE3/Looper
//--------------------------------------------

#define MIDI_TYPE_NOTE_OFF		0x08
#define MIDI_TYPE_NOTE_ON		0x09
#define MIDI_TYPE_VELOCITY		0x0A
#define MIDI_TYPE_PGM_CHG		0x0C
#define MIDI_TYPE_AFTERTOUCH	0x0D
#define MIDI_TYPE_PITCHBEND		0x0E



#define MIDI_TYPE_CC			0x0B
	// The only one used by TE3_audio at this time

//----------------------
// msgUnion
//----------------------

#define MIDI_PORT_NUM_MASK    	0xf0

#define MIDI_OUTPUT_B2_FLAG  	0x80
    // set into the high order bit of the B2 byte


class msgUnion
    // USB raw input ports map to mine, but HOST0,1
    // are 4,5 in my scheme and SERIAL0 is 6,  The OUTPUT
    // state is weirdly stored in the high order bit
    // of the 3rd byte, b[2], param1.
{
    public:

		msgUnion(uint32_t msg32)  { i = msg32; }

        msgUnion(uint8_t port, uint32_t msg32)
        {
            i = (msg32 & ~MIDI_PORT_NUM_MASK) | port;
        }
        msgUnion(uint8_t port, uint8_t type, uint8_t channel, uint8_t p1, uint8_t p2)
        {
            b[0] = type | port;
            b[1] = (type<<4) | (channel - 1);
            b[2] = p1;
            b[3] = p2;
        }
		void setPort(uint8_t port)
		{
            i = (i & ~MIDI_PORT_NUM_MASK) | port;
		}
        void setOutput()
        {
            b[2] |= MIDI_OUTPUT_B2_FLAG;
        }

        inline uint8_t port()		const { return i & MIDI_PORT_NUM_MASK; }
		inline int     portEnum()	const { return port() >> 4; }
        inline bool    isOutput()	const { return b[2] & MIDI_OUTPUT_B2_FLAG; }
		
        inline uint8_t type()   	const { return i & 0x0f; }
        inline uint8_t channel()	const { return (b[1] & 0xf) + 1; }
        inline uint8_t param1() 	const { return b[2] & ~MIDI_OUTPUT_B2_FLAG; }
        inline uint8_t param2() 	const { return b[3]; }

        bool isActiveSense()		const { return (i & 0xff0f) == 0xfe0f; }

    union {
        uint32_t i;
        uint8_t b[4];
    };
};





// end of teMIDI.h