#pragma once
#include <string>

namespace sql {
class Connection {
public:
    void setSchema(const std::string& schema) { schema_ = schema; }
    std::string schema_;
};
}
