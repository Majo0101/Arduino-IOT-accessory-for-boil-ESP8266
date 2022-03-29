
#include "arduino_secrets.h"
#include "thingProperties.h"
#include <OneWire.h>                // One wire zbernica
#include <DallasTemperature.h>      // Dallas kniznica



#define tlakomer A0
#define rele D5 // Rele zopinanie pre dopustenie tlaku
#define rele1 D6 // Rele ohrev vody
#define ONE_WIRE_BUS D7
#define OLED_RESET D0



//Tlakomer global FUNGUJE....................................................................................................
const float Voffset = 0.49;   // Napatie na prazdno 
const float Vmin = 0.5;       // Volty 
const float Vmax = 4.5;       // Volty 
const float Pmin = 0;         // MPa 
const float Pmax = 1.2;       // MPa 
const float kPV = (Pmax - Pmin) / (Vmax - Vmin); //Prepocet hodnota do vzorca

const float SupplyVoltage = 3.3;    // Maximalne vstupne napatie 
const float MaxDAC = 1024.0;        // Rozlisenie A0 vstupu 
 //...........................................................................................................................





//Dallas FUNGUJE.............................................................................................................
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = {0x28, 0xFF, 0x64, 0x1E, 0x85, 0xF7, 0x60, 0xAE};  // adresa spravna 28 FF 64 1E 85 F7 60 AE
DeviceAddress sensor2 = {0x28, 0xFF, 0x64, 0x1E, 0x84, 0x79, 0xF1, 0xB9};  // adresa spravna 28 FF 64 1E 84 79 F1 B9
//DeviceAddress sensor3 = {0x28, 0xCC, 0x19, 0x49, 0x0C, 0x00, 0x00, 0xBB};  // adresa spravna 28 FF 64 1E 84 73 32 76
//..........................................................................................................................


void setup() {
  
  
  pinMode(rele , OUTPUT); //Vystupne rele 1
  pinMode(rele1, OUTPUT); //Vystupne rele 2

  sensors.begin();                                      //Sensor dallas start
  sensors.setResolution(sensor1, 9);                    //Rozlisenie pre senzor
  sensors.setResolution(sensor2, 9);                    //Rozlisenie pre senzor


  
  Serial.begin(9600);
  
  delay(1500);

  initProperties();
  
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

}

void loop() {
  ArduinoCloud.update();
 
  //Meranie tlaku a osetrenie....................................................................................................
  int sensorValx=analogRead(tlakomer);                      //Citanie hodnoty z A0

  int sensorVal = (sensorValx - 9);                         // odchylka na prazdno bez senzora

  float voltagex = (sensorVal* SupplyVoltage) / MaxDAC;

  float voltage = voltagex * 2;                             //Prepocet koli odporovemu delicu
  
  if( 70 < sensorVal && sensorVal < 78)                     // Hodenie pociatocnej odchylky do nuly 
    {
      tlak = 0;
    }
  if(sensorVal < 70 )                                       // Ak je senzor odpojeny hodnota kolise na 9
    {
      tlak = -127;
    }
    else
    {
      tlak = abs(kPV *( (float)voltage - Voffset ) ) * 10 ;  //Prepocet tlaku 
    }
  
  Serial.print("Sensor Value: "); Serial.print(sensorVal); Serial.print(" | ");
  Serial.print("Volts: ");        Serial.print(voltagex);   Serial.print(" | ");
  Serial.print("Pressure: ");     Serial.print(tlak); Serial.println(" bars ");
  Serial.print("kpv: ");          Serial.print(kPV); Serial.println(" hodnota "); 
  //............................................................................................................................

 
  // Dallas citanie teploty do premennej........................................................................................
  sensors.requestTemperatures();

  teplota1 = (sensors.getTempC(sensor1));
  teplota2 = (sensors.getTempC(sensor2));
  //............................................................................................................................

}




//Spinac na dopustanie tlaku....................................................................................................
void onSwitch1Change() {
  if (switch1 == 1 && tlak < 1.300)
  {
    digitalWrite(rele, HIGH);

  }
  else
  {
    digitalWrite(rele, LOW);

  }
}
//..............................................................................................................................

void onSwitch2Change() {
  if (switch2 == 1 && teplota1 < 55)
  {
    digitalWrite(rele1, HIGH);

  }
  else
  {
    digitalWrite(rele1, LOW);

  }
}
