#include <HttpHeader.h>
#include <cstring>

//#region Constructor

HttpHeader::HttpHeader() {

}

HttpHeader::HttpHeader(const char *key, const char *value) {

    this->_key = new char[strlen(key)];
    this->_value = new char[strlen(value)];

    strcpy(this->_key, key);
    strcpy(this->_value, value);
}

//#endregion

//#region Methods

void HttpHeader::getKey(char *key, int &length) {
    length = strlen(this->_key);
    key = new char[length];
    strcpy(key, this->_key);
}

void HttpHeader::getValue(char *value, int &length) {

    length = strlen(this->_value);
    value = new char[length];
    strcpy(value, this->_value);
}

//#endregion