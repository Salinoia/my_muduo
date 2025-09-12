#pragma once
#include <functional>
#include <future>

namespace sql {
class ResultSet;
}
class QueryCallback {
public:
    QueryCallback(std::future<std::unique_ptr<sql::ResultSet>>&& future, std::function<void(std::unique_ptr<sql::ResultSet>)>&& callback) :
        future_(std::move(future)), callback_(std::move(callback)) {}

    bool InvokeIfReady() {
        if (future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            callback_(std::move(future_.get()));
            return true;
        }
        return false;
    }

private:
    std::future<std::unique_ptr<sql::ResultSet>> future_;
    std::function<void(std::unique_ptr<sql::ResultSet>)> callback_;
};