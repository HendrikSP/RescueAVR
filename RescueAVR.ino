// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil -*-
/*
  Title:        RescueAVR

  based on Jeff Keyzer's HVRescue_Shield and manekinen's Fusebit Doctor.

  It uses the hardware dsigned by manekinen but the software is completely
  new, using ideas und snippets from the Jeff Keyzer's sketch HVRescue_Shield.

  Currently, it can deal only with a limited number of AVR chips, but it is, of course,
  easily expandable (see the array below).
  
  Copyright 2013 by Bernhard Nebel and parts are copyrighted by Jeff Keyzer.
 
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#define VERSION  "0.9"

#include <avr/pgmspace.h>

// table with signature
// 2 bytes signatures (no 1E), number of fuses, 
// 3 bytes of default fuses, flag = handled by RescueAVR already
#define MCU_NUM 47
uint16_t mcu_types[MCU_NUM][7] PROGMEM =
  {
    { 0x91, 0x01, 1, 0xFF, 0x00, 0x00, 1 }, // at90s2313
    { 0x91, 0x05, 1, 0xFF, 0x00, 0x00, 1 }, // at90s2333
    { 0x91, 0x03, 1, 0xFF, 0x00, 0x00, 1 }, // at90s2343
    { 0x92, 0x03, 1, 0xFF, 0x00, 0x00, 1 }, // at90s4433
    { 0x92, 0x02, 1, 0xFF, 0x00, 0x00, 1 }, // at90s4434
    { 0x93, 0x01, 1, 0xFF, 0x00, 0x00, 1 }, // at90s8515
    { 0x93, 0x03, 1, 0xFF, 0x00, 0x00, 1 }, // at90s8535
    { 0x90, 0x04, 1, 0xFF, 0x00, 0x00, 1 }, // attiny11
    { 0x90, 0x05, 1, 0xFF, 0x00, 0x00, 1 }, // attiny12
    { 0x90, 0x07, 2, 0x6A, 0xFF, 0x00, 1 }, // attiny13
    { 0x90, 0x06, 1, 0xFF, 0x00, 0x00, 1 }, // attiny15
    { 0x91, 0x0A, 3, 0x64, 0xDF, 0xFF, 1 }, // attiny2313
    { 0x92, 0x0D, 3, 0x64, 0xDF, 0xFF, 1 }, // attiny4313
    { 0x91, 0x0B, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny24
    { 0x92, 0x07, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny44
    { 0x93, 0x0C, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny84
    { 0x91, 0x08, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny25
    { 0x92, 0x06, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny45
    { 0x93, 0x0B, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny85
    { 0x91, 0x09, 2, 0x41, 0xF7, 0x00, 1 }, // attiny26
    { 0x91, 0x0C, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny261
    { 0x92, 0x08, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny461
    { 0x93, 0x0D, 3, 0x62, 0xDF, 0xFF, 1 }, // attiny861
    { 0x93, 0x06, 2, 0xC1, 0xD9, 0x00, 1 }, // atmega8515
    { 0x93, 0x08, 2, 0xC1, 0xD9, 0x00, 1 }, // atmega8535
    { 0x93, 0x07, 2, 0xE1, 0xD9, 0x00, 1 }, // atmega8
    { 0x94, 0x03, 2, 0xE1, 0x99, 0x00, 1 }, // atmega16
    { 0x95, 0x02, 2, 0xE1, 0x99, 0x00, 1 }, // atmega32
    { 0x96, 0x02, 3, 0xC1, 0x99, 0xFF, 1 }, // atmega64
    { 0x97, 0x02, 3, 0xC1, 0x99, 0xFD, 1 }, // atmega128
    { 0x92, 0x05, 3, 0x62, 0xDF, 0xFF, 1 }, // atmega48
    { 0x92, 0x0A, 3, 0x62, 0xDF, 0xFF, 1 }, // atmega48p
    { 0x93, 0x0A, 3, 0x62, 0xDF, 0xFF, 1 }, // atmega88
    { 0x93, 0x0F, 3, 0x62, 0xDF, 0xFF, 1 }, // atmega88p
    { 0x94, 0x06, 3, 0x62, 0xDF, 0xFF, 1 }, // atmega168
    { 0x94, 0x0B, 3, 0x62, 0xDF, 0xFF, 1 }, // atmega168p
    { 0x95, 0x14, 3, 0x62, 0xD9, 0xFF, 1 }, // atmega328
    { 0x94, 0x0F, 3, 0x42, 0x99, 0xFF, 1 }, // atmega164a
    { 0x94, 0x0A, 3, 0x42, 0x99, 0xFF, 1 }, // atmega164p
    { 0x95, 0x15, 3, 0x42, 0x99, 0xFF, 1 }, // atmega324a
    { 0x95, 0x08, 3, 0x42, 0x99, 0xFF, 1 }, // atmega324p
    { 0x95, 0x11, 3, 0x42, 0x99, 0xFF, 1 }, // atmega324pa
    { 0x96, 0x09, 3, 0x62, 0x99, 0xFF, 1 }, // atmega644
    { 0x96, 0x0A, 3, 0x62, 0x99, 0xFF, 1 }, // atmega644P
    { 0x97, 0x06, 3, 0x42, 0x99, 0xFF, 1 }, // atmega1284
    { 0x97, 0x05, 3, 0x42, 0x99, 0xFF, 1 }, // atmega1284p
    { 0x95, 0x0F, 3, 0x62, 0xD9, 0xFF, 1 }, // atmega328p

  };
char at90s2313[] PROGMEM = "AT90S2313";
char at90s2333[] PROGMEM = "AT90S2333";
char at90s2343[] PROGMEM = "AT90S2343";
char at90s4433[] PROGMEM = "AT90S4433";
char at90s4434[] PROGMEM = "AT90S4434";
char at90s8515[] PROGMEM = "AT90S8515";
char at90s8535[] PROGMEM = "AT90S8535";
char attiny11[] PROGMEM = "ATtiny11";
char attiny12[] PROGMEM = "ATtiny12";
char attiny13[] PROGMEM = "ATtiny13";
char attiny15[] PROGMEM = "ATtiny15";
char attiny2313[] PROGMEM = "ATtiny2313";
char attiny4313[] PROGMEM = "ATtiny4313";
char attiny24[] PROGMEM = "ATtiny24";
char attiny44[] PROGMEM = "ATtiny44";
char attiny84[] PROGMEM = "ATtiny84";
char attiny25[] PROGMEM = "ATtiny25";
char attiny45[] PROGMEM = "ATtiny45";
char attiny85[] PROGMEM = "ATtiny85";
char attiny26[] PROGMEM = "ATtiny26";
char attiny261[] PROGMEM = "ATtiny261";
char attiny461[] PROGMEM = "ATtiny461";
char attiny861[] PROGMEM = "ATtiny861";
char atmega8515[] PROGMEM = "ATmega8515";
char atmega8535[] PROGMEM = "ATmega8535";
char atmega8[] PROGMEM = "ATmega8";
char atmega16[] PROGMEM = "ATmega16";
char atmega32[] PROGMEM = "ATmega32";
char atmega64[] PROGMEM = "ATmega64";
char atmega128[] PROGMEM = "ATmega128";
char atmega48[] PROGMEM = "ATmega48";
char atmega48p[] PROGMEM = "ATmega48P";
char atmega88[] PROGMEM = "ATmega88";
char atmega88p[] PROGMEM = "ATmega88P";
char atmega168[] PROGMEM = "ATmega168";
char atmega168p[] PROGMEM = "ATmega168P";
char atmega328[] PROGMEM = "ATmega328";
char atmega164a[] PROGMEM = "ATmega164A";
char atmega164p[] PROGMEM = "ATmega164P";
char atmega324a[] PROGMEM = "ATmega324A";
char atmega324p[] PROGMEM = "ATmega324P";
char atmega324pa[] PROGMEM = "ATmega324PA";
char atmega644[] PROGMEM = "ATmega644";
char atmega644p[] PROGMEM = "ATmega644P";
char atmega1284[] PROGMEM = "ATmega1284";
char atmega1284p[] PROGMEM = "ATmega1284P";
char atmega328p[] PROGMEM = "ATmega328P";

PGM_P name_table[] PROGMEM = 
  {
    at90s2313,
    at90s2333,
    at90s2343,
    at90s4433,
    at90s4434,
    at90s8515,
    at90s8535,
    attiny11,
    attiny12,
    attiny13,
    attiny15,
    attiny2313,
    attiny4313,
    attiny24,
    attiny44,
    attiny84,
    attiny25,
    attiny45,
    attiny85,
    attiny26,
    attiny261,
    attiny461,
    attiny861,
    atmega8515,
    atmega8535,
    atmega8,
    atmega16,
    atmega32,
    atmega64,
    atmega128,
    atmega48,
    atmega48p,
    atmega88,
    atmega88p,
    atmega168,
    atmega168p,
    atmega328,
    atmega164a,
    atmega164p,
    atmega324a,
    atmega324p,
    atmega324pa,
    atmega644,
    atmega644p,
    atmega1284,
    atmega1284p,
    atmega328p,
  };


// User defined settings
#define  BAUD         19200    // Serial port rate at which to talk to PC

// Pin Assignments for the pins on the FBD board (you shouldn't need to change these)
#define  VCC      5 
#define  RDY     14 
#define  OE       2 
#define  WR       4 
#define  BS1     15 
#define  BS2      6 
#define  XA0     16 
#define  ORIGXA1 17 
#define  RST      7 
#define  XTAL1    3 
#define  ORIGPAGEL 19
#define  ECJUMPER 18

// These pin assignments change depending on which chip is being programmed
byte PAGEL = ORIGPAGEL; // ATtiny2313: PAGEL = BS1
byte XA1 = ORIGXA1; // ATtiny2313: XA1 = BS2

// Pin assignments for HVSP mode 
#define  SCI    RDY // note that the direction must be changed!
#define  SDO    BS1
#define  SII    XA0
#define  SDI    ORIGXA1

// Serial instructions for HVSP mode
// Based on the ATtiny85 datasheet Table 20-16 pg. 163-165.
// These instructions don't contain the necessary zero padding, which is added later.

// LFUSE
#define HVSP_READ_LFUSE_DATA     B00000100  // For the commands we are interested in
#define HVSP_READ_LFUSE_INSTR1   B01001100  // only the 1st instruction contains a fixed data packet.
#define HVSP_READ_LFUSE_INSTR2   B01101000  // Instructions 2-3 have data = all zeros.
#define HVSP_READ_LFUSE_INSTR3   B01101100

#define HVSP_WRITE_LFUSE_DATA    B01000000  // For the write instructions, the data contents
#define HVSP_WRITE_LFUSE_INSTR1  B01001100  // for instruction 2 are the desired fuse bits.
#define HVSP_WRITE_LFUSE_INSTR2  B00101100  // Instructions 3-4 have data = all zeros.
#define HVSP_WRITE_LFUSE_INSTR3  B01100100
#define HVSP_WRITE_LFUSE_INSTR4  B01101100

// HFUSE
#define HVSP_READ_HFUSE_DATA     B00000100
#define HVSP_READ_HFUSE_INSTR1   B01001100
#define HVSP_READ_HFUSE_INSTR2   B01111010
#define HVSP_READ_HFUSE_INSTR3   B01111110

#define HVSP_WRITE_HFUSE_DATA    B01000000
#define HVSP_WRITE_HFUSE_INSTR1  B01001100
#define HVSP_WRITE_HFUSE_INSTR2  B00101100
#define HVSP_WRITE_HFUSE_INSTR3  B01110100
#define HVSP_WRITE_HFUSE_INSTR4  B01111100

// EFUSE
// Note: not all ATtiny's have an EFUSE
#define HVSP_READ_EFUSE_DATA     B00000100  
#define HVSP_READ_EFUSE_INSTR1   B01001100
#define HVSP_READ_EFUSE_INSTR2   B01101010
#define HVSP_READ_EFUSE_INSTR3   B01101110

#define HVSP_WRITE_EFUSE_DATA    B01000000
#define HVSP_WRITE_EFUSE_INSTR1  B01001100
#define HVSP_WRITE_EFUSE_INSTR2  B00101100
#define HVSP_WRITE_EFUSE_INSTR3  B01100110
#define HVSP_WRITE_EFUSE_INSTR4  B01101110

// LOCK
#define HVSP_READ_LOCK_DATA      B00000100
#define HVSP_READ_LOCK_INSTR1    B01001100
#define HVSP_READ_LOCK_INSTR2    B01111000
#define HVSP_READ_LOCK_INSTR3    B01101100

#define HVSP_WRITE_LOCK_DATA     B00100000
#define HVSP_WRITE_LOCK_INSTR1   B01001100
#define HVSP_WRITE_LOCK_INSTR2   B00101100
#define HVSP_WRITE_LOCK_INSTR3   B01100100
#define HVSP_WRITE_LOCK_INSTR4   B01101100

// Signature read
#define HVSP_READ_SIG_DATA       B00001000
#define HVSP_READ_SIG_INSTR1     B01001100
#define HVSP_READ_SIG_INSTR2     B00001100
#define HVSP_READ_SIG_INSTR3     B01101000
#define HVSP_READ_SIG_INSTR4     B01101100

// Erase chip
#define HVSP_ERASE_CHIP_DATA     B10000000
#define HVSP_ERASE_CHIP_INSTR1   B01001100
#define HVSP_ERASE_CHIP_INSTR2   B01100100
#define HVSP_ERASE_CHIP_INSTR3   B01101100


// Internal definitions
enum modelist { ATMEGA, TINY2313, HVSP };
enum fusesel { LFUSE_SEL, HFUSE_SEL, EFUSE_SEL, LOCK_SEL };
enum ledcolor { RED, GREEN };

// Global variables
byte mcu_mode;  // programming mode
byte mcu_fuses = 0;
boolean interactive_mode = true; // interaction over terminal
int mcu_index = -1;
unsigned long mcu_signature = 0;

void setup() { // run once, when the sketch starts
  char modec = ' ';
  
  // Set up control lines for HV parallel programming
  
  PORTB = 0;
  DDRB = 0;

  pinMode(ECJUMPER, INPUT_PULLUP);
  pinMode(VCC, OUTPUT);
  pinMode(RDY, INPUT);
  pinMode(OE, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(BS1, OUTPUT);
  pinMode(XA0, OUTPUT);
  pinMode(XA1, OUTPUT);
  pinMode(PAGEL, OUTPUT);
  pinMode(RST, OUTPUT);  // 12V Reset signal
  pinMode(BS2, OUTPUT);
  pinMode(XTAL1, OUTPUT);

  if (digitalRead(0) == LOW) interactive_mode = false; // no RS232 connection!
  

  // Initialize output pins as needed
  digitalWrite(RST, LOW);  // Turn off 12V step-up converter (non-inverting, unlike original circuit)
  digitalWrite(VCC, HIGH); // Turn off VCC

  Serial.begin(BAUD);  // Open serial port
  Serial.println();
  Serial.println();
  Serial.print(F("RescueAVR Version "));
  Serial.print(VERSION);
  if (interactive_mode) Serial.println(F(" / interactive"));
  else Serial.println(F(" / non-interactive"));
  Serial.println();

  for (mcu_mode = ATMEGA; mcu_mode <= HVSP; mcu_mode++) {
    enterHVProgMode(mcu_mode);
    mcu_signature = readSig(mcu_mode);
    leaveHVProgMode();
    if ((mcu_signature>>16) == 0x1E) break;
  }
  if (mcu_mode > HVSP) {
    Serial.println(F("No chip found!"));
    Serial.println(F("Insert chip and press button or give details."));
    if (!interactive_mode) {
      ledOn(RED);
      delay(3000);
      ledOff(RED);
      while (true) { };
    }
    while (Serial.available()) Serial.read();
    while (true) {
      Serial.println();
      Serial.println(F("Choose from:"));
      Serial.println(F("  P - HVPP"));
      Serial.println(F("  T - HVPP for Tiny"));
      Serial.println(F("  S - HVSP"));
      Serial.println(F("  X - exit"));
      Serial.print(F("Choice: "));
      while (!Serial.available())  { };
      modec = toupper(Serial.read());
      Serial.println(modec);
      while (Serial.available()) Serial.read();
      if (modec == 'P' || modec == 'T' || modec == 'S' || modec == 'X') break;
      Serial.print("'");
      Serial.print(modec);
      Serial.println(F("' is not valid choice."));
    }
    switch (modec) {
    case 'P': mcu_mode = ATMEGA; break;
    case 'T': mcu_mode = TINY2313; break;
    case 'S': mcu_mode = HVSP; break;
    default: while (true) { };
    }
  }
  Serial.print(F("Signature: "));
  Serial.println(mcu_signature,HEX);
  mcu_index = searchMCU(mcu_signature);
  Serial.print(F("MCU name:  "));
  if (mcu_index >= 0) {
    if (pgm_read_byte(&(mcu_types[mcu_index][6])) == false) {
      // unfortunately, we do not have enough info about this chip
      printMCUName(mcu_index);
      Serial.print(F(" / "));
      if (!interactive_mode) {
        ledOn(GREEN);
        delay(1500);
        ledOff(GREEN);
      } 
      mcu_index = -1;
    }
  }
  if (mcu_index < 0 ) {
    Serial.println(F("Unknown"));
    if (!interactive_mode) {
      ledOn(RED);
      delay(3000);
      ledOff(RED);
      while (true) { };
    } else {
      ledOn(RED);
      delay(1500);
      ledOff(RED);
      mcu_fuses = 0;
      while (Serial.available()) Serial.read();
      while (mcu_fuses < 1 || mcu_fuses > 3) {
        Serial.print(F("# of fuse bytes (1-3): "));
        while (!Serial.available())  { };
        mcu_fuses = Serial.read();
        Serial.println((char)mcu_fuses);
        while (Serial.available()) Serial.read();
        mcu_fuses = mcu_fuses - '0';
      }
    }
  } else {
    printMCUName(mcu_index);
    Serial.println();
    mcu_fuses = pgm_read_byte(&(mcu_types[mcu_index][2]));
    ledOn(GREEN);
    if (interactive_mode) delay(1500);
    else delay(3000);
    ledOff(GREEN);
  }
}

void loop() {  // run over and over again
  
  byte hfuse, lfuse, efuse, lock;  // fuse and lock values
  byte dhfuse, dlfuse, defuse;  // default values
  char action = ' ';
  
  if (mcu_index >= 0) {
    dlfuse = pgm_read_byte(&(mcu_types[mcu_index][3]));
    dhfuse = pgm_read_byte(&(mcu_types[mcu_index][4]));
    defuse = pgm_read_byte(&(mcu_types[mcu_index][5]));
  }

  enterHVProgMode(mcu_mode);
  
  lfuse = readFuse(mcu_mode,LFUSE_SEL);
  if (mcu_fuses > 1) hfuse = readFuse(mcu_mode,HFUSE_SEL);
  if (mcu_fuses > 2) efuse = readFuse(mcu_mode,EFUSE_SEL);
  lock = readFuse(mcu_mode,LOCK_SEL);
  leaveHVProgMode();

  printFuses("Current ",mcu_fuses,lfuse,hfuse,efuse);
  if (mcu_index >= 0) printFuses("Default ",mcu_fuses,dlfuse,dhfuse,defuse);

  printLock(lock);
 

  if (interactive_mode) {
    while (true) {
      Serial.println();
      Serial.println(F("Choose:"));
      Serial.println(F("  E - Erase chip"));
      if (mcu_index >= 0) Serial.println(F("  D - Burn default fuse values"));
      Serial.println(F("  L - Change low fuse"));
      if (mcu_fuses > 1) Serial.println(F("  H - Change high fuse"));
      if (mcu_fuses > 2) Serial.println(F("  X - Change extended fuse"));
      Serial.println(F("  K - Change lock byte"));
      Serial.print(F("Action: "));
      
      while (!Serial.available()) { };
      action = toupper(Serial.read());
      Serial.println(action);
      
      if (action == 'E' || action == 'D' || action == 'L' ||
          action == 'H' || action == 'X' || action == 'K') break;
      Serial.print("'");
      Serial.print(action);
      Serial.println("' is not a valid command char.");
    }
  
    enterHVProgMode(mcu_mode);
    switch (action) {
    case 'E': 
      if (digitalRead(ECJUMPER) == LOW) {
        Serial.println(F("\nErasing ..."));
        eraseChip(mcu_mode); 
        Serial.println(F("...done"));
      }
      else Serial.println(F("'Erase chip' is not allowed"));
      break;
    case 'D': 
      if (mcu_index >= 0) {
        Serial.println(F("\nBurning ..."));
        burnFuse(mcu_mode, dlfuse, LFUSE_SEL);
        if (mcu_fuses > 1) burnFuse(mcu_mode, dhfuse, HFUSE_SEL);
        if (mcu_fuses > 2) burnFuse(mcu_mode, defuse, EFUSE_SEL);
        Serial.println(F("...done"));
      }
      break;
    case 'L': Serial.print(F("New low fuse: "));
      lfuse = askByte();
      Serial.println(F("\nBurning ..."));
      burnFuse(mcu_mode, lfuse, LFUSE_SEL);
      Serial.println(F("...done"));
      break;
    case 'H': 
      if (mcu_fuses > 1) {
        Serial.print(F("New high fuse: "));
        hfuse = askByte();
        Serial.println(F("\nBurning ..."));
        burnFuse(mcu_mode, hfuse, HFUSE_SEL);
        Serial.println(F("...done"));
      }
      break;
    case 'X':
      if (mcu_fuses > 2) {
        Serial.print(F("New extended fuse: "));
        efuse = askByte();
        Serial.println(F("\nBurning ..."));
        burnFuse(mcu_mode, efuse, EFUSE_SEL);
        Serial.println(F("...done"));
      }
      break;
    case 'K':
      Serial.print(F("New lock byte: "));
      lock = askByte();
      Serial.println(F("\nBurning ..."));
      burnFuse(mcu_mode, lock, LOCK_SEL);
      Serial.println(F("...done"));
      break;
    }
    Serial.println();
    leaveHVProgMode();
  } else { // non-interactive mode

    enterHVProgMode(mcu_mode);

    // try to reset all locks
    burnFuse(mcu_mode, 0xFF, LOCK_SEL);

    // remove lock by erasing chip - if necessary and allowed
    lock = readFuse(mcu_mode, LOCK_SEL);
    if (lock != 0xFF && digitalRead(ECJUMPER) == LOW) eraseChip(mcu_mode); 
      
    // try to reset fuses
    burnFuse(mcu_mode, dlfuse, LFUSE_SEL);
    if (mcu_fuses > 1) burnFuse(mcu_mode, dhfuse, HFUSE_SEL);
    if (mcu_fuses > 2) burnFuse(mcu_mode, defuse, EFUSE_SEL);
    delay(100);
    // see whether successful
    lock = readFuse(mcu_mode, LOCK_SEL);
    lfuse = readFuse(mcu_mode,LFUSE_SEL);
    if (mcu_fuses > 1) hfuse = readFuse(mcu_mode,HFUSE_SEL);
    if (mcu_fuses > 2) efuse = readFuse(mcu_mode,EFUSE_SEL);
    
    leaveHVProgMode();
    // if successul blink green, otherwise blink red
    if (verifyFuses(mcu_fuses,
                    lock,0xFF,
                    lfuse,dlfuse,
                    hfuse,dhfuse,
                    efuse,defuse)) ledBlink(GREEN,5);
    else ledBlink(RED,5);
    while (true) { };
  }
}



int searchMCU(unsigned long sig) {
  uint8_t b1, b2;
  if ((sig>>16) != 0x1E) return -1;
  b1 = (sig>>8)&(0xFF);
  b2 = (sig&0xFF);
  for (int i=0; i < MCU_NUM; i++) {
    if (b1 == pgm_read_byte(&(mcu_types[i][0])) &&
        b2 == pgm_read_byte(&(mcu_types[i][1]))) {
      return i;
    }
  }
  return -1;
}

void printMCUName(int ix) {
  char buf[15];
  if (ix < 0 || ix >= MCU_NUM) return;
  strcpy_P(buf, (PGM_P)pgm_read_word(&(name_table[ix])));
  Serial.print(buf);
}

boolean verifyFuses(int num, byte k1, byte k2, byte l1, byte l2, byte h1, byte h2, byte e1, byte e2) {
  boolean correct;
  correct = (k1 == k2);
  correct = (correct && (l1 == l2));
  if (num > 1) correct = (correct && (h1 == h2));
  if (num > 2) correct = (correct && (e1 == e2));
  return correct;
}

unsigned long readSig(int mode) {
  if (mode == HVSP) return readHVSPSig();
  else return readHVPPSig(mode);
}

byte readFuse(int mode, int selection) {
  if (mode == HVSP) return readHVSPFuse(selection);
  else return readHVPPFuse(mode,selection);
}

void burnFuse(int mode, byte val, int selection) {
  if (mode == HVSP) burnHVSPFuse(val,selection);
  else burnHVPPFuse(mode,val,selection);
}

void eraseChip(int mode) {
  if (mode == HVSP) eraseHVSP();
  else eraseHVPP(mode);  
}

void enterHVProgMode(int mode) {
  // Initialize pins to enter programming mode
  if (mode == TINY2313) {
    PAGEL = BS1;
    XA1 = BS2;
  } else {
    PAGEL = ORIGPAGEL;
    XA1 = ORIGXA1;
  }
   

  PORTB = 0;
  DDRB = 0;
  digitalWrite(PAGEL, LOW);
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);
  digitalWrite(BS2, LOW);
  digitalWrite(WR, LOW);  // ATtiny2313 needs this to be low to enter programming mode, ATmega doesn't care
  digitalWrite(OE, LOW);
  delay(100); // to let it settle, since otherwise Vcc may be already high due to XA1 high
 
  if(mode == HVSP) {
    digitalWrite(SDI, LOW);  // set necessary pin values to enter programming mode
    digitalWrite(SII, LOW);
    digitalWrite(SDO, LOW);  // needs to be low to enter programming mode
    pinMode(SCI, OUTPUT);    // SCI is same as RDY pin
  }
  
  // Enter programming mode
  digitalWrite(VCC, LOW);  // Apply VCC to start programming process
  delayMicroseconds(80);
  digitalWrite(RST, HIGH);   // Apply 12V to !RESET thru level shifter
  
  if(mode == HVSP) {
    // reset SDO after short delay, longer leads to logic contention because target sets SDO high after entering programming mode
    delayMicroseconds(1);   // datasheet says 10us, 1us is needed to avoid drive contention on SDO 
    pinMode(SDO, INPUT);    // set to input to avoid logic contention
  }
  
  delayMicroseconds(10);  // Give lots of time for part to enter programming mode
  digitalWrite(OE, HIGH);
  digitalWrite(WR, HIGH);   // Now that we're in programming mode we can disable !WR
  delay(1);
}

void leaveHVProgMode() {
  PORTB = 0;
  DDRB = 0;
  
  digitalWrite(OE, HIGH);
  digitalWrite(RST, LOW);  // exit programming mode
  delay(1);
  digitalWrite(OE, LOW);
  digitalWrite(WR, LOW);
  digitalWrite(PAGEL, LOW);
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);
  digitalWrite(BS2, LOW);
  digitalWrite(VCC, HIGH);
}

void eraseHVPP(int mode) {
  
  sendHVPPCmdOrAddr(mode,true,B10000000); 
  digitalWrite(WR, LOW); 
  delay(1);
  digitalWrite(WR, HIGH);
  //delay(100);
  
  while(digitalRead(RDY) == LOW);  // when RDY goes high, burn is done
}
  
void eraseHVSP(void) {
  HVSP_write(HVSP_ERASE_CHIP_DATA, HVSP_ERASE_CHIP_INSTR1);
  HVSP_write(0x00, HVSP_ERASE_CHIP_INSTR2);
  HVSP_write(0x00, HVSP_ERASE_CHIP_INSTR3);
  while (digitalRead(SDO) == LOW) { };
}

void burnHVPPFuse(int mode, byte fuse, int select)  // write high or low fuse to AVR
{
  
  // Send command to enable fuse programming mode
  if (select == LOCK_SEL)   sendHVPPCmdOrAddr(mode,true,B00100000);  
  else sendHVPPCmdOrAddr(mode,true,B01000000);  
  
  // Enable data loading
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, HIGH);
  // Specify low byte
  digitalWrite(BS1, LOW); 
  if (mode != TINY2313)
    digitalWrite(BS2, LOW);  
  delay(1);
  
  // Load fuse value into target
  PORTB = fuse;
  DDRB = 0xFF;
  
  strobe_xtal();  // latch DATA

  PORTB = 0;
  DDRB = 0;
   
  // Decide which fuse location to burn
  switch (select) { 
  case HFUSE_SEL:
    digitalWrite(BS1, HIGH); // program HFUSE
    digitalWrite(BS2, LOW);
    break;
  case LFUSE_SEL:
    digitalWrite(BS1, LOW);  // program LFUSE
    digitalWrite(BS2, LOW);
    break;
  case EFUSE_SEL:
    digitalWrite(BS1, LOW);  // program EFUSE
    digitalWrite(BS2, HIGH);
    break;
  }
  delay(1);
   // Burn the fuse
  digitalWrite(WR, LOW); 
  delay(1);
  digitalWrite(WR, HIGH);
  //delay(100);
  
  while(digitalRead(RDY) == LOW);  // when RDY goes high, burn is done
  
  // Reset control lines to original state
  digitalWrite(BS1, LOW);
  digitalWrite(BS2, LOW);
}

void burnHVSPFuse(byte fuse, int select) {

  switch (select) {
  case LFUSE_SEL:
    HVSP_write(HVSP_WRITE_LFUSE_DATA, HVSP_WRITE_LFUSE_INSTR1);
    HVSP_write(fuse, HVSP_WRITE_LFUSE_INSTR2);
    HVSP_write(0x00, HVSP_WRITE_LFUSE_INSTR3);
    HVSP_write(0x00, HVSP_WRITE_LFUSE_INSTR4);
    break;

  case HFUSE_SEL:    
    HVSP_write(HVSP_WRITE_HFUSE_DATA, HVSP_WRITE_HFUSE_INSTR1);
    HVSP_write(fuse, HVSP_WRITE_HFUSE_INSTR2);
    HVSP_write(0x00, HVSP_WRITE_HFUSE_INSTR3);
    HVSP_write(0x00, HVSP_WRITE_HFUSE_INSTR4);
    break;
    
  case EFUSE_SEL:
    HVSP_write(HVSP_WRITE_EFUSE_DATA, HVSP_WRITE_EFUSE_INSTR1);
    HVSP_write(fuse, HVSP_WRITE_EFUSE_INSTR2);
    HVSP_write(0x00, HVSP_WRITE_EFUSE_INSTR3);
    HVSP_write(0x00, HVSP_WRITE_EFUSE_INSTR4);
    break;

  case LOCK_SEL:
    HVSP_write(HVSP_WRITE_LOCK_DATA, HVSP_WRITE_LOCK_INSTR1);
    HVSP_write(fuse, HVSP_WRITE_LOCK_INSTR2);
    HVSP_write(0x00, HVSP_WRITE_LOCK_INSTR3);
    HVSP_write(0x00, HVSP_WRITE_LOCK_INSTR4);
    break;
  }
  while(digitalRead(SDO) == LOW);
  return;
}


byte readHVPPFuse(int mode, int select) {
  byte fuse;
  
  sendHVPPCmdOrAddr(mode,true,B00000100);  // Send command to read fuse bits
  
  // Configure DATA as input so we can read back fuse values from target
  PORTB = 0;
  DDRB = 0;

  // Set control lines
  switch (select) {
  case LFUSE_SEL:  
      // Read LFUSE
      digitalWrite(BS2, LOW);
      digitalWrite(BS1, LOW);
      break;
  case HFUSE_SEL:
      // Read HFUSE
      digitalWrite(BS2, HIGH);
      digitalWrite(BS1, HIGH);
      break;
  case EFUSE_SEL:
      // Read EFUSE
      digitalWrite(BS2, HIGH);
      digitalWrite(BS1, LOW);
      break;
  case LOCK_SEL:
      // Read LOCK
      digitalWrite(BS2, LOW);
      digitalWrite(BS1, HIGH);
      break;    
  }
  
  //  Read fuse
  digitalWrite(OE, LOW);
  delay(1);
  
  fuse = PINB;
  
  digitalWrite(OE, HIGH);  // Done reading, disable output enable line
  return fuse;
}

byte readHVSPFuse(int select) {
  byte fuse;
  
  switch (select) {

  case LFUSE_SEL:
    HVSP_read(HVSP_READ_LFUSE_DATA, HVSP_READ_LFUSE_INSTR1);
    HVSP_read(0x00, HVSP_READ_LFUSE_INSTR2);
    fuse=HVSP_read(0x00, HVSP_READ_LFUSE_INSTR3);
    break;

  case HFUSE_SEL:
    HVSP_read(HVSP_READ_HFUSE_DATA, HVSP_READ_HFUSE_INSTR1);
    HVSP_read(0x00, HVSP_READ_HFUSE_INSTR2);
    fuse=HVSP_read(0x00, HVSP_READ_HFUSE_INSTR3);
    break;

  case EFUSE_SEL:
    HVSP_read(HVSP_READ_EFUSE_DATA, HVSP_READ_EFUSE_INSTR1);
    HVSP_read(0x00, HVSP_READ_EFUSE_INSTR2);
    fuse=HVSP_read(0x00, HVSP_READ_EFUSE_INSTR3);
    break;

  case LOCK_SEL:
    HVSP_read(HVSP_READ_LOCK_DATA, HVSP_READ_LOCK_INSTR1);
    HVSP_read(0x00, HVSP_READ_LOCK_INSTR2);
    fuse=HVSP_read(0x00, HVSP_READ_LOCK_INSTR3);
    break;
  }
  return fuse;
}

unsigned long readHVPPSig(int mode) {

  unsigned long result = 0;

  // Configure DATA as input so we can read back fuse values from target
  PORTB = 0;
  DDRB = 0;
  
  for (byte i=0;i<3;i++) {
    sendHVPPCmdOrAddr(mode,true,B00001000);
    sendHVPPCmdOrAddr(mode,false,i);
    digitalWrite(BS1,LOW);
    digitalWrite(OE, LOW);
    delay(1);
    result = (result<<8)+PINB;
    digitalWrite(OE, HIGH); 
  }
  return result;
}

unsigned long readHVSPSig() {

  unsigned long result = 0;

  for (byte i=0;i<3;i++) {
    HVSP_read(HVSP_READ_SIG_DATA, HVSP_READ_SIG_INSTR1);
    HVSP_read(i, HVSP_READ_SIG_INSTR2);
    HVSP_read(0x00, HVSP_READ_SIG_INSTR3);
    result = (result<<8) + HVSP_read(0x00, HVSP_READ_SIG_INSTR4);
  }
  return result;
}


byte HVSP_read(byte data, byte instr) { // Read a byte using the HVSP protocol

  byte response = 0x00; // a place to hold the response from target

  digitalWrite(SCI, LOW);  // set clock low
  // 1st bit is always zero
  digitalWrite(SDI, LOW);
  digitalWrite(SII, LOW);
  sclk();
  
  // We capture a response on every readm even though only certain responses contain
  // valid data.  For fuses, the valid response is captured on the 3rd instruction write.
  // It is up to the program calling this function to figure out which response is valid.
  
  // The MSB of the response byte comes "early", that is, 
  // before the 1st non-zero-padded byte of the 3rd instruction is sent to the target.
  // For more information, see the ATtiny25/45/85 datasheet, Table 20-16 (pg. 164).
  if (digitalRead(SDO) == HIGH) // target sent back a '1'?
    response |= 0x80;  // set MSB of response byte high
  
  // Send each bit of data and instruction byte serially, MSB first
  // I do this by shifting the byte left 1 bit at a time and checking the value of the new MSB
  for (int i=0; i<8; i++) {  // i is bit number
    if ((data << i) & 0x80)  // shift data byte left and check if new MSB is 1 or 0
      digitalWrite(SDI, HIGH);  // bit was 1, set pin high
    else
      digitalWrite(SDI, LOW);   // bit was 0, set pin low
      
    if ((instr << i) & 0x80)   // same process for instruction byte
      digitalWrite(SII, HIGH);
    else
      digitalWrite(SII, LOW);
   sclk();
       
    if (i < 7) {  // remaining 7 bits of response are read here (one at a time)
      // note that i is one less than the bit position of response we are reading, since we read
      // the MSB above.  That's why I shift 0x40 right and not 0x80. 
      if(digitalRead(SDO) == HIGH)  // if we get a logic 1 from target,
        response |= (0x40 >> i);    // set corresponding bit of response to 1
    }
  }
  
  // Last 2 bits are always zero
  for (int i=0; i<2; i++) {
    digitalWrite(SDI, LOW);
    digitalWrite(SII, LOW);
    sclk();
  }
  
  return response;
}

void HVSP_write(byte data, byte instr) { // Write to target using the HVSP protocol

  digitalWrite(SCI, LOW);  // set clock low
  
  // 1st bit is always zero
  digitalWrite(SDI, LOW);
  digitalWrite(SII, LOW);
  sclk();  // latch bit
  
  // Send each bit of data and instruction byte serially, MSB first
  // I do this by shifting the byte left 1 bit at a time and checking the value of the new MSB
  for (int i=0; i<8; i++) {  // i is bit number
    if ((data << i) & 0x80)  // shift data byte left and check if new MSB is 1 or 0
      digitalWrite(SDI, HIGH);  // bit was 1, set pin high
    else
      digitalWrite(SDI, LOW);   // bit was 0, set pin low
      
    if ((instr << i) & 0x80)  // same process for instruction byte
      digitalWrite(SII, HIGH);
    else
      digitalWrite(SII, LOW);
      
   sclk();  // strobe SCI (serial clock) to latch data
  }
  
  // Last 2 bits are always zero
  for (int i=0; i<2; i++) {
    digitalWrite(SDI, LOW);
    digitalWrite(SII, LOW);   
    sclk();
  }
}

void sendHVPPCmdOrAddr(int mode, boolean is_cmd, byte cmd_or_addr)  // Send command to target AVR
{
  // Set controls for command mode
  if (is_cmd) digitalWrite(XA1, HIGH);
  else   digitalWrite(XA1,LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);  
  
  if (mode != TINY2313)
    digitalWrite(BS2, LOW);  // Command load seems not to work if BS2 is high 
  
  PORTB = cmd_or_addr;
  DDRB = 0xFF;
  
  strobe_xtal();  // latch DATA
  
  PORTB = 0;
  DDRB = 0;
  digitalWrite(XA1, LOW);
}


void sclk(void) {  // send serial clock pulse, used by HVSP commands

  // These delays are much  longer than the minimum requirements,
  // but we don't really care about speed.
  delay(1);  
  digitalWrite(SCI, HIGH);
  delay(1);
  digitalWrite(SCI, LOW);
}

void strobe_xtal(void) {  // strobe xtal (usually to latch data on the bus)
  
  delay(1);
  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delay(1);
  digitalWrite(XTAL1, LOW);
}


int askByte(void) {  // get new byte value
  char serbuffer[2];
  byte i = 0;
  char next;
  
  while (i < 2) {
    while (Serial.available() == 0);   // wait for a character to come in
    next = Serial.read();
    if (i == 1 && (next == '\b' || next == '\x7F')) {
      Serial.print("\b \b");
      i = 0;
    }
    if (next >= 'a' && next <= 'f') next = toupper(next);
    if ((next >= '0' && next <= '9') || (next >= 'A' && next <= 'F')) {
      serbuffer[i++] = next;
      Serial.print(next);
    } 
  }
  Serial.println();
  return(hex2dec(serbuffer[1]) + hex2dec(serbuffer[0]) * 16);
}

int hex2dec(byte c) { // converts one HEX character into a number
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } 
  else return 0;
}

void printFuses(char *str, byte num, byte l, byte h, byte e) {
  Serial.print(str);
  switch (num) {
  case 1: Serial.print(F("Fuse byte:   ")); break;
  case 2: Serial.print(F("L/H-Fuses:   ")); break;
  case 3: Serial.print(F("L/H/E-Fuses: ")); break;
  }
  print2Hex(l,true);
  if (num > 1) print2Hex(h,true);
  if (num > 2) print2Hex(e,true);
  Serial.println();
}

void printLock(byte l) {
  Serial.print(F("Current lock byte:   "));
  print2Hex(l,true);
  Serial.println();
}

void print2Hex(byte val, boolean blank) 
{ // print two hex digits
  if (val < 0x10) Serial.print('0');
  Serial.print(val,HEX);
  if (blank) Serial.print(' ');
}

void ledOn(int color) 
{
  switch (color) {
  case RED: digitalWrite(XA0,HIGH);
    break;
  case GREEN: digitalWrite(XA1,HIGH);
    break;
  }
}

void ledOff(int color) 
{
  switch (color) {
  case RED: digitalWrite(XA0,LOW);
    break;
  case GREEN: digitalWrite(XA1,LOW);
    break;
  }
}

void ledBlink(int color,int sec) {
  
  for (int i=0; i < 5*sec; i++) {
    ledOn(color);
    delay(100);
    ledOff(color);
    delay(100);
  }
}
