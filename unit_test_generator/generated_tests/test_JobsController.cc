#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/JobsController.h"
#include "../utils/utils.h"
#include "../models/Person.h"
#include "../models/Job.h"

#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <functional>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(JobsControllerGet)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;

    controller->get(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
        callbackCalled = true;
    });

    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerGetOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;
    int jobId = 1;

    controller->getOne(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
        callbackCalled = true;
    }, jobId);

    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerCreateOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;
    Job job;
    job.setTitle("Test Job");

    controller->createOne(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
        callbackCalled = true;
    }, std::move(job));

    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerUpdateOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;
    int jobId = 1;
    Job job;
    job.setTitle("Updated Job Title");

    controller->updateOne(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
        callbackCalled = true;
    }, jobId, std::move(job));

    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerDeleteOne)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;
    int jobId = 1;

    controller->deleteOne(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
        callbackCalled = true;
    }, jobId);

    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerGetJobPersons)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;
    int jobId = 1;

    controller->getJobPersons(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
        callbackCalled = true;
    }, jobId);

    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerNotFound)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;
    int jobId = 9999;

    controller->getOne(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
        callbackCalled = true;
    }, jobId);

    CHECK(callbackCalled);

    callbackCalled = false;
    controller->getJobPersons(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
        callbackCalled = true;
    }, jobId);

    CHECK(callbackCalled);

    Job job;
    job.setTitle("Updated Job Title");
    req->setJsonObject(std::make_shared<Json::Value>());
    callbackCalled = false;
    controller->updateOne(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
        callbackCalled = true;
    }, jobId, std::move(job));

    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerUpdateOneBadRequest)
{
    auto req = HttpRequest::newHttpRequest();
    auto controller = std::make_shared<JobsController>();
    bool callbackCalled = false;
    int jobId = 1;
    Job job;

    controller->updateOne(req, [&](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
        callbackCalled = true;
    }, jobId, std::move(job));

    CHECK(callbackCalled);
}