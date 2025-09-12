#include <cassert>
#include <iostream>
#include "ConnectionPool.h"
#include "ConnectionPool.cpp"
#include "cppconn/driver.h"

static sql::Driver mock_driver;
sql::Driver* get_driver_instance() { return &mock_driver; }

int main() {
    DBConfig cfg;
    cfg.database = "testdb";
    cfg.pool_size = 1;
    auto& pool = ConnectionPool::Instance();
    pool.Init(cfg);
    assert(mock_driver.connect_count == 1);
    {
        auto conn = pool.Acquire();
        assert(conn);
    }
    auto conn2 = pool.Acquire();
    assert(conn2);
    std::cout << "ConnectionPoolTest passed" << std::endl;
    return 0;
}
