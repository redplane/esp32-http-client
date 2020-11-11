//
// Created by Redplane on 2020-10-23.
//

#include <EthernetClient.h>
#include "WeatherService.h"
#include "HttpClient.h"
#include "ArduinoJson.h"

//// Get weather status.
//int WeatherService::getWeatherStatus(Client *client) {
//
//    int err = 0;
//    // Name of the server we want to connect to
//    const char kHostname[] = "nm-solution-ltd-staging.azurewebsites.net";
//
//    // Path to download (this is the bit after the hostname in the URL
//    // that you want to download
//    const char kPath[] = "/swagger/index.html";
//
//    // Number of milliseconds to wait without receiving any data before we give up
//    const int kNetworkTimeout = 30 * 1000;
//
//    // Number of milliseconds to wait if no data is available before trying again
//    const int kNetworkDelay = 1000;
//
//    HttpClient httpClient(*client);
//
//    int statusCode = httpClient.get(kHostname, 443, kPath);
//    if (statusCode != 0) {
//        Serial.print("Sending request to host failed.");
//        return WEATHER_STATUS_UNKNOWN;
//    }
//
//
//    Serial.println("startedRequest ok");
//
//    int httpStatusCode = httpClient.responseStatusCode();
//
//    Serial.print("Got status code: ");
//    Serial.println(httpStatusCode);
//
//    if (httpStatusCode != 200) {
//        httpClient.stop();
//        return WEATHER_STATUS_UNKNOWN;
//    }
//
//    // Usually you'd check that the response code is 200 or a
//    // similar "success" code (200-299) before carrying on,
//    // but we'll print out whatever response we get
//    int skipResponseHeaderStatus = httpClient.skipResponseHeaders();
//    if (skipResponseHeaderStatus != HTTP_SUCCESS) {
//        Serial.print("Failed to skip response headers: ");
//        Serial.println(skipResponseHeaderStatus);
//        return WEATHER_STATUS_UNKNOWN;
//    }
//
//    int bodyLen = httpClient.contentLength();
//    Serial.print("Content length is: ");
//    Serial.println(bodyLen);
//    Serial.println();
//    Serial.println("Body returned follows:");
//
//    // Now we've got to the body, so we can print it out
//    unsigned long timeoutStart = millis();
//    char c;
//
//    // Whilst we haven't timed out & haven't reached the end of the body
//    while ((httpClient.connected() || httpClient.available()) &&
//           ((millis() - timeoutStart) < kNetworkTimeout)) {
//
//        if (!httpClient.available()) {
//            continue;
//        }
//
//        c = (byte) httpClient.read();
//        // Print out this character
//        Serial.print(c);
//
//        bodyLen--;
//        // We read something, reset the timeout counter
//        timeoutStart = millis();
//    }
//
//
//    httpClient.stop();
//    return WEATHER_STATUS_UNKNOWN;
//
//}

// Get weather status.
int WeatherService::getWeatherStatus(Client *client) {

    // Name of the server we want to connect to
    const char kHostname[] = "nm-solution-ltd-staging.azurewebsites.net";

    // Path to download (this is the bit after the hostname in the URL
    // that you want to download
    const char kPath[] = "/swagger/index.html";

    // Number of milliseconds to wait without receiving any data before we give up
    const int kNetworkTimeout = 30 * 1000;

    // Number of milliseconds to wait if no data is available before trying again
    const int kNetworkDelay = 1000;

    HttpClient httpClient(*client);

    HttpResponse httpResponse = this->sendRequest(client, kHostname, 443, HTTP_METHOD_GET, kPath);
    Serial.print("Http status code: ");
    Serial.println(httpResponse.getStatusCode());

    Serial.print("Http content: ");
    Serial.println(httpResponse.getContent());

    httpClient.stop();
    return WEATHER_STATUS_UNKNOWN;

}
