#ifndef _TCON_MPICO_
#define _TCON_MPICO_

#if defined (PARTICLE)
#include "application.h"
#else
#include "Arduino.h"
#endif

#if PLATFORM_ID == 0 // Core
#define MYSPI SPI
#elif PLATFORM_ID == 6 // Photon
#define MYSPI SPI1
#elif PLATFORM_ID == 8 // P1
#define MYSPI SPI1
#elif PLATFORM_ID == 10 // Electron
#define MYSPI SPI1
#elif PLATFORM_ID == 103 // BluzDK
#define MYSPI SPI
#else
  #error "*** PLATFORM_ID not supported by this firmware. PLATFORM should be Core, Photon, P1 or Electron ***"
#endif

static char uploadImageData[3]=         {0x20,0x01,0x00};       //
static char resetDataPointer[3]=        {0x20,0x0D,0x00};       //
static char displayUpdate[3]=           {0x24,0x01,0x00};       //
static char getDeviceInfo[4]=           {0x30,0x01,0x01,0x00};  //LE=0
static char getDeviceId[4]=             {0x30,0x02,0x01,0x14};  //LE=0x14
static char getSystemInfo[4]=           {0x31,0x01,0x01,0x00};  //LE=0
static char getSystemVersionCode[4]=    {0x31,0x02,0x01,0x10};  //LE=0x10
static char readSensorData[4]=          {0xE5,0x01,0x00,0x02};  //LE=2

class TCON_MPICO
{
public:
  TCON_MPICO(uint8_t ss, int spiclkspeed, uint8_t tcbusy);
  void begin(void);
  void sendCommand(char *buffer, int size);
  void getResponse(int le);
  void uploadImage(char *buffer);

private:
  uint8_t _ss;
  uint8_t _tcbusy;
  int _spiclkspeed;

  void waitTCbusy(void);
  void waitInit(void);
  void printHex(int input, bool ln);
  
};
#endif
