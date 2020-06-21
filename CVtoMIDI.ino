
#include <MIDI.h> 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MIDICHANNEL 1
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define TEXTLINEHEIGHT 11
#define OSCHEIGHT 25
#define CCMAX 127

int oldy0=SCREEN_HEIGHT-1-OSCHEIGHT;
int oldy1=SCREEN_HEIGHT-1;

int ccNo=0;
int analogmax0=100;  // auto range
int analogmax1=100;

bool led=false;

MIDI_CREATE_DEFAULT_INSTANCE();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void textoutln(int line, const String& text) {
  
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, TEXTLINEHEIGHT*line);
  display.println(text); 
}

void clearln(int line) {
   display.fillRect(0,TEXTLINEHEIGHT*line,SCREEN_WIDTH, TEXTLINEHEIGHT*(line+1)-1, SSD1306_BLACK);
}

String followSpace(int v) {
  if (v<10)
     return String(v)+"  ";
  if (v<100)
     return String(v)+" "; 
     
 return String(v);     
}
 
// the setup function runs once when you press reset or power the board
void setup() {

  MIDI.begin();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLUP); 
  pinMode(3,INPUT_PULLUP);   
  
  digitalWrite(LED_BUILTIN, LOW);   

  attachInterrupt(digitalPinToInterrupt(2), intd2, FALLING   );
  attachInterrupt(digitalPinToInterrupt(3), intd3, FALLING   );
 
  display.clearDisplay();
   
}

void invertLED()
{
  led=!led;
  if (led)
  digitalWrite(LED_BUILTIN, HIGH);
  else
  digitalWrite(LED_BUILTIN, LOW);
  
}

void intd2() {  
  invertLED();
  ccNo++;
  if (ccNo>CCMAX) 
      ccNo=0;   
  
}

void intd3() {
  invertLED();
  ccNo--;
  if (ccNo<0) 
      ccNo=CCMAX;     
}

int ccToY(float cc) {
  return SCREEN_HEIGHT-1-min(cc*OSCHEIGHT/CCMAX,OSCHEIGHT);
}

int analogToCC(float v, float anamax) {
  return min(v*(CCMAX+1)/anamax,CCMAX);
}

// the loop function runs over and over again forever
void loop() {

for (int lx=0; lx<SCREEN_WIDTH; lx++) {
  
  int v0 = analogRead(A0);
  int v1 = analogRead(A1);

  // autorange
  if (v0>analogmax0) 
    analogmax0=v0;
  if (v1>analogmax1) 
    analogmax1=v1;
    

  int cc0=analogToCC(v0,analogmax0);
  int cc1=analogToCC(v1,analogmax1);

  // Serial.println(String(lx));

  clearln(0);
  textoutln(0,"CC="+followSpace(ccNo)+" A="+followSpace(cc0)+" B="+String(cc1)); 
  
  int ly0=ccToY(cc0)-OSCHEIGHT;
  int ly1=ccToY(cc1);

  for (int lxb=lx; lxb<lx+10; lxb++)
     display.drawLine(lxb,SCREEN_HEIGHT-1,lxb,TEXTLINEHEIGHT,SSD1306_BLACK); // clean previewous
  
  display.drawLine(lx-1,oldy0,lx,ly0,SSD1306_WHITE);
  display.drawLine(lx-1,oldy1,lx,ly1,SSD1306_WHITE);
  
  oldy0=ly0;
  oldy1=ly1;
  
  display.display();  

  MIDI.sendControlChange(ccNo,cc0,MIDICHANNEL);
  MIDI.sendControlChange(ccNo+1,cc1,MIDICHANNEL);
     
}
      
}
