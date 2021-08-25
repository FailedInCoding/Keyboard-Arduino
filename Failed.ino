// ----------------------------
// Standard Libraries
// ----------------------------
#include "Arduino.h"
#include <Wire.h>
#include <Keyboard.h>
#include <Mouse.h> //there are some mouse move functions for encoder_Mode 2 and 3

int check_State = 0;

#include <Encoder.h>

Encoder RotaryEncoderB(14, 15); //the RIGHT encoder (encoder B)

#include <Keypad.h>

//Define the rols column matrix roots
const byte ROWS = 2; //two rows
const byte COLS = 4; //four columns


//define the physical matrix layout
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'B'},  //  the keyboard hardware is a 2x4 grid... 
  {'5', '6', '7', '8'},

};
// The library will return the character inside this array when the appropriate
// button is pressed then look for that case statement. This is the key assignment lookup table.
// Layout(key/button order) looks like this
//     |------------------------|
//     |   [6/5]*               |     *TRS breakout connection. Keys 5 and 6 are duplicated at the TRS jack
//     |      [1] [2] [3] [4]   |     * Encoder B location = Key[4]
//     |      [5] [6] [7] [8]   |     
//     |------------------------|


// Variables that will change:
int modePushCounter = 0;     // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int mouseMove;
String password;             // string for rando password generator, its a global variable because i might do something fancy with it?

long positionEncoderA  = -999; //encoderA LEFT position variable
long positionEncoderB  = -999; //encoderB RIGHT position variable

const int ModeButton = A0;    // the pin that the Modebutton is attached to
const int pot = A1;           // pot for adjusting attract mode demoTime or mouseMouse pixel value
const int Mode1= A2;
const int Mode2= A3;          //Mode status LEDs

byte rowPins[ROWS] = {4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9 }; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void encoderB_Mode0(){
  long newPos = RotaryEncoderB.read()/4; //When the encoder lands on a valley, most encoders use an increment of 4.
  if (newPos != positionEncoderB && newPos > positionEncoderB) {
    positionEncoderB = newPos;
     Keyboard.press(KEY_DOWN_ARROW);
    Keyboard.release(KEY_DOWN_ARROW);                      }

  if (newPos != positionEncoderB && newPos < positionEncoderB) {
    positionEncoderB = newPos;

    Keyboard.press(KEY_UP_ARROW);
    Keyboard.release(KEY_UP_ARROW);                         }
}

void checkModeButton(){
  buttonState = digitalRead(ModeButton);
  if (buttonState != lastButtonState) { // compare the buttonState to its previous state
    if (buttonState == LOW) { // if the state has changed, increment the counter
      // if the current state is LOW then the button cycled:
      modePushCounter++;
    } 
    delay(50); // Delay a little bit to avoid bouncing
  }
  lastButtonState = buttonState; // save the current state as the last state, for next time through the loop
   if (modePushCounter >3){ //reset counter after 4 presses (remember we start counting at 0) change this number to add more modes
      modePushCounter = 0;}
}

