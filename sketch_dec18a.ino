#include <WiFiNINA.h>

#define ENA 9   // Motor A speed control
#define ENB 10  // Motor B speed control

#define IN1 4  // Motor A direction control
#define IN2 5
#define IN3 6  // Motor B direction control
#define IN4 7

const char* ssid = "Legilimens";          // Your WiFi SSID
const char* password = "hotspotkey";  // Your WiFi Password

WiFiServer server(80);  // Start a server on port 80

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);  // Start the serial monitor

  while (!Serial)
    ;  // Wait for Serial Monitor to open (for some boards)
  Serial.println("Attempting to connect to WiFi...");

  // Check if the WiFi module is working
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not found!");
    while (true)
      ;  // Stop execution if module is not detected
  }

  // Attempt to connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print the IP address once connected
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Start the server
}

void loop() {
  WiFiClient client = server.available(); // Check if a client has connected

  if (client) { // If a client is connected
    Serial.println("New Client Connected");
    String currentLine = ""; // Store the incoming HTTP request

    while (client.connected()) { // While the client is connected
      if (client.available()) { // If there's data available
        char c = client.read(); // Read the data
        Serial.write(c);        // Write it to Serial Monitor for debugging

        // If the end of the request is reached
        if (c == '\n' && currentLine.length() == 0) {
          // Send a simple HTTP response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println(); // Blank line separates headers from body
          client.println("<!DOCTYPE html><html>");
          client.println("<head><title>Arduino Web Server</title></head>");
          client.println("<body><h1>Hello from Arduino!</h1></body></html>");
          break;
        }

        // Clear current line when new line character is received
        if (c == '\n') {
          currentLine = "";
        } else if (c != '\r') {
          currentLine += c; // Add characters to the current line
        }
      }
    }

    // Close the connection
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

/*
void loop() {
  WiFiClient client = server.available();  // Check if a client is connected

  if (client) {
    Serial.println("New Client Connected");
    String command = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        command += c;
        if (c == '\n') break;  // End of command
      }
    }

    Serial.println("Command received: " + command);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("Command received: " + command);

    processCommand(command);  // Handle motor commands
    delay(10);
    client.stop();
  }
}

void processCommand(String command) {
  command.trim();

  if (command.indexOf("forward") != -1) {
    moveForward();
  } else if (command.indexOf("backward") != -1) {
    moveBackward();
  } else if (command.indexOf("left") != -1) {
    turnLeft();
  } else if (command.indexOf("right") != -1) {
    turnRight();
  } else if (command.indexOf("stop") != -1) {
    stopMotors();
  }
}


void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);  // something's wronv with the hardware, the wheels are spinning in the opposite direction
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void speedControl(int speed) {
  // Set the motor speed
  analogWrite(ENA, speed);  // Set Motor A speed
  analogWrite(ENB, speed);  // Set Motor B speed
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  // Stop Motor A
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // Run Motor B forward

  // analogWrite(ENA, 0);
  // analogWrite(ENB, 128);  // separate speed control
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // Run Motor A forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  // Stop Motor B

  // analogWrite(ENA, 0);
  // analogWrite(ENB, 128);
}


/*
#include <WiFiNINA.h>

// Please use our own WiFi network information
char ssid[] = "Legilimens"; // Network SSID (name)
char pass[] = "hotspotkey"; // Network Password

int status = WL_IDLE_STATUS; // Initial WiFi status uses the idle state
WiFiServer server(80); // Create a server that listens on port 80

void setup() {
  Serial.begin(9600); // Start serial communication
  // while (!Serial); // Wait for the serial port to connect. Necessary only for boards with a native serial port.

  + pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED to output mode
  + pinMode(2, OUTPUT); // Set the built-in LED to output mode

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Attempt to connect to the WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass); // Connect to a WPA/WPA2 network

    // Wait 10 seconds for the connection to establish
    delay(10000);
  }

  // Once connected, print the IP address
  Serial.print("Successfully connected to ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Start the server
}

// Function to move forward
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to move backward
void moveBackward() {
  digitalWrite(IN1, LOW);    // something's wronv with the hardware, the wheels are spinning in the opposite direction
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Function to stop
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void speedControl(int speed) {
  // Set the motor speed
  analogWrite(ENA, speed); // Set Motor A speed
  analogWrite(ENB, speed); // Set Motor B speed
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  // Stop Motor A
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // Run Motor B forward

  // analogWrite(ENA, 0);    
  // analogWrite(ENB, 128);  // separate speed control
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // Run Motor A forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);  // Stop Motor B

  // analogWrite(ENA, 0);    
  // analogWrite(ENB, 128);  
}

void loop() {
  WiFiClient client = server.available(); // Listen for client connections

  if (client) { // If a client is connected
    Serial.println("New Client."); // Print a message that a new client is connected
    String currentLine = ""; // Used to store characters received from the client
    while (client.connected()) { // While the client stays connected
      if (client.available()) { // If there's data available from the client
        char c = client.read(); // Read the data
        Serial.write(c); // Write the data to the serial monitor
        if (c == '\n') { // If the character is a newline

          // If the current line is empty, i.e., the end of the client's request
          if (currentLine.length() == 0) {
            // Send a standard HTTP response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Control the LED
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
            client.println("<body><h2>Heater Control</h2>");
            client.println("<p><a href=\"/ON\">Turn On</a></p>");
            client.println("<p><a href=\"/OFF\">Turn Off</a></p>");
            client.println("</body></html>");

            // Disconnect
            break;
          } else { // If a new line is received, clear the current line
            currentLine = "";
          }
        } else if (c != '\r') { // If the character is a carriage return, ignore it
          currentLine += c; // Add other characters to the current line
        }

        // Check if the current line contains a command to control the LED
        if (currentLine.endsWith("GET /ON")) {
          digitalWrite(LED_BUILTIN, HIGH); // Turn on the LED
          digitalWrite(2, HIGH); // Turn on the LED
        }
        if (currentLine.endsWith("GET /OFF")) {
          digitalWrite(LED_BUILTIN, LOW); // Turn off the LED
          digitalWrite(2, LOW); // Turn off the LED
        }
      }
    }
    // When the client disconnects
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

/*
void loop() {
  speedControl(128); // 0 - 255

  Serial.println("Moving Forward");
  moveForward();
  delay(2000);

  Serial.println("Stopping");
  stopMotors();
  delay(1000);
  
  Serial.println("Turning Right");
  turnRight();
  delay(2000);

  Serial.println("Stopping");
  stopMotors();
  delay(1000);

  Serial.println("Turning Left");
  turnLeft();
  delay(2000);

  Serial.println("Stopping");
  stopMotors();
  delay(1000);

  Serial.println("Moving Backward");
  moveBackward();
  delay(2000);

  Serial.println("Stopping");
  stopMotors();
  delay(1000);
}
*/
