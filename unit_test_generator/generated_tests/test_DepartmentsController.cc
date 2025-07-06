#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/DepartmentsController.h"
#include "../utils/utils.h"
#include "../models/Person.h"
#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(DepartmentsControllerGet)
{
    // Mock request and callback
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    };

    // Instantiate the controller
    DepartmentsController controller;

    // Call the method
    controller.get(req, std::move(callback));
}

DROGON_TEST(DepartmentsControllerCreateOne)
{
    // Mock request and callback
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
    };

    // Instantiate the controller
    DepartmentsController controller;
    Department department;
    department.setName("Test Department");

    // Call the method
    controller.createOne(req, std::move(callback), std::move(department));
}

DROGON_TEST(DepartmentsControllerUpdateOne)
{
    // Mock request and callback
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    };

    // Instantiate the controller
    DepartmentsController controller;
    Department department;
    department.setName("Updated Department");

    // Call the method
    controller.updateOne(req, std::move(callback), 1, std::move(department));
}

DROGON_TEST(DepartmentsControllerDeleteOne)
{
    // Mock request and callback
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    };

    // Instantiate the controller
    DepartmentsController controller;

    // Call the method
    controller.deleteOne(req, std::move(callback), 1);
}

DROGON_TEST(DepartmentsControllerGetDepartmentPersons)
{
    // Mock request and callback
    auto req = HttpRequest::newHttpRequest();
    auto callback = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    };

    // Instantiate the controller
    DepartmentsController controller;

    // Call the method
    controller.getDepartmentPersons(req, std::move(callback), 1);
}

DROGON_TEST(DepartmentsControllerNotFound)
{
    // Mock request and callback
    auto req = HttpRequest::newHttpRequest();

    // Instantiate the controller
    DepartmentsController controller;
    Department department;
    department.setName("Updated Department");

    // Lambda for checking 404 Not Found
    auto checkNotFound = [](const HttpResponsePtr &resp) {
        CHECK(resp != nullptr);
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    };

    // Test getOne
    controller.getOne(req, checkNotFound, 99999);

    // Test updateOne
    controller.updateOne(req, checkNotFound, 99999, std::move(department));

    // Test getDepartmentPersons
    controller.getDepartmentPersons(req, checkNotFound, 99999);
}