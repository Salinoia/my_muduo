#pragma once

#include "http/HttpResponse.h"

class JsonResponse : public HttpResponse {
public:
    explicit JsonResponse(bool close = false) : HttpResponse(close) {
        setContentType("application/json");
    }
};
