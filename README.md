# Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino
Demonstrates how a clock pulse functions in a system and the storage of received bits in a memory-like manner

<img width="959" alt="image" src="https://github.com/Jeydori/Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino/assets/92672461/abbb65fc-3b95-4b99-a2ae-e947ab789e9c">


 **Purpose:** 
  The aim is to showcase the reception and accumulation of serial data through clock pulses, forming sets of 8 bits each. The process repeats for subsequent data sets.

 **Display:** 
  The received serial data will be shown as 8-bit characters using LEDs. Upon pressing play, each set will be displayed as a character with a 3-second interval. This activity resembles receiving data bit by bit and presenting it byte by byte. Additionally, it illustrates clock pulse functionality and memory-like bit storage.


In this tutorial, we will create an Arduino sketch that demonstrates the reception and visualization of byte-by-byte serial data using LEDs. The objective of this project is to show how serial data can be received and displayed one character (8 bits) at a time using a clock pulse to accumulate the data. This project will also serve as an example of how to use Arduino to interact with external components, like LEDs, and handle serial data.

 **Components Needed:**
  1. 1x Arduino board (e.g., Arduino Uno)
  
<img width="129" alt="image" src="https://github.com/Jeydori/Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino/assets/92672461/ec6427ba-f89a-4088-8dd4-c1d55889a79f">

  2. 8x LEDs
  
<img width="141" alt="image" src="https://github.com/Jeydori/Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino/assets/92672461/43653c26-838c-451f-91cc-bc22f93d6c60">

  3. 8x resistors (220-470 ohms) for current limiting of LEDs
  
<img width="146" alt="image" src="https://github.com/Jeydori/Traffic-Light-System-/assets/92672461/03585b92-4628-4a32-804e-b393a7cfe76b">
<img width="164" alt="image" src="https://github.com/Jeydori/Traffic-Light-System-/assets/92672461/6f3962cc-b209-46dd-8119-61ae915768ff">

  4. 1x Breadboard and jumper wires

<img width="151" alt="image" src="https://github.com/Jeydori/Traffic-Light-System-/assets/92672461/c0a4a434-33dd-4376-9bdd-bbe15d2cf894">
<img width="190" alt="image" src="https://github.com/Jeydori/Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino/assets/92672461/60943337-2c48-4790-86be-aa797d9ecd46">

  5. 3x Pushbutton

<img width="86" alt="image" src="https://github.com/Jeydori/Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino/assets/92672461/edd89fa1-8ea2-49fd-8fed-a5cf1c9f9384">




 **Step 1: Wiring the Circuit**

Connect the LEDs to pins 5 through 12 on the Arduino board.
Add current-limiting resistors in series with each LED (e.g., connect the resistor to the positive (long) leg of the LED and the other leg to the Arduino pins).
Connect three pushbuttons to analog pins A1, A2, and A3, respectively, for clockButton, inputButton, and playButton.

 **Step 2: Setting Up the Sketch**
 
