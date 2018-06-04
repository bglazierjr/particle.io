#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

int clk_sync = D1; //Pi GPIO17, Header Pin 11
int pin11 = D4;  //Pi GPIO5, Header Pin 29
int pin12 = D5;  //Pi GPIO6, Header Pin 31
int pin13 = D13;  //Pi GPIO12, Header Pin 32
int pin14 = D6;  //Pi GPIO13, Header Pin 33
int pin15 = D7;  //Pi GPIO19, Header Pin 35
int pin16 = D8;  //Pi GPIO26, Header Pin 37


std::map<std::string, int> clks; //mapping to determine clock for a specified button
std::map<std::string, int> pins; //mapping to determine gpio for a specified button


//Method: SendButton
//Argument: String - button name as text, see map initialization for complete list of available buttons
//Action: Gets the GPIO on the PI that is attached to the provided button and gets the CLK for the particular button.
//Generates a pulse, synchronized to CLK1 and offset accordingly, on the specified pin as to simulate a button press.
void sendButton(std::string button){
    
    Serial.println("Send Button entered");
    int clk_offset = clks[button];
    int pin = pins[button];

    Serial.print("clk=");
    Serial.println(clk_offset, DEC);  // print as an ASCII-encoded decimal
    Serial.print("GPIO#=");
    Serial.println(pin, DEC);  // print as an ASCII-encoded decimal
    
    
    pinMode(pin, OUTPUT); //Ready to use pin so switch it from input to output
    
    //wait for a HIGH
    while (!(digitalRead(clk_sync)));
    
    //wait for a LOW
    Serial.printlnf("ms: %d ", millis()); //report the time for debug purposes
    while (digitalRead(clk_sync));
    
    //wait for another HIGH
    Serial.printlnf("ms: %d ", millis()); //report the time for debug purposes
    while (!(digitalRead(clk_sync)));
    
    
    Serial.printlnf("ms: %d ", millis());//report the time for debug purposes
    
    //if CLK1, no need to wait, after detecting clock edge HIGH, we should within CLK1.
    if (clk_offset != 1) {
        //Wait one pulse width for each CLK after CLK1.
       wait_us(2000 * (clk_offset -1));
    }
    
    
    //I've chosen 8 pulse to generate for a button press which is in the range for a single button press.
    //You can use less pulses or more, doesn't really matter as long as it is in the range.
    for (int i = 0; i < 8; i++) {
        pinSetFast(pin);
        wait_us(2000); //pulse-width measured by logic controller
        pinResetFast(pin);
        wait_us(12010); //time between pulses measured by logic controller
    };
    
    pinMode(pin, INPUT); //pins must be left as INPUT or the interfere with the physical use of the keypad
    Serial.println("Send Button completed");
    
}


//Method: setBake
//Argument: const char pointer - number passed from IFTTT applet from Google Assistant command "set oven to bake at # degrees".
//Action: Executes a series of button presses to simulate a user setting the oven to bake at a certain temperature. 
void setBake(const char *data){
   sendButton("bake"); //tell the Oven to bake
   wait_ms(500); //wait 0.5 seconds
   
   std::stringstream ss;
   ss << data;
   std::string str = ss.str(); //used to convert const char to string.
    
    //iterate through each character in the string.
   for(std::string::size_type i = 0; i < str.size(); ++i) {
       sendButton(std::string(1, str[i])); //send as a keypress
       wait_ms(500); //wait 0.5 seconds
  }
  
  //todo: sendButton("start") to actually begin the bake process. 
  //This key doesn't appear to be working.
    
}


