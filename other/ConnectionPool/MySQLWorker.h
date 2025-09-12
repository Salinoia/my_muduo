#pragma once
#include <memory>
#include <thread>
class MySQLConn;  // 前向声明
template <typename T>
class BlockingQueuePro;

class SQLOperation;

class MySQLWorker {  // 对应一个线程
public:
    MySQLWorker(MySQLConn* conn, BlockingQueuePro<SQLOperation*>& task_queue);
    ~MySQLWorker();

    void Start();
    void Stop();

private:
    void Worker();

    MySQLConn* conn_;  // 该线程对应的数据库连接
    std::thread worker_;  // 线程从任务队列中弹出 Operation
    BlockingQueuePro<SQLOperation*>& task_queue_;
};