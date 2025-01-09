#include <WiFi.h>
#include <WebServer.h>

const int moisturePower = 7;
const int moisturePin = 17;
int val = 0 ;


// Declare the server object globally
WebServer server(80);

// Handle base route
void handleRoot() {
  Serial.println("Handling root request!");
  server.send(200, "text/html", "<h1 style='font-family:sans-serif; text-align:center; margin-top:200px;'>It's a beautiful day to build!</h1>");
  digitalWrite(LED_GREEN, LOW);
  delay(2000);
  digitalWrite(LED_GREEN, HIGH);
}

// Handle 404
void handleNotFound() {
  server.send(404, "text/plain", "404: Resource not found");
  digitalWrite(LED_RED, LOW);
  delay(1000);
  digitalWrite(LED_RED, HIGH);
}


void getMoisture() {

  String content;

  // Power On
  digitalWrite(moisturePower, HIGH);
  delay(10);

  // Take Reading
  val = analogRead(moisturePin);     //Read the SIG value form sensor
    
  Serial.println(val);

  // Power off
  digitalWrite(moisturePower, LOW);  //send current moisture value

  delay(1000);  // be polite


  content = "<p>Moisture:</p><p>" +  String(val) + "</p>";

  server.send(200, "text/html", content);
}



void setup() {

  // Wifi Creds
  const char* ssid = "D4BY8";
  const char* password = "206FCBAC7B";

  // Initialize onboard RGB LED pins as outputs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Turn off all LEDs initially
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  // Start the serial monitor
  Serial.begin(9600);


  // Init GPIO Pin
  pinMode(moisturePower, OUTPUT);    //Set D7 as an OUTPUT
  digitalWrite(moisturePower, LOW);  //Set to LOW so no power is flowing through the sensor

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");

  // While connecting
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BLUE, LOW);
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BLUE, HIGH);
  }

  // On connect
  digitalWrite(LED_GREEN, LOW);
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP address: ");
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
