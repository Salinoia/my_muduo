#include "MySQLConn.h"

#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <exception>
#include <string>
#include <string_view>  // C++17
#include <vector>

#include "MySQLWorker.h"
// "tcp://127.0.0.1:3306;root;123456"
static std::vector<std::string_view> Tokenize(std::string_view str, char delimiter, bool keepEmpty) {
    std::vector<std::string_view> tokens;

    size_t start = 0;
    for (size_t end = str.find(delimiter); end != std::string_view::npos; end = str.find(delimiter, start)) {
        if (keepEmpty || (start < end)) {
            tokens.push_back(str.substr(start, end - start));
        }
        start = end + 1;
    }
    if (keepEmpty || (start < str.size())) {
        tokens.push_back(str.substr(start));
    }
    return tokens;
}
MySQLConnInfo::MySQLConnInfo(const std::string& info, const std::string db) {
    auto tokens = Tokenize(info, ';', false);
    if (tokens.size() != 3) {
        return;
    }
    url.assign(tokens[0]);
    user.assign(tokens[1]);
    password.assign(tokens[2]);
    database = db;
}

MySQLConn::MySQLConn(const std::string& info, const std::string& db, BlockingQueuePro<SQLOperation*>& task_queue) : info_(info, db) {
    worker_ = new MySQLWorker(this, task_queue);
    worker_->Start();
}
MySQLConn::~MySQLConn() {
    if (worker_) {
        worker_->Stop();
        delete worker_;
        worker_ = nullptr;
    }

    if (conn_) {
        delete conn_;
    }
}

int MySQLConn::Open() {
    int err = 0;
    try {
        driver_ = get_driver_instance();
        conn_ = driver_->connect(info_.url, info_.user, info_.password);
        conn_->setSchema(info_.database);
    } catch (sql::SQLException& e) {
        HandleExecption(e);
        err = e.getErrorCode();
    }
    return err;
}

void MySQLConn::Close() {
    if (conn_) {
        conn_->close();
        delete conn_;
        conn_ = nullptr;
    }
}

sql::ResultSet* MySQLConn::Query(const std::string& sql) {
    sql::ResultSet* res = nullptr;
    try {
        if (conn_) {
            sql::Statement* stmt = conn_->createStatement();
            res = stmt->executeQuery(sql);
            delete stmt;
        }
    } catch (sql::SQLException& e) {
        HandleExecption(e);
    }
    return res;
}

void MySQLConn::HandleExecption(sql::SQLException& e) {
    if (e.getErrorCode() != 0) {
        // Log the error message
    }
}
