#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/Person.h"
#include "../../models/Department.h"
#include "../../models/Job.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace drogon_model::org_chart;

DROGON_TEST(PersonModelConstructor)
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
}

DROGON_TEST(PersonModelRowConstructor)
{
    // Mock a Row object
    drogon::orm::Row row;
    row["id"] = 1;
    row["job_id"] = 2;
    row["department_id"] = 3;
    row["manager_id"] = 4;
    row["first_name"] = "John";
    row["last_name"] = "Doe";
    row["hire_date"] = "2023-10-26";

    // Construct Person object from the Row
    Person person(row);

    // Verify data
    CHECK_EQ(*person.getId(), 1);
    CHECK_EQ(*person.getJobId(), 2);
    CHECK_EQ(*person.getDepartmentId(), 3);
    CHECK_EQ(*person.getManagerId(), 4);
    CHECK_EQ(*person.getFirstName(), "John");
    CHECK_EQ(*person.getLastName(), "Doe");
    CHECK(person.getHireDate() != nullptr);
}

DROGON_TEST(PersonModelGetterSetter)
{
    // Create a Person object
    Person person;

    // Set values using setters
    person.setId(1);
    person.setJobId(2);
    person.setDepartmentId(3);
    person.setManagerId(4);
    person.setFirstName("Alice");
    person.setLastName("Johnson");
    person.setHireDate(trantor::Date(2023, 10, 28));

    // Verify values using getters
    CHECK_EQ(person.getValueOfId(), 1);
    CHECK_EQ(person.getValueOfJobId(), 2);
    CHECK_EQ(person.getValueOfDepartmentId(), 3);
    CHECK_EQ(person.getValueOfManagerId(), 4);
    CHECK_EQ(person.getValueOfFirstName(), "Alice");
    CHECK_EQ(person.getValueOfLastName(), "Johnson");
    CHECK_EQ(person.getValueOfHireDate().year(), 2023);
    CHECK_EQ(person.getValueOfHireDate().month(), 10);
    CHECK_EQ(person.getValueOfHireDate().day(), 28);
}

DROGON_TEST(PersonModelUpdateByJson)
{
    // Create a Person object
    Person person;
    person.setId(1); // Set the primary key for update

    // Create a Json::Value object with updated values
    Json::Value json;
    json["job_id"] = 5;
    json["first_name"] = "UpdatedFirstName";

    // Update the Person object using the JSON
    person.updateByJson(json);

    // Verify that the values have been updated
    CHECK_EQ(person.getValueOfJobId(), 5);
    CHECK_EQ(person.getValueOfFirstName(), "UpdatedFirstName");
}

DROGON_TEST(PersonModelToJson)
{
    // Create a Person object and set its values
    Person person;
    person.setId(1);
    person.setJobId(2);
    person.setDepartmentId(3);
    person.setManagerId(4);
    person.setFirstName("Bob");
    person.setLastName("Williams");
    person.setHireDate(trantor::Date(2023, 10, 29));

    // Convert the Person object to JSON
    Json::Value json = person.toJson();

    // Verify the JSON values
    CHECK_EQ(json["id"].asInt(), 1);
    CHECK_EQ(json["job_id"].asInt(), 2);
    CHECK_EQ(json["department_id"].asInt(), 3);
    CHECK_EQ(json["manager_id"].asInt(), 4);
    CHECK_EQ(json["first_name"].asString(), "Bob");
    CHECK_EQ(json["last_name"].asString(), "Williams");
    CHECK_EQ(json["hire_date"].asString(), "2023-10-29");
}

DROGON_TEST(PersonModelValidateJsonForCreation)
{
    Json::Value json;
    json["job_id"] = 2;
    json["department_id"] = 3;
    json["manager_id"] = 4;
    json["first_name"] = "ValidFirstName";
    json["last_name"] = "ValidLastName";
    json["hire_date"] = "2023-10-27";

    std::string err;
    bool isValid = Person::validateJsonForCreation(json, err);

    CHECK(isValid);

    json.removeMember("job_id");
    isValid = Person::validateJsonForCreation(json, err);
    CHECK(!isValid);
}

DROGON_TEST(PersonModelValidateJsonForUpdate)
{
    Json::Value json;
    json["id"] = 1; // Primary key is required for update
    json["first_name"] = "UpdatedFirstName";

    std::string err;
    bool isValid = Person::validateJsonForUpdate(json, err);

    CHECK(isValid);

    json.removeMember("id");
    isValid = Person::validateJsonForUpdate(json, err);
    CHECK(!isValid);
}

DROGON_TEST(PersonModelStringLengthValidation) {
    Json::Value json;
    json["job_id"] = 2;
    json["department_id"] = 3;
    json["manager_id"] = 4;
    json["first_name"] = "ThisFirstNameIsTooLongExceedingTheMaximumAllowedLength";
    json["last_name"] = "ValidLastName";
    json["hire_date"] = "2023-10-27";

    std::string err;
    bool isValid = Person::validateJsonForCreation(json, err);

    CHECK(!isValid);
    CHECK(!err.empty());
}

