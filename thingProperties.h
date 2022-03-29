#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[]           = "***";  //Enter THING ID
const char DEVICE_LOGIN_NAME[]  = "***"; //Enter DEVICE ID

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onSwitch1Change();
void onSwitch2Change();

int teplota1;
int teplota2;
float tlak;

CloudSwitch switch1;
CloudSwitch switch2;


void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(switch1, READWRITE, ON_CHANGE, onSwitch1Change);
  
  ArduinoCloud.addProperty(switch2, READWRITE, ON_CHANGE, onSwitch2Change);

  ArduinoCloud.addProperty(teplota2, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(teplota1, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(tlak, READ, ON_CHANGE, NULL);
  
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
