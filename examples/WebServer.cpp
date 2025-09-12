#include "../src/framework/ioc/Container.h"
#include "../src/storage/UserRepository.h"
#include "../src/services/UserService.h"
#include <iostream>
#include <memory>

using namespace ioc;

class UserController {
public:
    UserController() : service_(Container::instance().resolve<UserService>()) {}
    void get(int id) {
        std::cout << service_->getUser(id) << std::endl;
    }
private:
    std::shared_ptr<UserService> service_;
};

int main() {
    Container::instance().registerType<UserRepository>(Lifetime::Singleton);
    Container::instance().registerType<UserService>(Lifetime::Singleton);
    Container::instance().registerType<UserController>(Lifetime::Prototype);

    auto controller = Container::instance().resolve<UserController>();
    if (controller) {
        controller->get(1);
    }
    return 0;
}

