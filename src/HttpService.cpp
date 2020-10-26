#include "HttpService.h"
#include <HttpClient.h>
#include <LinkedList.h>
#include <ArduinoJson.h>

#define HTTP_LOG   1

//#region Internal methods

void HttpService::appendHeaders(HttpClient *httpClient, LinkedList<HttpHeader> *httpHeaders) {

    if (httpClient == nullptr) {
        return;
    }

    if (httpHeaders == nullptr) {
        return;
    }

    for (int index = 0; index < httpHeaders->size(); index++) {
        HttpHeader httpHeader = httpHeaders->get(index);

        char key[64];
        int keyLength;

        char value[64];
        int valueLength;

        httpHeader.getKey(key, keyLength);
        httpHeader.getValue(value, valueLength);

        httpClient->sendHeader(key, value);
    }
}

//#endregion

int HttpService::sendRequest(Client network, const char *host, uint16_t port, const char *httpMethod, const char *path,
                             char *content, int *contentLength,
                             JsonDocument *contentDocument,
                             const char *agent, LinkedList<HttpHeader> *httpHeaders) {

    // Number of milliseconds to wait without receiving any data before we give up
    const int kNetworkTimeout = 30 * 1000;

    // Initialize http client for sending requests.
    HttpClient httpClient(network);

    // Http headers are defined.
    this->appendHeaders(&httpClient, httpHeaders);

    // Append request body
    String body;
    serializeJson(*contentDocument, body);
    httpClient.println(body);

    // Send request to server.
    int statusCode = httpClient.startRequest(host, port, path, httpMethod, agent == nullptr ? this->_userAgent : agent);

    if (statusCode != 0) {
        httpClient.stop();
        return -1;
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
        return -1;
    }

    // Skip the response header.
    int skipResponseHeaderStatus = httpClient.skipResponseHeaders();
    if (skipResponseHeaderStatus != HTTP_SUCCESS) {
#ifdef HTTP_LOG
        Serial.print("Failed to skip response headers: ");
        Serial.println(skipResponseHeaderStatus);
#endif
        httpClient.stop();
        return -1;
    }

    // Get the content length
    *contentLength = httpClient.contentLength();
    int loadedContentLength = 0;

#ifdef HTTP_LOG
    Serial.print("Content length is: ");
    Serial.println(*contentLength);
    Serial.println();
    Serial.println("Body returned follows:");
#endif

    // Now we've got to the body, so we can print it out
    unsigned long timeoutStart = millis();
    char httpContent[*contentLength];

    // Whilst we haven't timed out & haven't reached the end of the body
    while ((httpClient.connected() || httpClient.available()) &&
           ((millis() - timeoutStart) < kNetworkTimeout) && (loadedContentLength < *contentLength)) {

        if (!httpClient.available()) {
            continue;
        }

        // Read content.
        char character = (byte) httpClient.read();
        httpContent[loadedContentLength] = character;

#ifdef HTTP_LOG
        // Print out this character
        Serial.print(character);
#endif
        loadedContentLength++;
        // We read something, reset the timeout counter
        timeoutStart = millis();
    }

    // Copy the content.
    content = new char[*contentLength];
    strcpy(content, httpContent);

    httpClient.stop();
    return 0;
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
    this->_userAgent = new char[]{
            "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36"};
}

//#endregion