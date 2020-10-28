#include "HttpService.h"
#include <LinkedList.h>
#include <ArduinoJson.h>
#include <HttpResponse.h>
#include <ArduinoHttpClient.h>

#define HTTP_LOG   1

//#region Internal methods
//
//void HttpService::appendHeaders(HttpClient *httpClient, LinkedList<HttpHeader> *httpHeaders) {
//
//    if (httpClient == nullptr) {
//        return;
//    }
//
//    if (httpHeaders == nullptr) {
//        return;
//    }
//
//    for (int index = 0; index < httpHeaders->size(); index++) {
//        HttpHeader httpHeader = httpHeaders->get(index);
//
//        char key[64];
//        int keyLength;
//
//        char value[64];
//        int valueLength;
//
//        httpHeader.getKey(key, keyLength);
//        httpHeader.getValue(value, valueLength);
//
//        httpClient->sendHeader(key, value);
//    }
//}

//#endregion

HttpResponse HttpService::sendRequest(Client *network, const char *host, uint16_t port,
                                      const char *httpMethod, const char *path,
                                      JsonDocument *contentDocument) {

    HttpResponse defaultHttpResponse;

    // Number of milliseconds to wait without receiving any data before we give up
    const int kNetworkTimeout = 30 * 1000;

    // Initialize http client for sending requests.
    HttpClient httpClient(*network, host, port);

    // Http headers are defined.
    // this->appendHeaders(&httpClient, httpHeaders);
//

    String body;

    if (contentDocument != nullptr) {
        // Append request body

        serializeJson(*contentDocument, body);

        Serial.println("Serialized content: ");
        Serial.println(body);
    }

    // Send request to server.
    char contentType[] = "application/json";
    int statusCode = httpClient.startRequest(path, httpMethod,
                                             contentType, body.length(), (const byte *) body.c_str());

    if (statusCode != 0) {
        httpClient.stop();
        return defaultHttpResponse;
    }

#ifdef HTTP_LOG
    Serial.println("startedRequest ok");
#endif

    // Get http status code.
    int httpStatusCode = httpClient.responseStatusCode();

#ifdef HTTP_LOG
    Serial.print("Got status code: ");
    Serial.println(httpStatusCode);
#endif

    if (httpStatusCode != 200) {
        httpClient.stop();
        return defaultHttpResponse;
    }

    // Skip the response header.
    int skipResponseHeaderStatus = httpClient.skipResponseHeaders();
    if (skipResponseHeaderStatus != HTTP_SUCCESS) {
#ifdef HTTP_LOG
        Serial.print("Failed to skip response headers: ");
        Serial.println(skipResponseHeaderStatus);
#endif
        httpClient.stop();
        return defaultHttpResponse;
    }

    // Get the content length
    int contentLength = httpClient.contentLength();

#ifdef HTTP_LOG
    Serial.print("Content length is: ");
    Serial.println(contentLength);
    Serial.println();
    Serial.println("Body returned follows:");
#endif


    String httpContent = httpClient.readString();
    HttpResponse httpResponse(httpStatusCode, httpContent);

    httpClient.stop();
    return httpResponse;
}

void HttpService::setUserAgent(const char *userAgent) {

    // User agent is not valid.
    if (userAgent == nullptr) {
        return;
    }

    this->_userAgent = new char[strlen(userAgent)];
    strcpy(this->_userAgent, userAgent);
}

void HttpService::getUserAgent(char *agent, int &length) {

    if (this->_userAgent == nullptr) {
        agent = new char[1]{'\0'};
        length = 1;

        return;
    }

    agent = new char[strlen(this->_userAgent)];
    strcpy(agent, this->_userAgent);
}

//#region Constructor

HttpService::HttpService() {

    char userAgent[] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36";

    this->_userAgent = new char[strlen(userAgent)];
    strcpy(this->_userAgent, userAgent);
}

//#endregion