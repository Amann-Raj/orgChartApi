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

    // Test case 3: Constructor with Row and indexOffset that causes out-of-bounds access
    {
        Row r;
        r[0] = 1;
        r[1] = 2;
        r[2] = 3;
        r[3] = 4;
        r[4] = "Alice";
        r[5] = "Smith";
        r[6] = "2023-01-01";

        PersonInfo personInfo(r, 1); // offset + 7 > r.size()
        CHECK_EQ(personInfo.getId(), nullptr);
    }

     // Test case 4: Constructor with Row and indexOffset = 0 (indexed columns)
    {
        Row r;
        r[0] = 1;
        r[1] = 2;
        r[2] = "Software Engineer";
        r[3] = 3;
        r[4] = "Engineering";
        r[5] = 4;
        r[6] = "John Doe";
        r[7] = "Alice";
        r[8] = "Smith";
        r[9] = "2023-01-01";

        PersonInfo personInfo(r, 0);

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
    REQUIRE(personInfo.getId() != nullptr);
    CHECK_EQ(*personInfo.getId(), 1);

    // Test getJobId() and getValueOfJobId()
    REQUIRE(personInfo.getJobId() != nullptr);
    CHECK_EQ(*personInfo.getJobId(), 2);

    // Test getJobTitle() and getValueOfJobTitle()
    REQUIRE(personInfo.getJobTitle() != nullptr);
    CHECK_EQ(*personInfo.getJobTitle(), "Software Engineer");

    // Test getDepartmentId() and getValueOfDepartmentId()
    REQUIRE(personInfo.getDepartmentId() != nullptr);
    CHECK_EQ(*personInfo.getDepartmentId(), 3);

    // Test getDepartmentName() and getValueOfDepartmentName()
    REQUIRE(personInfo.getDepartmentName() != nullptr);
    CHECK_EQ(*personInfo.getDepartmentName(), "Engineering");

    // Test getManagerId() and getValueOfManagerId()
    REQUIRE(personInfo.getManagerId() != nullptr);
    CHECK_EQ(*personInfo.getManagerId(), 4);

    // Test getManagerFullName() and getValueOfManagerFullName()
    REQUIRE(personInfo.getManagerFullName() != nullptr);
    CHECK_EQ(*personInfo.getManagerFullName(), "John Doe");

    // Test getFirstName() and getValueOfFirstName()
    REQUIRE(personInfo.getFirstName() != nullptr);
    CHECK_EQ(*personInfo.getFirstName(), "Alice");

    // Test getLastName() and getValueOfLastName()
    REQUIRE(personInfo.getLastName() != nullptr);
    CHECK_EQ(*personInfo.getLastName(), "Smith");

    // Test getHireDate() and getValueOfHireDate()
    REQUIRE(personInfo.getHireDate() != nullptr);
    CHECK_EQ(personInfo.getHireDate()->toDbStringLocal(), "2023-01-01");
}

DROGON_TEST(PersonInfoGettersNull)
{
    // Create a PersonInfo object with null values
    Row r;
    PersonInfo personInfo(r, -1);

    // Test getId() when id_ is null
    CHECK_EQ(personInfo.getId(), nullptr);

    // Test getJobId() when jobId_ is null
    CHECK_EQ(personInfo.getJobId(), nullptr);

    // Test getJobTitle() when jobTitle_ is null
    CHECK_EQ(personInfo.getJobTitle(), nullptr);

    // Test getDepartmentId() when departmentId_ is null
    CHECK_EQ(personInfo.getDepartmentId(), nullptr);

    // Test getDepartmentName() when departmentName_ is null
    CHECK_EQ(personInfo.getDepartmentName(), nullptr);

    // Test getManagerId() when managerId_ is null
    CHECK_EQ(personInfo.getManagerId(), nullptr);

    // Test getManagerFullName() when managerFullName_ is null
    CHECK_EQ(personInfo.getManagerFullName(), nullptr);

    // Test getFirstName() when firstName_ is null
    CHECK_EQ(personInfo.getFirstName(), nullptr);

    // Test getLastName() when lastName_ is null
    CHECK_EQ(personInfo.getLastName(), nullptr);

    // Test getHireDate() when hireDate_ is null
    CHECK_EQ(personInfo.getHireDate(), nullptr);
}

DROGON_TEST(PersonInfoToJson)
{
    // Test case 1: All fields are present
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

    // Test case 2: Some fields are missing (null)
    {
        Row r;
        r["first_name"] = "Alice";
        r["last_name"] = "Smith";

        PersonInfo personInfo(r, -1);
        Json::Value json = personInfo.toJson();

        CHECK(json["id"].isNull());
        CHECK(json["job_id"].isNull());
        CHECK(json["job_title"].isNull());
        CHECK(json["department_id"].isNull());
        CHECK(json["department_name"].isNull());
        CHECK(json["manager_id"].isNull());
        CHECK(json["manager_full_name"].isNull());
        CHECK_EQ(json["first_name"].asString(), "Alice");
        CHECK_EQ(json["last_name"].asString(), "Smith");
        CHECK(json["hire_date"].isNull());
    }

    // Test case 3: All fields are null
    {
        Row r;
        PersonInfo personInfo(r, -1);
        Json::Value json = personInfo.toJson();

        CHECK(json["id"].isNull());
        CHECK(json["job_id"].isNull());
        CHECK(json["job_title"].isNull());
        CHECK(json["department_id"].isNull());
        CHECK(json["department_name"].isNull());
        CHECK(json["manager_id"].isNull());
        CHECK(json["manager_full_name"].isNull());
        CHECK(json["first_name"].isNull());
        CHECK(json["last_name"].isNull());
        CHECK(json["hire_date"].isNull());
    }
}

DROGON_TEST(PersonInfoGetValueOrDefaults) {
    Row r;
    PersonInfo personInfo(r, -1);

    CHECK_EQ(personInfo.getValueOfId(0), 0);
    CHECK_EQ(personInfo.getValueOfJobId(0), 0);
    CHECK_EQ(personInfo.getValueOfDepartmentId(0), 0);
    CHECK_EQ(personInfo.getValueOfManagerId(0), 0);
    CHECK_EQ(personInfo.getValueOfJobTitle("N/A"), "N/A");
    CHECK_EQ(personInfo.getValueOfDepartmentName("N/A"), "N/A");
    CHECK_EQ(personInfo.getValueOfManagerFullName("N/A"), "N/A");
    CHECK_EQ(personInfo.getValueOfFirstName("N/A"), "N/A");
    CHECK_EQ(personInfo.getValueOfLastName("N/A"), "N/A");
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/PersonInfo.h"
#include <iostream>
#include <string>
#include <memory>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(PersonInfoGetValueOrDefaultsWithValue) {
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

    PersonInfo personInfo(r, -1);

    CHECK_EQ(personInfo.getValueOfId(0), 1);
    CHECK_EQ(personInfo.getValueOfJobId(0), 2);
    CHECK_EQ(personInfo.getValueOfDepartmentId(0), 3);
    CHECK_EQ(personInfo.getValueOfManagerId(0), 4);
    CHECK_EQ(personInfo.getValueOfJobTitle("N/A"), "Software Engineer");
    CHECK_EQ(personInfo.getValueOfDepartmentName("N/A"), "Engineering");
    CHECK_EQ(personInfo.getValueOfManagerFullName("N/A"), "John Doe");
    CHECK_EQ(personInfo.getValueOfFirstName("N/A"), "Alice");
    CHECK_EQ(personInfo.getValueOfLastName("N/A"), "Smith");
}