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

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
Client *client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

WeatherService *m_WeatherService = new WeatherService();

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

void setup() {

    Serial.begin(115200);
    Serial.println("Accessing WIFI network");
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    WiFi.begin("Linh 2.4Ghz", "000000000");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("WIFI Status: ");
        Serial.println(WiFi.status());
    }

    Serial.println("Connected to the WiFi network");

    const char* certificate = \
            "-----BEGIN CERTIFICATE-----\n"
                              "MIIHJTCCBg2gAwIBAgISA/c80WOrBS1B0YKU1WnbOIwuMA0GCSqGSIb3DQEBCwUA\n" \
                              "MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n" \
                              "ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0yMDEwMDUxMzAyNDRaFw0y\n" \
                              "MTAxMDMxMzAyNDRaMB4xHDAaBgNVBAMMEyouc3RhY2tleGNoYW5nZS5jb20wggEi\n" \
                              "MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDgvEf4788HVB81wIAnFbY556Qb\n" \
                              "7BOB5IhjozLwLS9OsOAn2Dmr+P/456nysCXQAFw/Y98R+INfjTScScZa+WfKM9tk\n" \
                              "TSLrrHuPyFQ0IEwpy59+cdnPoJQWrAu6Y0RGRv27yOOVRyeAqge2pArDiYqrc0sE\n" \
                              "HSrBSS1wsq/nnzcaSZroL9uBqGi8hhe5GJUYk2F5EiexsYxv9jx8uLQ7vpBmk3Et\n" \
                              "JbOlP00unQZH5Wd6swTntOhFUHSE2g3Bj3Wi/Mjhq6spTQmvjazN6+ZT6l+UEFSI\n" \
                              "8PdlS9cH99DlPyVxiZfezobk9CGAfkhWhFRoecXKIeMGY49jUmicuZJfa5A7AgMB\n" \
                              "AAGjggQvMIIEKzAOBgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwEG\n" \
                              "CCsGAQUFBwMCMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYEFK+7kfNW1XVWKaiJnPL+\n" \
                              "LA+dQ6qqMB8GA1UdIwQYMBaAFKhKamMEfd265tE5t6ZFZe/zqOyhMG8GCCsGAQUF\n" \
                              "BwEBBGMwYTAuBggrBgEFBQcwAYYiaHR0cDovL29jc3AuaW50LXgzLmxldHNlbmNy\n" \
                              "eXB0Lm9yZzAvBggrBgEFBQcwAoYjaHR0cDovL2NlcnQuaW50LXgzLmxldHNlbmNy\n" \
                              "eXB0Lm9yZy8wggHkBgNVHREEggHbMIIB14IPKi5hc2t1YnVudHUuY29tghIqLmJs\n" \
                              "b2dvdmVyZmxvdy5jb22CEioubWF0aG92ZXJmbG93Lm5ldIIYKi5tZXRhLnN0YWNr\n" \
                              "ZXhjaGFuZ2UuY29tghgqLm1ldGEuc3RhY2tvdmVyZmxvdy5jb22CESouc2VydmVy\n" \
                              "ZmF1bHQuY29tgg0qLnNzdGF0aWMubmV0ghMqLnN0YWNrZXhjaGFuZ2UuY29tghMq\n" \
                              "LnN0YWNrb3ZlcmZsb3cuY29tghUqLnN0YWNrb3ZlcmZsb3cuZW1haWyCDyouc3Vw\n" \
                              "ZXJ1c2VyLmNvbYINYXNrdWJ1bnR1LmNvbYIQYmxvZ292ZXJmbG93LmNvbYIQbWF0\n" \
                              "aG92ZXJmbG93Lm5ldIIUb3BlbmlkLnN0YWNrYXV0aC5jb22CD3NlcnZlcmZhdWx0\n" \
                              "LmNvbYILc3N0YXRpYy5uZXSCDXN0YWNrYXBwcy5jb22CDXN0YWNrYXV0aC5jb22C\n" \
                              "EXN0YWNrZXhjaGFuZ2UuY29tghJzdGFja292ZXJmbG93LmJsb2eCEXN0YWNrb3Zl\n" \
                              "cmZsb3cuY29tghNzdGFja292ZXJmbG93LmVtYWlsghFzdGFja3NuaXBwZXRzLm5l\n" \
                              "dIINc3VwZXJ1c2VyLmNvbTBMBgNVHSAERTBDMAgGBmeBDAECATA3BgsrBgEEAYLf\n" \
                              "EwEBATAoMCYGCCsGAQUFBwIBFhpodHRwOi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCC\n" \
                              "AQMGCisGAQQB1nkCBAIEgfQEgfEA7wB1AJQgvB6O1Y1siHMfgosiLA3R2k1ebE+U\n" \
                              "PWHbTi9YTaLCAAABdPkSXP4AAAQDAEYwRAIgVay70Cu9d46NEOmUt3XUu7bXIqkS\n" \
                              "h+DQXw0Rdy5qIQ0CIH4GmNouXeCovRlx/T4B9Hh//+VvA1tBakgiq+6WEPR8AHYA\n" \
                              "fT7y+I//iFVoJMLAyp5SiXkrxQ54CX8uapdomX4i8NcAAAF0+RJdVgAABAMARzBF\n" \
                              "AiEAs4iZyvg1zC2zaFCs9CNuiGhkuD3cdmcuPCx1qi7rZqcCIAQIaxcyd5wkVWNj\n" \
                              "1CeXrUriThrMyOElkNXaN34j3WqUMA0GCSqGSIb3DQEBCwUAA4IBAQA5BQYZcDBu\n" \
                              "h1NnUYspMTFcuDjYSmZDlD9MBTSaA4alsHN2l+jsz/cLgPNZWdOhn1NPb6OU3x4J\n" \
                              "AOz/4waQvqQ0VYhjBplLMiH3HPXHIiaHJw+p+Hdz0gi3gMcvuoz7ifu+9GemmdGV\n" \
                              "wdpeGuZP4NQXJCnuNhwjrqFQHuoimKvm2M555fJB+ij+p3K2KhbQnq2BKnn2EqIR\n" \
                              "OX9Euhv1TVpUz+rSSJJ89tIUAqzpHSS6CJt3Z3Ljgtyy1u0J1+UNlJ69JNEZIhsG\n" \
                              "fcfc6rV6/wF3uRRBdJck9qyMCejg7NESyxTGnj+QcgbzEpMbGdzZ0PCyvaJWccl7\n" \
                              "qysRzGiJF1WI\n" \
                              "-----END CERTIFICATE-----\n";
    WiFiClientSecure *wiFiClient = new WiFiClientSecure();
    wiFiClient->setCACert(certificate);
    client = wiFiClient;
    m_WeatherService->getWeatherStatus(wiFiClient);

//    if (wiFiClient.connect(WEATHER_END_POINT, 80)) {
//        Serial.println("Connected to Google");
//        Serial.print("Device IP: ");
//        Serial.println(wiFiClient.remoteIP());
//
//        m_WeatherService->getWeatherStatus(&wiFiClient);
//    }
}

void loop() {

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

