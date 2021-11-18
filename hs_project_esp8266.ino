#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial(D2, D3); //RX, TX

WiFiClient client;

//val configuration
int ID=0;
long valueX;
int value1=0;
String value2="none";

// Replace with your SSID and Password
const char* ssid = "HUAWEI Y9 Prime 2019";
const char* password = "123456789";

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/RFID_sheet/with/key/api_key_here";

// How your resource variable should look like, but with your own API KEY (that API KEY below is just an example):
//const char* resource = "/trigger/DHT11_TO_GSHEETS/with/key/bEE-kSDiWDxdHyFWUNltMG";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

void sending_to_sheets(int a,String b){
  if (client.connect(server, 80)) {
    Serial.println("connected");
    Serial.print("Request resource: "); 
    Serial.println(resource);
    String jsonObject = String("{\"value1\":\"") + a + "\",\"value2\":\"" + b + "\"}";
                      
    client.println(String("POST ") + resource + " HTTP/1.1");
    client.println(String("Host: ") + server); 
    client.println("Connection: close\r\nContent-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonObject.length());
    client.println();
    client.println(jsonObject);
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void setup(){
   Serial.begin(9600);
   NodeSerial.begin(4800);
   pinMode(D2, INPUT);
   pinMode(D3, OUTPUT);
   Serial.println("Starting...");
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(250);
      Serial.print(".");
   }

   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   Serial.println("Finish linked WIFI");

   Serial.print("Connecting to ");
  Serial.print(server);
  while(!!!client.connect(server, 80)) {
    Serial.print(".");
    delay(250);
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
    delay(250);
  }
  Serial.println("Finish connected all");
}

void loop(){
  
  ID=NodeSerial.parseInt();
  Serial.println(ID);
  
  if(ID==0){
    value1=0;
    value2="none";
  }
  
  if(ID>0){
    value1=abs(ID);
    value2="log in";
    Serial.print(value1);Serial.print(' ');Serial.print(value2);
    Serial.println();
    sending_to_sheets(value1,value2);
    value1=0;
    value2="none";
  }
  
  if(ID<0){
    value1=abs(ID);
    value2="log out";
    Serial.print(value1);Serial.print(' ');Serial.print(value2);
    Serial.println();
    sending_to_sheets(value1,value2);
    value1=0;
    value2="none";
  }
  
  /*if(value1>0){
    Serial.println("sending");
    Serial.print(value1);Serial.print(' ');Serial.print(value2);
    Serial.println();
    //sending_to_sheets(value1,value2);
    value1=0;
  }*/
  
  delay(500);
}

/*valueX=random(0,6);
  if(valueX>0){
    Serial.println("sending");
    sending_to_sheets(valueX,value2);
    valueX=0;
  }
  else{
    Serial.println("not sending");
  }
  delay(3000);*/
