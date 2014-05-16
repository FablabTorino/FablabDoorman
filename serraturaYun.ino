#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Bridge.h>

//#define DEBUG

#define IRQ   (4)
#define RESET (6)  // Not connected by default on the NFC Shield
#define PYPATH F("/mnt/sda1/arduino/www/serraturaYun/logger.py")

const int lockEnable = 9,
          feedbackLed = 13;

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

uint8_t uidPrev[7];

void setup() {
  pinMode(lockEnable, OUTPUT);
  pinMode(feedbackLed, OUTPUT);
  digitalWrite(lockEnable, HIGH);  // make sure the door is locked
  digitalWrite(feedbackLed, LOW);

  Bridge.begin();
  nfc.begin();

#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("Officine Arduino door opener"));
#endif

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // configure board to read RFID tags
  nfc.SAMConfig();
}

void loop() {

  uint8_t successID;
  uint8_t uid[] = {
    0, 0, 0, 0, 0, 0, 0
  };  // Buffer to store the returned UID
  uint8_t uidLength;       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // it's a blocking function
  successID = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (successID)
  {
#ifdef DEBUG
    Serial.println("Read TAG");
    Serial.print("ID: ");
    //nfc.PrintHex(uid, uidLength);
    Serial.println(intArrayToHexString(uid, uidLength));
#endif
    Process p;
    p.begin("python");
    p.addParameter(PYPATH);
    p.addParameter(intArrayToHexString(uid, uidLength));
    p.run();

#ifdef DEBUG
    Serial.print(F("The user is trying to open the door\n"));
#endif
    while (p.available()) {
      char c = p.read();
#ifdef DEBUG
      Serial.print(c);
#endif
      if (c == '\n')
        break;
      else if (c == 'y')
      {
        digitalWrite(lockEnable, HIGH);
        delayAndBlink(2000);
        digitalWrite(lockEnable, LOW);
      }
      else {
        digitalWrite(lockEnable, LOW);
      }
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

String intArrayToHexString(uint8_t array[], int length)
{
  String output = "";

  for (int i = 0; i < length; i++)
    output += String(array[i], HEX);

  output.toUpperCase();

  return output;
}

void delayAndBlink(unsigned long delayTime)
{
  int blinkInterval = 100;
  unsigned long repetitions = delayTime / blinkInterval;
  boolean ledState = LOW;

  for (int i = 0; i < repetitions; i++)
  {
    ledState = !ledState;
    digitalWrite(feedbackLed, ledState);
    delay(blinkInterval);
  }
  digitalWrite(feedbackLed, LOW);

}


