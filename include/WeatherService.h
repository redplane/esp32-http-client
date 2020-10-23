//
// Created by Redplane on 2020-10-22.
//

#ifndef ESP32_HTTP_CLIENT_WEATHERSERVICE_H
#define ESP32_HTTP_CLIENT_WEATHERSERVICE_H

#include <Ethernet.h>
#include <Client.h>

// Weather status
#define WEATHER_STATUS_UNKNOWN  -1
#define WEATHER_STATUS_SUNNY    0
#define WEATHER_STATUS_CLOUDY   1
#define WEATHER_STATUS_RAINY    2
#define WEATHER_STATUS_STORMY   3
#define WEATHER_STATUS_WINDY    4

#define WEATHER_ICON_UNKNOWN    -1
#define WEATHER_ICON_SUNNY      14
#define WEATHER_ICON_CLOUDY     11
#define WEATHER_ICON_RAINY      12
#define WEATHER_ICON_STORMY     13
#define WEATHER_ICON_WINDY      15

#define WEATHER_END_POINT       "http://nm-solution-ltd-staging.azurewebsites.net"

class WeatherService {

public:

    // Get weather status
    int getWeatherStatus(Client *client);
};

#endif //ESP32_HTTP_CLIENT_WEATHERSERVICE_H
