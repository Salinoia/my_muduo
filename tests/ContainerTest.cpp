#include "../framework/ioc/Container.h"
#include <cassert>
#include <iostream>
#include <memory>

using namespace ioc;

struct Repo {
    int value{0};
};

struct Service {
    Service() : repo(Container::instance().resolve<Repo>()) {}
    std::shared_ptr<Repo> repo;
};

void TestRegistrationAndResolution() {
    Container::instance().clear();
    Container::instance().registerType<Repo>(Lifetime::Prototype);
    auto repo = Container::instance().resolve<Repo>();
    assert(repo != nullptr);
}

void TestSingletonLifecycle() {
    Container::instance().clear();
    Container::instance().registerType<Repo>(Lifetime::Singleton);
    auto a = Container::instance().resolve<Repo>();
    auto b = Container::instance().resolve<Repo>();
    assert(a == b);
}

void TestPrototypeLifecycle() {
    Container::instance().clear();
    Container::instance().registerType<Repo>(Lifetime::Prototype);
    auto a = Container::instance().resolve<Repo>();
    auto b = Container::instance().resolve<Repo>();
    assert(a != b);
}

void TestDependencyResolution() {
    Container::instance().clear();
    Container::instance().registerType<Repo>(Lifetime::Singleton);
    Container::instance().registerType<Service>(Lifetime::Prototype);
    auto svc = Container::instance().resolve<Service>();
    assert(svc != nullptr);
    assert(svc->repo != nullptr);
}

int main() {
    TestRegistrationAndResolution();
    TestSingletonLifecycle();
    TestPrototypeLifecycle();
    TestDependencyResolution();
    std::cout << "All container tests passed!" << std::endl;
    return 0;
}

