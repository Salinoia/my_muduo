#pragma once

#include <memory>
#include <vector>

#include "PostRepository.h"

class PostService {
public:
    explicit PostService(std::shared_ptr<PostRepository> repo) : repo_(std::move(repo)) {}

    int createPost(const std::string& title, const std::string& content) {
        return repo_->create(title, content);
    }
    bool getPost(int id, Post& out) const { return repo_->get(id, out); }
    bool updatePost(int id, const std::string& title, const std::string& content) {
        return repo_->update(id, title, content);
    }
    bool deletePost(int id) { return repo_->remove(id); }
    std::vector<Post> listPosts() const { return repo_->list(); }

private:
    std::shared_ptr<PostRepository> repo_;
};

