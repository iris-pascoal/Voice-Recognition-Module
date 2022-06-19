# VR Module MQTT Specifications

**Server**:  192.168.0.100
**Port**:1883

## "mood/state"

**Description**: Turn ON/OFF the mood service or select the desired mood<br>
**Payload**: string<br>
**Payload format**: "status"<br>
**Example**: "0"<br>
| QoS    | 0    |
|--------|------|
| Retain | false |

| Name   | Type   | Description    | Values          | Required |
|--------|--------|----------------|-----------------------------------|----------|
| status | string | Controller for the led moods service. Turns ON/OFF or specifies the required mood| 0-OFF <br> 1-ON <br> 2-Negative mood<br> 3-Medium negative <br> 4-Neutral <br> 5-Medium positive<br> 6-Positive mood | true     |

## "mood/state/status"

**Description**: Control topic. Announces the status of the mood service<br>
**Payload**: string<br>
**Payload format**: "status"<br>
**Example**: "0"<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description               | Values                              | Required |
|--------|--------|---------------------------|-------------------------------------|----------|
| status | string | Status of the mood service| 0-OFF <br> 1-ON <br> 2-Negative mood<br> 3-Medium negative <br> 4-Neutral <br> 5-Medium positive<br> 6-Positive mood | true     |

## "mood/color/*"<br>

### *from 1 to 6 each representing one mood service status in which:

|Value  | Description   |
|-------|---------------|
|1      |ON             |
|2      |Negative Mood  |
|3      |Medium Negative|
|4      |Neutral        |
|5      |Medium Positive|
|6      |Positive Mood  |

**Description**: Topic for controlling the color of each mood state<br>
**Payload**: string<br>
**Payload format**: “R,G,B”<br>
**Example**: “255,255,255”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                       | Values                         | Required |
|--------|--------|-----------------------------------|--------------------------------|----------|
| color  | string | RGB string with the required color| Any value from 0 to 255 | true     |


## "mood/color/*/status"

### *from 1 to 6 each representing one mood service status in which:

|Value  | Description   |
|-------|---------------|
|1      |ON             |
|2      |Negative Mood  |
|3      |Medium Negative|
|4      |Neutral        |
|5      |Medium Positive|
|6      |Positive Mood  |

**Description**: Control topic, announces the color of the respective mood state<br>
**Payload**: string<br>
**Payload format**: “R,G,B”<br>
**Example**: “255,255,255”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                       | Values                         | Required |
|--------|--------|-----------------------------------|--------------------------------|----------|
| color  | string | RGB string with the required color| Any value from 0 to 255 | true     |

## "mood/section"

**Description**: Controls the closet sections on the main led strip<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                       | Values                         | Required |
|--------|--------|-----------------------------------|--------------------------------|----------|
| section  | string | Section of the main led strip to be turned ON| 0-OFF<br>1- Closet 1<br> | true     |

## "mood/section/status"

**Description**: Anounnces the closet section that is being used<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                       | Values                         | Required |
|--------|--------|-----------------------------------|--------------------------------|----------|
| section  | string | Section of the main led strip to be turned ON| 0-OFF<br>1- Closet 1<br>2- Closet 2 | true     |

## "mood/section/color"

**Description**: Topic to define the color of the main led strip’s sections<br>
**Payload**: string<br>
**Payload format**: “R,G,B”<br>
**Example**: “255,255,255”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                | Values                  | Required |
|--------|--------|----------------------------|-------------------------|----------|
| color  | string | RGB string separated by ‘,’| Any value from 0 to 255 | true     |


## "mood/section/color/status"

**Description**: Topic to publish the color of the main led strip’s sections<br>
**Payload**: string<br>
**Payload format**: “R,G,B”<br>
**Example**: “255,255,255”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                | Values                  | Required |
|--------|--------|----------------------------|-------------------------|----------|
| color  | string | RGB string separated by ‘,’| Any value from 0 to 255 | true     |


## "shelf1"

**Description**: Turn ON one section of the Nº1 shelf<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                   | Values                                                   | Required |
|--------|--------|-------------------------------|----------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Water or Milk<br> 2- Wine or Champagne<br>3-Soda or Coke<br>4-Apples<br>| true     |

