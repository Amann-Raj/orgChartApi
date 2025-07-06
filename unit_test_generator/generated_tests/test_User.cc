#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/User.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(UserConstructorRow)
{
    // Mock Row object
    Row row;
    row["id"] = 1;
    row["username"] = "testuser";
    row["password"] = "testpassword";

    // Create User object from Row
    User user(row);

    // Check if values are correctly initialized
    CHECK_EQ(user.getValueOfId(), 1);
    CHECK_EQ(user.getValueOfUsername(), "testuser");
    CHECK_EQ(user.getValueOfPassword(), "testpassword");
}

DROGON_TEST(UserConstructorNullValues)
{
    // Mock Row object with null values
    Row row;
    row["id"] = nullptr;
    row["username"] = nullptr;
    row["password"] = nullptr;

    // Create User object from Row
    User user(row);

    // Check if values are correctly initialized (should be default values)
    CHECK_EQ(user.getId(), nullptr);
    CHECK_EQ(user.getUsername(), nullptr);
    CHECK_EQ(user.getPassword(), nullptr);
}

DROGON_TEST(UserGettersSetters)
{
    User user;

    // Set values using setters
    user.setId(1);
    user.setUsername("testuser");
    user.setPassword("testpassword");

    // Check if values are correctly set and retrieved using getters
    CHECK_EQ(user.getValueOfId(), 1);
    CHECK_EQ(user.getValueOfUsername(), "testuser");
    CHECK_EQ(user.getValueOfPassword(), "testpassword");
}

DROGON_TEST(UserUpdateByJson)
{
    User user;
    user.setId(1); // Set primary key for update

    // Mock Json::Value object with updated values
    Json::Value json;
    json["username"] = "newuser";
    json["password"] = "newpassword";

    // Update User object using Json::Value
    user.updateByJson(json);

    // Check if values are correctly updated
    CHECK_EQ(user.getValueOfUsername(), "newuser");
    CHECK_EQ(user.getValueOfPassword(), "newpassword");
}

DROGON_TEST(UserToJson)
{
    User user;
    user.setId(1);
    user.setUsername("testuser");
    user.setPassword("testpassword");

    // Convert User object to Json::Value
    Json::Value json = user.toJson();

    // Check if Json::Value contains correct values
    CHECK_EQ(json["id"].asInt(), 1);
    CHECK_EQ(json["username"].asString(), "testuser");
    CHECK_EQ(json["password"].asString(), "testpassword");
}

DROGON_TEST(UserValidateJsonForCreation)
{
    // Mock Json::Value object for creation
    Json::Value json;
    json["username"] = "testuser";
    json["password"] = "testpassword";

    std::string err;
    bool isValid = User::validateJsonForCreation(json, err);

    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");
}

DROGON_TEST(UserValidateJsonForCreationFailure)
{
    // Mock Json::Value object for creation with null username
    Json::Value json;
    json["password"] = "testpassword";

    std::string err;
    bool isValid = User::validateJsonForCreation(json, err);

    CHECK_EQ(isValid, false);
    CHECK_EQ(err, "The username column cannot be null");
}

DROGON_TEST(UserValidateJsonForUpdateSuccess)
{
    // Mock Json::Value object for update
    Json::Value json;
    json["id"] = 1;
    json["username"] = "testuser";
    json["password"] = "testpassword";

    std::string err;
    bool isValid = User::validateJsonForUpdate(json, err);

    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");
}

DROGON_TEST(UserValidateJsonForUpdateFailure)
{
    // Mock Json::Value object for update with missing id
    Json::Value json;
    json["username"] = "testuser";
    json["password"] = "testpassword";

    std::string err;
    bool isValid = User::validateJsonForUpdate(json, err);

    CHECK_EQ(isValid, false);
    CHECK_EQ(err, "The value of primary key must be set in the json object for update");
}

DROGON_TEST(UserInsertColumns)
{
    const auto& cols = User::insertColumns();
    CHECK_EQ(cols.size(), 2);
    CHECK_EQ(cols[0], "username");
    CHECK_EQ(cols[1], "password");
}

DROGON_TEST(UserOutputArgs)
{
    User user;
    user.setUsername("testuser");
    user.setPassword("testpassword");
    user.dirtyFlag_[1] = true;
    user.dirtyFlag_[2] = true;

    drogon::orm::internal::SqlBinder binder;
    user.outputArgs(binder);

    // Cannot directly check the binder content, but we can verify that the method executes without errors.
    // This test primarily ensures that the method executes without errors.
    SUCCEED();
}

DROGON_TEST(UserUpdateColumns)
{
    User user;
    user.setUsername("testuser");
    user.setPassword("testpassword");
    user.dirtyFlag_[1] = true;
    user.dirtyFlag_[2] = true;

    const auto& cols = user.updateColumns();
    CHECK_EQ(cols.size(), 2);
    CHECK_EQ(cols[0], "username");
    CHECK_EQ(cols[1], "password");
}

DROGON_TEST(UserUpdateArgs)
{
    User user;
    user.setUsername("testuser");
    user.setPassword("testpassword");
    user.dirtyFlag_[1] = true;
    user.dirtyFlag_[2] = true;

    drogon::orm::internal::SqlBinder binder;
    user.updateArgs(binder);

    // Cannot directly check the binder content, but we can verify that the method executes without errors.
    // This test primarily ensures that the method executes without errors.
    SUCCEED();
}

DROGON_TEST(UserValidateJsonForCreationEmptyUsername) {
    Json::Value json;
    json["username"] = "";
    json["password"] = "password";

    std::string err;
    bool isValid = User::validateJsonForCreation(json, err);

    CHECK_EQ(isValid, false);
    CHECK_EQ(err, "The username column cannot be empty");
}

DROGON_TEST(UserValidateJsonForUpdateEmptyUsername) {
    Json::Value json;
    json["id"] = 1;
    json["username"] = "";
    json["password"] = "password";

    std::string err;
    bool isValid = User::validateJsonForUpdate(json, err);

    CHECK_EQ(isValid, false);
    CHECK_EQ(err, "The username column cannot be empty");
}

DROGON_TEST(UserDefaultValues) {
    User user;
    CHECK_EQ(user.getValueOfId(), 0);
    CHECK_EQ(user.getValueOfUsername(), "");
    CHECK_EQ(user.getValueOfPassword(), "");
}

DROGON_TEST(UserIsPrimaryKeyAutoIncrement) {
    CHECK_EQ(User::hasAutoIncrementPrimaryKey(), true);
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "../../models/User.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

DROGON_TEST(UserCopyConstructor)
{
    User user;
    user.setId(1);
    user.setUsername("testuser");
    user.setPassword("testpassword");

    User user2 = user;

    CHECK_EQ(user2.getValueOfId(), 1);
    CHECK_EQ(user2.getValueOfUsername(), "testuser");
    CHECK_EQ(user2.getValueOfPassword(), "testpassword");
}

DROGON_TEST(UserAssignmentOperator)
{
    User user;
    user.setId(1);
    user.setUsername("testuser");
    user.setPassword("testpassword");

    User user2;
    user2 = user;

    CHECK_EQ(user2.getValueOfId(), 1);
    CHECK_EQ(user2.getValueOfUsername(), "testuser");
    CHECK_EQ(user2.getValueOfPassword(), "testpassword");
}

DROGON_TEST(UserPrimaryKeyName)
{
    CHECK_EQ(User::primaryKeyName(), "id");
}

DROGON_TEST(UserTableName)
{
    CHECK_EQ(User::tableName(), "users");
}