#include <SoftwareSerial.h>

#include "Motec_Interp.h"
#include "MotecUtil.h"

//==========================
//  @author:Sherrod James
//  @date_created: aug-2012
//==========================


const si16 numRpmPins = 9;
const si16 rpmPins[numRpmPins] = {9, 11, 7, 6, 8, 10, 1, 5, 4};

RpmLedHandler rpmHnd(rpmPins, numRpmPins);

const si8 mssgPin = 2;
const si8 MotecChannelCount = 5;

SerialMssgBuffer moTecBuffer(mssgPin, MotecChannelCount);
SoftwareSerial lcd(0, 3);

si32 timer = 0;
si32 frameRefresh = 10;
bool tggl = 0;
si16 prv = 0;
si16 prtp = 0000;

void setup(){

  
  lcd.begin(9600);  
  clearLcd(lcd);
  lcd.print("WAR EAGLE RS0");
  rpmHnd.Fill(2);
  delay(500);
  

    
  rpmHnd.SetRpmBounds(6000, 13000);
  cli();//stop interrupts 

//set timer0 interrupt at 9.6kHz
  TCCR1A = 0;// set entire TCCR2A register to 0
  TCCR1B = 0;// same for TCCR2B
  TCNT1  = 0;//initialize counter value to 0

  OCR1A = 1667;// = (16*10^6) / (1*1024) - 1 (must be <65536)

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS12 and CS10 bits for 1 prescaler
  TCCR1B |=  (1 << CS10);  

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);


  sei();//allow interrupts

  delay(100);

  clearLcd(lcd);
}//end setup

ISR(TIMER1_COMPA_vect){
  moTecBuffer.Read();

}
si16 rpm =0;
si16 tp=0;
si16 et=0;
si16 lambda=0;
si16 bt=0;

si32 timerF = 0;
si16 pret = 0;
si16 highTempCnt = 0;
char str[17];

si16 lambdaP=0;
si16 prbt = 0;
void loop(){
  rpmHnd.SetLedsRpm(prv);
  
  

  if( millis()-timerF > 500 ){
      timerF = millis();
      clearLcd( lcd );

      if(pret > 110 ){
        for(si16 i=4; i<7; digitalWrite( rpmHnd(i++), HIGH));
          sprintf(str, "   High ET:%3i", pret);
          lcd.print(str);

          blink(lcd);
        for(si16 i=4; i<7; digitalWrite( rpmHnd(i++), LOW));  
      }
      else{
        sprintf(str, "TP:%4i  ET:%3i", prtp, pret);
        lcd.print(str);
        setLine1(lcd);

        sprintf(str, "LA:%04i  BV:%3i", lambdaP,  prbt);
        lcd.print(str);
        setLine0(lcd);
      }
  }    


    rpm = moTecBuffer.GetChannel(1);

    tp = moTecBuffer.GetChannel(2)/10;

    et = moTecBuffer.GetChannel(3)/10;

    lambda = moTecBuffer.GetChannel(4);

    bt = moTecBuffer.GetChannel(5)/100;


    if( 0 <= tp && tp < 110 ) prtp = tp; //else prtp = 0;

    if( 0 <= et && et < 120 ) pret = et; //else pret =0;

    if( 0 <= bt && bt < 15) prbt = bt; //else prbt = 0;

    if( lambda >= 0) lambdaP = lambda; else lambdaP = 0;

    if( (500 < rpm) && (rpm < 14000) && (abs(rpm-prv) < 4000) ){ 
      prv = rpm;
    }

    if( rpm == 0 ){
      prv = 0;
    };
    
}