## "shelf1/status"

**Description**:Announces which section of the Nº1 shelf is ON<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                   | Values                                                   | Required |
|--------|--------|-------------------------------|----------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Water or Milk<br> 2- Wine or Champagne<br>3-Soda or Coke<br>4-Apples<br>| true     |

## "shelf2"

**Description**: Turn ON one section of the Nº2 shelf<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                   | Values                                                       | Required |
|--------|--------|-------------------------------|--------------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Candy or Cookies<br> 2-Croissant or Bread<br> 3-Pate<br> 4-Foie Gras<br>| true     |

## "shelf2/staus"

**Description**:Announces which section of the Nº2 shelf is ON<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                   | Values                                                       | Required |
|--------|--------|-------------------------------|--------------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Candy or Cookies<br> 2-Croissant or Bread<br> 3-Pate<br> 4-Foie Gras<br>| true     |

## "shelf3"

**Description**: Turn ON one section of the Nº3 shelf<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                   | Values                                                       | Required |
|--------|--------|-------------------------------|--------------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Pasta <br> 2-Tomatos<br> 3-Beans<br> 4-Bacon<br>| true     |

## "shelf3/status"

**Description**:Announces which section of the Nº3 shelf is ON<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                   | Values                                                       | Required |
|--------|--------|-------------------------------|--------------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Pasta <br> 2-Tomatos<br> 3-Beans<br> 4-Bacon<br>| true     |

## "shelf4"

**Description**: Turn ON one section of the Nº4 shelf<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                   | Values                                                       | Required |
|--------|--------|-------------------------------|--------------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Eggs<br> 2-Pepper or Salt<br> 3-Sugar<br> 4-Ketchup<br>| true     |

## "shelf4/status"

**Description**:Announces which section of the Nº4 shelf is ON<br>
**Payload**: string<br>
**Payload format**: “section”<br>
**Example**: “3”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                   | Values                                                       | Required |
|--------|--------|-------------------------------|--------------------------------------------------------------|----------|
| section| string | Section of led to be turned ON| 0-OFF<br> 1-Eggs<br> 2-Pepper or Salt<br> 3-Sugar<br> 4-Ketchup<br>| true     |

## "shelf/color/*"

### *from  0 to 15:

|Value  | Description   |
|-------|---------------|
|0      |Water/Milk section   |
|1      |Wine/Champagne section  |
|2      |Soda/Coke section|
|3      |Apples section |
|4      |Candy/Cookies section  |
|5      |Croissant/Bread section|
|6      |Pate section   |
|7      |Foie Gras section  |
|8      |Pasta section   |
|9      |Tomatos section    |
|10     |Beans section|
|11     |Bacon section |
|12     |Eggs section  |
|13     |Pepper/salt section|
|14     |Sugar section   |
|15     |Ketchup section  |

**Description**: Changes the color of a section of the shelves<br>
**Payload**: string<br>
**Payload format**: “R,G,B”<br>
**Example**: “255,255,255”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                       | Values                         | Required |
|--------|--------|-----------------------------------|--------------------------------|----------|
| color  | string | RGB string with the required color| Any value from 0 to 255        | true     |

## "shelf/color/*/status"

### *from  0 to 15:

|Value  | Description   |
|-------|---------------|
|0      |Water/Milk section   |
|1      |Wine/Champagne section  |
|2      |Soda/Coke section|
|3      |Apples section |
|4      |Candy/Cookies section  |
|5      |Croissant/Bread section|
|6      |Pate section   |
|7      |Foie Gras section  |
|8      |Pasta section   |
|9      |Tomatos section    |
|10     |Beans section|
|11     |Bacon section |
|12     |Eggs section  |
|13     |Pepper/salt section|
|14     |Sugar section   |
|15     |Ketchup section  |

**Description**: Announces the color of the respective shelf section<br>
**Payload**: string<br>
**Payload format**: “R,G,B”<br>
**Example**: “255,255,255”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                       | Values                         | Required |
|--------|--------|-----------------------------------|--------------------------------|----------|
| color  | string | RGB string with the required color| Any value from 0 to 255 | true     |

## " automatic_cabinet_door/controls"

