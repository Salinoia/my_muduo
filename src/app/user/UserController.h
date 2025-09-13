#pragma once

#include "controller/BaseController.h"
#include "UserService.h"

class UserController : public BaseController {
public:
    void registerRoutes(Router& router) override {
        auto service = getDependency<UserService>();
        router.addRoute("/user/list", [service](HttpRequest&, HttpResponse& res) {
            auto list = service->listUsers();
            std::string body;
            for (const auto& u : list) {
                body += std::to_string(u.id) + ":" + u.name + "\n";
            }
            res.setStatusCode(HttpResponse::k200Ok);
            res.setBody(body);
        });

        router.addRoute("POST", "/user/create", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("name");
            if (it != params.end()) {
                int id = service->createUser(it->second);
                res.setStatusCode(HttpResponse::k200Ok);
                res.setBody("created:" + std::to_string(id));
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("name required");
            }
        });

        router.addRoute("/user/get", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("id");
            if (it != params.end()) {
                User u{};
                if (service->getUser(std::stoi(it->second), u)) {
                    res.setStatusCode(HttpResponse::k200Ok);
                    res.setBody(u.name);
                } else {
                    res.setStatusCode(HttpResponse::k404NotFound);
                    res.setBody("not found");
                }
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("id required");
            }
        });

        router.addRoute("POST", "/user/update", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto itId = params.find("id");
            auto itName = params.find("name");
            if (itId != params.end() && itName != params.end()) {
                if (service->updateUser(std::stoi(itId->second), itName->second)) {
                    res.setStatusCode(HttpResponse::k200Ok);
                    res.setBody("updated");
                } else {
                    res.setStatusCode(HttpResponse::k404NotFound);
                    res.setBody("not found");
                }
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("params required");
            }
        });

        router.addRoute("POST", "/user/delete", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("id");
            if (it != params.end()) {
                if (service->deleteUser(std::stoi(it->second))) {
                    res.setStatusCode(HttpResponse::k200Ok);
                    res.setBody("deleted");
                } else {
                    res.setStatusCode(HttpResponse::k404NotFound);
                    res.setBody("not found");
                }
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("id required");
            }
        });
    }
};

