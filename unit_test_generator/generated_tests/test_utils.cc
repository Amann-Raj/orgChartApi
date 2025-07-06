#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <drogon/utils/Utilities.h>
#include "../../utils/utils.h"

#include <iostream>
#include <string>
#include <functional>

using namespace drogon;

DROGON_TEST(BadRequestTest)
{
    bool callbackCalled = false;
    HttpResponsePtr capturedResponse;

    auto callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        capturedResponse = resp;
    };

    std::string errorMessage = "Test error message";
    HttpStatusCode statusCode = k400BadRequest;

    badRequest(std::move(callback), errorMessage, statusCode);

    CHECK(callbackCalled);
    REQUIRE(capturedResponse != nullptr);
    CHECK_EQ(capturedResponse->getStatusCode(), statusCode);

    auto jsonResponse = capturedResponse->getJsonObject();
    REQUIRE(jsonResponse != nullptr);
    CHECK_EQ((*jsonResponse)["error"].asString(), errorMessage);
}

DROGON_TEST(MakeErrRespTest)
{
    std::string errorMessage = "Another test error message";
    Json::Value errorResponse = makeErrResp(errorMessage);

    CHECK_EQ(errorResponse["error"].asString(), errorMessage);
}

DROGON_TEST(BadRequestTest_DifferentStatusCode)
{
    bool callbackCalled = false;
    HttpResponsePtr capturedResponse;

    auto callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        capturedResponse = resp;
    };

    std::string errorMessage = "Test error message with different status code";
    HttpStatusCode statusCode = k500InternalServerError;

    badRequest(std::move(callback), errorMessage, statusCode);

    CHECK(callbackCalled);
    REQUIRE(capturedResponse != nullptr);
    CHECK_EQ(capturedResponse->getStatusCode(), statusCode);

    auto jsonResponse = capturedResponse->getJsonObject();
    REQUIRE(jsonResponse != nullptr);
    CHECK_EQ((*jsonResponse)["error"].asString(), errorMessage);
}