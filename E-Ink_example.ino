// This #include statement was automatically added by the Particle IDE.
#include "hello_world.h"

// This #include statement was automatically added by the Particle IDE.
#include "tcon_mpico.h"

#define SS_PIN D5
#define SPI_SPEED 400000
#define _TCBUSY_PIN D6

TCON_MPICO mpico(SS_PIN, SPI_SPEED, _TCBUSY_PIN);

long last_entry=0;

void setup() {
    mpico.begin();

    mpico.sendCommand(resetDataPointer,3);
    Serial.print("resetDataPointer: ");
    mpico.getResponse(-1);

	mpico.uploadImage(squirrel_xbm);
	Serial.println("Upload Image...");

    mpico.sendCommand(displayUpdate,3);
    Serial.print("Display update... ");
    mpico.getResponse(-1);
}

void loop() {
if ((millis()-last_entry)>4000){
    mpico.sendCommand(resetDataPointer,3);
    Serial.print("resetDataPointer: ");
    mpico.getResponse(-1);

    mpico.sendCommand(getDeviceInfo,4);
    Serial.print("getDeviceInfo: ");
    mpico.getResponse(0x00);

    mpico.sendCommand(getDeviceId,4);
    Serial.print("getDeviceID: ");
    mpico.getResponse(0x14);

    mpico.sendCommand(getSystemInfo,4);
    Serial.print("getSystemInfo: ");
    mpico.getResponse(0x00);

    mpico.sendCommand(getSystemVersionCode,4);
    Serial.print("getSystemVersion: ");
    mpico.getResponse(0x10);

    mpico.sendCommand(readSensorData,4);
    Serial.print("readSensorData: ");
    mpico.getResponse(0x02);
    
    last_entry=millis();
    }
}
