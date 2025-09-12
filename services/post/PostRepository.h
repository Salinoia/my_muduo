#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Post {
    int id;
    std::string title;
    std::string content;
};

class PostRepository {
public:
    int create(const std::string& title, const std::string& content) {
        int id = nextId_++;
        data_[id] = Post{id, title, content};
        return id;
    }

    bool get(int id, Post& out) const {
        auto it = data_.find(id);
        if (it != data_.end()) {
            out = it->second;
            return true;
        }
        return false;
    }

    bool update(int id, const std::string& title, const std::string& content) {
        auto it = data_.find(id);
        if (it != data_.end()) {
            it->second.title = title;
            it->second.content = content;
            return true;
        }
        return false;
    }

    bool remove(int id) { return data_.erase(id) > 0; }

    std::vector<Post> list() const {
        std::vector<Post> result;
        for (const auto& kv : data_) {
            result.push_back(kv.second);
        }
        return result;
    }

private:
    int nextId_ = 1;
    std::unordered_map<int, Post> data_;
};