// setup() is run only once, it's where we set up GPIO and initialise peripherals
void setup() {
    
    //Open Particle-agent serial connection for terminal debugging
    Serial.begin(9600);
    Serial.println("Entering Setup");
    
//Create a map of buttons to clocks for easy lookup
  clks.insert(std::make_pair("oven light", 1));    
  clks.insert(std::make_pair("9", 1));  
  clks.insert(std::make_pair("6", 1));  
  clks.insert(std::make_pair("3", 1));
  clks.insert(std::make_pair("start", 1));
  clks.insert(std::make_pair("0", 2));
  clks.insert(std::make_pair("8", 2));
  clks.insert(std::make_pair("5", 2));
  clks.insert(std::make_pair("2", 2));
  clks.insert(std::make_pair("options", 2));
  clks.insert(std::make_pair("clock", 3));
  clks.insert(std::make_pair("7", 3));
  clks.insert(std::make_pair("4", 3));
  clks.insert(std::make_pair("1", 3));
  clks.insert(std::make_pair("convect broil", 4));
  clks.insert(std::make_pair("other", 4));
  clks.insert(std::make_pair("broil", 5));
  clks.insert(std::make_pair("bread proof", 5));
  clks.insert(std::make_pair("keep warm", 5));
  clks.insert(std::make_pair("convect bake", 5));
  clks.insert(std::make_pair("bake", 5));
  clks.insert(std::make_pair("roast", 6));
  clks.insert(std::make_pair("baked goods", 6));
  clks.insert(std::make_pair("pizza", 6));
  clks.insert(std::make_pair("meats", 6));
  
      
//Create a map of buttons to pins for easy lookup
  pins.insert(std::make_pair("oven light", pin13));    
  pins.insert(std::make_pair("9", pin14));  
  pins.insert(std::make_pair("6", pin15));  
  pins.insert(std::make_pair("3", pin16));
  pins.insert(std::make_pair("start", pin12));
  pins.insert(std::make_pair("0", pin13));
  pins.insert(std::make_pair("8", pin14));
  pins.insert(std::make_pair("5", pin15));
  pins.insert(std::make_pair("2", pin16));
  pins.insert(std::make_pair("options", pin12));
  pins.insert(std::make_pair("clock", pin13));
  pins.insert(std::make_pair("7", pin14));
  pins.insert(std::make_pair("4", pin15));
  pins.insert(std::make_pair("1", pin16));
  pins.insert(std::make_pair("convect broil", pin15));
  pins.insert(std::make_pair("other", pin16));
  pins.insert(std::make_pair("broil", pin11));
  pins.insert(std::make_pair("bread proof", pin12));
  pins.insert(std::make_pair("keep warm", pin14));
  pins.insert(std::make_pair("convect bake", pin15));
  pins.insert(std::make_pair("bake", pin16));
  pins.insert(std::make_pair("roast", pin11));
  pins.insert(std::make_pair("baked goods", pin13));
  pins.insert(std::make_pair("pizza", pin14));
  pins.insert(std::make_pair("meats", pin15));
  
  Serial.println("Maps populated"); //debug message
  
 //Clock Synchronization Pin is an input
  pinMode(clk_sync, INPUT); 
  
 //All keypad pins must be inputs in idle state else they interfere with the keypad
  pinMode(pin11, INPUT); 
  pinMode(pin12, INPUT); 
  pinMode(pin13, INPUT); 
  pinMode(pin14, INPUT); 
  pinMode(pin15, INPUT);
  pinMode(pin16, INPUT);

  
 // pinMode(pin16, INPUT_PULLDOWN); 
  Serial.println("Pins initialized");
  
  // Subscribe to events published by IFTTT using Particle.subscribe
  Particle.subscribe("oven_bake", bakeHandler); //Set Bake function declaration
  Particle.subscribe("oven_light", lightHandler); //set function declaration
  
  Particle.subscribe("oven_bake_convect", bakeConvectHandler); //Set Bake function declaration  - not used yet 
  Particle.subscribe("oven_cancel", cancelHandler); //set function declaration - not used yet 

}

//Function to wait us
void wait_us(unsigned long us) {
   unsigned long start = micros(); //get time in us that this app has been running
   while(micros() - start <  us) {
      //do nothing until our timer has elapsed
   }
}

//Function to wait ms
void wait_ms(unsigned long ms) {
   unsigned long start = millis(); //get time in ms that this app has been running
   while(millis() - start <  ms) {
      //do nothing until our timer has elapsed
   }
}

// loop() runs continuously, it's our infinite loop.
void loop() {
    //the code does nothing after setup other than listen for IFTTT events.
}

//our events are called when IFTTT applets are triggered
void bakeHandler(const char *event, const char *data)
{
    setBake(data);
}

//our events are called when IFTTT applets are triggered
void lightHandler(const char *event, const char *data)
{
    sendButton("oven light");
}

void bakeConvectHandler(const char *event, const char *data)
{ 

}

void cancelHandler(const char *event, const char *data)
{

}




