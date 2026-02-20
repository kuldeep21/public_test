#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// --- CONFIGURATION VARIABLES ---
const int RELAY_PIN = 2;

// Set these to a few minutes from "now" to test it
const int TARGET_HOUR = 15;   // 24-hour format (e.g., 15 for 3 PM)
const int TARGET_MINUTE = 30; // Minute to trigger

// 2 minutes = 120,000 milliseconds
const unsigned long WATERING_DURATION = 120000; 
// -------------------------------

void setup() {
  Serial.begin(9600);
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  
  // Set HIGH immediately. Most relay modules are "Active-Low", 
  // meaning HIGH keeps the valve CLOSED.
  digitalWrite(RELAY_PIN, HIGH); 
  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC. Check your SDA/SCL wiring!");
    while (1); // Halt execution
  }
  
  Serial.println("System initialized. Waiting for scheduled time...");
}

void loop() {
  DateTime now = rtc.now();
  
  // Print time to Serial Monitor every second so you can verify it's accurate
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  
  // Check if current time matches target time
  if (now.hour() == TARGET_HOUR && now.minute() == TARGET_MINUTE) {
    Serial.println("\n*** TARGET TIME REACHED! OPENING VALVE ***");
    
    digitalWrite(RELAY_PIN, LOW);  // Send LOW signal to turn relay ON
    delay(WATERING_DURATION);      // Block execution and wait for 2 minutes
    
    Serial.println("*** WATERING COMPLETE! CLOSING VALVE ***\n");
    digitalWrite(RELAY_PIN, HIGH); // Send HIGH signal to turn relay OFF
    
    // Wait an extra minute so the loop doesn't immediately re-trigger 
    // while we are still in the TARGET_MINUTE window.
    delay(60000); 
  }
  
  delay(1000); // Wait 1 second before checking the clock again
}
