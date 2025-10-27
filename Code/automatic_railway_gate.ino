#include <Servo.h>                 // Include Servo library
#include <Wire.h>                  // Include I2C communication library
#include <LiquidCrystal_I2C.h>     // Include LCD library for I2C displays

// Create servo objects for both gates
Servo servoMain;      // Left gate
Servo servoMain2;     // Right gate

// Define pin numbers
const int trigPin = 10;   // Ultrasonic sensor trigger pin
const int echoPin = 11;   // Ultrasonic sensor echo pin
const int buzzer  = 6;    // Buzzer pin
const int redLED  = 7;    // Red LED pin
const int greenLED = 8;   // Green LED pin

// Create LCD object (I2C address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables for distance calculation
float duration;   // Echo time
float cm;         // Distance in cm (float)
int distance;     // Distance as integer

// Function to scroll text on LCD
void scrollText(String message, int delayTime) {
  for (int i = 0; i < message.length() - 15; i++) {
    lcd.clear();                                  // Clear screen
    lcd.setCursor(0, 0);                          // Start at top row
    lcd.print(message.substring(i, i + 16));      // Show 16-char window
    delay(delayTime);                             // Wait before next shift
  }
}

void setup() {
  // Attach servos to their respective pins
  servoMain.attach(9);    // Gate 1 (Left)
  servoMain2.attach(5);   // Gate 2 (Right)

  // Configure sensor and alert pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Initialize the LCD display
  lcd.init();             
  lcd.backlight();        

  // Show scrolling welcome message
  scrollText("  Automatic Railway Gate System  ", 200);

  // Initial gate status: open
  servoMain.write(0);           // Open Gate 1
  servoMain2.write(0);          // Open Gate 2
  digitalWrite(greenLED, HIGH); // Green LED ON
  digitalWrite(redLED, LOW);    // Red LED OFF
  digitalWrite(buzzer, LOW);    // Buzzer OFF

  // Display system ready status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Gates are OPEN");
  delay(2000); // Wait 2 seconds
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delay(1);                                // Small delay before trigger
  digitalWrite(trigPin, HIGH);            // Start trigger pulse
  delayMicroseconds(50);                  
  digitalWrite(trigPin, LOW);             // End trigger pulse

  // Measure echo time
  duration = pulseIn(echoPin, HIGH);

  // Convert time to distance using your formula
  cm = (duration / 68.82);     // Your preferred formula
  distance = cm;               // Store as integer for comparison

  // If train is detected (closer than 10 cm)
  if (distance < 10) {
    servoMain.write(90);       // Close Gate 1
    servoMain2.write(90);      // Close Gate 2
    digitalWrite(buzzer, HIGH);    // Turn buzzer ON
    digitalWrite(redLED, HIGH);    // Red LED ON
    digitalWrite(greenLED, LOW);   // Green LED OFF

    // Show alert message on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Train Detected!");
    lcd.setCursor(0, 1);
    lcd.print("Gates CLOSING...");
    delay(3000);  // Keep gates closed for 4 seconds
  } 
  else {
    // No train nearby
    servoMain.write(0);         // Open Gate 1
    servoMain2.write(0);        // Open Gate 2
    digitalWrite(buzzer, LOW);  // Buzzer OFF
    digitalWrite(redLED, LOW);  // Red LED OFF
    digitalWrite(greenLED, HIGH); // Green LED ON

    // Show normal status on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Train Nearby");
    lcd.setCursor(0, 1);
    lcd.print("Gates are OPEN");
    delay(500); // Small delay to avoid LCD flicker
  }
}