#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/Job.h"
#include "../../models/Person.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(JobModelTest)
{
    // Test default constructor
    Job job;
    CHECK(!job.getId());
    CHECK(!job.getTitle());

    // Test constructor with Row
    Row row;
    row["id"] = 1;
    row["title"] = "Software Engineer";
    Job jobFromRow(row);
    CHECK_EQ(jobFromRow.getValueOfId(), 1);
    CHECK_EQ(jobFromRow.getValueOfTitle(), "Software Engineer");

    // Test constructor with Json::Value and masquerading vector
    Json::Value masqueradedJson;
    masqueradedJson["job_id"] = 3;
    masqueradedJson["job_title"] = "Data Scientist";
    std::vector<std::string> masqueradingVector = {"job_id", "job_title"};
    Job jobFromMasqueradedJson(masqueradedJson, masqueradingVector);
    CHECK_EQ(jobFromMasqueradedJson.getValueOfId(), 3);
    CHECK_EQ(jobFromMasqueradedJson.getValueOfTitle(), "Data Scientist");

    // Test setters and getters
    job.setId(4);
    job.setTitle("Team Lead");
    CHECK_EQ(job.getValueOfId(), 4);
    CHECK_EQ(job.getValueOfTitle(), "Team Lead");

    // Test updateByMasqueradedJson
    Json::Value updateMasqueradedJson;
    updateMasqueradedJson["job_title"] = "Principal Engineer";
    std::vector<std::string> updateMasqueradingVector = {"id", "job_title"};
    job.updateByMasqueradedJson(updateMasqueradedJson, updateMasqueradingVector);
    CHECK_EQ(job.getValueOfTitle(), "Principal Engineer");

    // Test toJson and toMasqueradedJson
    std::vector<std::string> masqueradeVector = {"job_id", "job_title"};
    job.setId(4);
    Json::Value jobJson = job.toJson();
    Json::Value masqueradedJobJson = job.toMasqueradedJson(masqueradeVector);
    CHECK_EQ(jobJson["id"].asInt(), 4);
    CHECK_EQ(jobJson["title"].asString(), "Principal Engineer");
    CHECK_EQ(masqueradedJobJson["job_id"].asInt(), 4);
    CHECK_EQ(masqueradedJobJson["job_title"].asString(), "Principal Engineer");

    // Test insertColumns and updateColumns
    job.setTitle("Software Architect");
    auto insertCols = Job::insertColumns();
    auto updateCols = job.updateColumns();
    CHECK_EQ(insertCols.size(), 1);
    CHECK_EQ(insertCols[0], "title");
    CHECK_EQ(updateCols.size(), 1);
    CHECK_EQ(updateCols[0], "title");

    // Test validateJsonForCreation - success and failure
    Json::Value creationJsonValid;
    creationJsonValid["title"] = "New Job Title";
    Json::Value creationJsonInvalid;
    std::string err;
    bool isValidValid = Job::validateJsonForCreation(creationJsonValid, err);
    bool isValidInvalid = Job::validateJsonForCreation(creationJsonInvalid, err);
    CHECK(isValidValid);
    CHECK(err.empty());
    CHECK(!isValidInvalid);
    CHECK(!err.empty());

    // Test validateJsonForUpdate - success and failure
    Json::Value updateJsonValid;
    updateJsonValid["id"] = 5;
    updateJsonValid["title"] = "Updated Job Title";
    Json::Value updateJsonInvalid;
    updateJsonInvalid["title"] = "Updated Job Title";
    err = "";
    isValidValid = Job::validateJsonForUpdate(updateJsonValid, err);
    isValidInvalid = Job::validateJsonForUpdate(updateJsonInvalid, err);
    CHECK(isValidValid);
    CHECK(err.empty());
    CHECK(!isValidInvalid);
    CHECK(!err.empty());

    // Test validateMasqueradedJsonForCreation - success and failure
    Json::Value masqueradedCreationJsonValid;
    masqueradedCreationJsonValid["job_title"] = "Masqueraded Job Title";
    Json::Value masqueradedCreationJsonInvalid;
    std::vector<std::string> masqueradingVectorCreation = {"job_id", "job_title"};
    err = "";
    isValidValid = Job::validateMasqueradedJsonForCreation(masqueradedCreationJsonValid, masqueradingVectorCreation, err);
    isValidInvalid = Job::validateMasqueradedJsonForCreation(masqueradedCreationJsonInvalid, masqueradingVectorCreation, err);
    CHECK(isValidValid);
    CHECK(err.empty());
    CHECK(!isValidInvalid);
    CHECK(!err.empty());

    // Test validateMasqueradedJsonForUpdate - success and failure
    Json::Value masqueradedUpdateJsonValid;
    masqueradedUpdateJsonValid["job_id"] = 6;
    masqueradedUpdateJsonValid["job_title"] = "Masqueraded Updated Job Title";
    Json::Value masqueradedUpdateJsonInvalid;
    masqueradedUpdateJsonInvalid["job_title"] = "Masqueraded Updated Job Title";
    std::vector<std::string> masqueradingVectorUpdate = {"job_id", "job_title"};
    err = "";
    isValidValid = Job::validateMasqueradedJsonForUpdate(masqueradedUpdateJsonValid, masqueradingVectorUpdate, err);
    isValidInvalid = Job::validateMasqueradedJsonForUpdate(masqueradedUpdateJsonInvalid, masqueradingVectorUpdate, err);
    CHECK(isValidValid);
    CHECK(err.empty());
    CHECK(!isValidInvalid);
    CHECK(!err.empty());

    // Test validJsonOfField - success and failure
    Json::Value validJsonValue;
    validJsonValue["title"] = "Valid Title";
    std::string longTitle(51, 'A');
    Json::Value invalidJsonValue;
    invalidJsonValue["title"] = longTitle;
    err = "";
    isValidValid = Job::validJsonOfField(1, "title", validJsonValue["title"], err, true);
    bool isValidInvalid = Job::validJsonOfField(1, "title", invalidJsonValue["title"], err, true);
    CHECK(isValidValid);
    CHECK(err.empty());
    CHECK(!isValidInvalid);
    CHECK(!err.empty());
}