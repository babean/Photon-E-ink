#define SS_PIN D5
#define _TCBUSY_PIN D6

#define LEMARKER 0x00

static char uploadImageData[3]=         {0x20,0x01,0x00};       //
static char resetDataPointer[3]=        {0x20,0x0D,0x00};       //
static char displayUpdate[3]=           {0x24,0x01,0x00};       //
static char getDeviceInfo[4]=           {0x30,0x01,0x01,0x00};  //LE=0
static char getDeviceId[4]=             {0x30,0x02,0x01,0x14};  //LE=0x14
static char getSystemInfo[4]=           {0x31,0x01,0x01,0x00};  //LE=0
static char getSystemVersionCode[4]=    {0x31,0x02,0x01,0x10};  //LE=0x10
static char readSensorData[4]=          {0xE5,0x01,0x00,0x02};  //LE=2

//static char tc_p441_230_epd_hdr[16]=    {0x33,0x01,0x90,0x01,0x2C,0x01,0x01,0x00,
//                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // type1

void waitInit(){
    delay(5);
    if (digitalRead(_TCBUSY_PIN)) Serial.println("TCon initialized Completed");
    else Serial.println("TCon initialized Failed");
    delay(5);
}
void waitTCbusy(){
    delayMicroseconds(20);
    while (!digitalRead(_TCBUSY_PIN));
}

void sendCommand(char *buffer, int size){
    waitTCbusy();
    digitalWrite(SS_PIN,LOW);
    for (int i=0;i<size;i++) {
        SPI1.transfer(buffer[i]);
    }
    digitalWrite(SS_PIN,HIGH);
}
void getResponse(int le){
    int data, data2;

    waitTCbusy();
    digitalWrite(SS_PIN,LOW);
    if (le==0) {
        data=SPI1.transfer(0x00);
        while (data!=LEMARKER) {
            printHex(data,0);
            data=SPI1.transfer(0x00);
        }
    }
    else if (le>0) {
        for (int i=0;i<le;i++){
            data=SPI1.transfer(0x00);
            printHex(data,0);
        }
    }
    Serial.print(" Status: ");
    data=SPI1.transfer(0x00);
    data2=SPI1.transfer(0x00);
    digitalWrite(SS_PIN,HIGH);
    if ((data == 0x90) && (data2 == 0x00)) Serial.println("Done");
    else {
        Serial.printf(" **%.2x%.2x\n", (unsigned)data, (unsigned)data2);
    }
    digitalWrite(SS_PIN,HIGH);
}

void uploadImage(char *buffer){
    waitTCbusy();
    digitalWrite(SS_PIN,LOW);
    SPI1.transfer(0x20);
    SPI1.transfer(0x01);
    SPI1.transfer(0x00);
    SPI1.transfer(0x10);
    for (int i=0;i<16;i++) SPI1.transfer(buffer[i]);
    digitalWrite(SS_PIN,HIGH);
    Serial.print("Uploadheader: ");
    getResponse(-1);
    
    waitTCbusy();
    for (int i=0;i<60;i++){
        waitTCbusy();
        digitalWrite(SS_PIN,LOW);
        SPI1.transfer(0x20);
        SPI1.transfer(0x01);
        SPI1.transfer(0x00);
        SPI1.transfer(0xFA);
        for (int j=0;j<250;j++) SPI1.transfer(buffer[16+(250*i)+j]);
        digitalWrite(SS_PIN,HIGH);
        getResponse(-1);
    }
}
