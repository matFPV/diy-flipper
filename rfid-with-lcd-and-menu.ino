/*
  LiquidCrystal Library

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)


 //////last updates
 rfid submenu added


*/

// include libs
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

//lcd
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lcdCur = - 3;                                      //set where the text gets printed


//ui
String optionsL[]={"rfid          ","nrf24l01+     ","wipe lcd      ","debug mode    "};
String rfidSubMenu[] = {"read          ", "write         ", "copy          ", "exit          "};


int opt;
int rfidOpt;
bool select;
bool rfidSelect;
int numOpts = 3;
int numRfidOpts = 3;


//rfid -rc522
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_1_PIN        10         // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 2
#define SS_2_PIN        8          // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 1
#define NR_OF_READERS   2
byte ssPins[] = {SS_1_PIN, SS_2_PIN};
MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.


void setup() {
      //lcd
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  
      //rfid
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    mfrc522[reader].PCD_DumpVersionToSerial();


  }
      //interface
  Serial.begin(9600);
}


int button1=analogRead(A0);
int button2=analogRead(A1);

void loop() {
                          //ui
      int button1=analogRead(A0);
      int button2=analogRead(A1);

      if (button2 >= 800){
        if(select==false){
          select=true;}
        else{
          select=false;}
      delay(500);
      }
      if (select==false){ 
        if (button1 >= 800){
          if(opt == numOpts){opt=0;}
          else{opt++;}
          delay(500);
        }
      }

                              //lcd
    lcdCurFn();
        
    if(select == false){  //second line when in menu
    lcd.setCursor(0,1);
    if (opt == numOpts){
      lcd.print(optionsL[0]);
    }
    else{
      lcd.print(optionsL[opt+1]);
      }
      }
      
/////////////////////////////////////////////////////////////////////////////////stuff that happens once you select something from the menu
    if (opt==0){         //rfid
      if (select==true){
        rfid();            
      }
    }

    
    if (select==true){    //clean lcd
      if(opt==2){
        cleanLcd();
      }
    }
    
    if (opt==3){     //debug 
      if (select==true){
            debugMode();
        }
      }
}//end of loop


                                                                                //all main menu the functions
                                                                                
void lcdCurFn(){
    lcd.setCursor(0,0);
    lcd.print(optionsL[opt]);
    lcd.setCursor(14,0);
    if (select == false){
      lcd.print("<-");
      }
    else{
      lcd.print("<0");
      }
    }
    
    
void rfid(){ 
        while(select==true){
             int button1=analogRead(A0);
             int button2=analogRead(A1);
            ////////////////////////////////  rfid menu button functionality
            if (button2 >= 800){
              if(rfidSelect==false){
                rfidSelect=true;}
              else{
                rfidSelect=false;}
            delay(500);
            }
            //if (rfidSelect==false){ 
              if (button1 >= 800){
                if(rfidOpt == numRfidOpts){rfidOpt=0;}
                else{rfidOpt++;}
                delay(500);
              }
              ////////////////////////////////// first line when in rfid menu
      lcd.setCursor(0,0);
      lcd.print(rfidSubMenu[rfidOpt]);

        ////////////////////////////////// second line when in rfid menu
        if(rfidSelect == false){        
        lcd.setCursor(0,1);
        if (rfidOpt == numRfidOpts){
          lcd.print(rfidSubMenu[0]);
        }
        else{
          lcd.print(rfidSubMenu[rfidOpt+1]);
          }
        }
        ///////////////////////////////////////////////////// doing what was selected in the rfid menu

      if (rfidOpt == 0){
        if (rfidSelect==true){
            rfidRead();
          }
        }
        
      if (rfidOpt == 3){ //if exit was selected
        if (rfidSelect==true){
        select = false;
        rfidSelect=false;
        rfidOpt=0;
        }
      }


       

        
      }//end while loop
    }


    
void cleanLcd(){
      lcd.setCursor(0,0);
      lcd.print("              ");
      lcd.setCursor(0,1);
      lcd.print("              ");
      //select==false;
      }
                                                        
void debugMode(){
              int button1=analogRead(A0);
              int button2=analogRead(A1);
              Serial.print(optionsL[opt]);
              Serial.print("  ");
              Serial.print(opt);
              Serial.print("  ");
              Serial.print(select);
              Serial.print("  ");
              Serial.print(button1);
              Serial.print("  ");
              Serial.println(button2);  
  }


void dump_byte_array(byte *buffer, byte bufferSize) {
  
  lcd.setCursor(0, 1);
  lcd.print("               ");
  lcd.setCursor(0, 0);
  lcd.print("rfid");
  lcd.setCursor(0, 0);
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.println(buffer[i], HEX);
    lcdCur += 3;
    lcd.setCursor(lcdCur, 1);
    //print to lcd 
    lcd.print(buffer[i], HEX);
  }
  lcdCur = -3;
}

void rfidRead(){  //rfid reading function start
               for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
              // Look for new cards
              if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
                dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
                /*
                Serial.print(F("PICC type: "));
                MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
                Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));
                // Halt PICC
                mfrc522[reader].PICC_HaltA();*/
                // Stop encryption on PCD
                mfrc522[reader].PCD_StopCrypto1();
              } //if (mfrc522[reader].PICC_IsNewC
            } //for(uint8_t reader
}//rfid reading function end
