#include <chrono>
#include <iostream>
#include <thread>

#include <cppconn/resultset.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>

#include "AsyncProcess.h"
#include "MySQLConnPool.h"

void HandleQueryResult(std::unique_ptr<sql::ResultSet> res) {
    if (!res) {
        std::cout << "No result set.\n";
        return;
    }
    while (res->next()) {
        std::cout << "cid: " << res->getInt("cid")
                  << " caption: " << res->getString("caption") << std::endl;
    }
}

int main() {
    // 简单检测 MySQL 服务是否可连
    sql::Driver* driver = get_driver_instance();
    sql::Connection* test_conn = nullptr;
    try {
        test_conn = driver->connect("tcp://127.0.0.1:3306", "root", "123456");
    } catch (...) {
        std::cerr << "MySQL 服务未启动，程序退出。\n";
        return 1;  // 直接退出，不继续执行
    }
    delete test_conn;

    // 初始化连接池
    MySQLConnPool* pool1 = MySQLConnPool::GetInstance("edu_svc");
    pool1->InitPool("tcp://127.0.0.1:3306;root;123456", 10);
    AsyncProcesser response_handler;

    // 获取查询结果
    auto query_callback1 = pool1->Query("SELECT * FROM class", HandleQueryResult);
    response_handler.AddQueryCallback(std::move(query_callback1));

    // 处理查询结果
    while (true) {
        response_handler.InvokeIfReady();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
