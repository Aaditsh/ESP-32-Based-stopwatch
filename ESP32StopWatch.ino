#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include <EEPROM.h>
#define EEPROM_SIZE 3

// DEFINING TOGGLE BUTTONS
#define B1 18
#define B2 19
#define B3 5


int B1_current_status;
int B2_current_status;
int B3_current_status;

int B1_previous_status;
int B2_previous_status;
int B3_previous_status;

int start_status;


int hh = 0; //HOURS
int mm = 0; //MINUTES
int ss = 0; //SECONDS


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




void setup()
{
  Serial.begin(9600);
  
  EEPROM.begin(EEPROM_SIZE);

  pinMode(B1,INPUT_PULLUP);
  pinMode(B2,INPUT_PULLUP);
  pinMode(B3,INPUT_PULLUP);
  
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);} // Don't proceed, loop forever
  
  
  B1_current_status =digitalRead(B1);
  B2_current_status =digitalRead(B2);
  B3_current_status =digitalRead(B3);


  display.clearDisplay();


  display.setTextSize(4);
  display.println("GOOD");
  display.println("LUCK");


  hh = EEPROM.read(0);
  mm = EEPROM.read(1);
  ss = EEPROM.read(2);


  start_status=0;
  
  

  
  display.clearDisplay();

  
}

void loop() {


  B1_previous_status = B1_current_status;
  B1_current_status=digitalRead(B1);
  B2_previous_status = B2_current_status;
  B2_current_status=digitalRead(B2);
  B3_previous_status = B3_current_status;
  B3_current_status=digitalRead(B3);

  


  //FUNCTIONING OF START/STOP BUTTON
  if(B2_current_status==LOW & B2_previous_status==HIGH){
    if(start_status==1){
      start_status=0;}

    else{start_status=1;
    }
      
      }

    
  //FUNCTIONING OF 10 minute ADDER
  if(B3_current_status==LOW && B3_previous_status==HIGH && start_status==0){
      mm = mm+10;}


  //FUNCTIONING OF RESET BUTTON  
  if(start_status==0){
    if(B1_current_status==LOW && B1_previous_status==HIGH){
      hh = 0;
      ss= 0;
      mm = 0;
      }
    }

    
   //TIME ADDER
   if(start_status==1){
    ss=ss+1;}

   if(ss>=60){
    mm=mm+1;
    ss=0;}

   if(mm>=60){
    hh=hh+1;
    mm=0;}



  Serial.println(start_status);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(3);
  display.print(hh);
  display.print(":");
  display.println(mm);
  display.println(ss);
  display.setTextSize(2);
  display.println("STOPWATCH");
  display.display();
  delay(980);
  display.clearDisplay();


  //COMMITING DATA TO MEMORY FOR POWER CUTS


  EEPROM.write(0, hh);
  EEPROM.write(1, mm);
  EEPROM.write(2, ss);
  EEPROM.commit();

  

  
}
