//Code by Maldonado
// C++ code

//Modulo I2C para LCD 2x16:
#include<Wire.h>
#include <LiquidCrystal_I2C.h> // Debe descargar la Libreria que controla el I2C

LiquidCrystal_I2C lcd(0x27,16,2);
//Semaforo 1:
int green1 = 8;
int yellow1 = 9;
int red1 = 10;
//Semaforo 2:
int green2 = 11;
int yellow2 = 12;
int red2 = 13;
// switch:
int buttonOff = 2;
int buttonOn = 7;
// Tiempos:
int t = 3000;
int tPreventiva = 2000;
int tRojoVerde = 1000;
//program time:
int estadoMasT = 0;
int estadoMenosT = 0;
int estadoAnteriorMasT = 0;
int estadoAnteriorMenosT = 0;
//+, -, time in sec:
int menosT = 6;
int masT = 5;

void setup()
{
  //Activar Serial Monitor:
  Serial.begin(9600);
  
  // initialize the lcd
  lcd.init();                       
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Tiempo:");
  lcd.setCursor(2,1);
  lcd.print(t/1000);
  lcd.print(" seg");
  //delay(1000);
  //lcd.noBacklight();

  //Semaforo 1:
  pinMode(green1, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(red1, OUTPUT);
  //Semaforo 2:
  pinMode(green2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(red2, OUTPUT);
  // switch:
  attachInterrupt(digitalPinToInterrupt(buttonOff),stopAll,HIGH);
  pinMode(buttonOn, INPUT);
  // switch time:
  pinMode(menosT, INPUT);
  pinMode(masT, INPUT);
}

void loop()
{
  changeLights();
}

void changeLights(){
  //rojo 2
  digitalWrite(yellow2, LOW);
  digitalWrite(red2, HIGH);
  delay(tRojoVerde);
  
  //verde 1
  digitalWrite(red1, LOW);
  digitalWrite(green1, HIGH);
  delay(t);
  
  //amarillo 1
  digitalWrite(green1, LOW);
  digitalWrite(yellow1, HIGH);
  delay(tPreventiva);
  
  //rojo 1
  digitalWrite(yellow1, LOW);
  digitalWrite(red1, HIGH);
  delay(tRojoVerde);
  
  //verde 2
  digitalWrite(red2, LOW);
  digitalWrite(green2, HIGH);
  delay(t);
  
  //amarillo 2
  digitalWrite(green2, LOW);
  digitalWrite(yellow2, HIGH);
  delay(tPreventiva);
}

//Stop loop
void stopAll(){

  digitalWrite(green1, LOW);
  digitalWrite(red1, HIGH);
  digitalWrite(yellow1, HIGH);
  digitalWrite(green2, LOW);
  digitalWrite(red2, HIGH);
  digitalWrite(yellow2, HIGH);
  //lcd.clear();
  //lcd.setCursor(2,0);
  //lcd.print("Time:");
  //lcd.setCursor(2,1);
  //lcd.print("seg");
  // clear the screen

  while (digitalRead(buttonOn) != HIGH){
    //Change time mode
    Serial.println(t);
    //
    //lcd.backlight();

    estadoMasT = digitalRead(masT);
    estadoMenosT = digitalRead(menosT);
    
    if ((estadoMasT == HIGH) && (estadoAnteriorMasT == LOW)){
      if (t < 7000){            //Cambiat tiempo                                 
        t=t+1000;            //Cambiat tiempo
      }
      else{
        t=1000;            //Cambiat tiempo
      }
      delay(20);
      lcdDisplay();
    }
    else if ((estadoMenosT == HIGH) && (estadoAnteriorMenosT == LOW)){
      if (t > 1000){            //Cambiat tiempo
        t=t-1000;            //Cambiat tiempo
      }
      else{
        t=7000;            //Cambiat tiempo
      }
      delay(20);
    }
    
    estadoAnteriorMasT = estadoMasT;
    estadoAnteriorMenosT = estadoMenosT;
    
    
    
  }
  digitalWrite(green1, LOW);
  digitalWrite(red1, LOW);
  digitalWrite(yellow1, LOW);
  digitalWrite(green2, LOW);
  digitalWrite(red2, LOW);
  digitalWrite(yellow2, LOW);
}

void lcdDisplay(){
  //lcd.init();                       
  // Print a message to the LCD.
  //lcd.backlight();
  //lcd.setCursor(2,0);
  //lcd.print("Tiempo:");
  lcd.setCursor(2,1);
  lcd.print(t/1000);
  lcd.print(" seg");
}
