/************************************************
 *  Includes
 ***********************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Local includes */
#include "config.h"

/************************************************
 *  Defines / Macros
 ***********************************************/
#define HTTP_RELAY_COMMAND          ("/relay_command")
#define HTTP_GET_RELAY_STATUS       ("/relay_status")

#define RELAY_OPEN_STRING           ("0")
#define RELAY_CLOSE_STRING          ("1")

#define RELAY_PIN                   (0U)
#define DEFAULT_WAIT_TIME           (1000)

#define RELAY_OPEN                  (false)
#define RELAY_CLOSE                 (true)

/************************************************
 *  Typedef definition
 ***********************************************/

/************************************************
 *  Global variables
 ***********************************************/
static ESP8266WebServer server(HTTP_SERVER_PORT);
static unsigned long previousWifiCheckMillis = 0U;
static bool relayStatus = RELAY_OPEN;

/************************************************
 *  Public function definition
 ***********************************************/
bool connectToWifi(void);
void checkWifiStatus(void);
bool checkWifiConnection(void);
void handleRelayStatus();
void handleRelayCommand(void);
void sendRelayCommand(const bool command);

void setup() {
    bool networkStatus;

    /* Small delay before the initialization */
    delay(DEFAULT_WAIT_TIME);

    /* Enable debug options */
    Serial.begin(115200);

    /* Ensure that the relay is always open at startup */
    pinMode(RELAY_PIN, OUTPUT);
    sendRelayCommand(RELAY_OPEN);

    /* Try to connect to the WiFi*/
    networkStatus = connectToWifi();
    if (networkStatus == false) {
        /* Could not connect to the local network, reset the board */
        ESP.reset();
    } else {
        /* Define the HTTP GET for controlling the relay */
        server.on(HTTP_RELAY_COMMAND, HTTP_GET, handleRelayCommand);
        server.on(HTTP_GET_RELAY_STATUS, HTTP_GET, handleRelayStatus);

        server.begin();
    }
}

void loop() {
    /* Listen for commands from the clients */
    server.handleClient();

    /* Periodically check for Wifi Status */
    checkWifiStatus();
}

bool connectToWifi(void) {
    bool status = false;
    uint8_t retry = 0U;

    Serial.println("Connecting to WiFi...");
    /* Configure the device */
    //WiFi.config(ESP01S_STATIC_IP, ESP01S_GATEWAY_IP, ESP01S_SUBNET_MASK, ESP01S_DNS_IP);
    WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);

    /* Try to connect to the local network */
    while ((WiFi.status() != WL_CONNECTED) && (retry < MAX_WIFI_CONNECTION_ATTEMPTS)) {
        Serial.println("Connecting...");
        delay(DEFAULT_WAIT_TIME);
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        /* Debug Network */
        Serial.println("Connected to WiFi");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC Address: ");
        Serial.println(WiFi.macAddress());

        /* Wifi connected to the local network */
        status = true;
    } else {
        Serial.println("Failed to connect to WiFi");
    }

    return (status);
}

void checkWifiStatus(void) {
    bool status = true;
    unsigned long currentMillis = millis();

    /* Check the status of the connection every CHECK_NETWORK_STATUS_TIME_IN_MS */
    if (currentMillis - previousWifiCheckMillis >= CHECK_NETWORK_STATUS_TIME_IN_MS) {
        previousWifiCheckMillis = currentMillis;
        status = checkWifiConnection();
    }

    /* No Network Connection, do a hard reset */
    if (status == false) {
        ESP.restart();
    }
}

bool checkWifiConnection(void) {
    bool status = false;

    /* Check if the Wifi is still connected to the Internet */
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi is not connected. Attempting to reconnect...");
        /* Attempt to reconnect */
        status = connectToWifi();
    } else {
        /* Wifi is still up */
        status = true;
    }

    return (status);
}

void handleRelayStatus() {
    String statusMessage = relayStatus ? RELAY_CLOSE_STRING : RELAY_OPEN_STRING;
    server.send(200, "text/plain", statusMessage);
}

void handleRelayCommand(void) {
    String value = server.arg("val");

    /* Check the received command */
    if (value == RELAY_CLOSE_STRING) {
        /* Close the relay */
        server.send(200, "text/plain", "Relay turned ON");
        relayStatus = RELAY_CLOSE;
        sendRelayCommand(RELAY_CLOSE);
    } else if (value == RELAY_OPEN_STRING) {
        /* Open the relay */
        server.send(200, "text/plain", "Relay turned OFF");
        relayStatus = RELAY_OPEN;
        sendRelayCommand(RELAY_OPEN);
    } else {
        server.send(400, "text/plain", "Invalid command");
    }
}

void sendRelayCommand(const bool command) {
    digitalWrite(RELAY_PIN, (uint8_t)command);
    delay(DEFAULT_WAIT_TIME);
}