#pragma once
#include <string>
#include "connection.h"

namespace sql {
class Driver {
public:
    Driver() : connect_count(0) {}
    Connection* connect(const std::string& url, const std::string& user, const std::string& password) {
        ++connect_count;
        last_url = url; last_user = user; last_password = password;
        return new Connection();
    }
    int connect_count;
    std::string last_url, last_user, last_password;
};
}

sql::Driver* get_driver_instance();
