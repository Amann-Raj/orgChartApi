#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/JobsController.h"
#include "../utils/utils.h"
#include "../models/Job.h"
#include "../models/Person.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

DROGON_TEST(JobsControllerGet)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k200OK);
    };
    JobsController controller;
    controller.get(req, std::move(callback));
}

DROGON_TEST(JobsControllerGetWithParams)
{
    auto req = HttpRequest::newHttpRequest();
    req->setParameter("offset", "0");
    req->setParameter("limit", "10");
    req->setParameter("sort_field", "title");
    req->setParameter("sort_order", "desc");

    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k200OK);
    };
    JobsController controller;
    controller.get(req, std::move(callback));
}

DROGON_TEST(JobsControllerGetOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k200OK);
    };
    JobsController controller;
    controller.getOne(req, std::move(callback), 1);
}

DROGON_TEST(JobsControllerGetOneNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k404NotFound);
    };
    JobsController controller;
    controller.getOne(req, std::move(callback), 9999);
}

DROGON_TEST(JobsControllerCreateOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k201Created);
    };
    JobsController controller;
    Job job;
    job.setTitle("Test Job");
    controller.createOne(req, std::move(callback), std::move(job));
}

DROGON_TEST(JobsControllerUpdateOne)
{
    auto req = HttpRequest::newHttpRequest();
    Json::Value json;
    json["title"] = "Updated Job Title";
    req->setJsonObject(json);

    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k204NoContent);
    };
    JobsController controller;
    controller.updateOne(req, std::move(callback), 1, {});
}

DROGON_TEST(JobsControllerUpdateOneNoJsonObject)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k400BadRequest);
    };
    JobsController controller;
    controller.updateOne(req, std::move(callback), 1, {});
}

DROGON_TEST(JobsControllerDeleteOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k204NoContent);
    };
    JobsController controller;
    controller.deleteOne(req, std::move(callback), 1);
}

DROGON_TEST(JobsControllerGetJobPersons)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k200OK);
    };
    JobsController controller;
    controller.getJobPersons(req, std::move(callback), 1);
}

DROGON_TEST(JobsControllerGetJobPersonsNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k404NotFound);
    };
    JobsController controller;
    controller.getJobPersons(req, std::move(callback), 9999);
}

DROGON_TEST(JobsControllerUpdateOneNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    Json::Value json;
    json["title"] = "Updated Job Title";
    req->setJsonObject(json);

    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k404NotFound);
    };
    JobsController controller;
    controller.updateOne(req, std::move(callback), 9999, {});
}

DROGON_TEST(JobsControllerDeleteOneNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k404NotFound);
    };
    JobsController controller;
    controller.deleteOne(req, std::move(callback), 9999);
}

DROGON_TEST(JobsControllerCreateOneInvalidData)
{
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k400BadRequest);
    };
    JobsController controller;
    Job job;
    job.setTitle(""); // Invalid data
    controller.createOne(req, std::move(callback), std::move(job));
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/JobsController.h"
#include "../utils/utils.h"
#include "../models/Job.h"
#include "../models/Person.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

DROGON_TEST(JobsControllerUpdateOneEmptyTitle)
{
    auto req = HttpRequest::newHttpRequest();
    Json::Value json;
    json["title"] = "";
    req->setJsonObject(json);

    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), k400BadRequest);
    };
    JobsController controller;
    controller.updateOne(req, std::move(callback), 1, {});
}