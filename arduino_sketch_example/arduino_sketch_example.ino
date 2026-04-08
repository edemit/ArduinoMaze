/**
 * ArduinoMaze - Example Sketch for Arduino Mega
 * 
 * This is a basic example to test serial communication with the Node.js server
 * 
 * Instructions:
 * 1. Connect your Arduino Mega via USB
 * 2. Upload this sketch to your Arduino
 * 3. Run the Node.js server: npm start
 * 4. Open http://localhost:3000 in Firefox
 * 5. Connect to the serial port
 * 6. Send commands like: LED_ON, LED_OFF, STATUS
 */

// Pin definitions
const int LED_PIN = 8;

// Timing variables
unsigned long lastStatusTime = 0;
const unsigned long STATUS_INTERVAL = 5000;  // Send status every 5 seconds

void setup() {
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Send startup message
  delay(500);  // Wait for serial connection to stabilize
  Serial.println("========================================");
  Serial.println("ArduinoMaze - Maze Game Controller");
  Serial.println("========================================");
  Serial.println("Initialized successfully!");
  Serial.println("Send commands: LED_ON, LED_OFF, STATUS");
  Serial.println("========================================");
}

void loop() {
  // Check for incoming serial data
  if (Serial.available() > 0) {
    // Read the incoming command
    String command = Serial.readStringUntil('\n');
    
    // Remove any trailing carriage returns or spaces
    command.trim();
    
    // Convert to uppercase for case-insensitive comparison
    command.toUpperCase();
    
    // Process the command
    processCommand(command);
  }
  
  // Send periodic status message
  if (millis() - lastStatusTime >= STATUS_INTERVAL) {
    sendStatus();
    lastStatusTime = millis();
  }
}

/**
 * Process incoming commands from the serial port
 */
void processCommand(String command) {
  Serial.print("[Command Received] ");
  Serial.println(command);
  
  if (command == "LED_ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("[Response] LED turned ON");
  }
  else if (command == "LED_OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("[Response] LED turned OFF");
  }
  else if (command == "TOGGLE") {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    Serial.print("[Response] LED toggled to ");
    Serial.println(digitalRead(LED_PIN) ? "ON" : "OFF");
  }
  else if (command == "STATUS") {
    sendStatus();
  }
  else if (command == "HELLO") {
    Serial.println("[Response] Hello from Arduino Mega!");
  }
  else if (command == "PING") {
    Serial.println("[Response] PONG");
  }
  else if (command == "HELP") {
    printHelp();
  }
  else {
    Serial.print("[Response] Unknown command: ");
    Serial.println(command);
    Serial.println("[Response] Type HELP for available commands");
  }
}

/**
 * Send current status
 */
void sendStatus() {
  Serial.println("[Status Report]");
  Serial.print("  LED Status: ");
  Serial.println(digitalRead(LED_PIN) ? "ON" : "OFF");
  Serial.print("  Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  Serial.print("  Free RAM: ");
  Serial.print(freeRam());
  Serial.println(" bytes");
  Serial.println("[End Status]");
}

/**
 * Print available commands
 */
void printHelp() {
  Serial.println("[Available Commands]");
  Serial.println("  LED_ON    - Turn on the built-in LED");
  Serial.println("  LED_OFF   - Turn off the built-in LED");
  Serial.println("  TOGGLE    - Toggle LED state");
  Serial.println("  STATUS    - Get device status");
  Serial.println("  PING      - Test connection (responds with PONG)");
  Serial.println("  HELLO     - Test communication");
  Serial.println("  HELP      - Show this message");
  Serial.println("[End Help]");
}

/**
 * Calculate free RAM (useful for debugging)
 */
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
