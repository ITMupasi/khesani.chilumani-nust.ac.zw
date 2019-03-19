

/*Written by I.T. Mupasi, National University of science
 * and Technology part 4 Computer Science student, final 
 * project
 * 
 * Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 
* MISO: Pin 12 
* SCK: Pin 13 
* SS: Pin 10
* RST: Pin 9
* 
* 
* Fingerprint scanner connections as follows (on Arduino UNO)
* pin #2 is IN from sensor (GREEN wire)
* pin #3 is OUT from arduino  (WHITE wire)

*/
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal.h>


#define RST_PIN 9
#define SS_PIN 10


MFRC522 mfrc522(SS_PIN, RST_PIN);

int getFingerprintIDez();
uint8_t getFingerprintEnroll();
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);



Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);
LiquidCrystal lcd(A4, A5, A0, A1, A2, A3);
int FingerMode = 0 ;
int SkipHead = 0;
int State1=0;
int State2=0;
uint8_t id;

String lastRfid = "";
String card1 = "";
String card2 = "";
MFRC522::MIFARE_Key key;

uint8_t RFID();

void setup()
{
  pinMode(4, INPUT_PULLUP);  //yellow delete
  pinMode(5, INPUT_PULLUP);  //orange finger detect
  pinMode(6, INPUT_PULLUP);  //white enroll

  pinMode(8, OUTPUT);   //relay
  pinMode(7, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID Card Reader");
  Serial.println("--------------------------");
  Serial.println();
  readEEPROM();

  lcd.begin(16, 2);                      // initialize the lcd
 // lcd.backlight();  //default backlight on
  lcd.clear();
  // while (!Serial);  // For Yun/Leo/Micro/Zero/...

  
  lcd.print("Initializing...");

  lcd.setCursor(0,1);
  lcd.print("Please Wait");
  delay(2000);
  // set the data rate for the sensor serial port
  finger.begin(57600);


  if (finger.verifyPassword()) {
    //   lcd.print("Found fingerprint!");
  } else {
    //   lcd.print("Did not find fingerprint sensor :(");
    while (1);
  }
}

void loop()                     // run over and over again
{
  
  if (digitalRead(4) == LOW ) {
    delay(10);
    if (digitalRead(4) == LOW ) 
    FingerMode = 2; SkipHead = 0; //delete
  }

  if (digitalRead(5) == LOW ) {
    delay(10);
    if (digitalRead(5) == LOW ) 
    FingerMode = 0; SkipHead = 0; //finger
  }

  if (digitalRead(6) == LOW ) {
    delay(10);
    if (digitalRead(6) == LOW ) FingerMode = 1; SkipHead = 0; //enroll
  }
  Serial.println("FingerMode"); Serial.print(FingerMode); Serial.println();
  switch (FingerMode) {
    case 1:
      {
        if (SkipHead == 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enrolling Mode");
        }
        lcd.setCursor(0, 1);
        id = 1;
        getFingerprintEnroll();
        delay(2000);

        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        id = 2;
        lcd.print("Wait enroll ID#");
        lcd.print(id);
        delay(1000);
        getFingerprintEnroll();
        delay(1000);

 lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        id = 3;
        lcd.print("Wait enroll ID#");
        lcd.print(id);
        delay(1000);
        getFingerprintEnroll();
        delay(1000);


 lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        id = 4;
        lcd.print("Wait enroll ID#");
        lcd.print(id);
        delay(1000);
        getFingerprintEnroll();
        delay(1000);
        
        FingerMode = 0;
      }
      break;
    case 2:
      {
        if (SkipHead == 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Delete Mode");
        }
        deleteFingerprint(1);
        delay(500);
        deleteFingerprint(2);
        delay(1000);
        deleteFingerprint(3);
        delay(1000);
        deleteFingerprint(4);
        delay(1000);
        FingerMode = 0;

      }
      break;
    default:
      {
        if (SkipHead == 0)
        {
            
           lcd.clear();
          lcd.setCursor(0, 0);
          
          lcd.print("Scan your ID");

      if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

 String rfid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    rfid += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    rfid += String(mfrc522.uid.uidByte[i], HEX);
  }
  rfid.trim();
  rfid.toUpperCase();
  
  if (rfid == lastRfid)
    return;
  lastRfid = rfid;
  

   if (rfid == card1)
  {
    lcd.clear();
    lcd.print("Authorized");
    getFingerprintIDez();

    delay (1000);
    
  }
  if (rfid == card2)
  { 
    lcd.clear();
    lcd.print("Authorized");
    getFingerprintIDez();

    delay (1000);
    

  }
        
  Serial.println();
  //delay(200);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Place");
          lcd.setCursor(0, 1);
          lcd.print("your Finger..!");
          delay(50);
          getFingerprintIDez();
        }
        SkipHead = 1;
        lcd.setCursor(0, 1);
        //getFingerprintIDez();
      //FingerMode = 0;
      }
      break;
    }
   

  // delay(50);            //don't ned to run this at full speed.
  }


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      // lcd.setCursor(0, 1);
      // lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!

  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  // Serial.print(" with confidence of "); Serial.println(finger.confidence);
}

