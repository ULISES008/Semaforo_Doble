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
//Estado de interrupcion:
bool interrupted = false;
//Condicionales para intervalos entre semaforos:
unsigned long previousMillis = millis();
int x=1;

void setup()
{
  //Activar Serial Monitor:
  Serial.begin(9600);                 //borrar
  
  // initialize the lcd
  lcd.init();                       
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Tiempo:");                                                //NOTAS: Cambiar texto de inicio y apagar pantalla mientras no se usa
  lcd.setCursor(2,1);
  lcd.print(t/1000);
  lcd.print(" seg");

  //Semaforo 1:
  pinMode(green1, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(red1, OUTPUT);
  //Semaforo 2:
  pinMode(green2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(red2, OUTPUT);
  // switch:
  attachInterrupt(digitalPinToInterrupt(buttonOff),interrupt,HIGH);
  pinMode(buttonOn, INPUT);
  // switch time:
  pinMode(menosT, INPUT);
  pinMode(masT, INPUT);
  //Configuración inicial semaforos:
  digitalWrite(yellow2, LOW);
  digitalWrite(red1, LOW);
}

void loop()
{
  if (!interrupted){
    changeLights();
  }
  else if (interrupted){
    stopAll();
  }
}

void changeLights()
{
  unsigned long currentMillis = millis();
  //rojo 2
  if ((currentMillis - previousMillis >= tPreventiva) && (x == 1)){
    previousMillis = currentMillis;
    digitalWrite(yellow2, HIGH);
    digitalWrite(red2, LOW);
    x=2;
  }
  //verde 1
  else if ((currentMillis - previousMillis >= tRojoVerde) && (x == 2)){
    previousMillis = currentMillis;
    digitalWrite(red1, HIGH);
    digitalWrite(green1, LOW);
    x=3;
  }
  //amarillo 1
  else if ((currentMillis - previousMillis >= t) && (x == 3)){
    previousMillis = currentMillis;
    digitalWrite(green1, HIGH);
    digitalWrite(yellow1, LOW);
    x=4;
  }
  //rojo 1
  else if ((currentMillis - previousMillis >= tPreventiva) && (x == 4)){
    previousMillis = currentMillis;
    digitalWrite(yellow1, HIGH);
    digitalWrite(red1, LOW);
    x=5;
  }
  //verde 2
  else if ((currentMillis - previousMillis >= tRojoVerde) && (x == 5)){
    previousMillis = currentMillis;
    digitalWrite(red2, HIGH);
    digitalWrite(green2, LOW);
    x=6;
  }
  //amarillo 2
  else if ((currentMillis - previousMillis >= t) && (x == 6)){
    previousMillis = currentMillis;
    digitalWrite(green2, HIGH);
    digitalWrite(yellow2, LOW);
    x=1;
  }
}

//interrupt
void interrupt()
{
  interrupted= true;
}

//Stop loop
void stopAll()
{
  digitalWrite(green1, HIGH);
  digitalWrite(red1, LOW);
  digitalWrite(yellow1, LOW);
  digitalWrite(green2, HIGH);
  digitalWrite(red2, LOW);
  digitalWrite(yellow2, LOW);
  //Ciclo mientras esta detenido el semaforo
  while (digitalRead(buttonOn) != HIGH){
    Serial.println(t);                                                     //Borrar
    //Change time mode
    estadoMasT = digitalRead(masT);
    estadoMenosT = digitalRead(menosT);
    if ((estadoMasT == HIGH) && (estadoAnteriorMasT == LOW)){
      if (t < 7000){                                                      //Cambiat tiempo                                 
        t=t+1000;                                                            //Cambiat tiempo
      }
      else{
        t=1000;                                                            //Cambiat tiempo
      }
      delay(20);
      lcd.setCursor(2,1);
      lcd.print(t/1000);
    }
    else if ((estadoMenosT == HIGH) && (estadoAnteriorMenosT == LOW)){
      if (t > 1000){            //Cambiat tiempo
        t=t-1000;            //Cambiat tiempo
      }
      else{
        t=7000;            //Cambiat tiempo
      }
      delay(20);
      lcd.setCursor(2,1);
      lcd.print(t/1000);
    }
    estadoAnteriorMasT = estadoMasT;
    estadoAnteriorMenosT = estadoMenosT;
  }
  //Restaurar configuracion del semaforo y continuar el ciclo
  digitalWrite(green1, HIGH);
  digitalWrite(yellow1, HIGH);
  digitalWrite(green2, HIGH);
  digitalWrite(red2, HIGH);
  x=1;
  interrupted= false;
}
