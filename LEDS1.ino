/*****************************************************
This is the demo sketch for the command line interface
by FreakLabs. It's a simple command line interface
where you can define your own commands and pass arguments
to them. 
*****************************************************/
#include <Cmd.h>
#include "ws2812.h" 

#define DEBUG                     // выводить отладку 
#define VER           1.00
#define TEST_TIME     200         // время горения одного светодиода в режиме теста

#define BANK            4         // Число шкафов
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN1            9
#define PIN2            8
#define PIN3            7
#define PIN4            6
#define PIN_LED         13     // светодиод занятости командой
#define PIN_TEST        A4     // пин который используется для конфигурации прошивки в режим теста

// Геометрия кассы
#define LINES           20
#define ROWS            12

#define NUMPIXELS   LINES*ROWS   // размер кассы в светодиодах


WS2812 pixels[BANK]={WS2812(NUMPIXELS),WS2812(NUMPIXELS),WS2812(NUMPIXELS),WS2812(NUMPIXELS)};
cRGB value;

uint8_t r, g, b;  // текущий цвет

inline void command_Ok()
{
 #ifdef DEBUG
  Serial.println("Wait command >");
 #endif  
 digitalWrite(PIN_LED,LOW);   // выключить светодиод
}

void setup()
{
   int i,j;
   pinMode(PIN_LED,OUTPUT);
   digitalWrite(PIN_LED,HIGH);   // включить светодиод 
   
   pixels[0].setOutput(PIN1);
   pixels[1].setOutput(PIN2);
   pixels[2].setOutput(PIN3);
   pixels[3].setOutput(PIN4);
   value.r=2; value.g=4; value.b=4;   // цвет по умолчанию 
    
 
  // init the command line and set it for a speed of 9600
  cmdInit(9600);
  #ifdef DEBUG
  Serial.println("Start . . .");
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
  cmdAdd("set", set_led);
  cmdAdd("test", test);

  for (int i=0; i<BANK; i++)  pixels[i].clear();  
 
// Если надо то перевести в режим теста
   pinMode(PIN_TEST,INPUT);
   digitalWrite(PIN_TEST,HIGH);
if(!digitalRead(PIN_TEST))  // режим теста
 {
  #ifdef DEBUG
  Serial.println("MODE: hard test >");
  #endif
  for(;;) // Бесконечный цикл
  {
    for (i=0; i<LINES; i++)
    {
     for (j=0; j<ROWS; j++)
     { 
      pixels[0].set_code_at(j*LINES+i, 255);   pixels[0].sync();
      pixels[1].set_code_at(j*LINES+i, 255);   pixels[1].sync();
      pixels[2].set_code_at(j*LINES+i, 255);   pixels[2].sync();
      pixels[3].set_code_at(j*LINES+i, 255);   pixels[3].sync();
     }
     delay(TEST_TIME);
     for (j=0; j<ROWS; j++) 
      { 
      pixels[0].set_code_at(j*LINES+i, 0);   pixels[0].sync();
      pixels[1].set_code_at(j*LINES+i, 0);   pixels[1].sync();
      pixels[2].set_code_at(j*LINES+i, 0);   pixels[2].sync();
      pixels[3].set_code_at(j*LINES+i, 0);   pixels[3].sync();
     }
   }  
  }
 }
else  command_Ok();
digitalWrite(PIN_LED,LOW);   // выключить светодиод
}

void loop()
{
  cmdPoll();
}
// КОМАНДЫ ----------------------------------------------------
void help(int arg_cnt, char **args)
{
  digitalWrite(PIN_LED,HIGH);   // включить светодиод 
  Serial.println("Define command:");
  Serial.println("help - this text");
  Serial.println("clear - off all leds");
  Serial.println("color r g b - set color for on led");
  Serial.println("on bank cell - set on cell in bank");
  Serial.println("off bank cell - set off cell in bank");
  Serial.println("set bank cell r g b - set color cell in bank ");
  Serial.println("test bank - testing hardware in bank");
  Serial.println("------------------------------------------------");
  command_Ok(); 
}
// погасить все светодиоды во всех банках
void clear(int arg_cnt, char **args)
{
   digitalWrite(PIN_LED,HIGH);   // включить светодиод 
   for (int i=0; i<BANK; i++)  pixels[i].clear(); 
   command_Ok();
}
// Установить текущий цвет цвет  
void color(int arg_cnt, char **args)
{
 digitalWrite(PIN_LED,HIGH);   // включить светодиод  
 value.r=atoi(args[1]);
 value.g=atoi(args[2]);
 value.b=atoi(args[3]);
 #ifdef DEBUG
 Serial.print("Color:");Serial.print(r);Serial.print(" ");Serial.print(g);Serial.print(" ");Serial.println(b);
 #endif
 command_Ok();
}
// Зажечь светодиод в банке установленным цветом (команда color)
void on_led(int arg_cnt, char **args)
{
digitalWrite(PIN_LED,HIGH);   // включить светодиод   
pixels[atoi(args[1])].set_crgb_at(atoi(args[2]), value);
pixels[atoi(args[1])].sync();
command_Ok();
}

void off_led(int arg_cnt, char **args)
{
digitalWrite(PIN_LED,HIGH);   // включить светодиод   
pixels[atoi(args[1])].set_code_at(atoi(args[2]), 0);
pixels[atoi(args[1])].sync();
command_Ok();
}

// Зажечь светодиод в банке цветом который передается в качестве аргуметов
void set_led(int arg_cnt, char **args)
{
cRGB val;
digitalWrite(PIN_LED,HIGH);   // включить светодиод 
val.r= atoi(args[3]); 
val.g= atoi(args[4]); 
val.b= atoi(args[5]); 
pixels[atoi(args[1])].set_crgb_at(atoi(args[2]), val);//  setPixelColor(atoi(args[2]), pixels[atoi(args[1])].Color(r,g,b)); 
pixels[atoi(args[1])].sync();
command_Ok();
}

// тест светодиодов отдельной банки
void test(int arg_cnt, char **args)
{
int i,j;
 digitalWrite(PIN_LED,HIGH);   // включить светодиод 
 for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].set_code_at(j*LINES+i, 3<<6);
   pixels[atoi(args[1])].sync();
   delay(TEST_TIME);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].set_code_at(j*LINES+i, 0);
   pixels[atoi(args[1])].sync();
 }

for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].set_code_at(j*LINES+i, 7<<3);
   pixels[atoi(args[1])].sync();
   delay(TEST_TIME);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].set_code_at(j*LINES+i, 0);
   pixels[atoi(args[1])].sync();
 }
 for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].set_code_at(j*LINES+i, 7);
   pixels[atoi(args[1])].sync();
   delay(TEST_TIME);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].set_code_at(j*LINES+i, 0);
   pixels[atoi(args[1])].sync();
 }
 for (i=0; i<LINES; i++)
 {
   for (j=0; j<ROWS; j++)  pixels[atoi(args[1])].set_code_at(j*LINES+i, 255);
   pixels[atoi(args[1])].sync();
   delay(TEST_TIME);
   for (j=0; j<ROWS; j++) pixels[atoi(args[1])].set_code_at(j*LINES+i, 0);
   pixels[atoi(args[1])].sync();
 } 
 command_Ok(); 
}


