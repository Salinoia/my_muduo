#pragma once
#include <memory>
#include <string>

class UserRepository;

class UserService {
public:
    UserService();
    std::string getUser(int id);
private:
    std::shared_ptr<UserRepository> repo_;
};

