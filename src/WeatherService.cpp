//
// Created by Redplane on 2020-10-23.
//

#include <EthernetClient.h>
#include "WeatherService.h"
#include "HttpClient.h"
#include "ArduinoJson.h"

// Get weather status.
int WeatherService::getWeatherStatus(EthernetClient *client) {

    const int kNetworkDelay = 1000;

    // Number of milliseconds to wait without receiving any data before we give up
    const int kNetworkTimeout = 30 * 1000;

    if (client == nullptr) {
        return 0;
    }

    // Block until we are able to connect to the WiFi access point
    if (client->connected())
        return WEATHER_STATUS_UNKNOWN;

    HttpClient httpClient(*client);
    httpClient.beginRequest();

    // Add custom header.
    httpClient.sendHeader("Content-Type", "application/json");

    int exception = httpClient.post(WEATHER_END_POINT, "/api/weather/status/search");
    if (exception != 0) {
        Serial.println("Exception occurred");
        return WEATHER_STATUS_UNKNOWN;
    }

    int httpResponseStatusCode = httpClient.responseStatusCode();
    Serial.print("Response status code: ");
    Serial.println(httpResponseStatusCode);

    exception = httpClient.skipResponseHeaders();
    if (exception < 1) {
        Serial.print("Failed to skip response headers: ");
        Serial.println(exception);
        return WEATHER_STATUS_UNKNOWN;
    }

    int bodyLen = httpClient.contentLength();
    Serial.print("Content length is: ");
    Serial.println(bodyLen);
    Serial.println();
    Serial.println("Body returned follows:");

    // Now we've got to the body, so we can print it out
    unsigned long timeoutStart = millis();
    char c;
    // Whilst we haven't timed out & haven't reached the end of the body
    while ((httpClient.connected() || httpClient.available()) &&
           ((millis() - timeoutStart) < kNetworkTimeout)) {
        if (httpClient.available()) {
            c = httpClient.read();
            // Print out this character
            Serial.print(c);

            bodyLen--;
            // We read something, reset the timeout counter
            timeoutStart = millis();
        } else {
            // We haven't got any data, so let's pause to allow some to
            // arrive
            delay(kNetworkDelay);
        }
    }

//    StaticJsonDocument<200> keyValuePairs;
//    keyValuePairs["cityCode"] = "HANOI";
//
//    String requestBody;
//    serializeJson(keyValuePairs, requestBody);
//
//    int httpResponseCode = http.POST(requestBody);
//
//    if (httpResponseCode > 0) {
//
//        String httpResponseBody = http.getString();
//        DynamicJsonDocument doc(1024);
//        deserializeJson(doc, httpResponseBody);
//
//        int weatherStatus = doc["status"];
//
//        return weatherStatus;
//    }

    return WEATHER_STATUS_UNKNOWN;

}
