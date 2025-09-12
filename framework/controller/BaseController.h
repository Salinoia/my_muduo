#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <string>
#include <sstream>

#include "router/Router.h"

class BaseController {
public:
    virtual ~BaseController() = default;
    virtual void registerRoutes(Router& router) = 0;

    template<typename T>
    void addDependency(const std::shared_ptr<T>& dep) {
        dependencies_[std::type_index(typeid(T))] = dep;
    }

protected:
    template<typename T>
    std::shared_ptr<T> getDependency() const {
        auto it = dependencies_.find(std::type_index(typeid(T)));
        if (it != dependencies_.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    std::unordered_map<std::string, std::string> parseQuery(const std::string& query) const {
        std::unordered_map<std::string, std::string> params;
        std::istringstream ss(query);
        std::string pair;
        while (std::getline(ss, pair, '&')) {
            auto pos = pair.find('=');
            if (pos != std::string::npos) {
                auto key = pair.substr(0, pos);
                auto value = pair.substr(pos + 1);
                params[key] = value;
            }
        }
        return params;
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> dependencies_;
};

