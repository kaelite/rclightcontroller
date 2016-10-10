/*
  RC Light Controller

 Turns on and off stops lights depending on signal from receiver.


 The circuit:
 * Element attached to PIN
 * outer tail ligths (*.---.*) attached to pin 5 (PB0)
 * inner tail lights (.*---*.) attached to pin 6 (PB1)
 * RC Signal attached to pin 7 (PB2)
 created 10 Oct 2016
 by kaelite (kaelite@msn.com)
*/

#include <avr/io.h>
#include <util/delay.h>

#define PIN_TAILLIGHTS_INS PB1
#define PIN_TAILLIGHTS_OUTS PB0
#define PIN_RCSINGAL  PB2
#define VALUE_THROTTLE_CENTER 1470
#define VALUE_LIGHT_MODE_BRAKING 255
#define VALUE_LIGHT_MODE_BACKUP  30
#define DELTA 100
#define BACKMODE_BKWD 0
#define BACKMODE_BRAKING 1
#define RUNMODE_FWD 1
#define RUNMODE_BKWD -1

unsigned long throttle = VALUE_THROTTLE_CENTER; //Throttle value from receiver
unsigned short backmode = BACKMODE_BKWD; //Mode for ESC with braking system
signed short lastrunmode = 0;
 
int main( void )
{
//Setup
  DDRB |= (1<<5);       // устанавливаем вывод PB5 как выход

//Main cycle
  while(1){
    throttle = pulseIn(PIN_RCSINGAL, HIGH, 1)-VALUE_THROTTLE_CENTER;
      
    if( abs(throttle) <= DELTA){
      //Stop/Stoping
      analogWrite(PIN_TAILLIGHTS_INS, VALUE_LIGHT_MODE_BRAKING);
      analogWrite(PIN_TAILLIGHTS_OUTS, VALUE_LIGHT_MODE_BRAKING);
      if ( lastrunmode == RUNMODE_BKWD ) {
        backmode = BACKMODE_BKWD;
      }
    }else{
      //Run/Running
      
      if(throttle > 0)
      {
        //Forward
        analogWrite(PIN_TAILLIGHTS_INS, VALUE_LIGHT_MODE_BACKUP);
        analogWrite(PIN_TAILLIGHTS_OUTS, VALUE_LIGHT_MODE_BACKUP);   
        backmode = BACKMODE_BRAKING;     

      }else{
        //Backward or Braking
        //Here to check if brake or backward or forward
        
        if (backmode == BACKMODE_BRAKING){
          //Braking..
          analogWrite(PIN_TAILLIGHTS_INS, VALUE_LIGHT_MODE_BRAKING);
          analogWrite(PIN_TAILLIGHTS_OUTS, VALUE_LIGHT_MODE_BRAKING);
        }else{
          //BKWD
          analogWrite(PIN_TAILLIGHTS_INS, VALUE_LIGHT_MODE_BACKUP);
          analogWrite(PIN_TAILLIGHTS_OUTS, VALUE_LIGHT_MODE_BACKUP);          
        }
      }
      lastrunmode = throttle > 0 ? RUNMODE_FWD : RUNMODE_BKWD;      
    }
  }
 
  return 0;
}
