//
// Created by Redplane on 2020-10-26.
//

#ifndef ESP32_HTTP_CLIENT_HTTP_RESPONSE_H
#define ESP32_HTTP_CLIENT_HTTP_RESPONSE_H

#include <Arduino.h>
#include <ArduinoJson.h>

class HttpResponse {

private:

    // Response status code.
    int _statusCode;

    // Content
    String _content;

public:

    HttpResponse();

    HttpResponse(int statusCode, String content);

    // Get status code.
    int getStatusCode();

    // Get content
    String getContent();

};

#endif //ESP32_HTTP_CLIENT_HTTP_RESPONSE_H
