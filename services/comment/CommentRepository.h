#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Comment {
    int id;
    std::string text;
};

class CommentRepository {
public:
    int create(const std::string& text) {
        int id = nextId_++;
        data_[id] = Comment{id, text};
        return id;
    }

    bool get(int id, Comment& out) const {
        auto it = data_.find(id);
        if (it != data_.end()) {
            out = it->second;
            return true;
        }
        return false;
    }

    bool update(int id, const std::string& text) {
        auto it = data_.find(id);
        if (it != data_.end()) {
            it->second.text = text;
            return true;
        }
        return false;
    }

    bool remove(int id) { return data_.erase(id) > 0; }

    std::vector<Comment> list() const {
        std::vector<Comment> result;
        for (const auto& kv : data_) {
            result.push_back(kv.second);
        }
        return result;
    }

private:
    int nextId_ = 1;
    std::unordered_map<int, Comment> data_;
};

