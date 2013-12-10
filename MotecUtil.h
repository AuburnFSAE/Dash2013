#ifndef MOTEC_UTIL
#define MOTEC_UTIL

#include "Motec_Interp.h"
#include <Arduino.h>
#include <SoftwareSerial.h>


class RpmLedState{
	public:
	static const ui8 Null = 0;
	static const ui8 Enable = 1 << 0;
	static const ui8 On = 1 << 1;
};

class RpmLedHandler{

private:
	si16 ledCount;
	si16* ledPins;
	si16 quanta;
	si16 maxRatio;
	si16 shiftRatio;
	si32 maxRpm;
	si32 minRpm;
	

public:
	ui8 State;


public:
	RpmLedHandler();
	RpmLedHandler(const si16* ledPins, si16 numLeds);

	~RpmLedHandler();

	void Init();

/** Turns on the ratio of leds if enabled
*	If the enabled flag of the State property is set, the ratio of the index range of led pins will turned on based on
*	onRatio, which  must be in the range of [0..1000] where 1000 is eqaul to 1.000. If there are 
*	10 leds and onRatio is 400 then 4 leds will be turned on, corresponding to 40% of the index range.
*	If there are only 9 leds and onRatio is 400 then only 3 leds will be on since 40% of 9 is 3.6
*/
	void SetLedsRatio( si16 onRation );

	void SetRpmBounds(si16 min, si16 max);

	void SetLedsRpm( si16 rpm );

	void AllLedsOn();

	void AllLedsOff();

	//void Blink();

	void Fill(si16 c);

	si16 operator()(si16 pinIndex);

	si16 Count();
};

#define COMMAND 0xFE
#define CLEAR   0x01
#define LINE0   0x80
#define LINE1   0xC0
#define DisplayON 0x0C
#define DisplayOFF 0x08

void clearLcd(SoftwareSerial& sws);

void setLine0(SoftwareSerial& sws);

void setLine1(SoftwareSerial& sws);

void blink(SoftwareSerial& sws);

void backlightOn(SoftwareSerial& sws);
void backlightOff(SoftwareSerial& sws);
#endif
