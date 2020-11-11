#ifndef ESP32_HTTP_CLIENT_STATION_H
#define ESP32_HTTP_CLIENT_STATION_H

#include <Arduino.h>

class Station {

private:

    String _id;
    
    String _code;

    String _name;

    int _availability;

    double _createdTime;

    double _lastModifiedTime;

public:

    Station(String id) {
        this->_id = id;
    }

    void setCode(String code) {
        this->_code = code;
    }

    void setName(String name) {
        this->_name = name;
    }

    void setAvailability(int availability) {
        this->_availability = availability;
    }


};

#endif //ESP32_HTTP_CLIENT_STATION_H
