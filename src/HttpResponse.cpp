#include <HttpResponse.h>

//#region Constructor

HttpResponse::HttpResponse() {
    this->_statusCode = -1;
}

HttpResponse::HttpResponse(int statusCode, String content) {
    this->_statusCode = statusCode;
    this->_content = content;
}

//#endregion

//#region Methods

int HttpResponse::getStatusCode() {
    return this->_statusCode;
}

String HttpResponse::getContent() {
    return this->_content;
}


//#endregion