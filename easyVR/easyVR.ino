#include "Arduino.h"
#if !defined(SERIAL_PORT_MONITOR)
  #error "Arduino version not supported. Please update your IDE to the latest version."
#endif

#if defined(__SAMD21G18A__)
  // Shield Jumper on HW (for Zero, use Programming Port)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_MONITOR
#elif defined(SERIAL_PORT_USBVIRTUAL)
  // Shield Jumper on HW (for Leonardo and Due, use Native Port)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_USBVIRTUAL
#else
  // Shield Jumper on SW (using pins 12/13 or 8/9 as RX/TX)
  #include "SoftwareSerial.h"
  SoftwareSerial port(12, 13);
  #define pcSerial SERIAL_PORT_MONITOR
#endif

#include "EasyVR.h"
#include "arduino_secrets.h"
#include<SPI.h>
#include<Bridge.h>
#include<PubSubClient.h>
#include <WiFiNINA.h>

char ssid[] = SECRET_SSID;       
char pass[] = SECRET_PASS; 

EasyVR easyvr(port);

const char* broker = "192.168.0.100";
const char* acdTopicPub = "automatic_cabinet_door/controls";//publish
const char* topicShelf1 = "shelf1";//publish
const char* topicShelf2 = "shelf2";//publish
const char* topicShelf3 = "shelf3";//publish
const char* topicShelf4 = "shelf4";//publish
const char* topicMoodStatus = "mood/state";//publish
const char* topicMoodSection = "mood/section"; //publish
const char* topicVRCommand = "voice_control/status";//publish
const char* topicVRCommandLog = "voice_control/commands";//publish

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

int setting = 0;
int fFood = 13;

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
  GROUP_2  = 2,
};

enum Group0 
{
  G0_KITCHEN = 0,
};

enum Group1 
{
G1_AUTOMATIC = 0,
  G1_MANUAL = 1,
  G1_OPEN = 2,
  G1_CLOSE = 3,
  G1_WINE = 4,
  G1_COOKIES = 5,
  G1_APPLES = 6,
  G1_PASTA = 7,
  G1_KETCHUP = 8,
  G1_TOMATOS = 9,
  G1_BEANS = 10,
  G1_SUGAR = 11,
  G1_SALT = 12,
  G1_CANDY = 13,
  G1_PEPPER = 14,
  G1_COKE = 15,
  G1_WATER = 16,
  G1_BREAD = 17,
  G1_MILK = 18,
  G1_SODA = 19,
  G1_BACON = 20,
  G1_FOIE_GRAS = 21,
  G1_EGGS = 22,
  G1_CROISSANT = 23,
  G1_CHAMPAGNE = 24,
  G1_PATE = 25,
  G1_VOICE = 26,
  G1_HAPPY = 27,
  G1_SAD = 28,
  G1_MOOD_OFF = 29,
  G1_OK = 30,
  G1_I_LIKE = 31,
};

enum Group2 
{
  G2_WINE = 0,
  G2_COOKIES = 1,
  G2_CANDY = 2,
  G2_COKE = 3,
  G2_SODA = 4,
  G2_BACON = 5,
  G2_CHAMPAGNE = 6,
  G2_BACK = 7,
};

// use negative group for wordsets
int8_t group, idx;


