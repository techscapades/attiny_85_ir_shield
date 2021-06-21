/*
TrinketHidCombo is a part of Adafruit's Trinket/Gemma USB HID Arduino Library
TrinketHidCombo allows the user to implement a simple USB keyboard inside the Arduino environment
Please see TrinketHidCombo.h for usage documentation, an example is also included
TrinketHidCombo uses V-USB (http://www.obdev.at/products/vusb/) to implement USB with a bitbang technique. See the folder "usbdrv" for V-USB files.
Adafruit's Trinket and Gemma can do almost anything that V-USB can do, including USB mouse, keyboards, joysticks, gamepads, MIDI devices, and much more (including custom devices that do not fit any existing category).

Copyright (c) 2013 Adafruit Industries
All rights reserved.

TrinketHidCombo is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

TrinketHidCombo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with TrinketHidCombo. If not, see
<http://www.gnu.org/licenses/>.

Code remixed from https://www.instructables.com/Digispark-IR-Receiver-Keyboard/ 

*/

#include <avr/delay.h>
#include "TrinketHidCombo.h"
#define button_pin 1

volatile uint8_t m = 0, tcnt = 0, startflag = 0;
uint32_t irdata = 0, keydata = 0 ;
bool pressed = false;
bool complete = false;

// **** TSOP is connected to port PB2 **** //////
#define REPEAT_DELAY 220

//Set to 0 after finding your codes, I recommend opening notepad 
#define DEBUG 1

//Specify your remote codes here: {point your remote at the IR reciever on the board to find codes}
#define REMOTE_OK 0
#define REMOTE_LEFT 0x4BF8004A
#define REMOTE_RIGHT 0x4BF800CA
#define REMOTE_DOWN 0x4BF8008A
#define REMOTE_UP 0x4BF8000A
#define REMOTE_MOUSE_SWITCH 0x4B98E11E
#define REMOTE_POWER 0x4B9821DE
#define REMOTE_VOL_UP 0x4B9841BE
#define REMOTE_VOL_DOWN 0x4B98C13E
#define REMOTE_RETURN 0x4BF80090
#define REMOTE_PLAYPAUSE 0x4BF800D8
#define REMOTE_NEXT 0x4BF800B8
#define REMOTE_PREV 0x4BF80078

void Action(uint32_t keycode) //switch statement to control what happens when IR remote is pressed
{
  switch (keycode) //more keycode commands @https://github.com/adafruit/Adafruit-Trinket-USB/blob/master/TrinketHidCombo/TrinketHidCombo.h
  {
    case REMOTE_POWER:
      TrinketHidCombo.pressSystemCtrlKey(SYSCTRLKEY_POWER); //alternatively you can make it un functions too
      break;
    case REMOTE_RETURN:
      TrinketHidCombo.pressKey(0, KEYCODE_BACKSPACE);
      break;
    case REMOTE_VOL_UP:
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
      break;
    case REMOTE_VOL_DOWN:
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
      break;
    case REMOTE_PREV:
      TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_PREV_TRACK);
      break;
    case REMOTE_NEXT:
      TrinketHidCombo.pressMultimediaKey(MMKEY_SCAN_NEXT_TRACK);
      break;
    case REMOTE_PLAYPAUSE:
      TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE);
      break;

    default:
      if (DEBUG)
        TrinketHidCombo.println(keydata, HEX);
      else
        return;
  }
}

void runPushButtonCmd() { //your custom command which will execute when push button is pressed
  TrinketHidCombo.pressKey(0, KEYCODE_TAB);
  TrinketHidCombo.pressKey(0, KEYCODE_ENTER);
  ms_delay(250); //added to simulate delay

  TrinketHidCombo.pressKey(0, 0); //add this inbetween keystrokes if keystrokes are being skipped
  TrinketHidCombo.pressKey(0, KEYCODE_TAB);
  TrinketHidCombo.pressKey(0, KEYCODE_TAB);
  ms_delay(290); //added to simulate delay

  TrinketHidCombo.pressKey(0, 0);
  TrinketHidCombo.pressKey(0, KEYCODE_TAB);
  TrinketHidCombo.pressKey(0, 0);

  ms_delay(270); //added to simulate delay
  TrinketHidCombo.pressKey(0, KEYCODE_ENTER);

  ms_delay(300); //needed for HID to stabilise
  TrinketHidCombo.print("hello world UwU"); //any string you want typed

  TrinketHidCombo.pressKey(0, 0); //to prevent the key from pressing repeatedly
  ms_delay(REPEAT_DELAY);
}


void setup() {
  //delay(30000);
  PORTB |= 1 << PB2; // a PB2 lift will not hurt. this is the IR data pin
  GIMSK |= 1 << INT0; //interrupt int0 enable
  MCUCR |= 1 << ISC00; //Any logical change on INT0 generates an interrupt request
  GTCCR |= 1 << PSR0; TCCR0A = 0;
  TCCR0B = (1 << CS02) | (1 << CS00); // divider /1024
  TIMSK = 1 << TOIE0; //interrupt Timer/Counter1 Overflow  enable
  TrinketHidCombo.begin(); // start the USB device engine and enumerate
  pinMode(button_pin, INPUT);
}

void loop() {

  if (digitalRead(button_pin) == HIGH) {
    runPushButtonCmd(); //function that executes when side button is pushed
  }

  if (complete) { // if a code has been received
    if (keydata != 0) //if a code is new
    {
      Action(keydata); //from your on remote
      pressed = true;
    }
    complete = false;
    ms_delay(REPEAT_DELAY);// to balance repeating/input delay of the remote
  }
  else if (pressed)
  {
    TrinketHidCombo.pressKey(0, 0);
    pressed = false;
  }
  else
  {
    _delay_ms(1);//restrain USB polling on empty cycles
    TrinketHidCombo.poll(); // check if USB needs anything done
  }
}

ISR (INT0_vect) {
  if (PINB & 1 << 2) { // If log1
    TCNT0 = 0;
  }
  else {
    tcnt = TCNT0; // If log0
    if (startflag) {
      if (30 > tcnt  && tcnt > 2) {
        if (tcnt > 15 && m < 32) {
          irdata |= (2147483648 >> m);
        }
        m++;
      }
    }
    else  startflag = 1;
  }
}

ISR (TIMER0_OVF_vect) {
  if (m)
    complete = true; m = 0; startflag = 0; keydata = irdata; irdata = 0; // if the index is not 0, then create an end flag
}

void ms_delay(uint16_t x) // USB polling delay function, needed to release USB device
{
  for (uint16_t m = 0; m < (x / 10); m++) {
    _delay_ms(10);
    TrinketHidCombo.poll();
  }
}
