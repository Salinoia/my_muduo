#pragma once

#include "http/HttpRequest.h"

class JsonRequest : public HttpRequest {
public:
    JsonRequest() = default;
    const nlohmann::json& json() const { return getJson(); }
};
