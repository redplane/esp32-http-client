#ifndef ESP32_HTTP_CLIENT_STATIONSERVICE_H
#define ESP32_HTTP_CLIENT_STATIONSERVICE_H

#include "HttpService.h"
#include "Station.h"

class StationService : HttpService {

public:

    // Get stations list.
    void getStations(Client* client, std::vector<Station> &stations) {
        // Name of the server we want to connect to
        const char kHostname[] = "nm-solution-ltd-staging.azurewebsites.net";

        // Path to download (this is the bit after the hostname in the URL
        // that you want to download
        const char kPath[] = "/api/station/search";

        // Number of milliseconds to wait without receiving any data before we give up
        const int kNetworkTimeout = 30 * 1000;

        // Number of milliseconds to wait if no data is available before trying again
        const int kNetworkDelay = 1000;

        HttpClient httpClient(*client);

        HttpResponse httpResponse = this->sendRequest(client, kHostname, 443, HTTP_METHOD_GET, kPath);
        Serial.print("Http status code: ");
        Serial.println(httpResponse.getStatusCode());

        auto httpContent = httpResponse.getContent();

        Serial.print("Http content: ");
        Serial.println(httpContent);

        httpClient.stop();
        StaticJsonDocument<128> document;
        auto iHasError = deserializeJson(document, httpContent);
        if (iHasError) {
            Serial.println("Has error while deserializing json");
        }

        auto items = document["item"].as<JsonArray>();
        for (JsonArray::iterator it = items.begin(); it != items.end(); ++it) {
            auto station = it->as<Station>();
            stations.push_back(station);
        }
    }

};

#endif
