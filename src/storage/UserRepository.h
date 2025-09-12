#pragma once
#include <string>

class UserRepository {
public:
    std::string getUserName(int id);
    bool updateUserName(int id, const std::string& name);
};

