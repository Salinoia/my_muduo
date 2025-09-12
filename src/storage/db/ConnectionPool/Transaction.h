#pragma once
#include <memory>
#include <cppconn/connection.h>

class Transaction {
public:
    explicit Transaction(std::shared_ptr<sql::Connection> conn)
        : conn_(std::move(conn)), committed_(false) {
        if (conn_) {
            conn_->setAutoCommit(false);
        }
    }
    ~Transaction() {
        if (conn_) {
            if (!committed_) {
                conn_->rollback();
            }
            conn_->setAutoCommit(true);
        }
    }
    sql::Connection* Get() const { return conn_.get(); }
    void Commit() {
        if (conn_) {
            conn_->commit();
            committed_ = true;
        }
    }
    void Rollback() {
        if (conn_) {
            conn_->rollback();
            committed_ = true;
        }
    }
private:
    std::shared_ptr<sql::Connection> conn_;
    bool committed_;
};
