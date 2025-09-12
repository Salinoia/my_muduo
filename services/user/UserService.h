#pragma once

#include <memory>
#include <vector>

#include "UserRepository.h"

class UserService {
public:
    explicit UserService(std::shared_ptr<UserRepository> repo) : repo_(std::move(repo)) {}

    int createUser(const std::string& name) { return repo_->create(name); }
    bool getUser(int id, User& out) const { return repo_->get(id, out); }
    bool updateUser(int id, const std::string& name) { return repo_->update(id, name); }
    bool deleteUser(int id) { return repo_->remove(id); }
    std::vector<User> listUsers() const { return repo_->list(); }

private:
    std::shared_ptr<UserRepository> repo_;
};

