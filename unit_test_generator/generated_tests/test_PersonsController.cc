#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/PersonsController.h"
#include <memory>
#include <utility>
#include <iostream>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace {

std::shared_ptr<HttpRequest> createHttpRequest(const std::string& method, const std::string& path, const Json::Value& body = Json::Value()) {
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(method);
    req->setPath(path);
    if (!body.isNull()) {
        req->setJsonObject(body);
    }
    return req;
}

} // namespace

DROGON_TEST(PersonsControllerGet, PersonsController) {
    auto req = createHttpRequest("GET", "/persons");
    auto callback = std::make_shared<bool>(false);
    std::function<void(const HttpResponsePtr &)> resp_callback =
        [&](const HttpResponsePtr &resp) {
            CHECK(resp != nullptr);
            CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
            *callback = true;
        };
    PersonsController controller;
    controller.get(req, std::move(resp_callback));
    CHECK_EQ(*callback, true);
}

DROGON_TEST(PersonsControllerGetOne, PersonsController) {
    auto req = createHttpRequest("GET", "/persons/1");
    auto callback = std::make_shared<bool>(false);
    std::function<void(const HttpResponsePtr &)> resp_callback =
        [&](const HttpResponsePtr &resp) {
            CHECK(resp != nullptr);
            CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
            *callback = true;
        };
    PersonsController controller;
    controller.getOne(req, std::move(resp_callback), 1);
    CHECK_EQ(*callback, true);
}

DROGON_TEST(PersonsControllerCreateOne, PersonsController) {
    Json::Value json;
    json["first_name"] = "John";
    json["last_name"] = "Doe";
    json["job_id"] = 1;
    json["department_id"] = 1;
    json["manager_id"] = 1;

    auto req = createHttpRequest("POST", "/persons", json);
    auto callback = std::make_shared<bool>(false);
    std::function<void(const HttpResponsePtr &)> resp_callback =
        [&](const HttpResponsePtr &resp) {
            CHECK(resp != nullptr);
            CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
            *callback = true;
        };
    PersonsController controller;
    Person person(json);
    controller.createOne(req, std::move(resp_callback), std::move(person));
    CHECK_EQ(*callback, true);
}

DROGON_TEST(PersonsControllerUpdateOne, PersonsController) {
    Json::Value json;
    json["first_name"] = "UpdatedFirstName";

    auto req = createHttpRequest("PUT", "/persons/1", json);
    auto callback = std::make_shared<bool>(false);
    std::function<void(const HttpResponsePtr &)> resp_callback =
        [&](const HttpResponsePtr &resp) {
            CHECK(resp != nullptr);
            CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
            *callback = true;
        };
    PersonsController controller;
    Person person(json);
    controller.updateOne(req, std::move(resp_callback), 1, std::move(person));
    CHECK_EQ(*callback, true);
}

DROGON_TEST(PersonsControllerDeleteOne, PersonsController) {
    auto req = createHttpRequest("DELETE", "/persons/1");
    auto callback = std::make_shared<bool>(false);
    std::function<void(const HttpResponsePtr &)> resp_callback =
        [&](const HttpResponsePtr &resp) {
            CHECK(resp != nullptr);
            CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
            *callback = true;
        };
    PersonsController controller;
    controller.deleteOne(req, std::move(resp_callback), 1);
    CHECK_EQ(*callback, true);
}

DROGON_TEST(PersonsControllerGetDirectReports, PersonsController) {
    auto req = createHttpRequest("GET", "/persons/1/reports");
    auto callback = std::make_shared<bool>(false);
    std::function<void(const HttpResponsePtr &)> resp_callback =
        [&](const HttpResponsePtr &resp) {
            CHECK(resp != nullptr);
            CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
            *callback = true;
        };
    PersonsController controller;
    controller.getDirectReports(req, std::move(resp_callback), 1);
    CHECK_EQ(*callback, true);
}