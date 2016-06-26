// This #include statement was automatically added by the Particle IDE.
#include "hello_world.h"

// This #include statement was automatically added by the Particle IDE.
#include "mpico.h"

long last_entry=0;

void setup() {
  pinMode(_TCBUSY_PIN, INPUT);
	SPI1.begin(SPI_MODE_MASTER,SS_PIN);			// Init SPI bus
	SPI1.setBitOrder(MSBFIRST);
	SPI1.setClockSpeed(400,KHZ);
	SPI1.setDataMode(SPI_MODE3);
	Serial.begin(115200);
	waitInit();

  sendCommand(resetDataPointer,3);
  Serial.print("resetDataPointer: ");
  getResponse(-1);


	uploadImage(squirrel_xbm);
	Serial.println("Upload Image...");

  sendCommand(displayUpdate,3);
  Serial.print("Display update... ");
  getResponse(-1);
}

void loop() {
if ((millis()-last_entry)>4000){
    sendCommand(resetDataPointer,3);
    Serial.print("resetDataPointer: ");
    getResponse(-1);

    sendCommand(getDeviceInfo,4);
    Serial.print("getDeviceInfo: ");
    getResponse(0x00);

    sendCommand(getDeviceId,4);
    Serial.print("getDeviceID: ");
    getResponse(0x14);

    sendCommand(getSystemInfo,4);
    Serial.print("getSystemInfo: ");
    getResponse(0x00);

    sendCommand(getSystemVersionCode,4);
    Serial.print("getSystemVersion: ");
    getResponse(0x10);

    sendCommand(readSensorData,4);
    Serial.print("readSensorData: ");
    getResponse(0x02);
    
    last_entry=millis();
    }
}

void printHex(int input, bool ln){
    if (input<10) Serial.print("0");
    if (ln) Serial.println(String(input,HEX));
    else Serial.print(String(input,HEX));
}
