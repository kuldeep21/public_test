#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// --- CONFIGURATION ---
const int RELAY_PIN = 2;
const int TARGET_HOUR = 7;    // 7 AM (Use 24-hour format, e.g., 19 for 7 PM)
const int TARGET_MINUTE = 0;  // 00 minutes
const unsigned long WATERING_DURATION = 120000; // 2 minutes in milliseconds

// Safety lock to prevent double-watering
bool hasWateredToday = false; 
// ---------------------

void setup() {
  Serial.begin(9600);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Keep valve CLOSED on startup (Active-Low)
  
  if (!rtc.begin()) {
    Serial.println("RTC module not found! Check wiring.");
    while (1); // Halt if no RTC is detected
  }
  
  Serial.println("Automated Water Tap System Active.");
}

void loop() {
  DateTime now = rtc.now();
  
  // 1. Reset the safety lock every night at Midnight
  if (now.hour() == 0 && now.minute() == 0 && now.second() == 0) {
    hasWateredToday = false;
    Serial.println("Midnight reset: Ready for tomorrow's watering.");
    delay(1000); // Wait 1 second so it doesn't trigger this reset twice
  }
  
  // 2. Check if it is time to water
  if (now.hour() == TARGET_HOUR && now.minute() == TARGET_MINUTE && !hasWateredToday) {
    
    Serial.println("7:00 AM reached! Opening the valve...");
    
    digitalWrite(RELAY_PIN, LOW);    // OPEN TAP
    delay(WATERING_DURATION);        // Wait exactly 2 minutes
    digitalWrite(RELAY_PIN, HIGH);   // CLOSE TAP
    
    Serial.println("Watering complete. Locking until tomorrow.");
    hasWateredToday = true;          // Engage the safety lock
  }
  
  delay(1000); // Rest the processor, check the time again in 1 second
}