Open the Arduino IDE and create a new sketch. Copy and paste the provided Arduino sketch code into the IDE.
    https://github.com/Jeydori/Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino/blob/main/Byte-by-Byte-Serial-Data-Reception-and-Visualization-with-LEDs-using-Arduino.ino

 **Step 3: Understanding the Code**

  setup() Function:
  
            void setup() {
              pinMode(clockButton, INPUT);
              pinMode(inputButton, INPUT);
              pinMode(playButton, INPUT);
              
              for (int i = 0; i < nBits; i++) {
                pinMode(ledRay[i], OUTPUT);
              }
              Serial.begin(9600);
              
              Serial.println(" ");
              Serial.println("BIT ORDER : BIT VALUE");
            }

  In the setup() function, we set the pins for clockButton, inputButton, and playButton as inputs using pinMode(). These buttons are connected to the Arduino board's analog pins A1, A2, and A3, respectively.

  We also set the pins for the LEDs (ledRay) as outputs to control their illumination. The for loop iterates through each LED pin and sets its mode to OUTPUT.

  Serial communication is initiated with a baud rate of 9600 using Serial.begin(9600). This is for debugging purposes to monitor the progress of the code. We also print some initial messages to the Serial Monitor to indicate that the Arduino is ready to receive and display data.


  loop() Function:

            void loop() {
              clockState = digitalRead(clockButton);
              inputState = digitalRead(inputButton);
              playState = digitalRead(playButton);
            
              // Register input state when clock button changes from low to high
              if (clockState != lastClockState) {
                if (clockState == 1) {
                  registerInputState();
                }
              }
              
              // Play the stored bytes when play button changes from low to high
              if (playState != lastPlayState) {
                if (playState == 1) {
                  playRegisteredBytes();
                  reset(); 
                }
              }
              
              lastClockState = clockState;
              lastInputState = inputState;
              lastPlayState = playState;
            }

  In the loop() function, we continuously monitor the state of the clockButton, inputButton, and playButton using digitalRead().
  
  When the clockButton changes from low to high (rising edge), we call the registerInputState() function. This function is responsible for capturing the inputState (0 or 1) from the inputButton and assembling 8 bits to form a byte. It then displays the received bits on the LEDs in real-time, visually representing the serial data reception process.
  
  Similarly, when the playButton changes from low to high (rising edge), we call the playRegisteredBytes() function. This function displays the stored bytes on the LEDs with a delay of 3 seconds between each byte, creating a visual representation of the received data.

  registerInputState() Function:  

              void registerInputState() {
                storedBytes[presentByte] = (storedBytes[presentByte] << 1) | inputState;
                presentBit++;
                
                for (int i = 0; i < nBytes; i++) {
                  Serial.print(" ");
                  for (int j = 0; j < nBits; j++) {
                    // Shift out the bits and write to the LED pins
                    digitalWrite(ledRay[j], (storedBytes[i] >> (nBits - 1 - j)) & 1);
                  }
                }
                
                // Move to the next byte if all bits in the current byte are stored
                if (presentBit >= nBits) {
                  presentByte++;
                  presentBit = 0;
                  
                  // Reset to the first byte if all bytes are stored
                  if (presentByte >= nBytes) {
                    presentByte = 0;
                  }
                }
                
                // Print the current byte, bit, and input state for debugging
                Serial.print(presentBit);
                Serial.print(" : ");
                Serial.print(inputState);
                Serial.println(" ");
              }

  The registerInputState() function is the heart of the project, as it manages the reception and storage of the serial data. It works as follows:

  Each time the clockButton is pressed (rising edge detected), the inputState (0 or 1) from the inputButton is captured and added to the currently accumulating byte in storedBytes.

  We use bitwise operations to perform left-shift and bitwise OR to add the new bit to the existing byte.

  The LEDs are then updated in real-time to visually display the received bits. The for loop iterates through the nBytes and nBits arrays, shifting out the bits and writing them to the corresponding LED pins using digitalWrite().

  If a complete byte (8 bits) is received (presentBit >= nBits), we move on to the next byte (presentByte++) and reset the presentBit counter to 0. If all bytes are received (presentByte >= nBytes), we reset to the first byte.

  For debugging purposes, we print the current bit number, input state, and the corresponding LED representation of the stored bytes to the Serial Monitor.


  playRegisteredBytes() Function:

                void playRegisteredBytes() {
                  Serial.print("DATA:");
                  
                  for (int i = 0; i < nBytes; i++) {
                    Serial.print(" ");
                    for (int j = 0; j < nBits; j++) {
                      // Shift out the bits and write to the LED pins
                      digitalWrite(ledRay[j], (storedBytes[i] >> (nBits - 1 - j)) & 1);
                      Serial.print((storedBytes[i] >> (nBits - 1 - j)) & 1);
                    }
                    
                    delay(delayTime);
                    
                    // Turn off all LEDs
                    for (int i = 0; i < nBits; i++) {
                      digitalWrite(ledRay[i], 0);
                    }
                  }
                }

  The playRegisteredBytes() function is triggered when the playButton is pressed. It displays the stored bytes on the LEDs one by one, with a 3-second delay between each byte. Additionally, it prints the corresponding bit representation of each byte to the Serial Monitor for debugging purposes.


  reset() Function:

            void reset() {
              for (int i = 0; i < nBytes; i++) {
                storedBytes[i] = 0;
                presentByte = 0;
                presentBit = 0;
              }
              
              Serial.println(" ");
              Serial.println("DATA HAS BEEN CLEARED");
              Serial.println("YOU CAN NOW INPUT BITS AGAIN");
            }

  The reset() function clears all the stored bytes, resetting the memory to prepare for new data reception. It sets all bytes to zero and resets the presentByte and presentBit counters. A message is printed to the Serial Monitor to indicate that the data has been cleared and the system is ready to receive new input.

  With a thorough understanding of the code's components, you can now create the circuit and upload the sketch to your Arduino board. Enjoy experimenting


  **Step 4: Uploading the Sketch**

  Connect your Arduino board to your computer using a USB cable. Select the correct board and port from the "Tools" menu in the Arduino IDE. Then, click the "Upload" button to upload the sketch to the Arduino board.


  **Step 5: Testing the Project**

  Once the sketch is uploaded successfully, the Arduino board will start receiving serial data when the inputButton is pressed (keep it pressed to simulate serial data input). The LEDs will light up one by one, representing each bit of the received data. When the playButton is pressed, the stored bytes will be displayed on the LEDs, with a 3-second interval between each byte.


