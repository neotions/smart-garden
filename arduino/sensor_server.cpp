#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


/* Const Init */
const int moisturePower = 7;
const int moisturePin = 17;


/* Init */
int moistureReading = 0 ;

// Declare the server object globally
WebServer server(80);


/* Routes */

// Handle base route
void handleRoot() {
    Serial.println("Handling root request!");
    server.send(200, "text/html", "<h1 style='font-family:sans-serif; text-align:center; margin-top:200px;'>It's a beautiful day to build!</h1>");
    toggleLED("green", 2000);
}

// Handle 404
void handleNotFound() {
    server.send(404, "text/plain", "404: Resource not found");
    toggleLED("red", 1000);
}

void getMoisture() {

    String content;

    // Power On
    digitalWrite(moisturePower, HIGH);
    delay(10);

    // Take Reading
    moistureReading = analogRead(moisturePin); //Read the SIG value form sensor
        
    Serial.println(moistureReading);

    // Power off
    digitalWrite(moisturePower, LOW); //send current moisture value
    delay(1000);  // be polite

    // Construct HTML string and serve
    content = "<p>Moisture:</p><p>" +  String(moistureReading) + "</p>";
    server.send(200, "text/html", content);
}


/* Helpers*/

// Toggles the onboard RGB LED - use lower
bool boardLED(const char* color, int delayMS) {
    if (color == "red") {
        digitalWrite(LED_RED, LOW);
        delay(delayMS);
        digitalWrite(LED_RED, HIGH);
        return;
    } else if (color == "green") {
        digitalWrite(LED_GREEN, LOW);
        delay(delayMS);
        digitalWrite(LED_GREEN, HIGH);
        return;
    } else if (color == "blue") {
        digitalWrite(LED_BLUE, LOW);
        delay(delayMS);
        digitalWrite(LED_BLUE, HIGH);
        return;
    }
    else {
        return false;
    }
}


// To Do: Make a function that uses a pointer to keep an LED on while function runs


/* Arduino Begin */

void setup() {

    // Start the serial monitor
    Serial.begin(9600);

    // Wifi Creds - don't push these!
    const char* ssid = ""; 
    const char* password = "";  

    // Initialize onboard RGB LED pins as outputs
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    // Turn off all LEDs initially
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);

    // Init GPIO Pins
    pinMode(moisturePower, OUTPUT);    //Set D7 as an OUTPUT
    
    //Set to LOW so no power is flowing through the sensors
    digitalWrite(moisturePower, LOW);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");

    // While connecting
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BLUE, LOW);
        delay(500);
        Serial.println(".");
        digitalWrite(LED_BLUE, HIGH);
    }

    // On connect
    digitalWrite(LED_GREEN, LOW);
    Serial.println("\nConnected to WiFi!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(5000);
    digitalWrite(LED_GREEN, HIGH);

    /* Configure routes */
    server.on("/", handleRoot); // root
    server.on("/moisture", getMoisture );
    
    server.onNotFound(handleNotFound);

    // Start server
    server.begin();
    Serial.println("Server started!");
}

void loop() {
    // Handle incoming HTTP requests
    server.handleClient();
}
