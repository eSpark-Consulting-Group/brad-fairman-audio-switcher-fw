# Audio Switcher Firmware Development
#### Client Name: Brad Fairman

## Project Requirements - Main Board (MUX IC)
The product is going to have an audio player constantly looping audio to the input of the device. The audio input can come in from two ways. Either the Headphone Jack 3.5mm OR RCA Left Input and Right input Jacks. The output device (Analog Audio Switcher) will be silent until a button is pressed. At the start, all pushbuttons are illuminated waiting for user input. When user presses the button, the Push button LED should turn off and then flash on/off on/off on/off while corresponding output is active. Based on the button press the outputs will go live for 30 seconds as per sheet attached below (separate for both relay board and MUX board) and then go silent again. The corresponding output LED will light up to indicate the audio channels that have been activated by the button press. If button is pressed again during 30 seconds, the output will turn OFF, its LED will also go OFF and the pushbutton LED will again go ON waiting for user input. Please also note that volume should have a range from 1 to 10 (min - max) with an inc/dec of 1.

### Button Layout
1. Towers
2. Bookshelves
3. Center Channel Bookshelves and heights
4. Add Subwoofer
5. Everything
6. Volume Down
7. Volume Up
8. TBD

### Mapping Information (MUX Board)
![image](https://github.com/user-attachments/assets/d4104f43-978c-4d91-883b-47e6d7742a9d)

### Mapping Information (Relay Board)
![image](https://github.com/user-attachments/assets/c83f34e2-5d85-474d-bbfc-afbb562ae76e)
