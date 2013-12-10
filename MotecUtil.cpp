#include "MotecUtil.h"

RpmLedHandler::RpmLedHandler(){}

RpmLedHandler::~RpmLedHandler(){
	if(ledPins)
	free(ledPins);
}

void RpmLedHandler::Init(){
	ledCount = 0;
	ledPins = 0;
	quanta = 0;
	maxRatio = 0;
	maxRpm = 0;
	minRpm = 0;
	State = 0;
}

RpmLedHandler::RpmLedHandler(const si16* n_ledPins, si16 numLeds){
	Init();
	ledCount = numLeds;
	if( ledCount > 0 ){

		maxRatio = 1000;

		quanta = maxRatio/ledCount;

		shiftRatio = 9 *quanta;

		ledPins = (si16*)malloc(2*ledCount);

		for(int i = 0; i < ledCount; i++){
			ledPins[i] = n_ledPins[i];
			pinMode( ledPins[i], OUTPUT); 
		}

		State = RpmLedState::Enable;
	}
}




void RpmLedHandler::SetLedsRatio(si16 onRatio ){
	if( onRatio > 0 ){
	
	
		if( onRatio < quanta ){ 
			AllLedsOff();
		}
		else{
			if( onRatio > shiftRatio ){ 
				for(int i = 0; i < 1; i++ ){
					
					AllLedsOff(); 
					delay(40); 
					AllLedsOn();
					delay(40);
					AllLedsOff(); 
				}
			}else{
		
				si16 onIndex = (onRatio * ledCount)/maxRatio;

				for(si16 onItr=0; onItr < onIndex; digitalWrite(ledPins[onItr++], HIGH)  );

				for(si16 off=onIndex; off < ledCount; digitalWrite(ledPins[off++], LOW) );
			}
		}
	}
return;}

void RpmLedHandler::SetLedsRpm( si16 rpm ){

	si32 rpmRatio = (rpm < minRpm)? quanta/2 : (maxRatio * (rpm - minRpm ))/(maxRpm - minRpm);


	SetLedsRatio( rpmRatio );
return;}



void RpmLedHandler::Fill(si16 c){
	si32 delayUs = 50;

	do{
		digitalWrite(ledPins[0], HIGH);
		delay(delayUs);
		si16 tCount = ledCount;
		for( int i = 0; i < ledCount; i++){

			for(si16 onItr=1; onItr < tCount; onItr++  ){
				digitalWrite(ledPins[onItr-1], LOW);

				digitalWrite(ledPins[onItr], HIGH);
				delay(delayUs);
			}
			tCount--;
		}

		AllLedsOff();
	}while(--c>0);
return;}

void RpmLedHandler::AllLedsOn(){

	for(si16 onItr=0; onItr < ledCount; digitalWrite(ledPins[onItr++], HIGH)  );
return;}


void RpmLedHandler::AllLedsOff(){

	for(si16 off=0; off < ledCount; digitalWrite(ledPins[off++], LOW)  );


return;}

si16 RpmLedHandler::operator()(si16 pinIndex){
	si16 pin = -1;
	if( ledCount > 0 && -1< pinIndex && pinIndex < ledCount){ pin = ledPins[pinIndex]; }
return pin;}

si16 RpmLedHandler::Count(){ return ledCount; }

void RpmLedHandler::SetRpmBounds(si16 n_min, si16 n_max){
	minRpm = n_min;
	maxRpm = n_max;
return;}



void clearLcd(SoftwareSerial& sws){

	sws.write(COMMAND);
  	sws.write(CLEAR);
return;} 

void setLine0(SoftwareSerial& sws){

	sws.write(COMMAND);
  	sws.write(LINE0);
return;}

void setLine1(SoftwareSerial& sws){

	sws.write(COMMAND);
	sws.write(LINE1);
}

void backlightOn(SoftwareSerial& sws){  //turns on the backlight
    sws.write(0x7C);   //command flag for backlight stuff
    sws.write(157);    //light level.
}
void backlightOff(SoftwareSerial& sws){  //turns off the backlight
    sws.write(0x7C);   //command flag for backlight stuff
    sws.write(128);     //light level for off.
}
void blink( SoftwareSerial& sws){
	
	sws.write(COMMAND);
	sws.write(DisplayOFF);

	delay(300);

	sws.write(COMMAND);
	sws.write(DisplayON);

}
