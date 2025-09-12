#include "EventLoop.h"
#include "InetAddress.h"
#include "http/HttpServer.h"
#include "router/Router.h"
#include "user/UserController.h"
#include "user/UserService.h"
#include "user/UserRepository.h"
#include "post/PostController.h"
#include "post/PostService.h"
#include "post/PostRepository.h"
#include "comment/CommentController.h"
#include "comment/CommentService.h"
#include "comment/CommentRepository.h"

int main() {
    EventLoop loop;
    InetAddress addr("0.0.0.0", 8080);
    HttpServer server(&loop, addr, "demo");

    Router router;

    auto userRepo = std::make_shared<UserRepository>();
    auto userService = std::make_shared<UserService>(userRepo);
    UserController userCtl;
    userCtl.addDependency(userService);
    userCtl.registerRoutes(router);

    auto postRepo = std::make_shared<PostRepository>();
    auto postService = std::make_shared<PostService>(postRepo);
    PostController postCtl;
    postCtl.addDependency(postService);
    postCtl.registerRoutes(router);

    auto commentRepo = std::make_shared<CommentRepository>();
    auto commentService = std::make_shared<CommentService>(commentRepo);
    CommentController commentCtl;
    commentCtl.addDependency(commentService);
    commentCtl.registerRoutes(router);

    server.setHttpCallback([&router](HttpRequest& req, HttpResponse& res) {
        router.handle(req, res);
    });

    server.start();
    loop.loop();
    return 0;
}

