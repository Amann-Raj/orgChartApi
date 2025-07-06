#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/PersonsController.h"
#include "utils/utils.h"
#include <drogon/orm/Mapper.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Result.h>

#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(PersonsControllerGet)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    };
    PersonsController controller;
    controller.get(req, std::move(callback));
}

DROGON_TEST(PersonsControllerGetNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    };
    PersonsController controller;
    controller.get(req, std::move(callback));
}

DROGON_TEST(PersonsControllerGetOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    };
    PersonsController controller;
    controller.getOne(req, std::move(callback), 1);
}

DROGON_TEST(PersonsControllerGetOneNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    };
    PersonsController controller;
    controller.getOne(req, std::move(callback), 9999);
}

DROGON_TEST(PersonsControllerUpdateOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    };
    PersonsController controller;
    Person person;
    person.setFirstName("UpdatedFirstName");
    controller.updateOne(req, std::move(callback), 1, std::move(person));
}

DROGON_TEST(PersonsControllerUpdateOneNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    };
    PersonsController controller;
    Person person;
    person.setFirstName("UpdatedFirstName");
    controller.updateOne(req, std::move(callback), 9999, std::move(person));
}

DROGON_TEST(PersonsControllerDeleteOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    };
    PersonsController controller;
    controller.deleteOne(req, std::move(callback), 1);
}

DROGON_TEST(PersonsControllerGetDirectReports)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    };
    PersonsController controller;
    controller.getDirectReports(req, std::move(callback), 1);
}

DROGON_TEST(PersonsControllerGetDirectReportsNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    };
    PersonsController controller;
    controller.getDirectReports(req, std::move(callback), 9999);
}

DROGON_TEST(PersonDetailsToJson)
{
    PersonsController::PersonInfo personInfo;
    personInfo.id_ = 1;
    personInfo.first_name_ = "John";
    personInfo.last_name_ = "Doe";
    personInfo.hire_date_ = trantor::Date::now();
    personInfo.manager_id_ = 2;
    personInfo.manager_full_name_ = "Jane Doe";
    personInfo.department_id_ = 3;
    personInfo.department_name_ = "IT";
    personInfo.job_id_ = 4;
    personInfo.job_title_ = "Developer";

    PersonsController::PersonDetails personDetails(personInfo);
    Json::Value json = personDetails.toJson();

    CHECK_EQ(json["id"].asInt(), 1);
    CHECK_EQ(json["first_name"].asString(), "John");
    CHECK_EQ(json["last_name"].asString(), "Doe");
    CHECK_EQ(json["manager"]["id"].asInt(), 2);
    CHECK_EQ(json["manager"]["full_name"].asString(), "Jane Doe");
    CHECK_EQ(json["department"]["id"].asInt(), 3);
    CHECK_EQ(json["department"]["name"].asString(), "IT");
    CHECK_EQ(json["job"]["id"].asInt(), 4);
    CHECK_EQ(json["job"]["title"].asString(), "Developer");
}

DROGON_TEST(PersonsControllerCreateOne)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(HttpMethod::Post);
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    Json::Value json;
    json["firstName"] = "NewFirstName";
    json["lastName"] = "NewLastName";
    json["hireDate"] = "2023-10-26";
    json["jobId"] = 1;
    json["departmentId"] = 1;
    json["managerId"] = 1;

    req->setBody(json.toStyledString());

    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
    };

    PersonsController controller;
    controller.createOne(req, std::move(callback));
}

DROGON_TEST(PersonsControllerCreateOneInvalidInput)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(HttpMethod::Post);
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    Json::Value json;
    json["firstName"] = ""; // Invalid input
    json["lastName"] = "NewLastName";
    json["hireDate"] = "2023-10-26";
    json["jobId"] = 1;
    json["departmentId"] = 1;
    json["managerId"] = 1;

    req->setBody(json.toStyledString());

    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    };

    PersonsController controller;
    controller.createOne(req, std::move(callback));
}

DROGON_TEST(PersonsControllerUpdateOneBadRequest)
{
    auto req = HttpRequest::newHttpRequest();

    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    };

    PersonsController controller;

    Person person;
    person.setFirstName("");

    controller.updateOne(req, std::move(callback), 1, std::move(person));
}

DROGON_TEST(PersonsControllerDeleteOneNotFound)
{
    auto req = HttpRequest::newHttpRequest();

    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    };

    PersonsController controller;

    controller.deleteOne(req, std::move(callback), 9999);
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/PersonsController.h"
#include "utils/utils.h"
#include <drogon/orm/Mapper.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Result.h>

#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(PersonsControllerCreateOneMissingField)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(HttpMethod::Post);
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    Json::Value json;
    json["lastName"] = "NewLastName";
    json["hireDate"] = "2023-10-26";
    json["jobId"] = 1;
    json["departmentId"] = 1;
    json["managerId"] = 1;

    req->setBody(json.toStyledString());

    auto callback = [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    };

    PersonsController controller;
    controller.createOne(req, std::move(callback));
}