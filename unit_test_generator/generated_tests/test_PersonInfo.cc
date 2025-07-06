#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/PersonInfo.h"
#include <iostream>
#include <string>
#include <memory>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(PersonInfoConstructor)
{
    // Test case 1: Constructor with Row and indexOffset = -1 (named columns)
    {
        Row r;
        r["id"] = 1;
        r["job_id"] = 2;
        r["job_title"] = "Software Engineer";
        r["department_id"] = 3;
        r["department_name"] = "Engineering";
        r["manager_id"] = 4;
        r["manager_full_name"] = "John Doe";
        r["first_name"] = "Alice";
        r["last_name"] = "Smith";
        r["hire_date"] = "2023-01-01";

        PersonInfo personInfo(r, -1);

        CHECK_EQ(personInfo.getValueOfId(), 1);
        CHECK_EQ(personInfo.getValueOfJobId(), 2);
        CHECK_EQ(personInfo.getValueOfJobTitle(), "Software Engineer");
        CHECK_EQ(personInfo.getValueOfDepartmentId(), 3);
        CHECK_EQ(personInfo.getValueOfDepartmentName(), "Engineering");
        CHECK_EQ(personInfo.getValueOfManagerId(), 4);
        CHECK_EQ(personInfo.getValueOfManagerFullName(), "John Doe");
        CHECK_EQ(personInfo.getValueOfFirstName(), "Alice");
        CHECK_EQ(personInfo.getValueOfLastName(), "Smith");
        CHECK_EQ(personInfo.getHireDate()->toDbStringLocal(), "2023-01-01");
    }

    // Test case 2: Constructor with empty Row
    {
        Row r;
        PersonInfo personInfo(r, -1);

        CHECK_EQ(personInfo.getId(), nullptr);
        CHECK_EQ(personInfo.getJobId(), nullptr);
        CHECK_EQ(personInfo.getJobTitle(), nullptr);
        CHECK_EQ(personInfo.getDepartmentId(), nullptr);
        CHECK_EQ(personInfo.getDepartmentName(), nullptr);
        CHECK_EQ(personInfo.getManagerId(), nullptr);
        CHECK_EQ(personInfo.getManagerFullName(), nullptr);
        CHECK_EQ(personInfo.getFirstName(), nullptr);
        CHECK_EQ(personInfo.getLastName(), nullptr);
        CHECK_EQ(personInfo.getHireDate(), nullptr);
    }

    // Test case 3: Constructor with Row and indexOffset out of bounds
    {
        Row r;
        r[0] = 1;
        r[1] = 2;
        r[2] = 3;
        r[3] = 4;
        r[4] = "Alice";
        r[5] = "Smith";
        r[6] = "2023-01-01";

        PersonInfo personInfo(r, 10); // Index offset is out of bounds.  This should not crash.
    }
}

DROGON_TEST(PersonInfoGetters)
{
    // Create a PersonInfo object with sample data
    Row r;
    r["id"] = 1;
    r["job_id"] = 2;
    r["job_title"] = "Software Engineer";
    r["department_id"] = 3;
    r["department_name"] = "Engineering";
    r["manager_id"] = 4;
    r["manager_full_name"] = "John Doe";
    r["first_name"] = "Alice";
    r["last_name"] = "Smith";
    r["hire_date"] = "2023-01-01";

    PersonInfo personInfo(r, -1);

    // Test getId() and getValueOfId()
    CHECK_EQ(*personInfo.getId(), 1);
    CHECK_EQ(personInfo.getValueOfId(), 1);

    // Test getJobTitle() and getValueOfJobTitle()
    CHECK_EQ(*personInfo.getJobTitle(), "Software Engineer");
    CHECK_EQ(personInfo.getValueOfJobTitle(), "Software Engineer");

    // Test getHireDate() and getValueOfHireDate()
    CHECK_EQ(personInfo.getHireDate()->toDbStringLocal(), "2023-01-01");

    // Test default values when data is not present
    Row emptyRow;
    PersonInfo emptyPersonInfo(emptyRow, -1);

    CHECK_EQ(emptyPersonInfo.getId(), nullptr);
    CHECK_EQ(emptyPersonInfo.getValueOfId(), int32_t()); // Default int32_t value

    CHECK_EQ(emptyPersonInfo.getJobTitle(), nullptr);
    CHECK_EQ(emptyPersonInfo.getValueOfJobTitle(), std::string());

    CHECK_EQ(emptyPersonInfo.getHireDate(), nullptr);
    CHECK_EQ(emptyPersonInfo.getValueOfHireDate().toDbStringLocal(), ::trantor::Date().toDbStringLocal());
}

DROGON_TEST(PersonInfoToJson)
{
    // Test case 1: All fields present
    {
        Row r;
        r["id"] = 1;
        r["job_id"] = 2;
        r["job_title"] = "Software Engineer";
        r["department_id"] = 3;
        r["department_name"] = "Engineering";
        r["manager_id"] = 4;
        r["manager_full_name"] = "John Doe";
        r["first_name"] = "Alice";
        r["last_name"] = "Smith";
        r["hire_date"] = "2023-01-01";

        PersonInfo personInfo(r, -1);
        Json::Value json = personInfo.toJson();

        CHECK_EQ(json["id"].asInt(), 1);
        CHECK_EQ(json["job_id"].asInt(), 2);
        CHECK_EQ(json["job_title"].asString(), "Software Engineer");
        CHECK_EQ(json["department_id"].asInt(), 3);
        CHECK_EQ(json["department_name"].asString(), "Engineering");
        CHECK_EQ(json["manager_id"].asInt(), 4);
        CHECK_EQ(json["manager_full_name"].asString(), "John Doe");
        CHECK_EQ(json["first_name"].asString(), "Alice");
        CHECK_EQ(json["last_name"].asString(), "Smith");
        CHECK_EQ(json["hire_date"].asString(), "2023-01-01");
    }

    // Test case 2: Some fields missing
    {
        Row r;
        r["first_name"] = "Alice";
        r["last_name"] = "Smith";

        PersonInfo personInfo(r, -1);
        Json::Value json = personInfo.toJson();

        CHECK(json["id"].isNull());
        CHECK(json["job_title"].isNull());
        CHECK_EQ(json["first_name"].asString(), "Alice");
        CHECK_EQ(json["last_name"].asString(), "Smith");
    }

    // Test case 3: All fields missing
    {
        Row r;
        PersonInfo personInfo(r, -1);
        Json::Value json = personInfo.toJson();

        CHECK(json["id"].isNull());
        CHECK(json["job_title"].isNull());
        CHECK(json["first_name"].isNull());
        CHECK(json["last_name"].isNull());
    }
}