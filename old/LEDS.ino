/*****************************************************
This is the demo sketch for the command line interface
by FreakLabs. It's a simple command line interface
where you can define your own commands and pass arguments
to them. 
*****************************************************/
#include <Cmd.h>
#include "lib.h"

#define DEBUG
#define BANK            4         // Число шкафов
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN1            9
#define PIN2            8
#define PIN3            7
#define PIN4            6

// How many NeoPixels are attached to the Arduino?
#define LINES           20
#define ROWS            12

#define NUMPIXELS   LINES*ROWS

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//NeoPixel pixels1 = NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);
NeoPixel pixels[BANK]={NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800),
                                NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800),
                                NeoPixel(NUMPIXELS, PIN3, NEO_GRB + NEO_KHZ800),
                                NeoPixel(NUMPIXELS, PIN4, NEO_GRB + NEO_KHZ800)
                               };


uint8_t r, g, b;  // текущий цвет

void setup()
{
  // цвет по умолчанию 
  r=150; g=150; b=150;
    
  // set the led pin as an output. its part of the demo.
 // pinMode(led_pin, OUTPUT); 
 
  // init the command line and set it for a speed of 57600
  cmdInit(9600);
  #ifdef DEBUG
  Serial.println("Ready command . . .");
  #endif
  // add the commands to the command table. These functions must
  // already exist in the sketch. See the functions below. 
  // The functions need to have the format:
  //
  // void func_name(int arg_cnt, char **args)
  //
  // arg_cnt is the number of arguments typed into the command line
  // args is a list of argument strings that were typed into the command line
  cmdAdd("help", help);
  cmdAdd("clear", clear);
  cmdAdd("color", color);
  cmdAdd("on", on_led);
  cmdAdd("off", off_led);
  cmdAdd("test", test);

 // pixels.begin(); // This initializes the NeoPixel library.
  for (int i=0; i<BANK; i++)  pixels[i].begin();  
  clear(0,NULL);
}

void loop()
{
  cmdPoll();
}
// КОМАНДЫ ----------------------------------------------------
void help(int arg_cnt, char **args)
{
  Serial.println("User command");
  Serial.println("help - this text");
  Serial.println("clear - off all leds");
  Serial.println("color r g b - set color for on led");
  Serial.println("on bank cell - set on cell in bank");
  Serial.println("off bank cell - set off cell in bank");
  Serial.println("test bank - testing hardware in bank");
  Serial.println("------------------------------------------------");
 
}
// погасить все светодиоды во всех банках
void clear(int arg_cnt, char **args)
{
    for (int j=0; j<NUMPIXELS; j++) // Погасить пиксель
     {
      for (int i=0; i<BANK; i++)  pixels[i].setPixelColor(j, pixels[i].Color(0,0,0));  
     }

     for (int i=0; i<BANK; i++)   pixels[i].show();
  //   pixels1.show(); // This sends the updated pixel color to the hardware.
}
// Установить текущий цвет цвет  
void color(int arg_cnt, char **args)
{
 r=atoi(args[1]);
 g=atoi(args[2]);
 b=atoi(args[3]);
 #ifdef DEBUG
 Serial.print("Color:");Serial.print(r);Serial.print(" ");Serial.print(g);Serial.print(" ");Serial.println(b);
 #endif
}
// Зажечь светодиод в банке установленным цветом
void on_led(int arg_cnt, char **args)
{
pixels[atoi(args[1])].setPixelColor(atoi(args[2]), pixels[atoi(args[1])].Color(r,g,b)); 
pixels[atoi(args[1])].show();
}

void off_led(int arg_cnt, char **args)
{
pixels[atoi(args[1])].setPixelColor(atoi(args[2]), pixels[atoi(args[1])].Color(0,0,0)); // Погасить пиксель
pixels[atoi(args[1])].show();
}

// тест светодиодов отдельной банки
void test(int arg_cnt, char **args)
{
int i,j;
 for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(150,0,0));
   pixels[atoi(args[1])].show();
   delay(500);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(0,0,0));
   pixels[atoi(args[1])].show();
 }
 for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(0,150,0));
   pixels[atoi(args[1])].show();
   delay(500);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(0,0,0));
   pixels[atoi(args[1])].show();
 }
  for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(0,0,150));
   pixels[atoi(args[1])].show();
   delay(500);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(0,0,0));
   pixels[atoi(args[1])].show();
 }
  for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(150,150,150));
   pixels[atoi(args[1])].show();
   delay(500);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].setPixelColor(j*LINES+i, pixels[atoi(args[1])].Color(0,0,0));
   pixels[atoi(args[1])].show();
 } 
  
}



