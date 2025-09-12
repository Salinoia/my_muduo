#pragma once

#include <memory>
#include <vector>

#include "CommentRepository.h"

class CommentService {
public:
    explicit CommentService(std::shared_ptr<CommentRepository> repo) : repo_(std::move(repo)) {}

    int createComment(const std::string& text) { return repo_->create(text); }
    bool getComment(int id, Comment& out) const { return repo_->get(id, out); }
    bool updateComment(int id, const std::string& text) { return repo_->update(id, text); }
    bool deleteComment(int id) { return repo_->remove(id); }
    std::vector<Comment> listComments() const { return repo_->list(); }

private:
    std::shared_ptr<CommentRepository> repo_;
};