void loop(){
char key = keypad.getKey();
mouseMove = (analogRead(pot)); //reading the analog input, pot = pin A1
mouseMove = map(mouseMove, 0,1023, 1,124); //remap the analog pot values fron 1 to 124
checkModeButton();

  switch (modePushCounter) { // switch between keyboard configurations:
    case 0:    //  Application Alpha or MODE 0 ========================================================================
      encoderB_Mode0();
       if (key) {
   //Serial.println(key);
    switch (key) {
      case '2': Keyboard.press(KEY_LEFT_CTRL);   
                Keyboard.press(0x25);                        
                break;
                
      case '3': Keyboard.press(KEY_LEFT_CTRL);   
                Keyboard.press(0x27);                        
                break;

                //4 is the encoder big man :)
      
      case '5': Keyboard.press(KEY_LEFT_SHIFT);
                Keyboard.press(KEY_RIGHT_CTRL);
                break;

      case '6': Keyboard.press(KEY_LEFT_GUI); 
                Keyboard.press('r'); 
                Keyboard.release(KEY_LEFT_GUI); 
                Keyboard.release('r'); 
                delay(50);
                Keyboard.println("cmd");
                break;}
                
      case '7': Keyboard.press(KEY_LEFT_ALT);  
                Keyboard.press(KEY_LEFT_CTRL);  
                Keyboard.print('0x46');                         
                break;
                
      case '8': Keyboard.press(KEY_LEFT_GUI); 
                Keyboard.press('r'); 
                Keyboard.release(KEY_LEFT_GUI); 
                Keyboard.release('r'); 
                delay(50);
                Keyboard.println("calc");
                break;
                          }

  delay(50); Keyboard.releaseAll(); // this releases the buttons 
  }
      
//=============Encoder B Function ====== Set 0 =========================================================
#define DEBUG

#define pinA 14
#define pinB 15

#define mutePin 11
bool isMutePressed = false;
bool isMuted = false;
bool ignoreMute = false;

void encodersetup()
{
#ifdef DEBUG
        Serial.begin(115200);
        while (!Serial)
                ;
        Serial.println("The setup starts");
#endif

        pinMode(pinA, INPUT);
        pinMode(pinB, INPUT);

        pinMode(mutePin, INPUT_PULLUP);

        Keyboard.begin();
}

void loop(){
        if (!isMutePressed
                && digitalRead(mutePin) == LOW)
        {
                isMutePressed = true; 
        }

        if (digitalRead(pinA) == LOW)
        {
                if (isMutePressed)
                {
                        ignoreMute = true;
                }
                 ///clockwise
                if (digitalRead(pinB) != LOW)
                {
#ifdef DEBUG
                        Serial.println("U");
#endif
                        Keyboard.press(KEY_LEFT_ALT);
                        Keyboard.press(KEY_LEFT_CTRL);
                        Keyboard.press(KEY_LEFT_SHIFT);
                        Keyboard.press(0xDE);
                        delay(100);
                        Keyboard.releaseAll();

                        if (isMutePressed)
                        {
#ifdef DEBUG
                                Serial.println("U2");
#endif
                                Keyboard.press(KEY_LEFT_ALT);
                                Keyboard.press(KEY_LEFT_CTRL);
                                Keyboard.press(KEY_LEFT_SHIFT);
                                Keyboard.press(0xDE);
                                delay(100);
                                Keyboard.releaseAll();
                        }
                } else
                {
#ifdef DEBUG
                         Serial.println("D");
#endif
                         Keyboard.press(KEY_LEFT_ALT);
                         Keyboard.press(KEY_LEFT_CTRL);
                         Keyboard.press(KEY_LEFT_SHIFT);
                         Keyboard.press(0xBF);
                         delay(100);
                         Keyboard.releaseAll();

                         if (isMutePressed)
                         {
#ifdef DEBUB
                                Serial.println("D2");
#endif
                                Keyboard.press(KEY_LEFT_ALT);
                                Keyboard.press(KEY_LEFT_CTRL);
                                Keyboard.press(KEY_LEFT_SHIFT);
                                Keyboard.press(0xBF);
                                delay(100);
                                Keyboard.releaseAll();

                         }
                }

                //wait until rotation to next detent is complete
                while (digitalRead(pinA) != HIGH)
                {
                        //debounce delay
                        delay(5);
                }
        }

        //mute button previoulsy pressed// ?~? //
        if (isMutePressed)
        {
                //mute button now released?
                if (digitalRead(mutePin) == HIGH)
                {
                        //resets flag and waits for a debounce interval
                        isMutePressed = false;

                        //should we ignore the mute request because
                        //we did a fast volume change?
                        if (ignoreMute)
                        {
                                //resets that flag but nothing else
                                ignoreMute = false;
                        }
                        else
                        {
                                Keyboard.press(KEY_LEFT_ALT);
                                Keyboard.press(KEY_LEFT_CTRL);
                                Keyboard.press(KEY_LEFT_SHIFT);
                                Keyboard.press(0x77);
                                delay(100);
                                Keyboard.releaseAll();
#ifdef DEBUG
                                Serial.println(isMuted ? "Unmuted" : "Muted");
#endif
                                isMuted = !isMuted;
                        }
                }
        }
}
