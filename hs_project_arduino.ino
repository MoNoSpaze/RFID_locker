#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#define SS_PIN 53
#define RST_PIN 49
int id_buffer=0;int Position=0;int Location=0;int id[3];const int solinoid[3]={22,24,26};int alert=0;int id_data=0;
int buzzer=23;
SoftwareSerial MCUsender(11, 10); //RX, TX

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

void setup(){
   Serial.begin(9600);
   MCUsender.begin(4800);
   
   pinMode(3, INPUT);
   pinMode(2, OUTPUT);
   pinMode(buzzer, OUTPUT);
   
   SPI.begin(); // Init SPI bus
   mfrc522.PCD_Init(); // Init MFRC522
   
   for(int i=0;i<=2;i++){
    pinMode(solinoid[i],OUTPUT);
    digitalWrite(solinoid[i],LOW);
   }
   
   digitalWrite(buzzer,LOW);
   Serial.println("Now RFID is reading UID:");  
}

void loop() {
  
    if ( mfrc522.PICC_IsNewCardPresent()){
      
      if ( mfrc522.PICC_ReadCardSerial()){
        digitalWrite(buzzer,HIGH);
        delay(250);
        digitalWrite(buzzer,LOW);
        Serial.print("Tag UID:");
        for (int i = 0; i < 3; i++){
          int uid_part = int(mfrc522.uid.uidByte[i]);
          id_buffer+=uid_part;
        }
        Serial.println(id_buffer);
      }
      for(int i=0;i<=2;i++){
        if(id[i]==id_buffer){
          Position=1;
          Location=i;
        }
      }
      /*check the position*/
      //"add" function
      if(Position==0){
        //if Position==0,Is mean This ID isn't in any array.
        for(int i=0;i<=2;i++){
          if(id[i]==0){
            //if didn't add this ID, Add it now.
            id[i]=id_buffer;
            alert=i;
            id_data=id_buffer;
            break;
          }
        }
      }
      //"delete" function
      if(Position==1){
        //if Position==1,Is mean This ID already in array.
        for(int i=0;i<=2;i++){
          if(id[i]==id_buffer){
            //if have same ID in array, Delete this now.
            id[Location]=0;
            alert=i;
            id_data=(id_buffer*(-1));
            break;
          }
        }
      }
      
      if(alert==0){
        digitalWrite(solinoid[alert],HIGH);
        delay(500);
        digitalWrite(solinoid[alert],LOW);
      }
      else if(alert==1){
        digitalWrite(solinoid[alert],HIGH);
        delay(500);
        digitalWrite(solinoid[alert],LOW);
      }
      else if(alert==2){
        digitalWrite(solinoid[alert],HIGH);
        delay(500);
        digitalWrite(solinoid[alert],LOW);
      }
      MCUsender.print(id_data);
      MCUsender.print("\n");
      delay(100);
      for(int i=0;i<=2;i++){
        Serial.print(id[i]);
        Serial.print(" ");
      }
      Serial.println();
      Serial.print(id_data);
      id_buffer=0;
      Position=0;
      Location=0;
      alert=0;
      Serial.println();
      mfrc522.PICC_HaltA();
    }
    delay(250);
}
