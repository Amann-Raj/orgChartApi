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

    // Test constructor with Json::Value and masquerading vector
    Json::Value masqueradedJson;
    masqueradedJson["dept_id"] = 3;
    masqueradedJson["dept_name"] = "Masqueraded Department";
    std::vector<std::string> masqueradingVector = {"dept_id", "dept_name"};
    Department departmentFromMasqueradedJson(masqueradedJson, masqueradingVector);
    CHECK_EQ(departmentFromMasqueradedJson.getValueOfId(), 3);
    CHECK_EQ(departmentFromMasqueradedJson.getValueOfName(), "Masqueraded Department");

    // Test updateByMasqueradedJson
    Json::Value updateMasqueradedJson;
    updateMasqueradedJson["dept_name"] = "Updated Masqueraded Department Name";
    departmentFromMasqueradedJson.updateByMasqueradedJson(updateMasqueradedJson, masqueradingVector);
    CHECK_EQ(departmentFromMasqueradedJson.getValueOfName(), "Updated Masqueraded Department Name");

    // Test getters and setters
    department.setId(4);
    department.setName("New Department Name");
    CHECK_EQ(department.getValueOfId(), 4);
    CHECK_EQ(department.getValueOfName(), "New Department Name");
    CHECK_EQ(department.getPrimaryKey(), 4);

    // Test toMasqueradedJson
    Json::Value masqueradedDepartmentJson = department.toMasqueradedJson(masqueradingVector);
    CHECK_EQ(masqueradedDepartmentJson["dept_id"].asInt(), 4);
    CHECK_EQ(masqueradedDepartmentJson["dept_name"].asString(), "New Department Name");

    // Test insertColumns
    auto insertCols = Department::insertColumns();
    CHECK_EQ(insertCols.size(), 1);
    CHECK_EQ(insertCols[0], "name");

    // Test validateJsonForCreation - failure (missing name)
    Json::Value invalidCreateJson;
    std::string err;
    invalidCreateJson["id"] = 1;
    bool isValid = Department::validateJsonForCreation(invalidCreateJson, err);
    CHECK_EQ(isValid, false);
    CHECK_EQ(err, "The name column cannot be null");

    // Test validateJsonForCreation - success
    Json::Value validCreateJson;
    validCreateJson["name"] = "Valid Department";
    err = "";
    isValid = Department::validateJsonForCreation(validCreateJson, err);
    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");

    // Test validateMasqueradedJsonForUpdate - failure (missing id)
    Json::Value invalidMasqueradedUpdateJson;
    invalidMasqueradedUpdateJson["dept_name"] = "Updated Name";
    err = "";
    isValid = Department::validateMasqueradedJsonForUpdate(invalidMasqueradedUpdateJson, masqueradingVector, err);
    CHECK_EQ(isValid, false);
    CHECK_EQ(err, "The value of primary key must be set in the json object for update");

    // Test validateMasqueradedJsonForUpdate - success
    Json::Value validMasqueradedUpdateJson;
    validMasqueradedUpdateJson["dept_id"] = 1;
    validMasqueradedUpdateJson["dept_name"] = "Updated Name";
    err = "";
    isValid = Department::validateMasqueradedJsonForUpdate(validMasqueradedUpdateJson, masqueradingVector, err);
    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");

    // Test validJsonOfField - failure (name - too long)
    Json::Value longNameJson;
    std::string longName(51, 'A');
    longNameJson = longName;
    err = "";
    CHECK_EQ(Department::validJsonOfField(1, "name", longNameJson, err, true), false);
    CHECK_EQ(err, "String length exceeds limit for the name field (the maximum value is 50)");

    // Test validJsonOfField - success (name - valid length)
    Json::Value validNameJson;
    std::string validName(50, 'A');
    validNameJson = validName;
    err = "";
    CHECK_EQ(Department::validJsonOfField(1, "name", validNameJson, err, true), true);
    CHECK_EQ(err, "");

    // Test validJsonOfField - failure (id - not int)
    Json::Value stringIdJson;
    stringIdJson = "not an int";
    err = "";
    CHECK_EQ(Department::validJsonOfField(0, "id", stringIdJson, err, false), false);
    CHECK_EQ(err, "Type error in the id field");

    // Test validJsonOfField - success (id - int)
    Json::Value intIdJson;
    intIdJson = 123;
    err = "";
    CHECK_EQ(Department::validJsonOfField(0, "id", intIdJson, err, false), true);
    CHECK_EQ(err, "");

    // Test outputArgs
    drogon::orm::internal::SqlBinder binder;
    department.outputArgs(binder);
    // No direct way to check binder content without accessing private members.
    // This test primarily ensures that the function executes without crashing.

    // Test setRow
    Row newRow;
    newRow["id"] = 5;
    newRow["name"] = "Another Department";
    department.setRow(newRow);
    CHECK_EQ(department.getValueOfId(), 5);
    CHECK_EQ(department.getValueOfName(), "Another Department");

    // Test isColumnNullable
    CHECK_EQ(Department::isColumnNullable("id"), false);
    CHECK_EQ(Department::isColumnNullable("name"), false);
    CHECK_EQ(Department::isColumnNullable("nonexistent_column"), false); //Should return false for non-existent columns
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/Department.h"
#include "../../models/Person.h"
#include <memory>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(DepartmentModelTestExtra)
{
    // Test constructor with Json::Value and default masquerading vector
    Json::Value jsonValue;
    jsonValue["id"] = 6;
    jsonValue["name"] = "Default Masquerading Department";
    Department departmentFromDefaultMasqueradedJson(jsonValue);
    CHECK_EQ(departmentFromDefaultMasqueradedJson.getValueOfId(), 6);
    CHECK_EQ(departmentFromDefaultMasqueradedJson.getValueOfName(), "Default Masquerading Department");

    // Test updateByJson
    Department departmentForUpdate;
    departmentForUpdate.setId(7);
    departmentForUpdate.setName("Original Name");
    Json::Value updateJson;
    updateJson["name"] = "Updated Name via Json";
    departmentForUpdate.updateByJson(updateJson);
    CHECK_EQ(departmentForUpdate.getValueOfName(), "Updated Name via Json");

    // Test toMasqueradedJson with empty masquerading vector
    Department departmentForMasquerade;
    departmentForMasquerade.setId(8);
    departmentForMasquerade.setName("Department For Empty Masquerade");
    std::vector<std::string> emptyMasqueradingVector;
    Json::Value emptyMasqueradedJson = departmentForMasquerade.toMasqueradedJson(emptyMasqueradingVector);
    CHECK(emptyMasqueradedJson.empty());

    // Test validateJsonForUpdate - failure (missing primary key)
    Json::Value invalidUpdateJson;
    invalidUpdateJson["name"] = "Updated Name";
    std::string err;
    bool isValid = Department::validateJsonForUpdate(invalidUpdateJson, err);
    CHECK_EQ(isValid, false);
    CHECK_EQ(err, "The value of primary key must be set in the json object for update");

    // Test validateJsonForUpdate - success
    Json::Value validUpdateJson;
    validUpdateJson["id"] = 9;
    validUpdateJson["name"] = "Updated Name";
    err = "";
    isValid = Department::validateJsonForUpdate(validUpdateJson, err);
    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");

    // Test validJsonOfField - failure (name - empty string, not nullable)
    Json::Value emptyNameJson;
    emptyNameJson = "";
    err = "";
    CHECK_EQ(Department::validJsonOfField(1, "name", emptyNameJson, err, true), false);
    CHECK_EQ(err, "The name column cannot be empty");

    // Test validJsonOfField - success (name - empty string, nullable = false)
    Json::Value emptyNameJson2;
    emptyNameJson2 = "";
    err = "";
    CHECK_EQ(Department::validJsonOfField(1, "name", emptyNameJson2, err, false), true);
    CHECK_EQ(err, "");

    // Test validJsonOfField - invalid column name
    Json::Value someValue;
    someValue = 123;
    err = "";
    CHECK_EQ(Department::validJsonOfField(0, "invalid_column", someValue, err, false), true);
    CHECK_EQ(err, "");

    // Test updateByMasqueradedJson with empty masquerading vector
    Department departmentForEmptyMasqUpdate;
    departmentForEmptyMasqUpdate.setId(10);
    departmentForEmptyMasqUpdate.setName("Original Name");
    Json::Value updateMasqueradedJson;
    updateMasqueradedJson["new_name"] = "Updated Name via Masqueraded Json";
    departmentForEmptyMasqUpdate.updateByMasqueradedJson(updateMasqueradedJson, emptyMasqueradingVector);
    CHECK_EQ(departmentForEmptyMasqUpdate.getValueOfName(), "Original Name");

    // Test validateMasqueradedJsonForCreation - success
    Json::Value validMasqueradedCreateJson;
    validMasqueradedCreateJson["dept_name"] = "Valid Department";
    std::vector<std::string> masqueradingVector = {"dept_name"};
    err = "";
    isValid = Department::validateMasqueradedJsonForCreation(validMasqueradedCreateJson, masqueradingVector, err);
    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");

    // Test validateMasqueradedJsonForCreation - failure (missing name)
    Json::Value invalidMasqueradedCreateJson;
    std::vector<std::string> masqueradingVector2 = {"dept_name"};
    std::string err2;
    bool isValid2 = Department::validateMasqueradedJsonForCreation(invalidMasqueradedCreateJson, masqueradingVector2, err2);
    CHECK_EQ(isValid2, false);
    CHECK_EQ(err2, "The dept_name column cannot be null");
}