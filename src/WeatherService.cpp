//
// Created by Redplane on 2020-10-23.
//

#include <EthernetClient.h>
#include "WeatherService.h"
#include "HttpClient.h"
#include "ArduinoJson.h"

// Get weather status.
int WeatherService::getWeatherStatus(Client *client) {

    int err =0;
    // Name of the server we want to connect to
    const char kHostname[] = "nm-solution-ltd-staging.azurewebsites.net";

    // Path to download (this is the bit after the hostname in the URL
    // that you want to download
    const char kPath[] = "/swagger/index.html";

    // Number of milliseconds to wait without receiving any data before we give up
    const int kNetworkTimeout = 30*1000;

    // Number of milliseconds to wait if no data is available before trying again
    const int kNetworkDelay = 1000;

    HttpClient httpClient(*client);
    err = httpClient.get(kHostname, 443, kPath);
    if (err == 0)
    {
        Serial.println("startedRequest ok");

        err = httpClient.responseStatusCode();
        if (err >= 0)
        {
            Serial.print("Got status code: ");
            Serial.println(err);

            // Usually you'd check that the response code is 200 or a
            // similar "success" code (200-299) before carrying on,
            // but we'll print out whatever response we get

            err = httpClient.skipResponseHeaders();
            if (err >= 0)
            {
                int bodyLen = httpClient.contentLength();
                Serial.print("Content length is: ");
                Serial.println(bodyLen);
                Serial.println();
                Serial.println("Body returned follows:");

                // Now we've got to the body, so we can print it out
                unsigned long timeoutStart = millis();
                char c;
                // Whilst we haven't timed out & haven't reached the end of the body
                while ( (httpClient.connected() || httpClient.available()) &&
                        ((millis() - timeoutStart) < kNetworkTimeout) )
                {
                    if (httpClient.available())
                    {
                        c = httpClient.read();
                        // Print out this character
                        Serial.print(c);

                        bodyLen--;
                        // We read something, reset the timeout counter
                        timeoutStart = millis();
                    }
                    else
                    {
                        // We haven't got any data, so let's pause to allow some to
                        // arrive
                        delay(kNetworkDelay);
                    }
                }
            }
            else
            {
                Serial.print("Failed to skip response headers: ");
                Serial.println(err);
            }
        }
        else
        {
            Serial.print("Getting response failed: ");
            Serial.println(err);
        }
    }
    else
    {
        Serial.print("Connect failed: ");
        Serial.println(err);
    }

    httpClient.stop();

    return WEATHER_STATUS_UNKNOWN;

}
