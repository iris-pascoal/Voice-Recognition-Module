# Voice Recognition Module

Voice recognition module for smart kitchen

## Authors

Íris Lisboa (Original Developer) & Jorge S. Calado (Supervisor)

## Hardware Requirements
 - Arduino Uno Wifi: https://store.arduino.cc/products/arduino-uno-wifi-rev2
 - EasyVR arduino shield: https://www.ptrobotics.com/shields-audio/3570-shield-easyvr-30-reconhecimento-de-voz.html
 - Microphone - Any mic compatible with the easyVR shield

## Required Libraries
 - EasyVR.h
 - SPI.h
 - Bridge.h
 - PubSubClient.h
 - WiFiNINA.h

## Other Requirements

 - MQTT broker - see MQTT_specs.md for moore info

## Installation/Getting started

To get started on the voice recognition module you should first connect the shield to the arduino. After connecting the program is ready to start. See table below for the avaliable voice commands

## Configurations

 - In the ```secrets.h``` file the network ssid and password must be updated to the respective network configurations
 - Configue the MQTT broker ```const char* server```

## Features

*Voice activated commands for interfacing a smart kitchen
*Auditory feedback

## Avaiable Voice Commands
|Commands       | Description |
|---------------|----------------|
|KITCHEN        |Activates the voice controlled kitchen|
|AUTOMATIC      |Cabinet door opens on approach and closes when moving away |
|MANUAL         |Cabinet door opens on approach and after the voice command “open” |
|VOICE          |Cabinet door allows the cabinet to stay continuously open until the voice command “close” |
|OPEN           |Voice command “open” |
|CLOSE          |Voice command “close” |
|MOOD_OFF       |Turns OFF the led mood service |
|HAPPY          |Sets happy mood |
|SAD            |Sets sad mood |
|OKAY  	        |Sets neutral mood |
|Wine           |Foods from the closet nº4 |
|Cookies        |Foods from the closet nº4 |
|Apples         |Foods from the closet nº4 |
|Pasta          |Foods from the closet nº4 |
|Ketchup        |Foods from the closet nº4 |
|Tomatos        |Foods from the closet nº4 |
|Beans          |Foods from the closet nº4 |
|Sugar          |Foods from the closet nº4 |
|Salt           |Foods from the closet nº1 |
|Candy          |Foods from the closet nº1 |
|Pepper         |Foods from the closet nº2 |
|Coke           |Foods from the closet nº3 |
|Water          |Foods from the closet nº5 |
|Bread          |Foods from the closet nº6 |
|Milk           |Foods from the closet nº1 |
|Soda           |Foods from the closet nº3 |
|Bacon          |Foods from the closet nº2 |
|Foie Gras      |Foods from the closet nº3 |
|Eggs           |Foods from the closet nº5 |
|Croissant      |Foods from the closet nº6 |
|Champagne      |Foods from the closet nº1 |
|Pate           |Foods from the closet nº2 |

->Check MQTT specs for more details
