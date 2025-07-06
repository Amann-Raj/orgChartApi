#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include "model/User.h"

using namespace drogon_model::org_chart;

DROGON_TEST(UserConstructorRow)
{
    // Create a mock Row object
    drogon::orm::Row row;
    row["id"] = 1;
    row["username"] = "testuser";
    row["password"] = "testpassword";

    // Create a User object from the Row
    User user(row);

    // Check that the User object was created correctly
    CHECK_EQ(user.getValueOfId(), 1);
    CHECK_EQ(user.getValueOfUsername(), "testuser");
    CHECK_EQ(user.getValueOfPassword(), "testpassword");
}

DROGON_TEST(UserConstructorNullValues)
{
    // Create a mock Row object with null values
    drogon::orm::Row row;
    row["id"] = nullptr;
    row["username"] = nullptr;
    row["password"] = nullptr;

    // Create a User object from the Row
    User user(row);

    // Check that the User object was created correctly with default values
    CHECK_EQ(user.getId(), nullptr);
    CHECK_EQ(user.getUsername(), nullptr);
    CHECK_EQ(user.getPassword(), nullptr);
}

DROGON_TEST(UserGettersSetters)
{
    // Create a User object
    User user;

    // Set the values using the setters
    user.setId(1);
    user.setUsername("testuser");
    user.setPassword("testpassword");

    // Check that the values were set correctly using the getters
    CHECK_EQ(user.getValueOfId(), 1);
    CHECK_EQ(user.getValueOfUsername(), "testuser");
    CHECK_EQ(user.getValueOfPassword(), "testpassword");
}

DROGON_TEST(UserUpdateByJson)
{
    // Create a User object
    User user;
    user.setId(1); // Set the primary key for update

    // Create a mock Json::Value object with updated values
    Json::Value json;
    json["username"] = "newuser";
    json["password"] = "newpassword";

    // Update the User object using the Json::Value
    user.updateByJson(json);

    // Check that the values were updated correctly
    CHECK_EQ(user.getValueOfId(), 1);
    CHECK_EQ(user.getValueOfUsername(), "newuser");
    CHECK_EQ(user.getValueOfPassword(), "newpassword");
}

DROGON_TEST(UserToJson)
{
    // Create a User object
    User user;
    user.setId(1);
    user.setUsername("testuser");
    user.setPassword("testpassword");

    // Convert the User object to a Json::Value
    Json::Value json = user.toJson();

    // Check that the Json::Value object was created correctly
    CHECK_EQ(json["id"].asInt(), 1);
    CHECK_EQ(json["username"].asString(), "testuser");
    CHECK_EQ(json["password"].asString(), "testpassword");
}

DROGON_TEST(UserValidateJsonForCreation)
{
    // Create a mock Json::Value object for creation
    Json::Value json;
    json["username"] = "testuser";
    json["password"] = "testpassword";

    // Validate the Json::Value object
    std::string err;
    bool isValid = User::validateJsonForCreation(json, err);

    // Check that the Json::Value object is valid
    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");

    // Create a mock Json::Value object for creation without username
    Json::Value jsonMissingUsername;
    jsonMissingUsername["password"] = "testpassword";

    // Validate the Json::Value object
    std::string errMissingUsername;
    bool isValidMissingUsername = User::validateJsonForCreation(jsonMissingUsername, errMissingUsername);

    // Check that the Json::Value object is invalid
    CHECK_EQ(isValidMissingUsername, false);
    CHECK_EQ(errMissingUsername, "The username column cannot be null");

    // Create a mock Json::Value object for creation without password
    Json::Value jsonMissingPassword;
    jsonMissingPassword["username"] = "testuser";

    // Validate the Json::Value object
    std::string errMissingPassword;
    bool isValidMissingPassword = User::validateJsonForCreation(jsonMissingPassword, errMissingPassword);

    // Check that the Json::Value object is invalid
    CHECK_EQ(isValidMissingPassword, false);
    CHECK_EQ(errMissingPassword, "The password column cannot be null");
}

DROGON_TEST(UserValidateJsonForUpdate)
{
    // Create a mock Json::Value object for update
    Json::Value json;
    json["id"] = 1;
    json["username"] = "testuser";
    json["password"] = "testpassword";

    // Validate the Json::Value object
    std::string err;
    bool isValid = User::validateJsonForUpdate(json, err);

    // Check that the Json::Value object is valid
    CHECK_EQ(isValid, true);
    CHECK_EQ(err, "");

    // Create a mock Json::Value object for update without id
    Json::Value jsonMissingId;
    jsonMissingId["username"] = "testuser";
    jsonMissingId["password"] = "testpassword";

    // Validate the Json::Value object
    std::string errMissingId;
    bool isValidMissingId = User::validateJsonForUpdate(jsonMissingId, errMissingId);

    // Check that the Json::Value object is invalid
    CHECK_EQ(isValidMissingId, false);
    CHECK_EQ(errMissingId, "The value of primary key must be set in the json object for update");

    // Create a mock Json::Value object for update with invalid username
    Json::Value jsonInvalidUsername;
    jsonInvalidUsername["id"] = 1;
    std::string longUsername(51, 'A');
    jsonInvalidUsername["username"] = longUsername;
    jsonInvalidUsername["password"] = "testpassword";

    // Validate the Json::Value object
    std::string errInvalidUsername;
    bool isValidInvalidUsername = User::validateJsonForUpdate(jsonInvalidUsername, errInvalidUsername);

    // Check that the Json::Value object is valid
    CHECK_EQ(isValidInvalidUsername, false);
}

DROGON_TEST(UserMasqueradedJson) {
    User user;
    user.setId(1);
    user.setUsername("testuser");
    user.setPassword("testpassword");

    std::vector<std::string> masqueradingVector = {"userId", "userName", "userPassword"};
    Json::Value json = user.toMasqueradedJson(masqueradingVector);

    CHECK_EQ(json["userId"].asInt(), 1);
    CHECK_EQ(json["userName"].asString(), "testuser");
    CHECK_EQ(json["userPassword"].asString(), "testpassword");

    std::vector<std::string> masqueradingVectorFail = {"userId", "userName"};
    Json::Value jsonFail = user.toMasqueradedJson(masqueradingVectorFail);

    CHECK_EQ(jsonFail["id"].asInt(), 1);
    CHECK_EQ(jsonFail["username"].asString(), "testuser");
    CHECK_EQ(jsonFail["password"].asString(), "testpassword");
}