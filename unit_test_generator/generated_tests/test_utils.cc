#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/Utilities.h>
#include "../../utils/utils.h"

#include <iostream>
#include <string>
#include <functional>

DROGON_TEST(BadRequestTest)
{
    bool callbackCalled = false;
    drogon::HttpResponsePtr capturedResponse;

    std::function<void(const drogon::HttpResponsePtr &)> callback =
        [&](const drogon::HttpResponsePtr &resp) {
            callbackCalled = true;
            capturedResponse = resp;
        };

    std::string errorMessage = "Test error message";
    drogon::HttpStatusCode statusCode = drogon::k400BadRequest;

    badRequest(std::move(callback), errorMessage, statusCode);

    CHECK(callbackCalled);
    REQUIRE(capturedResponse != nullptr);
    CHECK_EQ(capturedResponse->getStatusCode(), statusCode);

    auto jsonResponse = capturedResponse->getJsonObject();
    REQUIRE(jsonResponse != nullptr);
    CHECK_EQ((*jsonResponse)["error"].asString(), errorMessage);
}

DROGON_TEST(BadRequestTest_DifferentStatusCode)
{
    bool callbackCalled = false;
    drogon::HttpResponsePtr capturedResponse;

    std::function<void(const drogon::HttpResponsePtr &)> callback =
        [&](const drogon::HttpResponsePtr &resp) {
            callbackCalled = true;
            capturedResponse = resp;
        };

    std::string errorMessage = "Test error message with different status";
    drogon::HttpStatusCode statusCode = drogon::k500InternalServerError;

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

    CHECK(errorResponse.isObject());
    CHECK_EQ(errorResponse["error"].asString(), errorMessage);
}

DROGON_TEST(MakeErrRespTest_EmptyMessage)
{
    std::string errorMessage = "";
    Json::Value errorResponse = makeErrResp(errorMessage);

    CHECK(errorResponse.isObject());
    CHECK_EQ(errorResponse["error"].asString(), errorMessage);
}

DROGON_TEST(MakeErrRespTest_SpecialCharacters)
{
    std::string errorMessage = "Error with special chars: !@#$%^&*()_+=-`~[]\{}|;':\",./<>?";
    Json::Value errorResponse = makeErrResp(errorMessage);

    CHECK(errorResponse.isObject());
    CHECK_EQ(errorResponse["error"].asString(), errorMessage);
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/utils/Utilities.h>
#include "../../utils/utils.h"

#include <iostream>
#include <string>
#include <functional>

DROGON_TEST(MakeErrRespTest_Unicode)
{
    std::string errorMessage = "Unicode error message: 你好世界";
    Json::Value errorResponse = makeErrResp(errorMessage);

    CHECK(errorResponse.isObject());
    CHECK_EQ(errorResponse["error"].asString(), errorMessage);
}