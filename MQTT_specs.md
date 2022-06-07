# VR Module MQTT Specifications

**Server**:  192.168.0.100
**Port**:1883

# Subscribe

## "mood/state"

**Description**: Turn ON/OFF the mood service or select the desired mood
**Payload**: string
**Payload format**: "status"
**Example**: "0"
| QoS    | 0    |
|--------|------|
| Retain | false |

| Name   | Type   | Description    | Values          | Required |
|--------|--------|----------------|-----------------------------------|----------|
| status | string | Controller for the led moods service. Turns ON/OFF or specifies the required mood| 0-OFF <br> 1-ON <br> 2-Negative mood<br> 3-Medium negative <br> 4-Neutral <br> 5-Medium positive<br> 6-Positive mood | true     |

## "mood/section"

**Description**: Controls the closet sections on the main led strip
**Payload**: string
**Payload format**: “section”
**Example**: “3”
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                       | Values                         | Required |
|--------|--------|-----------------------------------|--------------------------------|----------|
| section  | string | Section of the main led strip to be turned ON| 0-OFF<br>1- Closet 1<br>2- Closet 2<br>3-Closet 3<br>4-Closet 4<br>5-Closet 5<br>6-Closet 6 | true     |


## "shelf1"

**Description**: Turn ON one section of the Nº1 shelf
**Payload**: string
**Payload format**: “section”
**Example**: “3”
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                | Values                  | Required |
|--------|--------|----------------------------|-------------------------|----------|
| section  | string | Section of led to be turned ON| 0-OFF<br>
1-Wine<br>
2-Cookies<br>
3-Apples<br>
4-Pasta<br>
| true     |

## "shelf2"

**Description**: Turn ON one section of the Nº2 shelf
**Payload**: string
**Payload format**: “section”
**Example**: “3”
| QoS    | 0     |
|--------|-------|
| Retain | false |

| Name   | Type   | Description                | Values                  | Required |
|--------|--------|----------------------------|-------------------------|----------|
| section  | string | Section of led to be turned ON| 0-OFF<br>
1-Ketchup<br>
2-Tomatos<br>
3-Beans<br>
4-Sugar<br>
| true     |


## "voice_control/status"

**Description**: Signals the voice recognition command that is being used
**Payload**: string
**Payload format**: “command”
**Example**: “Wine”
| QoS    | 0    |
|--------|------|
| Retain | true |

| Name   | Type   | Description                  | Values        | Required |
|--------|--------|------------------------------|---------------|----------|
| command| string |Command that is being executed|See table below| true     |


|Vaues          | Description |
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
|Spaghetti      |Foods from the closet nº4 |
|Bacon          |Foods from the closet nº2 |
|Foie Gras      |Foods from the closet nº3 |
|Eggs           |Foods from the closet nº5 |
|Croissant      |Foods from the closet nº6 |
|Champagne      |Foods from the closet nº1 |
|Pate           |Foods from the closet nº2 |

