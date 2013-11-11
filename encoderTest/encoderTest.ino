

#define ENC_A 4
#define ENC_B 6

volatile int encoderRawPos = 0;

static boolean rotatingA = false;
static boolean rotatingB = false;

int counter = 0;

void setup() {
  pinMode(ENC_A, INPUT_PULLUP); 
  digitalWrite(ENC_A, HIGH);       
  pinMode(ENC_B, INPUT_PULLUP); 
  digitalWrite(ENC_B, HIGH); 

  attachInterrupt(ENC_A, encoderA, CHANGE);
  attachInterrupt(ENC_B, encoderB, CHANGE);
  
  Serial.begin (9600);
}

void encoderA() {
  rotatingA = true;
}
void encoderB() {
  rotatingB = true;
}

void loop() {
  
  boolean rotatingA2 = rotatingA;
  boolean rotatingB2 = rotatingB;
  
  rotatingA = false;
  rotatingB = false;
  
  while(rotatingA2 || rotatingB2) {
    delayMicroseconds(100);
    
    int enc_a = digitalRead(ENC_A);
    int enc_b = digitalRead(ENC_B);
    
    if (rotatingA2) {
      
      if ((enc_a == 1) && (enc_b == 1)) encoderRawPos++;
      if ((enc_a == 1) && (enc_b == 0)) encoderRawPos--;
      if ((enc_a == 0) && (enc_b == 1)) encoderRawPos--;
      if ((enc_a == 0) && (enc_b == 0)) encoderRawPos++;
      rotatingA2 = false; // Reset the flag back to false
    }
    
    if (rotatingB2) {
      //if ((enc_a == 1) && (enc_b == 1)) encoderRawPos--;
      //if ((enc_a == 0) && (enc_b == 1)) encoderRawPos++;
      rotatingB2 = false;
    }
  }
  
  counter++;
  if ((counter % 5000) == 0) Serial.println(encoderRawPos);
}
