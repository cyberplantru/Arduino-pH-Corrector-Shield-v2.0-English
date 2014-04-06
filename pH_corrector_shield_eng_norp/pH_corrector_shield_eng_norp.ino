

/*
USE_GITHUB_USERNAME = cyberplanru
*/
// CiberPlant Inc
// Http :/ / www.cyberplant.ru
// Sample code for Arduino pH Corrector Shield v2.0
// The program allows you to control the level of pH in the automatic mode
// Reads the device with a pH probe and sends signals to the pump via the Arduino pH Corrector Shield
// For feeding solution into the correction fluid
// English Translation by Jason Mitchell fuznuts@gmail.com

#include <EEPROMVar.h>
#include <LiquidCrystal.h>     // LCD Library
#include <EEPROMex.h>          // extended library EEPROM
#include <SoftwareSerial.h>    // we have to include the SoftwareSerial library, or else we can't use it.
#define rx 5                   // pin 5 for a pH Circuit
#define tx 3                   // pin 3 to work with pH Circuit
#define pumpPhup 7             // pump pH Up
#define pumpPhdown 6           // pump pH Down
SoftwareSerial myserial(rx, tx);

LiquidCrystal lcd (8, 9, 10, 11, 12, 13);   // setup LCD display
int m=0;                                    // variable for the menu screens

// Addrres storing settings in non-volatile memory Arduino:
int addressPhlow = 0;
int addressPhhigh = addressPhlow+sizeof(double);
int addressDose = addressPhhigh+sizeof(double);
int addressPhdelay = addressDose+sizeof(double);
int addressL = addressPhdelay+sizeof(int);
int addressR = addressL+sizeof(int);

// Read the settings from non-volatile memory :
double pHlow = EEPROM.readDouble(addressPhlow);
double pHhigh = EEPROM.readDouble(addressPhhigh);
double dose = EEPROM.readDouble(addressDose);
int pHdelay = EEPROM.readInt(addressPhdelay);
int l = EEPROM.readInt(addressL);
int c = 0;
int r = EEPROM.readInt(addressR);

// Arrays to menu
char * ledString[2] = {"On", "Off"};
char * control[3] = {"pH 4", "pH 7", "pH 10"};
char * mode[2] = {"auto", "manual"};

int flag = 0;                     // variable to stabilize the button

long previousMillis = 0;
long interval = pHdelay * 60000;  // delay adjustments in minutes

byte setPin = A0;                 // save button in the non-volatile memory
byte menuPin = A1;                // button to switch the menu items
byte upPin = A3;                  // increase the value of the displayed parameter
byte downPin = A2;                // decrease the value of the displayed parameter

char ph_data[20];                 // we make a 20 byte character array to hold incoming data from the pH.
char computerdata[20];            // we make a 20 byte character array to hold incoming data from a pc / mac / other.
byte received_from_computer=0;    // we need to know how many characters have been received.
byte received_from_sensor=0;      // we need to know how many characters have been received.
byte arduino_only=0;              // if you would like to operate the pH Circuit with the Arduino only and not use the serial monitor to send it commands set this to 1. The data will still come out on the serial monitor, so you can see it working.
byte startup=0;                   // used to make sure the Arduino takes over control of the pH Circuit properly.
float ph=0;                       // used to hold a floating point number that is the pH.
byte string_received=0;
void phSetLEDs(byte enabled);

void setup () {

  // Set button pins as inputs
  pinMode (menuPin, INPUT);
  pinMode (setPin, INPUT);
  pinMode (upPin, INPUT);
  pinMode (downPin, INPUT);
  // Set pins as outputs for pumps
  pinMode (pumpPhup, OUTPUT);
  pinMode (pumpPhdown, OUTPUT);
  
     Serial.begin (38400);       // enable the hardware serial port
     myserial.begin (38400);
     
  // Set the number of columns and rows display
  lcd.begin (8, 2);
  // Display the welcome
  lcd.print ("ver 2.0");
  // Delay
  delay (500);
  lcd.clear ();
}


