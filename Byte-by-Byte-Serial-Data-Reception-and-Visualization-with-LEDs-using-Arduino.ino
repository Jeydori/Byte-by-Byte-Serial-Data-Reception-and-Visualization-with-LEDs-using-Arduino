#define clockButton A1
#define inputButton A2
#define playButton A3

const int ledRay[8] = {12, 11, 10, 9, 8, 7, 6, 5};
const int nBytes = 8;
const int nBits = 8;

byte storedBytes[nBytes];
byte clockState = 0;
byte lastClockState = 0;
byte inputState = 0;
byte lastInputState = 0;
byte playState = 0;
byte lastPlayState = 0;

int presentByte = 0;
int presentBit = 0;
int delayTime = 3000;

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

// Register the input state by shifting in the bits
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
  //digitalWrite(ledRay[i], (storedBytes[j] << 1) | inputState);
  
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

// Play the stored bytes by lighting up the corresponding LEDs
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

// Reset the stored bytes to zero
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
