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

const char* broker = "192.168.0.100";//"broker.mqtt-dashboard.com";
const char* acdTopicPub = "automatic_cabinet_door/controls";//publish
const char* topicShelf1 = "shelf1";//publish
const char* topicShelf2 = "shelf2";//publish
const char* topicMoodStatus = "mood/state";//publish
const char* topicMoodSection = "mood/section"; //publish
const char* topicVRCommand = "voice_control/status";//publish
const char* topicVRCommandLog = "voice_control/Command_log";//publish

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
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
  G1_SPAGHETTI = 20,
  G1_BACON = 21,
  G1_FOIE_GRAS = 22,
  G1_EGGS = 23,
  G1_CROISSANT = 24,
  G1_CHAMPAGNE = 25,
  G1_PATE = 26,
  G1_VOICE = 27,
  G1_HAPPY = 28,
  G1_SAD = 29,
  G1_MOOD_OFF = 30,
  G1_OK = 31,
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
  // Once connected, publish an announcement...
  // ... and subscribe to topic    
 } else {
  Serial.print("failed, rc=");
  Serial.print(mqttClient.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
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
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // print debug message
    pcSerial.println("Word: ROBOT");
    // write your action code here
    // group = GROUP_X\SET_X; <-- jump to another group or wordset
    return;
  }
  else if (idx >= 0)
  {
    // beep
    easyvr.playSound(0, EasyVR::VOL_FULL);
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
    easyvr.playSound(0, EasyVR::VOL_FULL);
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
      mqttClient.publish(topicVRCommand, "AUTOMATIC_MODE", true);
      mqttClient.publish(acdTopicPub, "AUTOMATIC_MODE");
      pcSerial.println("Automatic mode\n");
   
     

      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_MANUAL:
      // write your action code here
      mqttClient.publish(topicVRCommand, "MANUAL_MODE", true);
      mqttClient.publish(acdTopicPub, "MANUAL_MODE");
      pcSerial.println("Manual mode\n");
      
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_OPEN:
      // write your action code here
      mqttClient.publish(topicVRCommand, "OPEN", true);
      mqttClient.publish(acdTopicPub, "OPEN");
      pcSerial.println("Open\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CLOSE:
      // write your action code here
      mqttClient.publish(topicVRCommand, "CLOSE", true);
      mqttClient.publish(acdTopicPub, "CLOSE");
      pcSerial.println("Close\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_VOICE:
      // write your action code here 
      mqttClient.publish(topicVRCommand, "OPEN_MODE", true);
      mqttClient.publish(acdTopicPub, "OPEN_MODE");
      pcSerial.println("Open mode\n");
      
     
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_WINE:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Wine", true);
      mqttClient.publish(topicMoodSection, "4");
      
      mqttClient.publish(topicShelf1, "1");
      pcSerial.println("Wine\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_COOKIES:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Cookies", true);
      mqttClient.publish(topicMoodSection, "4");
      
      mqttClient.publish(topicShelf1, "2");
      pcSerial.println("Cookies\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_APPLES:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Apples", true);
      mqttClient.publish(topicMoodSection, "4");
      
      mqttClient.publish(topicShelf1, "3");
      pcSerial.println("Apples\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_PASTA:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Pasta", true);
      mqttClient.publish(topicMoodSection, "4");

      mqttClient.publish(topicShelf1, "4");
      pcSerial.println("Pasta\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_KETCHUP:
      // write your action code here
      mqttClient.publish(topicVRCommand,"Ketchup", true);
      mqttClient.publish(topicMoodSection, "4");
      
      mqttClient.publish(topicShelf2, "1");
      pcSerial.println("Ketchup\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_TOMATOS:
      // write your action code here
      mqttClient.publish(topicVRCommand,"Tomatos", true);
      mqttClient.publish(topicMoodSection, "4");
      
      mqttClient.publish(topicShelf2, "2");
   

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_BEANS:
      // write your action code here
      mqttClient.publish(topicVRCommand,"Beans", true);
      mqttClient.publish(topicMoodSection, "4");
      
      mqttClient.publish(topicShelf2, "3");
     

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SUGAR:
      // write your action code here
      mqttClient.publish(topicVRCommand,"Sugar", true);
      mqttClient.publish(topicMoodSection, "4");
      
      mqttClient.publish(topicShelf2, "4");
    

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SALT:
      // write your action code here
      mqttClient.publish(topicVRCommand,"Salt", true);
      mqttClient.publish(topicMoodSection, "1");

      mqttClient.publish(topicShelf2, "2");
     
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CANDY:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Candy", true);
      mqttClient.publish(topicMoodSection, "1");
      pcSerial.println("Candy\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_PEPPER:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Pepper", true);
      mqttClient.publish(topicMoodSection, "2");
      pcSerial.println("Pepper\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_COKE:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Soda", true);
      mqttClient.publish(topicMoodSection, "3");
      pcSerial.println("Soda\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_WATER:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Water", true);
      mqttClient.publish(topicMoodSection, "5");
      pcSerial.println("Water\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_BREAD:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Bread", true);
      mqttClient.publish(topicMoodSection, "6");
      pcSerial.println("Bread\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_MILK:
      // write your action code here  
      mqttClient.publish(topicVRCommand, "Milk", true);
      mqttClient.publish(topicMoodSection, "1");
      pcSerial.println("Milk\n");

    
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SODA:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Soda", true);
      mqttClient.publish(topicMoodSection, "3");
      pcSerial.println("Soda\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SPAGHETTI:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Pasta", true);
      mqttClient.publish(topicMoodSection, "4");

      mqttClient.publish(topicShelf2, "1");
      pcSerial.println("Pasta\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_BACON:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Bacon", true);
      mqttClient.publish(topicMoodSection, "2");
      pcSerial.println("Bacon\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_FOIE_GRAS:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Foie Gras", true);
      mqttClient.publish(topicMoodSection, "3");
      pcSerial.println("Foie Gras\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_EGGS:
      // write your action code her
      mqttClient.publish(topicVRCommand, "Eggs", true);
      mqttClient.publish(topicMoodSection, "5");
      pcSerial.println("Eggs\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CROISSANT:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Croissant", true);
      mqttClient.publish(topicMoodSection, "6");
      pcSerial.println("Croissant\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_CHAMPAGNE:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Champagne", true);
      mqttClient.publish(topicMoodSection, "1");
      pcSerial.println("Champagne\n");

     
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_PATE:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Pate", true);
      mqttClient.publish(topicMoodSection, "2");
      pcSerial.println("Pate\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_HAPPY:
      // write your action code here
      mqttClient.publish(topicVRCommand, "HAPPY MOOD", true);
      mqttClient.publish(topicMoodStatus, "6");
      pcSerial.println("Happy mood\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SAD:
      // write your action code here
      mqttClient.publish(topicVRCommand,"SAD MOOD", true);
      mqttClient.publish(topicMoodStatus, "2");
      pcSerial.println("sad Mood\n");

      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_MOOD_OFF:
      // write your action code here
      mqttClient.publish(topicVRCommand, "Mood OFF", true);
      mqttClient.publish(topicMoodStatus, "0");
      pcSerial.println("Neutral mood\n");
      
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_OK:
      // write your action code here
      mqttClient.publish(topicVRCommand, "NEUTRAL MOOD", true);
      mqttClient.publish(topicMoodStatus, "4");
      pcSerial.println("Neutral mood\n");
      
      
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    }
    break;
  }
}