void reconnect() {
 // Loop until we're reconnected
 //while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 String clientId = "Client";
 clientId += String(random(0xffff), HEX);//CHANGED ID HERE! 
 if (mqttClient.connect(clientId.c_str())) {
  Serial.println("connected");
  mqttClient.subscribe(topicVRCommandLog);
  // Once connected, publish an announcement...
  // ... and subscribe to topic    
  mqttClient.publish(topicVRCommand, "MOOD ON", true);
  mqttClient.publish(topicMoodStatus, "1");
  pcSerial.println("Mood On\n");
 } else {
  Serial.print("failed, rc=");
  Serial.print(mqttClient.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
   char payloadChar[length+1];
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int i = 0;

  for (i=0;i<length;i++) {
    payloadChar[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  }
  payloadChar[i] = '\0';
  Serial.println();

  if(strcmp(topic,topicVRCommandLog) == 0){
    if (strcmp(payloadChar, "KITCHEN")==0) {
      easyvr.playSound(39, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "WAKE_UP_KITCHEN", true);
      
      pcSerial.println("Hello Uninova, how can I help?\n");
      
    }else if (strcmp(payloadChar, "AUTOMATIC")==0) {
      easyvr.playSound(2, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "AUTOMATIC_MODE", true);
      mqttClient.publish(acdTopicPub, "AUTOMATIC_MODE");
      pcSerial.println("Automatic mode\n");    
        
    }else if (strcmp(payloadChar, "MANUAL")==0) {
      easyvr.playSound(22, EasyVR::VOL_FULL);
      // write your action code here
      mqttClient.publish(topicVRCommand, "MANUAL_MODE", true);
      mqttClient.publish(acdTopicPub, "MANUAL_MODE");
      pcSerial.println("Manual mode\n");
            
    }else if (strcmp(payloadChar, "OPEN")==0) {
      easyvr.playSound(25, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "OPEN", true);
      mqttClient.publish(acdTopicPub, "OPEN");
      pcSerial.println("Open\n");

    }else if (strcmp(payloadChar, "CLOSE")==0) {
      easyvr.playSound(8, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "CLOSE", true);
      mqttClient.publish(acdTopicPub, "CLOSE");
      pcSerial.println("Close\n");
      
    }else if (strcmp(payloadChar, "VOICE")==0) {
      easyvr.playSound(26, EasyVR::VOL_FULL);
      // write your action code here 
      mqttClient.publish(topicVRCommand, "OPEN_MODE", true);
      mqttClient.publish(acdTopicPub, "OPEN_MODE");
      pcSerial.println("Open mode\n");
      
    }else if (strcmp(payloadChar, "MOOD OFF")==0) {
      mqttClient.publish(topicVRCommand, "MOOD OFF", true);
      mqttClient.publish(topicMoodStatus, "0");
      pcSerial.println("Mood Off\n");
      
    }else if (strcmp(payloadChar, "HAPPY")==0) {
      easyvr.playSound(20, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "HAPPY", true);
      mqttClient.publish(topicMoodStatus, "6");
      pcSerial.println("Happy mood\n");
      
    }else if (strcmp(payloadChar, "SAD")==0) {
      easyvr.playSound(31, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"SAD", true);
      mqttClient.publish(topicMoodStatus, "2");
      pcSerial.println("Sad Mood\n");

      pcSerial.println("Have some\n");
      pcSerial.println(fFood);
      easyvr.playSound(37, EasyVR::VOL_FULL);

      switch(fFood){
          case 4:
            easyvr.playSound(40, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Wine", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf1, "2");
            pcSerial.println("Wine\n");
          break;
          case 5:
            easyvr.playSound(14, EasyVR::VOL_FULL);
            easyvr.playSound(10, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Cookies", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf2, "1");
          break;
          case 13:
            easyvr.playSound(6, EasyVR::VOL_FULL);
            easyvr.playSound(10, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Candy", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf2, "1");
          break;
          case 15:
                // write your action code here
            easyvr.playSound(13, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Coke", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf1, "3");
            pcSerial.println("Coke\n");
          break;
          case 19:
                // write your action code here
            easyvr.playSound(33, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Soda", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf1, "3");
            pcSerial.println("Soda\n");
          break;
          case 20:
            easyvr.playSound(3, EasyVR::VOL_FULL);
            easyvr.playSound(11, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Bacon", true);
            mqttClient.publish(topicMoodSection, "2");
      
            mqttClient.publish(topicShelf3, "4");
            pcSerial.println("Bacon\n");
          break;
          case 24:
          // write your action code here
            easyvr.playSound(7, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Champagne", true);
            mqttClient.publish(topicMoodSection, "1");
            
            mqttClient.publish(topicShelf1, "2");
            pcSerial.println("Champagne\n");
         break;
      }
      
      
    }else if (strcmp(payloadChar, "OK")==0) {
      easyvr.playSound(24, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "OKAY", true);
      mqttClient.publish(topicMoodStatus, "4");
      pcSerial.println("Neutral mood\n");
      
    }else if (strcmp(payloadChar, "Wine")==0) {
      easyvr.playSound(40, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      // write your action code here
      mqttClient.publish(topicVRCommand, "Wine", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "2");
      pcSerial.println("Wine\n");

    }else if (strcmp(payloadChar, "Cookies")==0) {
      easyvr.playSound(14, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Cookies", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "1");
      pcSerial.println("Cookies\n");

    }else if (strcmp(payloadChar, "Apples")==0) {
      easyvr.playSound(1, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Apples", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "4");
      pcSerial.println("Apples\n");
      
    }else if (strcmp(payloadChar, "Pasta")==0) {
      easyvr.playSound(27, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pasta", true);
      mqttClient.publish(topicMoodSection, "2");

      mqttClient.publish(topicShelf3, "1");
      pcSerial.println("Pasta\n");
      
    }else if (strcmp(payloadChar, "Ketchup")==0) {
      easyvr.playSound(21, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Ketchup", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "4");
      pcSerial.println("Ketchup\n");

    }else if (strcmp(payloadChar, "Tomatos")==0) {
      easyvr.playSound(36, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Tomatos", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf3, "2");
      pcSerial.println("Tomatos\n");
      
    }else if (strcmp(payloadChar, "Beans")==0) {
      easyvr.playSound(4, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Beans", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf3, "3");
      pcSerial.println("Beans\n");
      
    }else if (strcmp(payloadChar, "Sugar")==0) {
      easyvr.playSound(35, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Sugar", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "3");
      pcSerial.println("Sugar\n");
      
    }else if (strcmp(payloadChar, "Salt")==0) {
      easyvr.playSound(32, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Salt", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "2");
      pcSerial.println("Salt\n");
      
    }else if (strcmp(payloadChar, "Candy")==0) {
      easyvr.playSound(6, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Candy", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "1");
      pcSerial.println("Candy\n");
    }else if (strcmp(payloadChar, "Pepper")==0) {
      easyvr.playSound(29, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pepper", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "2");
      pcSerial.println("Pepper\n");
      
    }else if (strcmp(payloadChar, "Coke")==0) {
      easyvr.playSound(13, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Soda", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "3");
      pcSerial.println("Soda\n");
      
    }else if (strcmp(payloadChar, "Water")==0) {
      easyvr.playSound(38, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      
      mqttClient.publish(topicVRCommand, "Water", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "1");
      pcSerial.println("Water\n");
      
    }else if (strcmp(payloadChar, "Bread")==0) {
      easyvr.playSound(5, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Bread", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "2");
      pcSerial.println("Bread\n");
      
    }else if (strcmp(payloadChar, "Milk")==0) {
      easyvr.playSound(23, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Milk", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "1");
      pcSerial.println("Milk\n");
      
    }else if (strcmp(payloadChar, "Soda")==0) {
      easyvr.playSound(33, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Soda", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "3");
      pcSerial.println("Soda\n");
      
    }else if (strcmp(payloadChar, "Spaghetti")==0) {
      easyvr.playSound(34, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pasta", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "4");
      pcSerial.println("Pasta\n");
      
    }else if (strcmp(payloadChar, "Bacon")==0) {
      easyvr.playSound(3, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Bacon", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf3, "4");
      pcSerial.println("Bacon\n");
      
    }else if (strcmp(payloadChar, "Foie Gras")==0) {
      easyvr.playSound(17, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Foie Gras", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "4");
      pcSerial.println("Foie Gras\n");
      
    }else if (strcmp(payloadChar, "Eggs")==0) {
      easyvr.playSound(16, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Eggs", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "1");
      pcSerial.println("Eggs\n");
      
    }else if (strcmp(payloadChar, "Croissant")==0) {
      easyvr.playSound(15, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Croissant", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "2");
      pcSerial.println("Croissant\n");
    }else if (strcmp(payloadChar, "Champagne")==0) {
      easyvr.playSound(7, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Champagne", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "2");
      pcSerial.println("Champagne\n");
      
    }else if (strcmp(payloadChar, "Pate")==0) {
      easyvr.playSound(28, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pate", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "3");
      pcSerial.println("Pate NAO\n");
    }else{
      
      char * token = strtok(payloadChar, ":");
      if(token!=NULL){
        token = strtok(NULL, ":");
         pcSerial.println("I LIKEEEEEE\n");
         pcSerial.println(token);
        if (strcmp(payloadChar, "Wine")==0) {
          fFood = 4;
          pcSerial.println("Wine\n");

        }else if (strcmp(token, "Cookies")==0) {
          fFood = 5;
          pcSerial.println("CookiesZ\n");

        }else if (strcmp(token, "Candy")==0) {
          fFood = 13;
          pcSerial.println("Candy\n");
        }else if (strcmp(token, "Soda")==0) {
           fFood = 19;
          pcSerial.println("Soda\n");
      
       }else if (strcmp(token, "Bacon")==0) {
           fFood = 20;
          pcSerial.println("Bacon\n");
      
       }else if (strcmp(token, "Coke")==0) {
           fFood = 15;
          pcSerial.println("Soda\n");
          
       }else if (strcmp(token, "Champagne")==0) {
           fFood = 24;
          pcSerial.println("Champagne\n");
          
       }
      }
    }
  } 
}



void setup()
{
  // setup PC serial port
    // setup PC serial port
  Serial.begin(9600);
  pcSerial.begin(9600);
  Serial.println("setup");

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  mqttClient.setServer(broker, 1883);
  mqttClient.setCallback(callback);



  
bridge:
  // bridge mode?
  int mode = easyvr.bridgeRequested(pcSerial);
  switch (mode)
  {
  case EasyVR::BRIDGE_NONE:
    // setup EasyVR serial port
    port.begin(9600);
    // run normally
    pcSerial.println(F("Bridge not requested, run normally"));
    pcSerial.println(F("---"));
    break;
    
  case EasyVR::BRIDGE_NORMAL:
    // setup EasyVR serial port (low speed)
    port.begin(9600);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("Bridge connection aborted"));
    pcSerial.println(F("---"));
    break;
    
  case EasyVR::BRIDGE_BOOT:
    // setup EasyVR serial port (high speed)
    port.begin(115200);
    pcSerial.end();
    pcSerial.begin(115200);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("Bridge connection aborted"));
    pcSerial.println(F("---"));
    break;
  }

  // initialize EasyVR  
  while (!easyvr.detect())
  {
    pcSerial.println(F("EasyVR not detected!"));
    
    for (int i = 0; i < 10; ++i)
    {
      if (pcSerial.read() == '?')
        goto bridge;
      delay(100);
    }
  }

  pcSerial.print(F("EasyVR detected, version "));
  pcSerial.print(easyvr.getID());

  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, LOW); // Shield 2.0 LED off

  if (easyvr.getID() < EasyVR::EASYVR)
    pcSerial.print(F(" = VRbot module"));
  else if (easyvr.getID() < EasyVR::EASYVR2)
    pcSerial.print(F(" = EasyVR module"));
  else if (easyvr.getID() < EasyVR::EASYVR3)
    pcSerial.print(F(" = EasyVR 2 module"));
  else
    pcSerial.print(F(" = EasyVR 3 module"));
  pcSerial.print(F(", FW Rev."));
  pcSerial.println(easyvr.getID() & 7);

  easyvr.setDelay(0); // speed-up replies

  easyvr.setTimeout(5);
  easyvr.setLanguage(0); //<-- same language set on EasyVR Commander when code was generated

  group = EasyVR::TRIGGER; //<-- start group (customize)

  mqttClient.publish(topicVRCommand, "ON", true);

}

void loop()
{
  while(!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  init_easyVR();
}

void init_easyVR(){
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  if (group < 0) // SI wordset/grammar
  {
    pcSerial.print("Say a word in Wordset ");
    pcSerial.println(-group);
    easyvr.recognizeWord(-group);
  }
  else // SD group
  {
    pcSerial.print("Say a command in Group ");
    pcSerial.println(group);
    easyvr.recognizeCommand(group);
  }

  do
  {
    // allows Commander to request bridge on Zero (may interfere with user protocol)
    if (pcSerial.read() == '?')
    {
      setup();
      return;
    }
    // <<-- can do some processing here, while the module is busy
  }
  while (!easyvr.hasFinished());
  
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx == 0 && group == EasyVR::TRIGGER)
  {
    // beep
    
    // print debug message
    pcSerial.println("Word: ROBOT");
    // write your action code here
    // group = GROUP_X\SET_X; <-- jump to another group or wordset
    return;
  }
  else if (idx >= 0)
  {
    // beep
    
    // print debug message
    uint8_t flags = 0, num = 0;
    char name[32];
    pcSerial.print("Word: ");
    pcSerial.print(idx);
    if (easyvr.dumpGrammar(-group, flags, num))
    {
      for (uint8_t pos = 0; pos < num; ++pos)
      {
        if (!easyvr.getNextWordLabel(name))
          break;
        if (pos != idx)
          continue;
        pcSerial.print(F(" = "));
        pcSerial.println(name);
        break;
      }
    }
    // perform some action
    action();
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // beep
    
    // print debug message
    uint8_t train = 0;
    char name[32];
    pcSerial.print("Command: ");
    pcSerial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      pcSerial.print(" = ");
      pcSerial.println(name);
    }
    else
      pcSerial.println();
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout()){
      pcSerial.println("Timed out, try again...");
    }
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
     
      pcSerial.print("Error ");
      pcSerial.println(err, HEX);
    }
  }
}

void action()
{
  switch (group)
  {
  case GROUP_0:
    switch (idx)
    {
    case G0_KITCHEN:
      // write your action code here
      easyvr.playSound(39, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "WAKE_UP_KITCHEN", true);
      int len = strlen("WAKE_UP_KITCHEN");
      
      pcSerial.println("Hello Uninova, how can I help?\n");
      group = GROUP_1;
      break;
    }
    break;
  case GROUP_1:
    switch (idx)
    {
    case G1_AUTOMATIC:
      // write your action code here
      easyvr.playSound(2, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "AUTOMATIC_MODE", true);
      mqttClient.publish(acdTopicPub, "AUTOMATIC_MODE");
      pcSerial.println("Automatic mode\n");
   
     

      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_MANUAL:
      easyvr.playSound(22, EasyVR::VOL_FULL);
      // write your action code here
      mqttClient.publish(topicVRCommand, "MANUAL_MODE", true);
      mqttClient.publish(acdTopicPub, "MANUAL_MODE");
      pcSerial.println("Manual mode\n");
      
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_OPEN:
      // write your action code here
      easyvr.playSound(25, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "OPEN", true);
      mqttClient.publish(acdTopicPub, "OPEN");
      pcSerial.println("Open\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CLOSE:
      // write your action code here
      easyvr.playSound(8, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "CLOSE", true);
      mqttClient.publish(acdTopicPub, "CLOSE");
      pcSerial.println("Close\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_VOICE:
      easyvr.playSound(26, EasyVR::VOL_FULL);
      // write your action code here 
      mqttClient.publish(topicVRCommand, "OPEN_MODE", true);
      mqttClient.publish(acdTopicPub, "OPEN_MODE");
      pcSerial.println("Open mode\n");
      
     
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_WINE:
      easyvr.playSound(40, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      // write your action code here
      mqttClient.publish(topicVRCommand, "Wine", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "2");
      pcSerial.println("Wine\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_COOKIES:
      // write your action code here
      easyvr.playSound(14, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Cookies", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "1");
      pcSerial.println("Cookies\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_APPLES:
      // write your action code here
      easyvr.playSound(1, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Apples", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "4");
      pcSerial.println("Apples\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_PASTA:
      // write your action code here
      easyvr.playSound(27, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pasta", true);
      mqttClient.publish(topicMoodSection, "2");

      mqttClient.publish(topicShelf3, "1");
      pcSerial.println("Pasta\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_KETCHUP:
      // write your action code here
      easyvr.playSound(21, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Ketchup", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "4");
      pcSerial.println("Ketchup\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_TOMATOS:
      // write your action code here
      easyvr.playSound(36, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Tomatos", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf3, "2");
      pcSerial.println("Tomatos\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_BEANS:
      // write your action code here
      easyvr.playSound(4, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Beans", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf3, "3");
      pcSerial.println("Beans\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SUGAR:
      // write your action code here
      easyvr.playSound(35, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Sugar", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "3");
      pcSerial.println("Sugar\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SALT:
      // write your action code here
      easyvr.playSound(32, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"Salt", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "2");
      pcSerial.println("Salt\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CANDY:
      // write your action code here
      easyvr.playSound(6, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Candy", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "1");
      pcSerial.println("Candy\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_PEPPER:
      // write your action code here
      easyvr.playSound(29, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pepper", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "2");
      pcSerial.println("Pepper\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_COKE:
      // write your action code here
      easyvr.playSound(13, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Soda", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "3");
      pcSerial.println("Soda\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_WATER:
      // write your action code here
      easyvr.playSound(38, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      
      mqttClient.publish(topicVRCommand, "Water", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "1");
      pcSerial.println("Water\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_BREAD:
      // write your action code here
      easyvr.playSound(5, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Bread", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "2");
      pcSerial.println("Bread\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_MILK:
      // write your action code here  
      easyvr.playSound(23, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Milk", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "1");
      pcSerial.println("Milk\n");

    
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SODA:
      // write your action code here
      easyvr.playSound(33, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Soda", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "3");
      pcSerial.println("Soda\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    /*case G1_SPAGHETTI:
      // write your action code here
      easyvr.playSound(34, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pasta", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "4");
      pcSerial.println("Pasta\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;*/
    case G1_BACON:
      // write your action code here
      easyvr.playSound(3, EasyVR::VOL_FULL);
      easyvr.playSound(11, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Bacon", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf3, "4");
      pcSerial.println("Bacon\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_FOIE_GRAS:
      // write your action code here
      easyvr.playSound(17, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Foie Gras", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "4");
      pcSerial.println("Foie Gras\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_EGGS:
      // write your action code her
      easyvr.playSound(16, EasyVR::VOL_FULL);
      easyvr.playSound(12, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Eggs", true);
      mqttClient.publish(topicMoodSection, "2");
      
      mqttClient.publish(topicShelf4, "1");
      pcSerial.println("Eggs\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CROISSANT:
      // write your action code here
      easyvr.playSound(15, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Croissant", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "2");
      pcSerial.println("Croissant\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CHAMPAGNE:
      // write your action code here
      easyvr.playSound(7, EasyVR::VOL_FULL);
      easyvr.playSound(9, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Champagne", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf1, "2");
      pcSerial.println("Champagne\n");

     
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_PATE:
      // write your action code here
      easyvr.playSound(28, EasyVR::VOL_FULL);
      easyvr.playSound(10, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "Pate", true);
      mqttClient.publish(topicMoodSection, "1");
      
      mqttClient.publish(topicShelf2, "3");
      pcSerial.println("Pate\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_HAPPY:
      // write your action code here
      easyvr.playSound(20, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "HAPPY", true);
      mqttClient.publish(topicMoodStatus, "6");
      pcSerial.println("Happy mood\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SAD:
      // write your action code here
      easyvr.playSound(31, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand,"SAD", true);
      mqttClient.publish(topicMoodStatus, "2");
      pcSerial.println("Sad Mood\n");

      pcSerial.println("Have some\n");
      pcSerial.println(fFood);
      easyvr.playSound(37, EasyVR::VOL_FULL);
      switch(fFood){
          case 4:
            easyvr.playSound(40, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Wine", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf1, "2");
            pcSerial.println("Wine\n");
          break;
          case 5:
            easyvr.playSound(14, EasyVR::VOL_FULL);
            easyvr.playSound(10, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Cookies", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf2, "1");
          break;
          case 13:
            easyvr.playSound(6, EasyVR::VOL_FULL);
            easyvr.playSound(10, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Candy", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf2, "1");
          break;
          case 15:
                // write your action code here
            easyvr.playSound(13, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Soda", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf1, "3");
            pcSerial.println("Soda\n");
          break;
          case 19:
                // write your action code here
            easyvr.playSound(33, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Soda", true);
            mqttClient.publish(topicMoodSection, "1");
      
            mqttClient.publish(topicShelf1, "3");
            pcSerial.println("Soda\n");
          break;
          case 20:
            easyvr.playSound(3, EasyVR::VOL_FULL);
            easyvr.playSound(11, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Bacon", true);
            mqttClient.publish(topicMoodSection, "2");
      
            mqttClient.publish(topicShelf3, "4");
            pcSerial.println("Bacon\n");
          break;
          case 24:
          // write your action code here
            easyvr.playSound(7, EasyVR::VOL_FULL);
            easyvr.playSound(9, EasyVR::VOL_FULL);
            mqttClient.publish(topicVRCommand, "Champagne", true);
            mqttClient.publish(topicMoodSection, "1");
            
            mqttClient.publish(topicShelf1, "2");
            pcSerial.println("Champagne\n");
         break;
      }
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_MOOD_OFF:
      // write your action code here
      mqttClient.publish(topicVRCommand, "MOOD OFF", true);
      mqttClient.publish(topicMoodStatus, "0");
      pcSerial.println("Mood off\n");
      
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_OK:
      // write your action code here
      easyvr.playSound(24, EasyVR::VOL_FULL);
      mqttClient.publish(topicVRCommand, "OKAY", true);
      mqttClient.publish(topicMoodStatus, "4");
      pcSerial.println("Neutral mood\n");
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_I_LIKE:
      //signal that setting will be edited
      //save the next food to be mentioned
      //sugest it to the user
      easyvr.playSound(0, EasyVR::VOL_FULL);
      group = GROUP_2;
      break;
    }
    break;
  case GROUP_2:
    switch (idx)
    {
    case G2_WINE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      pcSerial.println("Favourite food set\n");
      fFood =4;
      easyvr.playSound(0, EasyVR::VOL_FULL);
      group = GROUP_1;
      break;
    case G2_COOKIES:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      pcSerial.println("Favourite food set\n");
      easyvr.playSound(0, EasyVR::VOL_FULL);
      fFood = 5;
      group = GROUP_1;
      break;
    case G2_CANDY:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      pcSerial.println("Favourite food set\n");
      fFood = 13;
      easyvr.playSound(0, EasyVR::VOL_FULL);
      group = GROUP_1;
      break;
    case G2_COKE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      pcSerial.println("Favourite food set\n");
      fFood = 15;
      easyvr.playSound(0, EasyVR::VOL_FULL);
      group = GROUP_1;
      break;
    case G2_SODA:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      pcSerial.println("Favourite food set\n");
      easyvr.playSound(0, EasyVR::VOL_FULL);
      fFood = 19;
      group = GROUP_1;
      break;
    case G2_BACON:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      pcSerial.println("Favourite food set\n");
      fFood = 20;
      easyvr.playSound(0, EasyVR::VOL_FULL);
      group = GROUP_1;
      break;
    case G2_CHAMPAGNE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      pcSerial.println("Favourite food set\n");
      fFood = 24;
      easyvr.playSound(0, EasyVR::VOL_FULL);
      group = GROUP_1;
      break;
    case G2_BACK:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      group = GROUP_1;
      break;
    }
    break;
  }
}
