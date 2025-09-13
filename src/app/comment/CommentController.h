#pragma once

#include "controller/BaseController.h"
#include "CommentService.h"

class CommentController : public BaseController {
public:
    void registerRoutes(Router& router) override {
        auto service = getDependency<CommentService>();
        router.addRoute("/comment/list", [service](HttpRequest&, HttpResponse& res) {
            auto list = service->listComments();
            std::string body;
            for (const auto& c : list) {
                body += std::to_string(c.id) + ":" + c.text + "\n";
            }
            res.setStatusCode(HttpResponse::k200Ok);
            res.setBody(body);
        });

        router.addRoute("POST", "/comment/create", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("text");
            if (it != params.end()) {
                int id = service->createComment(it->second);
                res.setStatusCode(HttpResponse::k200Ok);
                res.setBody("created:" + std::to_string(id));
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("text required");
            }
        });

        router.addRoute("/comment/get", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("id");
            if (it != params.end()) {
                Comment c{};
                if (service->getComment(std::stoi(it->second), c)) {
                    res.setStatusCode(HttpResponse::k200Ok);
                    res.setBody(c.text);
                } else {
                    res.setStatusCode(HttpResponse::k404NotFound);
                    res.setBody("not found");
                }
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("id required");
            }
        });

        router.addRoute("POST", "/comment/update", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto itId = params.find("id");
            auto itText = params.find("text");
            if (itId != params.end() && itText != params.end()) {
                if (service->updateComment(std::stoi(itId->second), itText->second)) {
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

        router.addRoute("POST", "/comment/delete", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("id");
            if (it != params.end()) {
                if (service->deleteComment(std::stoi(it->second))) {
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

