#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "model/Job.h"
#include "model/Person.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace drogon_model::org_chart;

DROGON_TEST(JobModelTest)
{
    // Test default constructor
    Job job;
    CHECK(!job.getId());
    CHECK(!job.getTitle());

    // Test constructor with Json::Value
    Json::Value json;
    json["id"] = 1;
    json["title"] = "Software Engineer";
    Job job2(json);
    CHECK_EQ(job2.getValueOfId(), 1);
    CHECK_EQ(job2.getValueOfTitle(), "Software Engineer");

    // Test setters and getters
    job.setId(3);
    job.setTitle("Data Scientist");
    CHECK_EQ(job.getValueOfId(), 3);
    CHECK_EQ(job.getValueOfTitle(), "Data Scientist");

    // Test updateByJson
    Json::Value updateJson;
    updateJson["title"] = "Senior Data Scientist";
    job.updateByJson(updateJson);
    CHECK_EQ(job.getValueOfTitle(), "Senior Data Scientist");

    // Test toJson
    Json::Value jobJson = job.toJson();
    CHECK_EQ(jobJson["id"].asInt(), 3);
    CHECK_EQ(jobJson["title"].asString(), "Senior Data Scientist");

    // Test insertColumns
    auto insertCols = Job::insertColumns();
    CHECK_EQ(insertCols.size(), 1);
    CHECK_EQ(insertCols[0], "title");

    // Test updateColumns
    auto updateCols = job.updateColumns();
    CHECK_EQ(updateCols.size(), 1);
    CHECK_EQ(updateCols[0], "title");

    // Test validateJsonForCreation - success and failure case
    Json::Value createJson;
    createJson["title"] = "New Job Title";
    std::string err;
    CHECK(Job::validateJsonForCreation(createJson, err));

    Json::Value createJsonMissingTitle;
    err = "";
    CHECK(!Job::validateJsonForCreation(createJsonMissingTitle, err));
    CHECK_EQ(err, "The title column cannot be null");

    // Test validateJsonForUpdate - success and failure cases
    Json::Value updateJsonValid;
    updateJsonValid["id"] = 4;
    updateJsonValid["title"] = "Updated Job Title";
    err = "";
    CHECK(Job::validateJsonForUpdate(updateJsonValid, err));

    Json::Value updateJsonMissingId;
    updateJsonMissingId["title"] = "Updated Job Title";
    err = "";
    CHECK(!Job::validateJsonForUpdate(updateJsonMissingId, err));
    CHECK_EQ(err, "The value of primary key must be set in the json object for update");

    // Test validateJsonForUpdate - failure case (invalid title length)
    Json::Value updateJsonInvalidTitle;
    updateJsonInvalidTitle["id"] = 5;
    std::string longTitle(51, 'A');
    updateJsonInvalidTitle["title"] = longTitle;
    err = "";
    CHECK(!Job::validateJsonForUpdate(updateJsonInvalidTitle, err));
    CHECK_EQ(err, "String length exceeds limit for the title field (the maximum value is 50)");

    // Test masquerading
    std::vector<std::string> masqueradingVector = {"job_id", "job_title"};
    Json::Value masqueradedJson;
    masqueradedJson["job_id"] = 6;
    masqueradedJson["job_title"] = "Masqueraded Title";
    Job job4(masqueradedJson, masqueradingVector);
    CHECK_EQ(job4.getValueOfId(), 6);
    CHECK_EQ(job4.getValueOfTitle(), "Masqueraded Title");

    Json::Value masqueradedJsonOutput = job4.toMasqueradedJson(masqueradingVector);
    CHECK_EQ(masqueradedJsonOutput["job_id"].asInt(), 6);
    CHECK_EQ(masqueradedJsonOutput["job_title"].asString(), "Masqueraded Title");

    // Test validateMasqueradedJsonForCreation - success and failure case
    Json::Value createMasqueradedJson;
    createMasqueradedJson["job_title"] = "New Masqueraded Job Title";
    err = "";
    CHECK(Job::validateMasqueradedJsonForCreation(createMasqueradedJson, masqueradingVector, err));

    Json::Value createMasqueradedJsonMissingTitle;
    err = "";
    CHECK(!Job::validateMasqueradedJsonForCreation(createMasqueradedJsonMissingTitle, masqueradingVector, err));
    CHECK_EQ(err, "The job_title column cannot be null");

    // Test validateMasqueradedJsonForUpdate - success and failure cases
    Json::Value updateMasqueradedJsonValid;
    updateMasqueradedJsonValid["job_id"] = 7;
    updateMasqueradedJsonValid["job_title"] = "Updated Masqueraded Job Title";
    err = "";
    CHECK(Job::validateMasqueradedJsonForUpdate(updateMasqueradedJsonValid, masqueradingVector, err));

    Json::Value updateMasqueradedJsonMissingId;
    updateMasqueradedJsonMissingId["job_title"] = "Updated Masqueraded Job Title";
    err = "";
    CHECK(!Job::validateMasqueradedJsonForUpdate(updateMasqueradedJsonMissingId, masqueradingVector, err));
    CHECK_EQ(err, "The value of primary key must be set in the json object for update");

    // Test validateMasqueradedJsonForUpdate - failure case (invalid title length)
    Json::Value updateMasqueradedJsonInvalidTitle;
    updateMasqueradedJsonInvalidTitle["job_id"] = 8;
    std::string longMasqueradedTitle(51, 'B');
    updateMasqueradedJsonInvalidTitle["job_title"] = longMasqueradedTitle;
    err = "";
    CHECK(Job::validateMasqueradedJsonForUpdate(updateMasqueradedJsonInvalidTitle, masqueradingVector, err));

    // Test validateJsonForCreation - failure case (invalid title length)
    Json::Value createJsonInvalidTitle;
    std::string longTitleCreate(51, 'C');
    createJsonInvalidTitle["title"] = longTitleCreate;
    err = "";
    CHECK(!Job::validateJsonForCreation(createJsonInvalidTitle, err));
    CHECK_EQ(err, "String length exceeds limit for the title field (the maximum value is 50)");

    // Test validateMasqueradedJsonForCreation - failure case (invalid title length)
    Json::Value createMasqueradedJsonInvalidTitle;
    std::string longMasqueradedTitleCreate(51, 'D');
    createMasqueradedJsonInvalidTitle["job_title"] = longMasqueradedTitleCreate;
    err = "";
    CHECK(!Job::validateMasqueradedJsonForCreation(createMasqueradedJsonInvalidTitle, masqueradingVector, err));
    CHECK_EQ(err, "String length exceeds limit for the job_title field (the maximum value is 50)");
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "model/Job.h"
#include "model/Person.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace drogon_model::org_chart;

DROGON_TEST(PersonModelTest)
{
    // Test default constructor
    Person person;
    CHECK(!person.getId());
    CHECK(!person.getFirstName());
    CHECK(!person.getLastName());
    CHECK(!person.getJobId());
    CHECK(!person.getDepartmentId());

    // Test constructor with Json::Value
    Json::Value json;
    json["id"] = 1;
    json["first_name"] = "John";
    json["last_name"] = "Doe";
    json["job_id"] = 1;
    json["department_id"] = 1;
    Person person2(json);
    CHECK_EQ(person2.getValueOfId(), 1);
    CHECK_EQ(person2.getValueOfFirstName(), "John");
    CHECK_EQ(person2.getValueOfLastName(), "Doe");
    CHECK_EQ(person2.getValueOfJobId(), 1);
    CHECK_EQ(person2.getValueOfDepartmentId(), 1);

    // Test setters and getters
    person.setId(3);
    person.setFirstName("Jane");
    person.setLastName("Smith");
    person.setJobId(2);
    person.setDepartmentId(2);
    CHECK_EQ(person.getValueOfId(), 3);
    CHECK_EQ(person.getValueOfFirstName(), "Jane");
    CHECK_EQ(person.getValueOfLastName(), "Smith");
    CHECK_EQ(person.getValueOfJobId(), 2);
    CHECK_EQ(person.getValueOfDepartmentId(), 2);

    // Test updateByJson
    Json::Value updateJson;
    updateJson["first_name"] = "UpdatedFirstName";
    updateJson["last_name"] = "UpdatedLastName";
    person.updateByJson(updateJson);
    CHECK_EQ(person.getValueOfFirstName(), "UpdatedFirstName");
    CHECK_EQ(person.getValueOfLastName(), "UpdatedLastName");

    // Test toJson
    Json::Value personJson = person.toJson();
    CHECK_EQ(personJson["id"].asInt(), 3);
    CHECK_EQ(personJson["first_name"].asString(), "UpdatedFirstName");
    CHECK_EQ(personJson["last_name"].asString(), "UpdatedLastName");
    CHECK_EQ(personJson["job_id"].asInt(), 2);
    CHECK_EQ(personJson["department_id"].asInt(), 2);

    // Test insertColumns
    auto insertCols = Person::insertColumns();
    CHECK_EQ(insertCols.size(), 4);
    CHECK_EQ(insertCols[0], "first_name");
    CHECK_EQ(insertCols[1], "last_name");
    CHECK_EQ(insertCols[2], "job_id");
    CHECK_EQ(insertCols[3], "department_id");

    // Test updateColumns
    auto updateCols = person.updateColumns();
    CHECK_EQ(updateCols.size(), 4);
    CHECK_EQ(updateCols[0], "first_name");
    CHECK_EQ(updateCols[1], "last_name");
    CHECK_EQ(updateCols[2], "job_id");
    CHECK_EQ(updateCols[3], "department_id");

    // Test validateJsonForCreation - success and failure case
    Json::Value createJson;
    createJson["first_name"] = "NewFirstName";
    createJson["last_name"] = "NewLastName";
    createJson["job_id"] = 3;
    createJson["department_id"] = 3;
    std::string err;
    CHECK(Person::validateJsonForCreation(createJson, err));

    Json::Value createJsonMissingFirstName;
    createJsonMissingFirstName["last_name"] = "NewLastName";
    createJsonMissingFirstName["job_id"] = 3;
    createJsonMissingFirstName["department_id"] = 3;
    err = "";
    CHECK(!Person::validateJsonForCreation(createJsonMissingFirstName, err));
    CHECK_EQ(err, "The first_name column cannot be null");

    // Test validateJsonForUpdate - success and failure cases
    Json::Value updateJsonValid;
    updateJsonValid["id"] = 4;
    updateJsonValid["first_name"] = "UpdatedFirstName";
    updateJsonValid["last_name"] = "UpdatedLastName";
    updateJsonValid["job_id"] = 4;
    updateJsonValid["department_id"] = 4;
    err = "";
    CHECK(Person::validateJsonForUpdate(updateJsonValid, err));

    Json::Value updateJsonMissingId;
    updateJsonMissingId["first_name"] = "UpdatedFirstName";
    updateJsonMissingId["last_name"] = "UpdatedLastName";
    updateJsonMissingId["job_id"] = 4;
    updateJsonMissingId["department_id"] = 4;
    err = "";
    CHECK(!Person::validateJsonForUpdate(updateJsonMissingId, err));
    CHECK_EQ(err, "The value of primary key must be set in the json object for update");

    // Test validateJsonForUpdate - failure case (invalid first_name length)
    Json::Value updateJsonInvalidFirstName;
    updateJsonInvalidFirstName["id"] = 5;
    std::string longFirstName(51, 'A');
    updateJsonInvalidFirstName["first_name"] = longFirstName;
    updateJsonInvalidFirstName["last_name"] = "LastName";
    updateJsonInvalidFirstName["job_id"] = 5;
    updateJsonInvalidFirstName["department_id"] = 5;
    err = "";
    CHECK(!Person::validateJsonForUpdate(updateJsonInvalidFirstName, err));
    CHECK_EQ(err, "String length exceeds limit for the first_name field (the maximum value is 50)");

    // Test validateJsonForUpdate - failure case (invalid last_name length)
    Json::Value updateJsonInvalidLastName;
    updateJsonInvalidLastName["id"] = 6;
    updateJsonInvalidLastName["first_name"] = "FirstName";
    std::string longLastName(51, 'B');
    updateJsonInvalidLastName["last_name"] = longLastName;
    updateJsonInvalidLastName["job_id"] = 6;
    updateJsonInvalidLastName["department_id"] = 6;
    err = "";
    CHECK(!Person::validateJsonForUpdate(updateJsonInvalidLastName, err));
    CHECK_EQ(err, "String length exceeds limit for the last_name field (the maximum value is 50)");

    // Test masquerading
    std::vector<std::string> masqueradingVector = {"person_id", "first", "last", "job", "dept"};
    Json::Value masqueradedJson;
    masqueradedJson["person_id"] = 7;
    masqueradedJson["first"] = "MasqueradedFirstName";
    masqueradedJson["last"] = "MasqueradedLastName";
    masqueradedJson["job"] = 7;
    masqueradedJson["dept"] = 7;
    Person person4(masqueradedJson, masqueradingVector);
    CHECK_EQ(person4.getValueOfId(), 7);
    CHECK_EQ(person4.getValueOfFirstName(), "MasqueradedFirstName");
    CHECK_EQ(person4.getValueOfLastName(), "MasqueradedLastName");
    CHECK_EQ(person4.getValueOfJobId(), 7);
    CHECK_EQ(person4.getValueOfDepartmentId(), 7);

    Json::Value masqueradedJsonOutput = person4.toMasqueradedJson(masqueradingVector);
    CHECK_EQ(masqueradedJsonOutput["person_id"].asInt(), 7);
    CHECK_EQ(masqueradedJsonOutput["first"].asString(), "MasqueradedFirstName");
    CHECK_EQ(masqueradedJsonOutput["last"].asString(), "MasqueradedLastName");
    CHECK_EQ(masqueradedJsonOutput["job"].asInt(), 7);
    CHECK_EQ(masqueradedJsonOutput["dept"].asInt(), 7);

    // Test validateMasqueradedJsonForCreation - success and failure case
    Json::Value createMasqueradedJson;
    createMasqueradedJson["first"] = "NewMasqueradedFirstName";
    createMasqueradedJson["last"] = "NewMasqueradedLastName";
    createMasqueradedJson["job"] = 8;
    createMasqueradedJson["dept"] = 8;
    err = "";
    CHECK(Person::validateMasqueradedJsonForCreation(createMasqueradedJson, masqueradingVector, err));

    Json::Value createMasqueradedJsonMissingFirstName;
    createMasqueradedJsonMissingFirstName["last"] = "NewMasqueradedLastName";
    createMasqueradedJsonMissingFirstName["job"] = 8;
    createMasqueradedJsonMissingFirstName["dept"] = 8;
    err = "";
    CHECK(!Person::validateMasqueradedJsonForCreation(createMasqueradedJsonMissingFirstName, masqueradingVector, err));
    CHECK_EQ(err, "The first column cannot be null");

    // Test validateMasqueradedJsonForUpdate - success and failure cases
    Json::Value updateMasqueradedJsonValid;
    updateMasqueradedJsonValid["person_id"] = 9;
    updateMasqueradedJsonValid["first"] = "UpdatedMasqueradedFirstName";
    updateMasqueradedJsonValid["last"] = "UpdatedMasqueradedLastName";
    updateMasqueradedJsonValid["job"] = 9;
    updateMasqueradedJsonValid["dept"] = 9;
    err = "";
    CHECK(Person::validateMasqueradedJsonForUpdate(updateMasqueradedJsonValid, masqueradingVector, err));

    Json::Value updateMasqueradedJsonMissingId;
    updateMasqueradedJsonMissingId["first"] = "UpdatedMasqueradedFirstName";
    updateMasqueradedJsonMissingId["last"] = "UpdatedMasqueradedLastName";
    updateMasqueradedJsonMissingId["job"] = 9;
    updateMasqueradedJsonMissingId["dept"] = 9;
    err = "";
    CHECK(!Person::validateMasqueradedJsonForUpdate(updateMasqueradedJsonMissingId, masqueradingVector, err));
    CHECK_EQ(err, "The value of primary key must be set in the json object for update");

    // Test validateMasqueradedJsonForUpdate - failure case (invalid first_name length)
    Json::Value updateMasqueradedJsonInvalidFirstName;
    updateMasqueradedJsonInvalidFirstName["person_id"] = 10;
    std::string longMasqueradedFirstName(51, 'C');
    updateMasqueradedJsonInvalidFirstName["first"] = longMasqueradedFirstName;
    updateMasqueradedJsonInvalidFirstName["last"] = "MasqueradedLastName";
    updateMasqueradedJsonInvalidFirstName["job"] = 10;
    updateMasqueradedJsonInvalidFirstName["dept"] = 10;
    err = "";
    CHECK(!Person::validateMasqueradedJsonForUpdate(updateMasqueradedJsonInvalidFirstName, masqueradingVector, err));
    CHECK_EQ(err, "String length exceeds limit for the first field (the maximum value is 50)");

    // Test validateMasqueradedJsonForUpdate - failure case (invalid last_name length)
    Json::Value updateMasqueradedJsonInvalidLastName;
    updateMasqueradedJsonInvalidLastName["person_id"] = 11;
    updateMasqueradedJsonInvalidLastName["first"] = "MasqueradedFirstName";
    std::string longMasqueradedLastName(51, 'D');
    updateMasqueradedJsonInvalidLastName["last"] = longMasqueradedLastName;
    updateMasqueradedJsonInvalidLastName["job"] = 11;
    updateMasqueradedJsonInvalidLastName["dept"] = 11;
    err = "";
    CHECK(!Person::validateMasqueradedJsonForUpdate(updateMasqueradedJsonInvalidLastName, masqueradingVector, err));
    CHECK_EQ(err, "String length exceeds limit for the last field (the maximum value is 50)");

    // Test validateJsonForCreation - failure case (invalid first_name length)
    Json::Value createJsonInvalidFirstName;
    std::string longFirstNameCreate(51, 'E');
    createJsonInvalidFirstName["first_name"] = longFirstNameCreate;
    createJsonInvalidFirstName["last_name"] = "LastName";
    createJsonInvalidFirstName["job_id"] = 12;
    createJsonInvalidFirstName["department_id"] = 12;
    err = "";
    CHECK(!Person::validateJsonForCreation(createJsonInvalidFirstName, err));
    CHECK_EQ(err, "String length exceeds limit for the first_name field (the maximum value is 50)");

    // Test validateJsonForCreation - failure case (invalid last_name length)
    Json::Value createJsonInvalidLastName;
    createJsonInvalidLastName["first_name"] = "FirstName";
    std::string longLastNameCreate(51, 'F');
    createJsonInvalidLastName["last_name"] = longLastNameCreate;
    createJsonInvalidLastName["job_id"] = 13;
    createJsonInvalidLastName["department_id"] = 13;
    err = "";
    CHECK(!Person::validateJsonForCreation(createJsonInvalidLastName, err));
    CHECK_EQ(err, "String length exceeds limit for the last_name field (the maximum value is 50)");

    // Test validateMasqueradedJsonForCreation - failure case (invalid first_name length)
    Json::Value createMasqueradedJsonInvalidFirstName;
    std::string longMasqueradedFirstNameCreate(51, 'G');
    createMasqueradedJsonInvalidFirstName["first"] = longMasqueradedFirstNameCreate;
    createMasqueradedJsonInvalidFirstName["last"] = "MasqueradedLastName";
    createMasqueradedJsonInvalidFirstName["job"] = 14;
    createMasqueradedJsonInvalidFirstName["dept"] = 14;
    err = "";
    CHECK(!Person::validateMasqueradedJsonForCreation(createMasqueradedJsonInvalidFirstName, masqueradingVector, err));
    CHECK_EQ(err, "String length exceeds limit for the first field (the maximum value is 50)");

    // Test validateMasqueradedJsonForCreation - failure case (invalid last_name length)
    Json::Value createMasqueradedJsonInvalidLastName;
    createMasqueradedJsonInvalidLastName["first"] = "MasqueradedFirstName";
    std::string longMasqueradedLastNameCreate(51, 'H');
    createMasqueradedJsonInvalidLastName["last"] = longMasqueradedLastNameCreate;
    createMasqueradedJsonInvalidLastName["job"] = 15;
    createMasqueradedJsonInvalidLastName["dept"] = 15;
    err = "";
    CHECK(!Person::validateMasqueradedJsonForCreation(createMasqueradedJsonInvalidLastName, masqueradingVector, err));
    CHECK_EQ(err, "String length exceeds limit for the last field (the maximum value is 50)");
}