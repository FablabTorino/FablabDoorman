#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Bridge.h>

#define DEBUG

#define IRQ   (4)
#define RESET (6)  // Not connected by default on the NFC Shield
#define PYPATH F("/mnt/sda1/arduino/www/serraturaYun/logger.py")

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

uint8_t uidPrev[7];

void setup() {
  Bridge.begin();

  Serial.begin(115200);
  while (!Serial);

  nfc.begin();

#ifdef DEBUG
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
#endif

  // configure board to read RFID tags
  nfc.SAMConfig();
}

void loop() {

  uint8_t successID;
  uint8_t uid[] = { 
    0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // it's a blocking function
  successID = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (successID && !compareArray(uid, uidPrev, uidLength))
  {
#ifdef DEBUG
    Serial.println("Read TAG");
    Serial.print("ID: ");
    nfc.PrintHex(uid, uidLength);
#endif
    Process p;
    p.begin("python");
    p.addParameter(PYPATH);
    
    for(int i = 0; i < uidLength; i++)
      p.addParameter(String(uid[i]));

    p.run();
    Serial.print("The user is trying to open the door\n");
    while(p.available()){
      char c = p.read();
      Serial.print(c);
    }
  }

  // save the previous TAG ID    
  memcpy(uidPrev, uid, uidLength);
}


boolean compareArray (uint8_t array1[], uint8_t array2[], uint8_t len)
{
  for (int i = 0; i < len; i++)
  {
    if (array1[i] != array2[i])
      return false;
  }
  return true;
}




