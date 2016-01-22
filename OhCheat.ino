////////////////////////////////////////////////////////////////////////////////   
//       ____  __       ________               __     __
//      / __ \/ /_     / ____/ /_  ___  ____ _/ /_   / /
//     / / / / __ \   / /   / __ \/ _ \/ __ `/ __/  / / 
//    / /_/ / / / /  / /___/ / / /  __/ /_/ / /_   /_/  
//    \____/_/ /_/   \____/_/ /_/\___/\__,_/\__/  (_)   
//                                                                                  
// Keystroke generator designed for introducing cheat codes in GTA games 
//
// Author: Daniel Jose Viana, danjovic@hotmail.com
//
// Version 0.6 - January, 2016 - Basic Release
// Version 0.7 - January, 2016 - Bug correction to work with GTA


////////////////////////////////////////////////////////////////////////////////
//    _    _ _                 _        
//   | |  (_) |__ _ _ __ _ _ _(_)___ ___
//   | |__| | '_ \ '_/ _` | '_| / -_|_-<
//   |____|_|_.__/_| \__,_|_| |_\___/__/
//                                      
#include "DigiKeyboard.h"


////////////////////////////////////////////////////////////////////////////////
//   __   __        _      _    _        
//   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
//    \ V / _` | '_| / _` | '_ \ / -_|_-<
//     \_/\__,_|_| |_\__,_|_.__/_\___/__/
//                                       
uint8_t ticks=0;
uint8_t lastkey=0;
uint8_t temp,key;
uchar   reportBuffer[2];


////////////////////////////////////////////////////////////////////////////////
//    ___             _   _
//   | __|  _ _ _  __| |_(_)___ _ _  ___
//   | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |_| \_,_|_||_\__|\__|_\___/_||_/__/
//

// Send a key press to the host but don't send a key up
void sendKeyStroke_nostop(byte keyStroke, byte modifiers) {
  while (!usbInterruptIsReady()) {
    usbPoll();
    _delay_ms(5);
  }
  memset(reportBuffer, 0, sizeof(reportBuffer));
  reportBuffer[0] = modifiers;
  reportBuffer[1] = keyStroke;
  usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
  while (!usbInterruptIsReady()) {
    usbPoll();
    _delay_ms(5);
  }
}

// Print a string to the host, and add a key up after a delay 
void DigiKeyboard_print ( char *str ) {
  uint8_t data;
  while (*str) {
    data = pgm_read_byte_near(ascii_to_scan_code_table + (*str++ - 8));
    sendKeyStroke_nostop(data & 0b01111111, data >> 7 ? MOD_SHIFT_RIGHT : 0);
    DigiKeyboard.delay(50);
    sendKeyStroke_nostop(0,0);
  }
}


////////////////////////////////////////////////////////////////////////////////
//    ___ ___ _____ _   _ ___ 
//   / __| __|_   _| | | | _ \
//   \__ \ _|  | | | |_| |  _/
//   |___/___| |_|  \___/|_|  
//                            
void setup() {
  
  pinMode(1, OUTPUT);
  digitalWrite (1,0); // pull down pin B1 to simulate ground
  
  digitalWrite(0,1); // set pullup
  pinMode(0, INPUT);

  digitalWrite(2,1); // set pullup
  pinMode(2, INPUT);

  digitalWrite(5,1); // set pullup
  pinMode(5, INPUT);
}


////////////////////////////////////////////////////////////////////////////////
//    __  __      _        _                  
//   |  \/  |__ _(_)_ _   | |   ___  ___ _ __ 
//   | |\/| / _` | | ' \  | |__/ _ \/ _ \ '_ \
//   |_|  |_\__,_|_|_||_| |____\___/\___/ .__/
//                                      |_|   
void loop() {
  
  // read key. We can have up to seven keys
  key=0;
  if (digitalRead(0)==0) key |=1;  // bit 0 
  if (digitalRead(2)==0) key |=2;  // bit 1
  if (digitalRead(5)==0) key |=4;  // bit 2


  // then debounce  
  if (key) {                // on press
    if (ticks<255) ticks++; // count up with saturation
    lastkey=key;
    } else {                // on release 
      temp=ticks;           // check if key stayed press over 100ms
      ticks=0;
      if (temp >10 ) {      // 10 * 10ms = 100ms debounce time
          DigiKeyboard.sendKeyStroke(0);
          switch (lastkey) {                                    // Cheats for GTA San Andreas
            case 1:  DigiKeyboard_print("hesoyam\0");   break;    // key 1  Restore Health and Give Money
            case 2:  DigiKeyboard_print("asnaeb\0");    break;    // key 2  Clear Wanted Level
            case 3:  DigiKeyboard_print("rocketman\0"); break;    // key 3  Have Jetpack
            case 4:  DigiKeyboard_print("ohdude\0");    break;    // key 4  Spawn Helicopter
            case 5:  DigiKeyboard_print("LXGIWYL\0");   break;    // key 5  Weapon Set 1
            case 6:  DigiKeyboard_print("KJKSZPJ\0");   break;    // key 6  Weapon Set 2
            case 7:  DigiKeyboard_print("UZUMYMW\0");   break;    // key 7  Weapon Set 3
          } // case
      } // key release after 100ms     
    } // key release
  // cycle each 10ms
  DigiKeyboard.delay(10);
}

////////////////////////////////////////////////////////////////////////////////
// Ascii Titles by Patrick Gillespie http://patorjk.com/software/taag
 

