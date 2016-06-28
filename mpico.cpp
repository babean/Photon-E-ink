#include "tcon_mpico.h"

//static char tc_p441_230_epd_hdr[16]=    {0x33,0x01,0x90,0x01,0x2C,0x01,0x01,0x00,
//                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // type1

TCON_MPICO::TCON_MPICO(uint8_t ss, int spiclkspeed, uint8_t tcbusy):
    _ss(ss),
    _spiclkspeed(spiclkspeed),
    _tcbusy(tcbusy)
{
    pinMode(_tcbusy, INPUT);
}

void  TCON_MPICO::begin(){
	MYSPI.begin(SPI_MODE_MASTER,_ss);			// Init SPI bus
	MYSPI.setBitOrder(MSBFIRST);
	MYSPI.setClockSpeed(_spiclkspeed);
	MYSPI.setDataMode(SPI_MODE3);
    Serial.begin(115200);
    waitInit();
}

void TCON_MPICO::waitInit(){
    delay(5);
    if (digitalRead(_tcbusy)) Serial.println("TCon initialized Completed");
    else Serial.println("TCon initialized Failed");
    delay(5);
}
void TCON_MPICO::waitTCbusy(){
    delayMicroseconds(20);
    while (!digitalRead(_tcbusy)) ;//Serial.println("Waiting for TCbusy");
}

void TCON_MPICO::sendCommand(char *buffer, int size){
    //Serial.println("Sending Command");
    waitTCbusy();
    //Serial.println("TC done");
    digitalWrite(_ss,LOW);
    for (int i=0;i<size;i++) {
        MYSPI.transfer(buffer[i]);
        //Serial.printlnf("Sending Command byte:%.2x",buffer[i]);
    }
    digitalWrite(_ss,HIGH);
}
void TCON_MPICO::getResponse(int le){
    int data, data2;

    waitTCbusy();
    digitalWrite(_ss,LOW);
    if (le==0) {
        data=MYSPI.transfer(0x00);
        while (data!=0x00) {
            printHex(data,0);
            data=MYSPI.transfer(0x00);
        }
    }
    else if (le>0) {
        for (int i=0;i<le;i++){
            data=MYSPI.transfer(0x00);
            printHex(data,0);
        }
    }
    Serial.print(" Status: ");
    data=MYSPI.transfer(0x00);
    data2=MYSPI.transfer(0x00);
    digitalWrite(_ss,HIGH);
    if ((data == 0x90) && (data2 == 0x00)) Serial.println("Done");
    else {
        Serial.printf(" **%.2x%.2x\n", (unsigned)data, (unsigned)data2);
    }
    digitalWrite(_ss,HIGH);
}

void TCON_MPICO::uploadImage(char *buffer){
    waitTCbusy();
    digitalWrite(_ss,LOW);
    MYSPI.transfer(0x20);
    MYSPI.transfer(0x01);
    MYSPI.transfer(0x00);
    MYSPI.transfer(0x10);
    for (int i=0;i<16;i++) MYSPI.transfer(buffer[i]);
    digitalWrite(_ss,HIGH);
    Serial.print("Uploadheader: ");
    getResponse(-1);
    
    waitTCbusy();
    for (int i=0;i<60;i++){
        waitTCbusy();
        digitalWrite(_ss,LOW);
        MYSPI.transfer(0x20);
        MYSPI.transfer(0x01);
        MYSPI.transfer(0x00);
        MYSPI.transfer(0xFA);
        for (int j=0;j<250;j++) MYSPI.transfer(buffer[16+(250*i)+j]);
        digitalWrite(_ss,HIGH);
        getResponse(-1);
    }
}

void TCON_MPICO::printHex(int input, bool ln){
    if (input<10) Serial.print("0");
    if (ln) Serial.println(String(input,HEX));
    else Serial.print(String(input,HEX));
}
