#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 5

Servo servo;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String ID = "";   // Set global variable ID

String accessGranted [] = {"A97E8459"};   // Array that has all the UIDs for access

int pos = 0;    //Initial position for servo

// Function to read in the UID for RFID
String readID() {
  // Read in values till end
  for (int i = 0; i < mfrc522.uid.size; i++) { 
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);

    // Add values to the string ID
    ID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  ID.toUpperCase();   // Make it all uppercase
  ID.replace(" ", "");  // Get rid of spaces
  return ID;    //Return ID value
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
  Serial.begin(9600);   // Begin Serial Monitor

  servo.attach(8);    // Pin for servo

  // Initialize everything
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();    // Create instance
  Serial.println("Scan Card...");
}

void loop() {
  
  // Reset when no new card is detected
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Selecting a card
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dumop all debug info
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Run function readID()
  readID();

  Serial.println(ID); // Print the ID

  String status = "Access Denied";  // Set status default to no access

  for (int i = 0; i < 1; i++){   // Scan the array accessGranted to see if ID is in it
    if (ID == accessGranted[i]) {
      status = "Access Granted";
    }
  Serial.println(status);
  }

  // When access is detected/granted move the servo
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

  ID = "";    // Set the ID back to an empty string

}