void loop () {
    if (myserial.available()>0){                                      // if we see that the pH Circuit has sent a character.
     received_from_sensor=myserial.readBytesUntil(13,ph_data,20);     // we read the data sent from ph Circuit untill we see a <CR>. We also count how many character have been recived.
     ph_data [received_from_sensor]=0;                                // we add a 0 to the spot in the array just after the last character we recived. This will stop us from transmiting incorrect data that may have been left in the buffer.
     string_received=1;                                               // a flag used when the arduino is controlling the pH Circuit to let us know that a complete string has been received.
    }
 if (digitalRead(menuPin)== LOW && flag==1)                           // stabilization buttons
  {
  flag=0;
  }

  // Handle pressing the menu button
  if (digitalRead(menuPin)== HIGH && flag==0)
  {
  m++;           // increase the variable level menu
  flag=1;
  if (m>7)       // if the level is greater than 3
  {
  m=0;           // then back to the top
  }

  delay (100);
  lcd.clear ();
  
 }
  // Heightening of parameter pHlow
if (digitalRead(upPin)== HIGH && m==1)
  {

  pHlow += 0.1;

  if (pHlow > 14)
  {
  pHlow = 1;
  }
  delay (100);
  lcd.clear ();
  }
  if (digitalRead(setPin)== HIGH && m==1)
  { 
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("SAVE");
  lcd.setCursor (0, 1);
  lcd.blink ();
  delay (3000); // save in nonvolatile memory
  EEPROM.updateDouble(addressPhlow, pHlow);
  lcd.noBlink ();
  lcd.print ("COMPLETE");
  delay (2000) ;
  lcd.clear ();
  }
  


if (digitalRead(upPin)== HIGH && m==2)     // + to pHhigh
  {
  pHhigh += 0.1;
  if (pHhigh > 14)
  {
  pHhigh = 14 ;
  }
  delay (100);
  lcd.clear ();
  }
  if (digitalRead(setPin)== HIGH && m==2)
  { 
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("SAVE");
  lcd.setCursor (0, 1);
  lcd.blink ();
  delay (3000);        // save in nonvolatile memory
  EEPROM.updateDouble(addressPhhigh, pHhigh);
  lcd.noBlink ();
  lcd.print ("COMPLETED");
  delay (2000);
  lcd.clear ();

  }
  
    // + To dose
    if (digitalRead(upPin)== HIGH && m==3)
  {
  dose += 0.1;
  if (dose > 10)       // if the dosage is reached chapel in 10 ml
  {
  dose = 10;           // then return it to 0
  }
  delay (100);
  lcd.clear ();
  }
  if (digitalRead(setPin)== HIGH && m==3)
  { 
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("SAVE");
  lcd.setCursor (0, 1);
  lcd.blink ();
  delay (3000);       // save in nonvolatile memory
  EEPROM.updateDouble(addressDose, dose);
  lcd.noBlink ();
  lcd.print ("COMPLETE");
  delay (2000);
  lcd.clear ();
  }
  // for pH delay
      if (digitalRead(upPin)== HIGH && m==4)
  {
  pHdelay++;
  if (pHdelay>60)     // if the variable has reached the chapel in 60 minutes
  {
  pHdelay=0;          // then return it to 0
  }
  delay (100);
  lcd.clear ();
  }
  if (digitalRead(setPin)== HIGH && m==4)
  {
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("SAVE");
  lcd.setCursor (0, 1);
  lcd.blink ();
  delay (3000);       // save in nonvolatile memory
  EEPROM.updateInt(addressPhdelay, pHdelay);
  lcd.noBlink ();
  lcd.print ("COMPLETE");
  delay (2000) ;
  lcd.clear ();
  }
  // Alarm LED
      if (digitalRead(upPin)== HIGH && m==5)
  {
  l++;
  if (l> 1)
  {
  l=1;
  }
  
  delay (100);
  lcd.clear ();
  }
  if (digitalRead(setPin)==HIGH && m==5 )
  {
  lcd.clear ();
  lcd.setCursor (0,0);
  lcd.print ("SAVE");
  lcd.setCursor (0,1);
  lcd.blink ();
  delay (3000);         // save in nonvolatile memory
  EEPROM.updateInt(addressL,l);
  lcd.noBlink ();
  lcd.print ("COMPLETE");
  delay (2000);
  lcd.clear ();
  }
  //  calibrate
        if (digitalRead(upPin)== HIGH && m==6)
  {
  c++;
  if (c>2)
  {
  c=2;
  }
  delay (100);
  lcd.clear ();
  }
  
  if (digitalRead(setPin)== HIGH && m==6)
  {
  lcd.clear ();
  lcd.setCursor (0,0);
  lcd.print (control[c]);
  lcd.setCursor (0,1);
  lcd.blink ();
  delay (3000) ;
  if (c==0)
  {
    myserial.write("f\r");       // calibrate to a pH of 4
  }
    if (c==1)
  {
    myserial.write("s\r");       // calibrate to a pH of 7
  }
      if (c==2)
  {
    myserial.write("t\r");       // calibrate to a pH of 10.00
  }
    
  lcd.noBlink ();
  lcd.print ("COMPLETE");
  delay (2000);
  lcd.clear ();
  }
  
  
  //  Mode
          if (digitalRead(upPin)== HIGH && m==7)
  {
  r++;
  if (r > 1)
  {
  r = 1 ;
  }
  delay (100);
  lcd.clear ();
  }
  if (digitalRead(setPin)== HIGH && m==7)
  {
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("SAVE");
  lcd.setCursor (0, 1);
  lcd.blink ();
  delay (3000); // save in nonvolatile memory
  EEPROM.updateInt(addressR, r);
  lcd.noBlink ();
  lcd.print ("COMPLETE");
  delay (2000);
  lcd.clear ();
  }
  
  
      // decrease the values for pH Low
    if (digitalRead(downPin)== HIGH && m==1)
  {
  pHlow -= 0.1;
  if (pHlow < 1)
  {
  pHlow = 1;

  }
  delay (100);
  lcd.clear ();

  }
  // pH High
      if (digitalRead(downPin)==HIGH && m==2)
  {
  pHhigh -= 0.1;
  if (pHhigh < 1)
  {
  pHhigh = 1;
  }
  delay (100);
  lcd.clear ();

  }
      // - For dose
    if (digitalRead(downPin)== HIGH && m==3)
  {
  dose -= 0.1;
  if (dose < 0)
  {
  dose = 0;
  }
  delay (100);
  lcd.clear ();

  }

   if (digitalRead(downPin)== HIGH && m==4)     // for delay
  {
  pHdelay --;
  if (pHdelay<1)
  {
  pHdelay = 1;
  }
  delay (100);
  lcd.clear ();

  }
if (digitalRead(downPin)== HIGH && m==5)       // for LED
  {
  l --;
  if (l<0)
  {
  l=0;
  }
  
  delay (100);
  lcd.clear ();

  }
  // calibrate
        if (digitalRead(downPin)== HIGH && m==6)
  {
  c --;
  if (c<0)
  {
  c = 0;
  }
  delay (100);
  lcd.clear ();
  }
  
  
  
  
  // Mode
          if (digitalRead(downPin)== HIGH && m==7)
  {
  r--;
  if (r<0)
  {
  r=0;
  }
  delay (100);
  lcd.clear ();

  }


  
  // output menu
  if (m==0)
  {
  lcd.setCursor (0,0);
  lcd.print ("pH  ");
  lcd.print (ph_data);
  lcd.setCursor (0,1);
  lcd.print ("");
  lcd.print (mode[r]);


  }
  else if (m==1)
  {
  lcd.setCursor (0,0);
  lcd.print ("pH low");
  lcd.setCursor (0,1);
  lcd.print ("");
  lcd.print (pHlow);
  }
  
 else if (m==2)
  {
  lcd.setCursor (0,0);
  lcd.print ("pH high");
  lcd.setCursor (0,1);
  lcd.print ("");
  lcd.print (pHhigh);
  }
 else if (m==3)
  {
  lcd.setCursor (0,0);
  lcd.print ("dose");
  lcd.setCursor (0,1);
  lcd.print (dose);
  lcd.print ("ml");
  }
  else if (m==4)
  {
  lcd.setCursor (0,0);
  lcd.print ("delay");
  lcd.setCursor (0,1);
  lcd.print (pHdelay);
  lcd.print (" min");

  }
  else if (m==5)
  {
  lcd.setCursor (0,0);
  lcd.print ("LED");
  lcd.setCursor (0,1);
  lcd.print (ledString[l]);
  }
   else if (m==6)
  {
  lcd.setCursor (0,0);
  lcd.print ("setting");
  lcd.setCursor (0,1);
  lcd.print (control[c]);
  }
  else if (m==7)
  {
  lcd.setCursor (0,0);
  lcd.print ("mode");
  lcd.setCursor (0,1);
  lcd.print (mode[r]);
  }
  
      if (l==1) // if selected "LED off" - turn off the LED
    {
  myserial.write ("L0\r");
    }
    
    
  unsigned long currentMillis = millis();       // for reading interval pH Delay
  if (currentMillis - previousMillis> interval)
  {  
    previousMillis = currentMillis;
if (pHlow>atof(ph_data) && r==0)               // if the automatic mode is selected and the pH dropped
                                               // Parameter below pH Low
{

  digitalWrite (pumpPhup,HIGH);                // turn on the pump pH UP
  delay (dose*1600);                           // * on selected dosage peristaltic pump performance
                                               // 1 ml pumped here for 3 seconds
  digitalWrite (pumpPhup,LOW);                 // turn off the pump
  }
       



  if (pHhigh<atof(ph_data) && r==0)           // if the automatic mode is selected and the pH rose
                                              // Parameter above pH High
{

  digitalWrite (pumpPhdown,HIGH);
  delay (dose*1600);
  digitalWrite (pumpPhdown,LOW);
  }
  if (l==0 && pHhigh<atof(ph_data) || pHlow>atof(ph_data))     // If you selected "LED on"
                                                               // When you exit for these parameters pH
                                                               // LED turns
    {
    myserial.write ("L1\r");
    }
    if (pHhigh>atof(ph_data) && pHlow<atof(ph_data) && l==0)   // If the pH is within the limits ,
                                                               // LED off
    {
    myserial.write ("L0\r");
    }
       


  }


 }



