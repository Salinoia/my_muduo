#pragma once

#include "storage/db/orm/Mapper.h"
#include "User.h"

// Data access object for `User` entity.
class UserDao : public Mapper<User> {
};

