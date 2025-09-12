#include <cassert>
#include <iostream>
#include <string>

#include "http/HttpContext.h"
#include "http/HttpResponse.h"
#include "Buffer.h"
#include "TimeStamp.h"
#include <nlohmann/json.hpp>

void TestJsonRequest() {
    Buffer buf;
    std::string req =
        "POST /json HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "{\"a\":1,\"b\":2}";
    buf.append(req.c_str(), req.size());
    HttpContext context;
    assert(context.parseRequest(&buf, TimeStamp()));
    assert(context.gotAll());
    const auto& j = context.request().getJson();
    assert(j["a"] == 1);
    assert(j["b"] == 2);
}

void TestJsonResponse() {
    nlohmann::json j;
    j["message"] = "ok";
    HttpResponse resp(false);
    resp.setStatusCode(HttpResponse::k200Ok);
    resp.setJson(j);
    Buffer out;
    resp.appendToBuffer(&out);
    std::string res = out.retrieveAllAsString();
    assert(res.find("Content-Type: application/json") != std::string::npos);
    assert(res.find("{\"message\":\"ok\"}") != std::string::npos);
}

int main() {
    TestJsonRequest();
    TestJsonResponse();
    std::cout << "JSON HTTP tests passed!" << std::endl;
    return 0;
}
