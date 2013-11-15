/* Buttons to USB Mouse Example

   You must select Mouse from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

float x = 0;
float y = 0;

void setup() {
  Serial.begin(115200);
}


void loop() {

    x = analogRead(A0);
    y = analogRead(A1);
    
    // scale them to be between -1 and 1
    x = -(x-512.)/512.;
    y = -(y-512.)/512.;
    
    // take the square root
    x = pow(abs(x), 1.5) * x/abs(x);
    y = pow(abs(y), 1.5) * y/abs(y);
    

    // scale up
    x = x * 20.;
    y = y * 20.;
    
    Serial.println("moving X: " + String(x) + ", Y: " + String(y));
    
    Mouse.move(x, y);
    delay(10);
}

