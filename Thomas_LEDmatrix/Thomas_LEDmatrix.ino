/***********************************************************************
 * Daft Punk Thomas helmet display
 * 
 * Authors: Ryan Pawling and Chelsea Pawling
 *
 * Adapted for 6x32 display
 ***********************************************************************/

#include <Arduino.h>

#define X_MAX 7
#define Y_MAX 5
#define C_MAX 3

#define X_WIDTH 32
#define Y_WIDTH 6
#define C_COUNT 4


// include the library for MAX72XX control
#include "LedControl.h"
#include <SoftwareSerial.h>

// include headers for animation data arrays
#include "HeartProgram.h"
#include "EyesProgram.h"
#include "OkProgram.h"
#include "EkgProgram.h"
#include "ArrowProgram.h"
//#include "UfoProgram.h"
#include "Alphabet.h"


SoftwareSerial Bluetooth(5, 6); // RX, TX
int LED = 2; // the on-board LED
char state = 'd';
/* 
 * Led control class
 * We use pins 12,11 and 10 on the Arduino for the SPI interface
 * Pin 12 is connected to the DATA IN-pin of the first MAX7221
 * Pin 11 is connected to the CLK-pin of the first MAX7221
 * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221   
 * There will be 4 MAX7221 attached to the arduino in series
 */
LedControl lc=LedControl(12,11,10,4);

// Setup
// --------------------------------------------
// Called when arduino is started
// Switch each MAX72XX into normal operation mode
// Active serial output for debug printing
void setup() {
  for(int index=0;index<lc.getDeviceCount();index++) {
      lc.shutdown(index,false);
      lc.setIntensity(index, 10);
      lc.clearDisplay(index);
  }
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Bluetooth.println("init...");
  pinMode(LED,OUTPUT);
}

// Main loop
// --------------------------------------------
// This main loop is called after setup and repeated by Arduino
void loop() {
  bluetooth_receiver();
  

  if(state == 'd') {
    dot_program();
  } else if(state == 'u') {
    ufo_program();
  } else if(state == 'a') {
    arrow_program();
  } else if(state == 'k') {
    ekg_program();
  } else if(state == 'o') {
    ok_program();
  } else if(state == 'h') {
    heart_program();
  } else if(state == 'e') {
    eyes_program();
  } else if(state == 't') {
    test_program();
  } else if(state == 'l') {
    //ufo_program();
    bluetooth_receiver();
    arrow_program();
    bluetooth_receiver();
    dot_program();
    bluetooth_receiver();
    print_string("GET LUCKY");
    bluetooth_receiver();
    ekg_program();
    bluetooth_receiver();
    ok_program();
    bluetooth_receiver();
    print_string("HAPPY HALLOWEEN!");
    bluetooth_receiver();
    heart_program();
    bluetooth_receiver();
    eyes_program();
    bluetooth_receiver();
    print_string("DAFT PUNK");
  } else if(state == 'c') {
    clear_display();
    delay(1000);
  } else {
    Bluetooth.println("?");
  }

}

void bluetooth_receiver() {
  if (Bluetooth.available()){ //wait for data received
    String str = Bluetooth.readStringUntil('\n');
    str.trim();
    if(str.length() == 1) {
      state = str[0];
      Bluetooth.println("state: " + state);
    } else {
      print_string(str);
    } 
  }
  delay(100);
}

// print string
// --------------------------------------------
// String passed to this function will scroll across the display
// Supported characters: A-Z,!,$,?
// str        - "String to be passed across the screen"
void print_string(String str) {
  Bluetooth.println("str: " + str);
  // Add a buffer so full text can be read
  str = str + "      ";
  byte buff[Y_WIDTH][C_COUNT] = {{0x00, 0x00, 0x00, 0x00},{0x00, 0x00, 0x00, 0x00},{0x00, 0x00, 0x00, 0x00},{0x00, 0x00, 0x00, 0x00}};

  // decode each letter in the string
  for(int i=0; i<str.length(); i++) {
    int index = 27;
    // A-Z
    if((str[i] >= 65) && (str[i] <= 90)) {
      index = str[i] - 65;
    // a-z get capitalized  
    } else if((str[i] >= 97) && (str[i] <= 122)) {
      index = str[i]-97;
    // !  
    } else if(str[i] == 33) {
      index = 26;
    // ?  
    } else if(str[i] == 63) {
      index = 27;
    // $  
    } else if(str[i] == 36) {
      index = 28;
    // space
    } else if (str[i] == 32) {
      index = 29;
    // other characters = ?  
    } else {
      index = 27;
    }

    // shift each letter into the buffer, column by column thru some nasty bitwise manipulation
    for(int col=0; col<=ALPHA_WIDTH; col++) {
      for(int row=0; row<=Y_MAX; row++) {
        buff[row][3] = ((buff[row][3] >> 1) | (0x01 & buff[row][2]) << 7);
        buff[row][2] = ((buff[row][2] >> 1) | (0x01 & buff[row][1]) << 7);
        buff[row][1] = ((buff[row][1] >> 1) | (0x01 & buff[row][0]) << 7);
        buff[row][0] = ((buff[row][0] >> 1) | (0x80 & (alpha_array[(index*6)+row] << (7-col))));

        // Display the buffer after each shift
        for(int chip=0; chip<=C_MAX; chip++) {
          lc.setRow(C_MAX-chip, Y_MAX-row, buff[row][chip]);
        }
      }
      delay(75);
    }
  }
}

