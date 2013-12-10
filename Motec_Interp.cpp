#include "Motec_Interp.h"


SerialMssgBuffer::SerialMssgBuffer(){}

SerialMssgBuffer::SerialMssgBuffer(ui8 n_mssgPin, ui8 n_numChannels){

    mssgPin = n_mssgPin;
    numChannels = n_numChannels;
    
    InitMssgBuff();  

    pinMode(mssgPin, INPUT);
return;}

void SerialMssgBuffer::InitMssgBuff(){
 
    ui32 stop = 0x01;
    
    header = (0x82);
    header |= stop << 8;

    header |= (ui32)0x81 << 10 ;
    header |= stop << 18;

    header |= (ui32)0x80 << 20;
    header |= stop<<28;

    headerMask = 0x3FFFFFFF;
   
    buffLength = 1 + 2*numChannels;

    headerCount = 0;
    bitQueue = 0;

    sampleByteCount = 10;

    data = (ui8*)malloc( buffLength );
    data[buffLength] = 0;
    dataIter = 0;

    for(si16 cnt = 0 ; cnt < buffLength ; data[cnt++] = 0 );
}

void SerialMssgBuffer::Read(){
    

    bitQueue = bitQueue >> 1;
    bitQueue |= ((ui32)digitalRead( mssgPin )) << 31;

    sampleByteCount--;
    
    if( ( bitQueue & headerMask ) == header ){
        sampleByteCount = 30;
        headerMask = 0;
    }

    if( sampleByteCount == 0 ){
        sampleByteCount = 10;
        
        if(headerMask == 0){ data[dataIter++] = (ui8) bitQueue;  }
        
        if( dataIter == buffLength ){ headerMask = 0x3FFFFFFF; dataIter = 0;}
    }

return;}

si8 SerialMssgBuffer::NumChannels(){
return *( (si8*)(data) );}

si16 SerialMssgBuffer::GetChannel(si16 channel){
    
    si16 channelVal = 0;
    si16 ind = 1 + 2 * (channel-1);
    channelVal = ( ((si16)data[ind]) << 8 ) | data[ind+1];
return channelVal;}

