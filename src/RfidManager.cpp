#include "RfidManager.h"
#include <SPI.h>
#include <MFRC522.h>

String uids[] = {"CE 39 19 03", "45 5E CE 01"};
String names[] = {"Card", "Token"};

MFRC522 mfrc522;
bool isInitialized = false;

void RfidManager::init(byte pinSda, byte pinReset)
{
  mfrc522 = MFRC522(pinSda, pinReset);
  /* Enable the SPI interface */
  SPI.begin();
  /* Initialise the RFID reader */
  // rfidReader.init();
  mfrc522.PCD_Init();                // Init MFRC522 card
  delay(4);                          // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  isInitialized = true;
}

bool RfidManager::isTokenPresent()
{
  if (!isInitialized)
  {
    Serial.println("RFID reader not initialized");
    return false;
  }

  return mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial();
}

bool RfidManager::isTokenValid()
{
  if (!isInitialized)
  {
    Serial.println("RFID reader not initialized");
    return false;
  }

  String uid = getTokenUid();

  int cardCount = sizeof(uids) / sizeof(uids[0]);
  for (int index = 0; index < cardCount; index++)
  {

    if (uid == uids[index])
    {
      return true;
    }
  }

  return false;
}

String RfidManager::getTokeName()
{
  if (!isInitialized)
  {
    Serial.println("RFID reader not initialized");
    return "";
  }

  String uid = getTokenUid();

  int cardCount = sizeof(uids) / sizeof(uids[0]);
  for (int index = 0; index < cardCount; index++)
  {

    if (uid == uids[index])
    {
      Serial.println("Known token found: " + names[index]);
      return names[index];
    }
  }

  Serial.print("No known token found for UID");
  Serial.println(uid);
  return "";
}

String RfidManager::getTokenUid()
{
  if (!isInitialized)
  {
    Serial.println("RFID reader not initialized");
    return "";
  }

  String tokenUid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    tokenUid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tokenUid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tokenUid.toUpperCase();
  String result = tokenUid.substring(1);

  Serial.print("Found token with UID: ");
  Serial.println(result);
  return result;
}