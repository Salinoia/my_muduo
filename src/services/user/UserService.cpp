#include "UserDao.h"

#include <iostream>

// Demonstrates basic CRUD operations using the generic Mapper.
void DemoUserService() {
    UserDao dao;
    User u{0, "Alice", "alice@example.com"};
    dao.insert(u);

    auto loaded = dao.selectById(u.id);
    if (loaded) {
        std::cout << "Loaded: " << loaded->name << std::endl;
        loaded->name = "AliceUpdated";
        dao.update(*loaded);
        dao.deleteById(loaded->id);
    }
}