DROGON_TEST(PersonModelMasqueradedJson) {
    Json::Value json;
    json["person_id"] = 1;
    json["person_job_id"] = 2;
    json["person_department_id"] = 3;
    json["person_manager_id"] = 4;
    json["person_first_name"] = "Jane";
    json["person_last_name"] = "Smith";
    json["person_hire_date"] = "2023-10-27";

    std::vector<std::string> masqueradingVector = {
        "person_id", "person_job_id", "person_department_id",
        "person_manager_id", "person_first_name", "person_last_name",
        "person_hire_date"
    };

    Person person(json, masqueradingVector);

    CHECK_EQ(*person.getId(), 1);
    CHECK_EQ(*person.getJobId(), 2);
    CHECK_EQ(*person.getDepartmentId(), 3);
    CHECK_EQ(*person.getManagerId(), 4);
    CHECK_EQ(*person.getFirstName(), "Jane");
    CHECK_EQ(*person.getLastName(), "Smith");
    CHECK(person.getHireDate() != nullptr);

    Json::Value masqueradedJson = person.toMasqueradedJson(masqueradingVector);

    CHECK_EQ(masqueradedJson["person_id"].asInt(), 1);
    CHECK_EQ(masqueradedJson["person_job_id"].asInt(), 2);
    CHECK_EQ(masqueradedJson["person_department_id"].asInt(), 3);
    CHECK_EQ(masqueradedJson["person_manager_id"].asInt(), 4);
    CHECK_EQ(masqueradedJson["person_first_name"].asString(), "Jane");
    CHECK_EQ(masqueradedJson["person_last_name"].asString(), "Smith");
    CHECK_EQ(masqueradedJson["person_hire_date"].asString(), "2023-10-27");

    person.updateByMasqueradedJson(json, masqueradingVector);
    CHECK_EQ(*person.getJobId(), 2);
    CHECK_EQ(*person.getFirstName(), "Jane");
}

DROGON_TEST(PersonModelValidateMasqueradedJson) {
    Json::Value json;
    json["person_job_id"] = 2;
    json["person_department_id"] = 3;
    json["person_manager_id"] = 4;
    json["person_first_name"] = "ValidFirstName";
    json["person_last_name"] = "ValidLastName";
    json["person_hire_date"] = "2023-10-27";

    std::vector<std::string> masqueradingVector = {
        "person_id", "person_job_id", "person_department_id",
        "person_manager_id", "person_first_name", "person_last_name",
        "person_hire_date"
    };

    std::string err;
    bool isValid = Person::validateMasqueradedJsonForCreation(json, masqueradingVector, err);
    CHECK(isValid);

    json.removeMember("person_job_id");
    isValid = Person::validateMasqueradedJsonForCreation(json, masqueradingVector, err);
    CHECK(!isValid);

    json["person_id"] = 1;
    json["person_first_name"] = "UpdatedFirstName";
    isValid = Person::validateMasqueradedJsonForUpdate(json, masqueradingVector, err);
    CHECK(isValid);

    json.removeMember("person_id");
    isValid = Person::validateMasqueradedJsonForUpdate(json, masqueradingVector, err);
    CHECK(!isValid);

    json["person_job_id"] = 2;
    json["person_department_id"] = 3;
    json["person_manager_id"] = 4;
    json["person_first_name"] = "ThisFirstNameIsTooLongExceedingTheMaximumAllowedLength";
    json["person_last_name"] = "ValidLastName";
    json["person_hire_date"] = "2023-10-27";
    isValid = Person::validateMasqueradedJsonForCreation(json, masqueradingVector, err);
    CHECK(!isValid);
}

DROGON_TEST(PersonModelColumns) {
    const auto& insert_columns = Person::insertColumns();
    CHECK_EQ(insert_columns.size(), 6);
    CHECK_EQ(insert_columns[0], "job_id");
    CHECK_EQ(insert_columns[1], "department_id");
    CHECK_EQ(insert_columns[2], "manager_id");
    CHECK_EQ(insert_columns[3], "first_name");
    CHECK_EQ(insert_columns[4], "last_name");
    CHECK_EQ(insert_columns[5], "hire_date");

    Person person;
    person.setJobId(2);
    person.setDepartmentId(3);
    person.setManagerId(4);
    person.setFirstName("Bob");
    person.setLastName("Williams");
    person.setHireDate(trantor::Date(2023, 10, 29));

    const auto& update_columns = person.updateColumns();
    CHECK_EQ(update_columns.size(), 6);
    CHECK_EQ(update_columns[0], "job_id");
    CHECK_EQ(update_columns[1], "department_id");
    CHECK_EQ(update_columns[2], "manager_id");
    CHECK_EQ(update_columns[3], "first_name");
    CHECK_EQ(update_columns[4], "last_name");
    CHECK_EQ(update_columns[5], "hire_date");
}