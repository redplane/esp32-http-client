#ifndef ESP32_HTTP_CLIENT_STATIONSERVICE_H
#define ESP32_HTTP_CLIENT_STATIONSERVICE_H

#include "HttpService.h"
#include "Station.h"
#include <ArduinoHttpClient.h>

class StationService : HttpService {

public:

    // Get stations list.
    void getStations(Client* client, std::vector<Station> &stations) {
        // Name of the server we want to connect to
        const char kHostname[] = "sensorapi.conveyor.cloud";

        // Path to download (this is the bit after the hostname in the URL
        // that you want to download
        const char kPath[] = "/api/station/search";

        // Number of milliseconds to wait without receiving any data before we give up
        const int kNetworkTimeout = 30 * 1000;

        // Number of milliseconds to wait if no data is available before trying again
        const int kNetworkDelay = 1000;

        StaticJsonDocument<64> requestDoc;
        requestDoc["pager"]["kind"] = "default";
        requestDoc["pager"]["page"] = 1;
        requestDoc["pager"]["totalRecords"] = 30;
        HttpResponse httpResponse = this->sendRequest(client, kHostname, 443, HTTP_METHOD_POST, kPath, &requestDoc);
        Serial.print("Http status code: ");
        Serial.println(httpResponse.getStatusCode());

        auto httpContent = httpResponse.getContent();

        Serial.print("Http content: ");
        Serial.println(httpContent);

        StaticJsonDocument<128> document;
        auto iHasError = deserializeJson(document, httpContent);
        if (iHasError) {
            Serial.println("Has error while deserializing json");
        }

        auto items = document["item"].as<JsonArray>();
//        for (JsonArray::iterator it = items.begin(); it != items.end(); ++it) {
//            auto station = it->as<Station>();
//            stations.push_back(station);
//        }
    }

};

#endif
