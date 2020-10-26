#ifndef ESP32_HTTP_CLIENT_HTTP_SERVICE_H
#define ESP32_HTTP_CLIENT_HTTP_SERVICE_H

#include <Client.h>
#include <LinkedList.h>
#include <HttpClient.h>
#include "HttpHeader.h"
#include <ArduinoJson.h>

class HttpService {

private:

    // User agent that is used for sending request to host.
    char *_userAgent;

protected:

    // Append headers into http client.
    void appendHeaders(HttpClient *httpClient, LinkedList<HttpHeader> *httpHeaders);

public:

    // Setup user agent which is for sending request to host.
    void setUserAgent(const char *userAgent);

    // Get user agent which is set to the service
    void getUserAgent(char *agent, int &length);

    // Send request to server.
    int sendRequest(Client network, const char *host, uint16_t port, const char *httpMethod, const char *path,
                    char *content, int *contentLength,
                    JsonDocument *document = nullptr,
                    const char *agent = nullptr, LinkedList<HttpHeader> *httpHeaders = nullptr);

    //#region Constructor

    HttpService();

    //#endregion
};

#endif //ESP32_HTTP_CLIENT_HTTP_SERVICE_H
