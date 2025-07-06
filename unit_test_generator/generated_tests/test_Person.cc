#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/Person.h"
#include <iostream>
#include <string>
#include <vector>

using namespace drogon_model::org_chart;

DROGON_TEST(PersonModelTest)
{
    // Test default constructor
    Person person;
    CHECK(!person.getId());
    CHECK(!person.getJobId());
    CHECK(!person.getDepartmentId());
    CHECK(!person.getManagerId());
    CHECK(!person.getFirstName());
    CHECK(!person.getLastName());
    CHECK(!person.getHireDate());

    // Test constructor with Row
    drogon::orm::Row row;
    row["id"] = 1;
    row["job_id"] = 2;
    row["department_id"] = 3;
    row["manager_id"] = 4;
    row["first_name"] = "John";
    row["last_name"] = "Doe";
    row["hire_date"] = "2023-10-26";

    Person personFromRow(row);
    CHECK_EQ(personFromRow.getValueOfId(), 1);
    CHECK_EQ(personFromRow.getValueOfJobId(), 2);
    CHECK_EQ(personFromRow.getValueOfDepartmentId(), 3);
    CHECK_EQ(personFromRow.getValueOfManagerId(), 4);
    CHECK_EQ(personFromRow.getValueOfFirstName(), "John");
    CHECK_EQ(personFromRow.getValueOfLastName(), "Doe");
    CHECK_EQ(personFromRow.getValueOfHireDate().toDbStringLocal(), "2023-10-26");

    // Test setters and getters
    person.setId(9);
    person.setJobId(10);
    person.setDepartmentId(11);
    person.setManagerId(12);
    person.setFirstName("Alice");
    person.setLastName("Johnson");
    person.setHireDate(trantor::Date(2023, 10, 28));

    CHECK_EQ(person.getValueOfId(), 9);
    CHECK_EQ(person.getValueOfJobId(), 10);
    CHECK_EQ(person.getValueOfDepartmentId(), 11);
    CHECK_EQ(person.getValueOfManagerId(), 12);
    CHECK_EQ(person.getValueOfFirstName(), "Alice");
    CHECK_EQ(person.getValueOfLastName(), "Johnson");
    CHECK_EQ(person.getValueOfHireDate().toDbStringLocal(), "2023-10-28");

    // Test toJson() method
    Json::Value personJson = person.toJson();
    CHECK_EQ(personJson["id"].asInt(), 9);
    CHECK_EQ(personJson["job_id"].asInt(), 10);
    CHECK_EQ(personJson["department_id"].asInt(), 11);
    CHECK_EQ(personJson["manager_id"].asInt(), 12);
    CHECK_EQ(personJson["first_name"].asString(), "Alice");
    CHECK_EQ(personJson["last_name"].asString(), "Johnson");
    CHECK_EQ(personJson["hire_date"].asString(), "2023-10-28");

    // Test toMasqueradedJson() method
    std::vector<std::string> masqueradingVector = {"masked_id", "masked_job_id", "masked_department_id", "masked_manager_id", "masked_first_name", "masked_last_name", "masked_hire_date"};
    Json::Value maskedPersonJson = person.toMasqueradedJson(masqueradingVector);
     CHECK_EQ(maskedPersonJson["masked_id"].asInt(), 9);
    CHECK_EQ(maskedPersonJson["masked_job_id"].asInt(), 10);
    CHECK_EQ(maskedPersonJson["masked_department_id"].asInt(), 11);
    CHECK_EQ(maskedPersonJson["masked_manager_id"].asInt(), 12);
    CHECK_EQ(maskedPersonJson["masked_first_name"].asString(), "Alice");
    CHECK_EQ(maskedPersonJson["masked_last_name"].asString(), "Johnson");
    CHECK_EQ(maskedPersonJson["masked_hire_date"].asString(), "2023-10-28");

    // Test updateByJson() method
    Json::Value updateJson;
    updateJson["first_name"] = "UpdatedFirstName";
    updateJson["last_name"] = "UpdatedLastName";
    person.updateByJson(updateJson);
    CHECK_EQ(person.getValueOfFirstName(), "UpdatedFirstName");
    CHECK_EQ(person.getValueOfLastName(), "UpdatedLastName");

    // Test insertColumns() method
    auto insertCols = Person::insertColumns();
    CHECK_EQ(insertCols.size(), 6);
    CHECK_EQ(insertCols[0], "job_id");
    CHECK_EQ(insertCols[1], "department_id");
    CHECK_EQ(insertCols[2], "manager_id");
    CHECK_EQ(insertCols[3], "first_name");
    CHECK_EQ(insertCols[4], "last_name");
    CHECK_EQ(insertCols[5], "hire_date");

    // Test updateColumns() method
    person.setFirstName("NewFirstName");
    auto updateCols = person.updateColumns();
    CHECK(std::find(updateCols.begin(), updateCols.end(), "first_name") != updateCols.end());

    // Test validateJsonForCreation() method
    Json::Value validCreateJson;
    validCreateJson["job_id"] = 1;
    validCreateJson["department_id"] = 2;
    validCreateJson["manager_id"] = 3;
    validCreateJson["first_name"] = "ValidFirstName";
    validCreateJson["last_name"] = "ValidLastName";
    validCreateJson["hire_date"] = "2023-10-29";
    std::string err;
    CHECK(Person::validateJsonForCreation(validCreateJson, err));

    Json::Value invalidCreateJson;
    invalidCreateJson["job_id"] = 1;
    invalidCreateJson["department_id"] = 2;
    invalidCreateJson["manager_id"] = 3;
    invalidCreateJson["first_name"] = "ThisFirstNameIsTooLongExceedingTheMaximumLimitOfFiftyCharacters";
    invalidCreateJson["last_name"] = "ValidLastName";
    invalidCreateJson["hire_date"] = "2023-10-29";
    CHECK(!Person::validateJsonForCreation(invalidCreateJson, err));

    // Test validateJsonForUpdate() method
    Json::Value validUpdateJson;
    validUpdateJson["id"] = 1;
    validUpdateJson["first_name"] = "UpdatedFirstName";
    CHECK(Person::validateJsonForUpdate(validUpdateJson, err));

    Json::Value invalidUpdateJson;
    invalidUpdateJson["id"] = 1;
    invalidUpdateJson["first_name"] = "ThisFirstNameIsTooLongExceedingTheMaximumLimitOfFiftyCharacters";
    CHECK(!Person::validateJsonForUpdate(invalidUpdateJson, err));

    // Test validateJsonForCreation() with missing fields
    Json::Value missingFieldJson;
    missingFieldJson["job_id"] = 1;
    missingFieldJson["department_id"] = 2;
    missingFieldJson["first_name"] = "ValidFirstName";
    missingFieldJson["last_name"] = "ValidLastName";
    missingFieldJson["hire_date"] = "2023-10-29";
    CHECK(!Person::validateJsonForCreation(missingFieldJson, err));

    // Test validateJsonForUpdate() with invalid id
    Json::Value invalidIdJson;
    invalidIdJson["id"] = "invalid";
    invalidIdJson["first_name"] = "UpdatedFirstName";
    CHECK(!Person::validateJsonForUpdate(invalidIdJson, err));

    // Test validateJsonForCreation() with empty strings
    Json::Value emptyStringJson;
    emptyStringJson["job_id"] = 1;
    emptyStringJson["department_id"] = 2;
    emptyStringJson["manager_id"] = 3;
    emptyStringJson["first_name"] = "";
    emptyStringJson["last_name"] = "";
    emptyStringJson["hire_date"] = "2023-10-29";
    CHECK(Person::validateJsonForCreation(emptyStringJson, err));

    // Test validateJsonForUpdate() with empty strings
    Json::Value emptyStringUpdateJson;
    emptyStringUpdateJson["id"] = 1;
    emptyStringUpdateJson["first_name"] = "";
    CHECK(Person::validateJsonForUpdate(emptyStringUpdateJson, err));

    // Test validateJsonForCreation() with null hire_date
    Json::Value nullHireDateJson;
    nullHireDateJson["job_id"] = 1;
    nullHireDateJson["department_id"] = 2;
    nullHireDateJson["manager_id"] = 3;
    nullHireDateJson["first_name"] = "ValidFirstName";
    nullHireDateJson["last_name"] = "ValidLastName";
    CHECK(Person::validateJsonForCreation(nullHireDateJson, err));

    // Test validateJsonForUpdate() with null hire_date
    Json::Value nullHireDateUpdateJson;
    nullHireDateUpdateJson["id"] = 1;
    nullHireDateUpdateJson["hire_date"] = nullptr;
    CHECK(Person::validateJsonForUpdate(nullHireDateUpdateJson, err));
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/Person.h"
#include <iostream>
#include <string>
#include <vector>

using namespace drogon_model::org_chart;

DROGON_TEST(PersonModelTestExtra)
{
    // Test constructor with Json::Value
    Json::Value json;
    json["id"] = 5;
    json["job_id"] = 6;
    json["department_id"] = 7;
    json["manager_id"] = 8;
    json["first_name"] = "Eve";
    json["last_name"] = "Smith";
    json["hire_date"] = "2023-11-01";

    Person personFromJson(json);
    CHECK_EQ(personFromJson.getValueOfId(), 5);
    CHECK_EQ(personFromJson.getValueOfJobId(), 6);
    CHECK_EQ(personFromJson.getValueOfDepartmentId(), 7);
    CHECK_EQ(personFromJson.getValueOfManagerId(), 8);
    CHECK_EQ(personFromJson.getValueOfFirstName(), "Eve");
    CHECK_EQ(personFromJson.getValueOfLastName(), "Smith");
    CHECK_EQ(personFromJson.getValueOfHireDate().toDbStringLocal(), "2023-11-01");

    // Test updateByJson() with empty Json
    Person personForUpdate;
    Json::Value emptyJson;
    personForUpdate.updateByJson(emptyJson);
    CHECK(!personForUpdate.getId());

    // Test insertColumns() method when no columns are specified
    Person personForInsertCols;
    auto insertColsEmpty = personForInsertCols.insertColumns(false);
    CHECK_EQ(insertColsEmpty.size(), 0);

    // Test updateColumns() method when no columns are specified
    Person personForUpdateCols;
    auto updateColsEmpty = personForUpdateCols.updateColumns(false);
    CHECK_EQ(updateColsEmpty.size(), 0);

    // Test validateJsonForCreation() with invalid data types
    Json::Value invalidDataTypeJson;
    invalidDataTypeJson["job_id"] = "invalid";
    invalidDataTypeJson["department_id"] = "invalid";
    invalidDataTypeJson["manager_id"] = "invalid";
    invalidDataTypeJson["first_name"] = 123;
    invalidDataTypeJson["last_name"] = 456;
    invalidDataTypeJson["hire_date"] = 789;
    std::string err;
    CHECK(!Person::validateJsonForCreation(invalidDataTypeJson, err));

    // Test validateJsonForUpdate() with invalid data types
    Json::Value invalidDataTypeUpdateJson;
    invalidDataTypeUpdateJson["id"] = "invalid";
    invalidDataTypeUpdateJson["first_name"] = 123;
    std::string errUpdate;
    CHECK(!Person::validateJsonForUpdate(invalidDataTypeUpdateJson, errUpdate));

    // Test validateJsonForCreation() with hire_date as null
    Json::Value nullHireDateCreateJson;
    nullHireDateCreateJson["job_id"] = 1;
    nullHireDateCreateJson["department_id"] = 2;
    nullHireDateCreateJson["manager_id"] = 3;
    nullHireDateCreateJson["first_name"] = "ValidFirstName";
    nullHireDateCreateJson["last_name"] = "ValidLastName";
    nullHireDateCreateJson["hire_date"] = Json::nullValue;
    std::string errNullHireDateCreate;
    CHECK(Person::validateJsonForCreation(nullHireDateCreateJson, errNullHireDateCreate));

    // Test validateJsonForUpdate() with hire_date as null
    Json::Value nullHireDateUpdateJson;
    nullHireDateUpdateJson["id"] = 1;
    nullHireDateUpdateJson["hire_date"] = Json::nullValue;
    std::string errNullHireDateUpdate;
    CHECK(Person::validateJsonForUpdate(nullHireDateUpdateJson, errNullHireDateUpdate));
}