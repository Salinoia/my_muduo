#pragma once

#include "controller/BaseController.h"
#include "PostService.h"

class PostController : public BaseController {
public:
    void registerRoutes(Router& router) override {
        auto service = getDependency<PostService>();
        router.addRoute("/post/list", [service](HttpRequest&, HttpResponse& res) {
            auto list = service->listPosts();
            std::string body;
            for (const auto& p : list) {
                body += std::to_string(p.id) + ":" + p.title + "\n";
            }
            res.setStatusCode(HttpResponse::k200Ok);
            res.setBody(body);
        });

        router.addRoute("POST", "/post/create", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto itTitle = params.find("title");
            auto itContent = params.find("content");
            if (itTitle != params.end() && itContent != params.end()) {
                int id = service->createPost(itTitle->second, itContent->second);
                res.setStatusCode(HttpResponse::k200Ok);
                res.setBody("created:" + std::to_string(id));
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("params required");
            }
        });

        router.addRoute("/post/get", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("id");
            if (it != params.end()) {
                Post p{};
                if (service->getPost(std::stoi(it->second), p)) {
                    res.setStatusCode(HttpResponse::k200Ok);
                    res.setBody(p.title + ":" + p.content);
                } else {
                    res.setStatusCode(HttpResponse::k404NotFound);
                    res.setBody("not found");
                }
            } else {
                res.setStatusCode(HttpResponse::k400BadRequest);
                res.setBody("id required");
            }
        });

        router.addRoute("POST", "/post/update", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto itId = params.find("id");
            auto itTitle = params.find("title");
            auto itContent = params.find("content");
            if (itId != params.end() && itTitle != params.end() && itContent != params.end()) {
                if (service->updatePost(std::stoi(itId->second), itTitle->second, itContent->second)) {
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

        router.addRoute("POST", "/post/delete", [service, this](HttpRequest& req, HttpResponse& res) {
            auto params = parseQuery(req.query());
            auto it = params.find("id");
            if (it != params.end()) {
                if (service->deletePost(std::stoi(it->second))) {
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

