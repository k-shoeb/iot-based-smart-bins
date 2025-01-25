#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "YOUR_DEVICE_LOGIN_NAME"; // Device login name

const char SSID[]               = "SSID";           // Network SSID (name)
const char PASS[]               = "PASSWORD";         // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = "YOUR_DEVICE_KEY";    // Secret device password


String message;
float trashLevel;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(message, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(trashLevel, READ, ON_CHANGE, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
