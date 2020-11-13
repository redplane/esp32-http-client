/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
#include <Ethernet.h>
#include <WiFi.h>
#include <WeatherService.h>
#include <WiFiClientSecure.h>
#include <StationService.h>
#include <SPIFFS.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "sensorapi.conveyor.cloud";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
Client *client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

WeatherService *m_WeatherService = new WeatherService();
StationService *stationService = new StationService();

//void setup1() {
//    // You can use Ethernet.init(pin) to configure the CS pin
//    Ethernet.init(2);  // Most Arduino shields
//    //Ethernet.init(10);  // Most Arduino shields
//    //Ethernet.init(5);   // MKR ETH shield
//    //Ethernet.init(0);   // Teensy 2.0
//    //Ethernet.init(20);  // Teensy++ 2.0
//    //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
//    //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
//
//    // Open serial communications and wait for port to open:
//    Serial.begin(115200);
//    while (!Serial) {
//        ; // wait for serial port to connect. Needed for native USB port only
//    }
//
//    // start the Ethernet connection:
//    Serial.println("Initialize Ethernet with DHCP:");
//    if (Ethernet.begin(mac) == 0) {
//        Serial.println("Failed to configure Ethernet using DHCP");
//        // Check for Ethernet hardware present
////        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
////            Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
////            while (true) {
////                delay(1); // do nothing, no point running without Ethernet hardware
////            }
////        }
//        if (Ethernet.linkStatus() == LinkOFF) {
//            Serial.println("Ethernet cable is not connected.");
//        }
//        // try to congifure using IP address instead of DHCP:
//        Ethernet.begin(mac, Ethernet.localIP());
//    } else {
//        Serial.print("  DHCP assigned IP ");
//        Serial.println(Ethernet.localIP());
//    }
//    // give the Ethernet shield a second to initialize:
//    delay(1000);
//    Serial.print("connecting to ");
//    Serial.print(server);
//    Serial.println("...");
//
//    // if you get a connection, report back via serial:
//    if (client.connect(server, 80)) {
//        Serial.print("connected to ");
//        Serial.println(client.remoteIP());
//        // Make a HTTP request:
//        client.println("GET /search?q=arduino HTTP/1.1");
//        client.println("Host: www.google.com");
//        client.println("Connection: close");
//        client.println();
//    } else {
//        // if you didn't get a connection to the server:
//        Serial.println("connection failed");
//    }
//    beginMicros = micros();
//}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event) {
        case SYSTEM_EVENT_WIFI_READY:
            Serial.println("WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case SYSTEM_EVENT_AP_START:
            Serial.println("WiFi access point started");
            break;
        case SYSTEM_EVENT_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            Serial.println("IPv6 is preferred");
            break;
        case SYSTEM_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default:
            break;
    }
}

void setup() {

    Serial.begin(115200);
    Serial.println("Accessing WIFI network");

    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    WiFi.begin("Linh 2.4Ghz", "000000000");
    WiFi.onEvent(WiFiEvent);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("WIFI Status: ");
        Serial.println(WiFi.status());
    }

//    if (!SPIFFS.begin(true)) {
//        Serial.println("An Error has occurred while mounting SPIFFS");
//        return;
//    }
//
//    auto file = SPIFFS.open("/cacert.pem");
//    if (!file) {
//        Serial.println("Failed to open file for reading");
//        return;
//    }
//
//    Serial.println("File Content:");
//    auto content = file.readString();
//    Serial.println(content);
//    file.close();

    Serial.println("Connected to the WiFi network");

    const char* test_root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n" \
"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n" \
"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n" \
"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n" \
"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n" \
"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n" \
"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n" \
"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n" \
"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n" \
"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n" \
"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n" \
"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n" \
"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n" \
"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n" \
"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n" \
"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n" \
"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n" \
"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n" \
"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n" \
"-----END CERTIFICATE-----\n" ;

    auto wiFiClient = new WiFiClientSecure();
    wiFiClient->setCACert(test_root_ca);
    client = wiFiClient;

    if (!wiFiClient->connect(server, 443)) {
        Serial.println("Connection failed!");
    } else {
        Serial.println("Connected to server!");
    }

//    client = new WiFiClient();
//
    m_WeatherService->getWeatherStatus(client);

//    if (wiFiClient.connect(WEATHER_END_POINT, 80)) {
//        Serial.println("Connected to Google");
//        Serial.print("Device IP: ");
//        Serial.println(wiFiClient.remoteIP());
//
//        m_WeatherService->getWeatherStatus(&wiFiClient);
//    }
}

void loop() {

    std::vector<Station> stations;

    Serial.println("Getting stations");
    stationService->getStations(client, stations);
    Serial.print("Got ");
    Serial.print(stations.size());
    Serial.println(" station(s)");
    delay(5000);

//    m_WeatherService->getWeatherStatus(client);

//    while (client->available()) {
//        char c = client->read();
//        Serial.print(c);
//    }
//
//    if (!client->connected()) {
//        Serial.println();
//        Serial.println("Disconnect");
//        client->stop();
//    }
//
//    Serial.println("Done");

//    // if there are incoming bytes available
//    // from the server, read them and print them:
//    int len = client.available();
//    if (len > 0) {
//        byte buffer[80];
//        if (len > 80) len = 80;
//        client.read(buffer, len);
//        if (printWebData) {
//            Serial.write(buffer, len); // show in the serial monitor (slows some boards)
//        }
//        byteCount = byteCount + len;
//    }
//
//    // if the server's disconnected, stop the client:
//    if (!client.connected()) {
//        endMicros = micros();
//        Serial.println();
//        Serial.println("disconnecting.");
//        client.stop();
//        Serial.print("Received ");
//        Serial.print(byteCount);
//        Serial.print(" bytes in ");
//        float seconds = (float)(endMicros - beginMicros) / 1000000.0;
//        Serial.print(seconds, 4);
//        float rate = (float)byteCount / seconds / 1000.0;
//        Serial.print(", rate = ");
//        Serial.print(rate);
//        Serial.print(" kbytes/second");
//        Serial.println();
//
//        // do nothing forevermore:
//        while (true) {
//            delay(1);
//        }
//    }
}

