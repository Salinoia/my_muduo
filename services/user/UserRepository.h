#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct User {
    int id;
    std::string name;
};

class UserRepository {
public:
    int create(const std::string& name) {
        int id = nextId_++;
        data_[id] = User{id, name};
        return id;
    }

    bool get(int id, User& out) const {
        auto it = data_.find(id);
        if (it != data_.end()) {
            out = it->second;
            return true;
        }
        return false;
    }

    bool update(int id, const std::string& name) {
        auto it = data_.find(id);
        if (it != data_.end()) {
            it->second.name = name;
            return true;
        }
        return false;
    }

    bool remove(int id) {
        return data_.erase(id) > 0;
    }

    std::vector<User> list() const {
        std::vector<User> result;
        for (const auto& kv : data_) {
            result.push_back(kv.second);
        }
        return result;
    }

private:
    int nextId_ = 1;
    std::unordered_map<int, User> data_;
};