// returns -1 if failed, otherwise returns ID #



int getFingerprintIDez() {


  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    //  Serial.println(p);
    // not place finger
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  {
    Serial.println(p);
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    if (p == 9) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("the result is...");
      lcd.setCursor(0, 1);
      lcd.print("Not Found");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Place");
      lcd.setCursor(0, 1);
      lcd.print("your Finger..!");
      return -1;

    } else {
      Serial.println(p); 
      return -1;
    }
  }

  // found a match!
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("the result is..");
  lcd.setCursor(0, 1);
  lcd.print("Found ID #"); lcd.print(finger.fingerID);
  if (finger.fingerID==1){
     digitalWrite(8,LOW);
      delay(1000);
     digitalWrite(8,HIGH);  
  }
  if (finger.fingerID==2){
     digitalWrite(7,LOW);
      delay(1000);
     digitalWrite(7,HIGH); 
  }  

  if (finger.fingerID==3){
     digitalWrite(4,LOW);
      delay(1000);
     digitalWrite(4,HIGH); 
  }  
  if (finger.fingerID==4){
     digitalWrite(5,LOW);
      delay(1000);
     digitalWrite(5,HIGH); 
  }  
    
 
  Serial.println();
  delay(200);
  
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place");
  lcd.setCursor(0, 1);
  lcd.print("your Finger..!");
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
//RFID();
  
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    lcd.setCursor(0, 1);
    lcd.print("             ");
    lcd.setCursor(0, 1);
    lcd.print("Deleted! #");
    lcd.print(id);
    Serial.println(); Serial.print("Deleted! #"); Serial.print(id); Serial.println();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Place finger #"); lcd.print(id);
  lcd.blink();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        lcd.blink();
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:

      Serial.println("Image converted");
      lcd.setCursor(0, 1);
      lcd.print("               ");
      lcd.setCursor(0, 1);
      lcd.print("Image converted");
      delay(500);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Remove finger");
  delay(2000);

  
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Place again to");
  lcd.setCursor(0, 1);
  lcd.print("verify");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  lcd.noBlink();
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Stored! #");
    lcd.print(id);
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

void readEEPROM()
{
  for (int i = 0 ; i < 4 ; i++)
  {
    card1 += EEPROM.read(i) < 0x10 ? " 0" : " ";
    card1 += String(EEPROM.read(i), HEX);
  }
  for (int i = 4 ; i < 8 ; i++)
  {
    card2 += EEPROM.read(i) < 0x10 ? " 0" : " ";
    card2 += String(EEPROM.read(i), HEX);
  }
  card1.trim();
  card1.toUpperCase();
  card2.trim();
  card2.toUpperCase();
}
