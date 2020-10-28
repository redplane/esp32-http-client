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

//    const char* certificate = "-----BEGIN CERTIFICATE-----\n"
//                              "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
//                              "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
//                              "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
//                              "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
//                              "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
//                              "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
//                              "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
//                              "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
//                              "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
//                              "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
//                              "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
//                              "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
//                              "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
//                              "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
//                              "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
//                              "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
//                              "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
//                              "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
//                              "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
//                              "-----END CERTIFICATE-----";
//
    const char* certificate = "-----BEGIN CERTIFICATE-----\n"
                              "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
                              "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
                              "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
                              "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
                              "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
                              "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
                              "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
                              "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
                              "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
                              "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
                              "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
                              "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
                              "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
                              "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
                              "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
                              "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
                              "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
                              "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
                              "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
                              "-----END CERTIFICATE-----";

//    WiFiClientSecure *wiFiClient = new WiFiClientSecure();
//    wiFiClient->setCACert(certificate);
//    client = wiFiClient;

    client = new WiFiClient();

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

