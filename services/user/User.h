#pragma once

#include <string>

#include "storage/db/orm/Entity.h"

struct User : public Entity<User> {
    int id{0};
    std::string name;
    std::string email;

    ENTITY_TABLE("users");
    ENTITY_FIELDS(
        FIELD(id),
        FIELD(name),
        FIELD(email)
    );
};

