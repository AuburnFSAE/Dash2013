#ifndef SerialMssgBuffer_H
#define SerialMssgBuffer_H
	#include <Arduino.h>
	typedef double				dflt;
	typedef float				sflt;

	typedef signed char 		si8;
	typedef signed int 			si16;
	typedef signed long 		si32;

	typedef unsigned char 		ui8;
	typedef unsigned int 		ui16;
	typedef unsigned long 		ui32;


	class SerialMssgBuffer{
		union Pcast{
			ui8*  p8;
			ui16* p16;
		};

	private:
		ui8 mssgPin;

		//Pcast cast;

		ui32 headerMask;
		ui32 headerCount;
		ui32 sampleByteCount;


		ui32 numChannels;
		ui32 header;
		ui32 bitQueue;

		ui8* data;
		ui16 dataIter;
		ui16 buffLength;

		SerialMssgBuffer();

	public:	

		SerialMssgBuffer(ui8 mssgPin, ui8 n_numChannels);

		void InitMssgBuff();

		void Read(void);

		si8 NumChannels();

		si16 GetChannel(si16 channel);
	};
#endif
