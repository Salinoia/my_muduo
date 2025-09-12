#pragma once

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace ioc {

// Lifetime of a registration
enum class Lifetime {
    Singleton,
    Prototype
};

class Container {
public:
    // Access singleton instance of container
    static Container& instance();

    // Register a type mapping. Interface defaults to Implementation.
    template <typename Interface, typename Implementation = Interface>
    void registerType(Lifetime lifetime = Lifetime::Prototype) {
        Registration reg;
        reg.factory = []() {
            return std::static_pointer_cast<void>(std::make_shared<Implementation>());
        };
        reg.lifetime = lifetime;
        registrations_[std::type_index(typeid(Interface))] = std::move(reg);
    }

    // Resolve instance of requested type
    template <typename T>
    std::shared_ptr<T> resolve() {
        auto it = registrations_.find(std::type_index(typeid(T)));
        if (it == registrations_.end()) {
            return nullptr;
        }
        Registration& reg = it->second;
        if (reg.lifetime == Lifetime::Singleton) {
            if (!reg.instance) {
                reg.instance = reg.factory();
            }
            return std::static_pointer_cast<T>(reg.instance);
        }
        return std::static_pointer_cast<T>(reg.factory());
    }

    // Clear all registrations (mainly for testing)
    void clear();

private:
    struct Registration {
        std::function<std::shared_ptr<void>()> factory;
        Lifetime lifetime{Lifetime::Prototype};
        std::shared_ptr<void> instance;  // for singleton
    };

    std::unordered_map<std::type_index, Registration> registrations_;
};

} // namespace ioc

