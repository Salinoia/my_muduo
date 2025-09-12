#include "Container.h"

namespace ioc {

Container& Container::instance() {
    static Container instance;
    return instance;
}

void Container::clear() {
    registrations_.clear();
}

} // namespace ioc