**Description**: Commands for the cabinet doors<br>
**Payload**: string<br>
**Payload format**: “command”<br>
**Example**: “AUTOMATIC_MODE”<br>
| QoS    | 0    |
|--------|------|
| Retain | false |

| Name   | Type   | Description                   | Values                         | Required |
|--------|--------|-------------------------------|--------------------------------|----------|
| command| string |Automatic cabinet door commands|AUTOMATIC_MODE,MANUAL_MODE,OPEN_MODE,OPEN,CLOSE
 | true     |



## "voice_control/status"

**Description**: Signals the voice recognition command that is being used<br>
**Payload**: string<br>
**Payload format**: “command”<br>
**Example**: “Wine”<br>
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                  | Values        | Required |
|--------|--------|------------------------------|---------------|----------|
| command| string |Command that is being executed|See table below| true     |

Avaliable voice commands

|Command        | Description |
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
|Wine           |Foods from the closet nº1 |
|Cookies        |Foods from the closet nº1 |
|Apples         |Foods from the closet nº1 |
|Pasta          |Foods from the closet nº2 |
|Ketchup        |Foods from the closet nº2 |
|Tomatos        |Foods from the closet nº2 |
|Beans          |Foods from the closet nº2 |
|Sugar          |Foods from the closet nº2 |
|Salt           |Foods from the closet nº1 |
|Candy          |Foods from the closet nº2 |
|Pepper         |Foods from the closet nº2 |
|Coke           |Foods from the closet nº1 |
|Water          |Foods from the closet nº1 |
|Bread          |Foods from the closet nº1 |
|Milk           |Foods from the closet nº1 |
|Soda           |Foods from the closet nº1 |
|Bacon          |Foods from the closet nº2 |
|Foie Gras      |Foods from the closet nº1 |
|Eggs           |Foods from the closet nº2 |
|Croissant      |Foods from the closet nº1 |
|Champagne      |Foods from the closet nº1 |
|Pate           |Foods from the closet nº1 |
|I_LIKE         |The "i like" voice command unlocks a new group of voice commands from which the user canchoose its favourite item. See table below. |


|Command  |
|---------|
|Wine     |
|Cookies  |
|Candy    |
|Coke     |
|Soda     |
|Bacon    |
|Champagne|
|Back     |

## "voice_control/commands"

**Description**: This topic commands the easyVR functionalities, it replaces the voice commands for MQTT topics<br>
**Payload**: string<br>
**Payload format**: “command”<br>
**Example**: “AUTOMATIC_MODE”<br>
| QoS    | 0    |
|--------|------|
| Retain | false|


| Name   | Type   | Description                  | Values        | Required |
|--------|--------|------------------------------|---------------|----------|
| command| string |Command                       |See table below| true     |


Syntax of commands

|Command        | Description |
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
|Wine           |Foods from the closet nº1 |
|Cookies        |Foods from the closet nº1 |
|Apples         |Foods from the closet nº1 |
|Pasta          |Foods from the closet nº2 |
|Ketchup        |Foods from the closet nº2 |
|Tomatos        |Foods from the closet nº2 |
|Beans          |Foods from the closet nº2 |
|Sugar          |Foods from the closet nº2 |
|Salt           |Foods from the closet nº1 |
|Candy          |Foods from the closet nº2 |
|Pepper         |Foods from the closet nº2 |
|Coke           |Foods from the closet nº1 |
|Water          |Foods from the closet nº1 |
|Bread          |Foods from the closet nº1 |
|Milk           |Foods from the closet nº1 |
|Soda           |Foods from the closet nº1 |
|Bacon          |Foods from the closet nº2 |
|Foie Gras      |Foods from the closet nº1 |
|Eggs           |Foods from the closet nº2 |
|Croissant      |Foods from the closet nº1 |
|Champagne      |Foods from the closet nº1 |
|Pate           |Foods from the closet nº1 |
|I like:food    |The "i like" commands sets a favourite food for the user. Replace "food" with values from table below|


|Command  |
|---------|
|Wine     |
|Cookies  |
|Candy    |
|Coke     |
|Soda     |
|Bacon    |
|Champagne|
|Back     |
