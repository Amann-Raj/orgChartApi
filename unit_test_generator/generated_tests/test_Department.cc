#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/Department.h"
#include "../../models/Person.h"
#include <memory>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(DepartmentModelTest)
{
    // Test default constructor
    Department department;
    CHECK(!department.getId());
    CHECK(!department.getName());

    // Test constructor with Row
    Row row;
    row["id"] = 1;
    row["name"] = "Test Department";
    Department departmentFromRow(row);
    CHECK_EQ(departmentFromRow.getValueOfId(), 1);
    CHECK_EQ(departmentFromRow.getValueOfName(), "Test Department");

    // Test constructor with Json::Value
    Json::Value json;
    json["id"] = 2;
    json["name"] = "Another Department";
    Department departmentFromJson(json);
    CHECK_EQ(departmentFromJson.getValueOfId(), 2);
    CHECK_EQ(departmentFromJson.getValueOfName(), "Another Department");

    // Test updateByJson
    Json::Value updateJson;
    updateJson["name"] = "Updated Department Name";
    departmentFromJson.updateByJson(updateJson);
    CHECK_EQ(departmentFromJson.getValueOfName(), "Updated Department Name");

    // Test getters and setters
    department.setId(4);
    department.setName("New Department Name");
    CHECK_EQ(department.getValueOfId(), 4);
    CHECK_EQ(department.getValueOfName(), "New Department Name");

    // Test toJson
    Json::Value departmentJson = department.toJson();
    CHECK_EQ(departmentJson["id"].asInt(), 4);
    CHECK_EQ(departmentJson["name"].asString(), "New Department Name");

    // Test insertColumns
    auto insertCols = Department::insertColumns();
    CHECK_EQ(insertCols.size(), 1);
    CHECK_EQ(insertCols[0], "name");

    // Test updateColumns (dirty flag needs to be set)
    department.setName("Another New Name");
    auto updateCols = department.updateColumns();
    CHECK_EQ(updateCols.size(), 1);
    CHECK_EQ(updateCols[0], "name");

    // Test validateJsonForCreation - success case
    Json::Value validCreateJson;
    validCreateJson["name"] = "Valid Department";
    std::string err;
    CHECK(Department::validateJsonForCreation(validCreateJson, err));

    // Test validateJsonForCreation - failure case (missing name)
    Json::Value invalidCreateJson;
    err = "";
    CHECK_FALSE(Department::validateJsonForCreation(invalidCreateJson, err));
    CHECK(!err.empty());

    // Test validateJsonForUpdate - failure case (missing id)
    Json::Value invalidUpdateJson;
    invalidUpdateJson["name"] = "Updated Invalid Department";
    err = "";
    CHECK_FALSE(Department::validateJsonForUpdate(invalidUpdateJson, err));
    CHECK(!err.empty());

    // Test validJsonOfField - valid cases
    Json::Value validIdJson = 7;
    Json::Value validNameJson = "Valid Name";
    err = "";
    CHECK(Department::validJsonOfField(0, "id", validIdJson, err, false));
    CHECK(Department::validJsonOfField(1, "name", validNameJson, err, true));

    // Test validJsonOfField - invalid cases
    Json::Value invalidIdJson = "invalid";
    Json::Value invalidNameJson = 123;
    err = "";
    CHECK_FALSE(Department::validJsonOfField(0, "id", invalidIdJson, err, false));
    CHECK_FALSE(Department::validJsonOfField(1, "name", invalidNameJson, err, true));

    // Test setName with move semantics
    std::string movedName = "Moved Name";
    department.setName(std::move(movedName));
    CHECK_EQ(department.getValueOfName(), "Moved Name");

    // Test outputArgs and updateArgs (cannot directly test output, but can ensure they don't crash)
    drogon::orm::internal::SqlBinder binder;
    department.outputArgs(binder);
    department.updateArgs(binder);
}