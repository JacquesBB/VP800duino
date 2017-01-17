/* VP800duino  jacquesBBB 9/1/2017
Sketch pour      la station de dessoudage Weller avec en plus un affichage OLED/ */
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

#define opto   6    // adresse d'activation de l optocoupleur  LOW ->  turn heater on. 
#define pot  A6     // potentiometre 
#define sens A9     // capteur temp

long val=0;
long vals=0;
float temp0;
int tempmax=400;  // temp max d'affichage 
int tempmin=50;  // temp min d'affichage 

float V;
float VV0=5;    // tension courante 
float R;
float R0=112;
float temp00=-273;   // coefs pour la conversion  R -> temp  
float tc=12.85;      //
float temp;

void setup() {
  pinMode(opto, OUTPUT);
  Serial.begin(19200);
  analogReference(DEFAULT);
  // oled
  oled.begin();
  oled.setFont(u8g2_font_inb24_mr ); // font 24 pt
}

void loop() {
  gettemp();   
  printterm();
  printoled();
  control();
}

void control(){
// regulation du fer
  if ( temp < temp0 ) {
      digitalWrite(opto, LOW);    // turn on the iron 
    }else{
      digitalWrite(opto, HIGH);    // turn off the iron 
   }     
}

void gettemp(){
// acquisition des temperatures
// temp de reference 
  val = analogRead(pot);
  temp0=((1024-val)*(tempmax-tempmin))/1024.0+tempmin;

// temp instantannee
  vals = analogRead(sens);
   R=(R0*vals)/(1024-vals);   // valeur de la resistance 
   temp=temp00+tc*R;
// correction apres test 
   temp = 0.79*temp -22;
}

void printoled(){
  int it0;
  int it1;
  it0=int(temp0);
  it1=int(temp);
  
   oled.clearBuffer();          // clear the internal memory
   oled.setCursor(0,31);
   oled.print("T0:");
   oled.print(it0);
   oled.setCursor(0,63);
   oled.print("T :");
   oled.print(it1);
   oled.sendBuffer();          // transfer internal memory to the display
}

void printterm(){
// for debugging 
  Serial.print(val);
  Serial.print("  T0 :");
  Serial.print(temp0);
  Serial.print("  vals :");  
  Serial.print(vals);  
  Serial.print("  V :");
  Serial.print( V);
  Serial.print("  R :");
  Serial.print( R);
  Serial.print("  T :");
  Serial.println( temp);  
}