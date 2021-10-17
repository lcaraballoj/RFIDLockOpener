#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 5

Servo servo;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String ID = "";

String accessGranted [] = {"A97E8459"};

int pos = 0;

String readID() {
  for (int i = 0; i < mfrc522.uid.size; i++) {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);

    ID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  ID.toUpperCase();
  ID.replace(" ", "");
  return ID;
  //Serial.println(ID);
}

//String searchAccess() {
//  String status = "Access Denied";
//
//  for (int i = 0; i < 2; i++)
//    if (ID == accessGranted[i]) {
//      status = "Access Granted";
//    }
//  Serial.println(status);
//}

void setup() {
  Serial.begin(9600);

  servo.attach(8);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("Scan Card...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  readID();

  Serial.println(ID);

  String status = "Access Denied";

  for (int i = 0; i < 1; i++)
    if (ID == accessGranted[i]) {
      status = "Access Granted";
    }
  Serial.println(status);

  if (status == "Access Granted") {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }

  ID = "";

}
