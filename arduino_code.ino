// --- CONFIGURATION ---
const int RELAY_PIN = 2;

// Time settings in milliseconds 
// (1 second = 1000 milliseconds)
const unsigned long ON_TIME = 10000;   // 10 seconds
const unsigned long OFF_TIME = 180000; // 3 minutes (3 * 60 * 1000)
// ---------------------

void setup() {
  Serial.begin(9600);
  
  pinMode(RELAY_PIN, OUTPUT);
  // Start with the valve CLOSED (assuming Active-Low relay)
  digitalWrite(RELAY_PIN, HIGH); 
  
  Serial.println("Cyclic Timer Started!");
}

void loop() {
  // 1. OPEN THE VALVE
  Serial.println("Valve OPEN for 10 seconds...");
  digitalWrite(RELAY_PIN, LOW);   // Send LOW to trigger relay
  delay(ON_TIME);                 // Wait 10 seconds
  
  // 2. CLOSE THE VALVE
  Serial.println("Valve CLOSED. Waiting 3 minutes...");
  digitalWrite(RELAY_PIN, HIGH);  // Send HIGH to turn off relay
  delay(OFF_TIME);                // Wait 3 minutes
  
  // The loop automatically goes back to step 1 and repeats forever
}
