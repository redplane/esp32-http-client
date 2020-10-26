//
// Created by Linh Nguyen on 10/25/20.
//

#ifndef ESP32_HTTP_CLIENT_HTTP_HEADER_H
#define ESP32_HTTP_CLIENT_HTTP_HEADER_H

class HttpHeader {

private:
    char *_key;

    char *_value;

public:

    // Get key
    void getKey(char *key, int &length);

    // Get value.
    void getValue(char *value, int &length);

    HttpHeader(const char *key, const char *value);
};

#endif //ESP32_HTTP_CLIENT_HTTP_HEADER_H
