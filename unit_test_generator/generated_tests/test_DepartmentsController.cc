```cpp
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/DepartmentsController.h"
#include "../../controllers/JobsController.h"
#include "../../controllers/PersonsController.h"
#include "../../models/Department.h"
#include "../../models/Job.h"
#include "../../models/Person.h"
#include "../../utils/utils.h"
#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <json/json.h>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace {

std::shared_ptr<HttpRequest> createHttpRequest(const std::string& method, const std::string& path, const std::string& body = "") {
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(method);
    req->setPath(path);
    if (!body.empty()) {
        req->setBody(body);
    }
    return req;
}

Json::Value createDepartmentJson(const std::string& name) {
    Json::Value departmentJson;
    departmentJson["name"] = name;
    return departmentJson;
}

Json::Value createPersonJson(const std::string& firstName, const std::string& lastName, int departmentId, int jobId) {
    Json::Value personJson;
    personJson["firstName"] = firstName;
    personJson["lastName"] = lastName;
    personJson["departmentId"] = departmentId;
    personJson["jobId"] = jobId;
    return personJson;
}

} // namespace

DROGON_TEST(DepartmentsControllerGet) {
    DepartmentsController controller;
    auto req = createHttpRequest("GET", "/departments");
    bool callbackCalled = false;
    controller.get(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerGetOne) {
    DepartmentsController controller;
    auto req = createHttpRequest("GET", "/departments/1");
    bool callbackCalled = false;
    controller.getOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerGetOneNotFound) {
    DepartmentsController controller;
    auto req = createHttpRequest("GET", "/departments/999");
    bool callbackCalled = false;
    controller.getOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerCreateOne) {
    DepartmentsController controller;
    Json::Value departmentJson = createDepartmentJson("Test Department");
    auto req = createHttpRequest("POST", "/departments", departmentJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.createOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerCreateOneInvalidInput) {
    DepartmentsController controller;
    auto req = createHttpRequest("POST", "/departments", "invalid json");
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.createOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerUpdateOne) {
    DepartmentsController controller;
    Json::Value departmentJson = createDepartmentJson("Updated Department Name");
    auto req = createHttpRequest("PUT", "/departments/1", departmentJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerUpdateOneNotFound) {
    DepartmentsController controller;
    Json::Value departmentJson = createDepartmentJson("Updated Department Name");
    auto req = createHttpRequest("PUT", "/departments/999", departmentJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerUpdateOneInvalidInput) {
    DepartmentsController controller;
    auto req = createHttpRequest("PUT", "/departments/1", "invalid json");
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerDeleteOne) {
    DepartmentsController controller;
    auto req = createHttpRequest("DELETE", "/departments/1");
    bool callbackCalled = false;
    controller.deleteOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerDeleteOneNotFound) {
    DepartmentsController controller;
    auto req = createHttpRequest("DELETE", "/departments/999");
    bool callbackCalled = false;
    controller.deleteOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerGetDepartmentPersons) {
    DepartmentsController controller;
    auto req = createHttpRequest("GET", "/departments/1/persons");
    bool callbackCalled = false;
    controller.getDepartmentPersons(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(DepartmentsControllerGetDepartmentPersonsNotFound) {
    DepartmentsController controller;
    auto req = createHttpRequest("GET", "/departments/999/persons");
    bool callbackCalled = false;
    controller.getDepartmentPersons(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerGet) {
    JobsController controller;
    auto req = createHttpRequest("GET", "/jobs");
    bool callbackCalled = false;
    controller.get(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerGetOne) {
    JobsController controller;
    auto req = createHttpRequest("GET", "/jobs/1");
    bool callbackCalled = false;
    controller.getOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerGetOneNotFound) {
    JobsController controller;
    auto req = createHttpRequest("GET", "/jobs/999");
    bool callbackCalled = false;
    controller.getOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerCreateOne) {
    JobsController controller;
    Json::Value jobJson;
    jobJson["title"] = "Test Job";
    jobJson["minSalary"] = 50000;
    jobJson["maxSalary"] = 100000;
    auto req = createHttpRequest("POST", "/jobs", jobJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.createOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerCreateOneInvalidInput) {
    JobsController controller;
    auto req = createHttpRequest("POST", "/jobs", "invalid json");
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.createOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerUpdateOne) {
    JobsController controller;
    Json::Value jobJson;
    jobJson["title"] = "Updated Job Title";
    jobJson["minSalary"] = 60000;
    jobJson["maxSalary"] = 120000;
    auto req = createHttpRequest("PUT", "/jobs/1", jobJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerUpdateOneNotFound) {
    JobsController controller;
    Json::Value jobJson;
    jobJson["title"] = "Updated Job Title";
    jobJson["minSalary"] = 60000;
    jobJson["maxSalary"] = 120000;
    auto req = createHttpRequest("PUT", "/jobs/999", jobJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerUpdateOneInvalidInput) {
    JobsController controller;
    auto req = createHttpRequest("PUT", "/jobs/1", "invalid json");
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerDeleteOne) {
    JobsController controller;
    auto req = createHttpRequest("DELETE", "/jobs/1");
    bool callbackCalled = false;
    controller.deleteOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(JobsControllerDeleteOneNotFound) {
    JobsController controller;
    auto req = createHttpRequest("DELETE", "/jobs/999");
    bool callbackCalled = false;
    controller.deleteOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerGet) {
    PersonsController controller;
    auto req = createHttpRequest("GET", "/persons");
    bool callbackCalled = false;
    controller.get(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerGetOne) {
    PersonsController controller;
    auto req = createHttpRequest("GET", "/persons/1");
    bool callbackCalled = false;
    controller.getOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerGetOneNotFound) {
    PersonsController controller;
    auto req = createHttpRequest("GET", "/persons/999");
    bool callbackCalled = false;
    controller.getOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerCreateOne) {
    PersonsController controller;
    Json::Value personJson = createPersonJson("John", "Doe", 1, 1);
    auto req = createHttpRequest("POST", "/persons", personJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.createOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerCreateOneInvalidInput) {
    PersonsController controller;
    auto req = createHttpRequest("POST", "/persons", "invalid json");
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.createOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    });
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerUpdateOne) {
    PersonsController controller;
    Json::Value personJson = createPersonJson("Jane", "Doe", 2, 2);
    auto req = createHttpRequest("PUT", "/persons/1", personJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k204NoContent);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerUpdateOneNotFound) {
    PersonsController controller;
    Json::Value personJson = createPersonJson("Jane", "Doe", 2, 2);
    auto req = createHttpRequest("PUT", "/persons/999", personJson.toStyledString());
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerUpdateOneInvalidInput) {
    PersonsController controller;
    auto req = createHttpRequest("PUT", "/persons/1", "invalid json");
    req->setContentType(ContentType::CT_APPLICATION_JSON);

    bool callbackCalled = false;
    controller.updateOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
    }, 1);
    CHECK(callbackCalled);
}

DROGON_TEST(PersonsControllerDeleteOne) {
    PersonsController controller;
    auto req = createHttpRequest("DELETE", "/persons/1");
    bool callbackCalled = false;
    controller.deleteOne(req, [&](
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../controllers/DepartmentsController.h"
#include "../../controllers/JobsController.h"
#include "../../controllers/PersonsController.h"
#include "../../models/Department.h"
#include "../../models/Job.h"
#include "../../models/Person.h"
#include "../../utils/utils.h"
#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <json/json.h>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace {

std::shared_ptr<HttpRequest> createHttpRequest(const std::string& method, const std::string& path, const std::string& body = "") {
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(method);
    req->setPath(path);
    if (!body.empty()) {
        req->setBody(body);
    }
    return req;
}

Json::Value createDepartmentJson(const std::string& name) {
    Json::Value departmentJson;
    departmentJson["name"] = name;
    return departmentJson;
}

Json::Value createPersonJson(const std::string& firstName, const std::string& lastName, int departmentId, int jobId) {
    Json::Value personJson;
    personJson["firstName"] = firstName;
    personJson["lastName"] = lastName;
    personJson["departmentId"] = departmentId;
    personJson["jobId"] = jobId;
    return personJson;
}

} // namespace

DROGON_TEST(PersonsControllerDeleteOneNotFound) {
    PersonsController controller;
    auto req = createHttpRequest("DELETE", "/persons/999");
    bool callbackCalled = false;
    controller.deleteOne(req, [&](const HttpResponsePtr& resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k404NotFound);
    }, 999);
    CHECK(callbackCalled);
}

DROGON_TEST(Utils_StringToMD5) {
    std::string input = "test string";
    std::string md5Hash = stringToMD5(input);
    CHECK(!md5Hash.empty());
}

DROGON_TEST(Utils_GenerateRandomString) {
    int length = 10;
    std::string randomString = generateRandomString(length);
    CHECK_EQ(randomString.length(), length);
}