#ifndef RFIDMANAGER_H
#define RFIDMANAGER_H

#include <MFRC522.h>

class RfidManager
{
public:
  void init(byte pinSda, byte pinReset);
  bool isTokenValid();
  bool isTokenPresent();
  String getTokeName();
  String getTokenUid();
};

#endif