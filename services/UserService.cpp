#include "UserService.h"
#include "../framework/ioc/Container.h"
#include "../storage/UserRepository.h"

using namespace ioc;

UserService::UserService()
    : repo_(Container::instance().resolve<UserRepository>()) {}

std::string UserService::getUser(int id) {
    if (repo_) {
        return repo_->getUserName(id);
    }
    return "";
}

