#pragma once
#include <exception>

namespace sql {
class SQLException : public std::exception {
public:
    const char* what() const noexcept override { return "sql exception"; }
};
}