// animate_array
// --------------------------------------------
// arr        - 2D array of animation data.  Generated by LED Matrix Studio
// num_frames - number of frames in animation
// num_rows   - number of rows in matrix
// t          - delay (in ms) between frames
void animate_array(byte arr[][4], int num_frames, int num_rows, int t)
{
  for(int frame=0; frame<num_frames; frame++) {
    for(int row=0; row<num_rows; row++) {
      for(int chip=0; chip<4; chip++) {
        lc.setRow(3-chip, num_rows-1-row, arr[(frame*num_rows)+row][chip]);
      }
    }
    delay(t);
  }

}

// draw_pixel
// --------------------------------------------
// Draw a pixel on 32x6 matrix
//
// x  - x coordinate
// y  - y coordinate
// on - true to turn on pixel, false to turn off
void draw_pixel(int x, int y, bool on) {
  int chip = x / 8;
  int col = 7 - (x % 8);
  int row = y;
  lc.setLed(chip,row,col,on);
}

// clear_display
// --------------------------------------------
// Simply clear LED matrix of all pixels
void clear_display() {
  Bluetooth.println("clear");
  for(int index=0;index<lc.getDeviceCount();index++) {
    lc.clearDisplay(index);
  }
}

// Display Programs
// --------------------------------------------
// display programs go below here

void dot_program() {
  Bluetooth.println("dot");
  clear_display();
  // Send dot right
  for (int index=0; index<34; index++) {
    draw_pixel(index-1,2,true);
    draw_pixel(index,2,true);
    draw_pixel(index-1,3,true);
    draw_pixel(index,3,true);
    delay(50);
    draw_pixel(index-1,2,false);
    draw_pixel(index-1,3,false);
  }
  // Send dot left
  for (int index=33; index>=-1; index--) {
    draw_pixel(index-1,2,true);
    draw_pixel(index,2,true);
    draw_pixel(index-1,3,true);
    draw_pixel(index,3,true);
    delay(50);
    draw_pixel(index,2,false);
    draw_pixel(index,3,false);
  }
}

void test_program() {
  Bluetooth.println("test");
  //Test line horizontal scroll
  for(int chip=0;chip<lc.getDeviceCount();chip++) {
    for(int col=7;col>=0;col--) {
      for (int row=7;row>=0;row--) {
        lc.setLed(chip,row,col,true);
      }
      delay(50);
      for (int row=7;row>=0;row--) {
        lc.setLed(chip,row,col,false);
      }
    }
  }

  // Test line vertical scroll
  for (int row=7;row>=0;row--) {
    for(int col=7;col>=0;col--) {
      for(int chip=0;chip<lc.getDeviceCount();chip++) {
        lc.setLed(chip,row,col,true);
      }
    }
    delay(50);
    for(int col=7;col>=0;col--) {
      for(int chip=0;chip<lc.getDeviceCount();chip++) {
        lc.setLed(chip,row,col,false);
      }
    }
  }

  // Test individual pixels
  for (int row=7;row>=0;row--) {
    for(int chip=0;chip<lc.getDeviceCount();chip++) {
      for(int col=7;col>=0;col--) {
        lc.setLed(chip,row,col,true);
        delay(20);
        lc.setLed(chip,row,col,false);
      }
    }
  }
}

void static_program() {
  Bluetooth.println("static");
  int num_rows = 6;
  for (int i=0; i<50; i++) {
    for(int row=0; row<num_rows; row++) {
      for(int chip=0; chip<4; chip++) {
        lc.setRow(chip, row, random(0xFF));        
      }
    }
    delay(10);
  }
}

void heart_program() {
  Bluetooth.println("heart");
  for(int i=0;i<8;i++) {
    animate_array(heart_array, HEART_FRAMES, HEART_ROWS, HEART_DELAY);
  }
}

void eyes_program() {
  Bluetooth.println("eyes");
  for(int i=0;i<8;i++) {
    animate_array(eyes_array, EYES_FRAMES, EYES_ROWS, EYES_DELAY);
  }
}

void ok_program() {
  Bluetooth.println("ok");
  animate_array(ok_array, OK_FRAMES, OK_ROWS, OK_DELAY);
  delay(1000);
}

void ufo_program() {
  //Bluetooth.println("ufo");
  //animate_array(ufo_array, UFO_FRAMES, UFO_ROWS, UFO_DELAY);
}

void arrow_program() {
  Bluetooth.println("arrow");
  for(int i=0;i<8;i++) {
    animate_array(arrow_array, ARROW_FRAMES, ARROW_ROWS, ARROW_DELAY);
  }
}

void ekg_program() {
  Bluetooth.println("ekg");
  clear_display();
  delay(100);
  byte mask[] = {0x00, 0x00, 0x00, 0x00};
  for(int col = 0; col<=31; col++) {
    mask[col/8] = ((mask[col/8] << 1) | 0x01);
    //Serial.print(col/8);
    //Serial.print(mask[col/8], BIN);
    //Serial.print("\n");
    for(int row=0; row<EKG_ROWS; row++) {
      for(int chip=0; chip<4; chip++) {
        lc.setRow(3-chip, EKG_ROWS-1-row, ekg_array[row][chip] & mask[3-chip]);
      }
    }
    delay(50);
  }
  delay(2000);
  for(int col = 31; col>=0; col--) {
    mask[col/8] = ((mask[col/8] >> 1));
    for(int row=0; row<EKG_ROWS; row++) {
      for(int chip=0; chip<4; chip++) {
        lc.setRow(3-chip, EKG_ROWS-1-row, ekg_array[row][chip] & mask[3-chip]);
      }
    }
    delay(50);
  }
  delay(100);
}



