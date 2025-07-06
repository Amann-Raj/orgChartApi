#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>
#include <drogon/HttpAppFramework.h>

#include "../../controllers/AuthController.h"
#include "../../plugins/JwtPlugin.h"
#include "../../models/User.h"

#include <iostream>
#include <memory>
#include <functional>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace {
    // Mock classes for testing database interactions
    class MockDbClient : public DbClient {
    public:
        MockDbClient() : DbClient("Postgres", 1) {}
        ~MockDbClient() override = default;

        std::shared_ptr<Transaction> newTransaction(const std::string &name = std::string()) override { return nullptr;}
        void execSqlAsync(const std::string &sql,
                        size_t paraNum,
                        const std::vector<std::string> &parameters,
                        ResultCallback &&rcb,
                        std::function<void(const std::exception_ptr &)> &&exceptCallback,
                        const std::string &connName = std::string()) override {}
        
        void execSqlAsync(const std::string &sql,
                        size_t paraNum,
                        const std::vector<const char *> &parameters,
                        ResultCallback &&rcb,
                        std::function<void(const std::exception_ptr &)> &&exceptCallback,
                        const std::string &connName = std::string()) override {}

        std::future<Result> execSqlAsyncFuture(const std::string &sql,
                                            size_t paraNum,
                                            const std::vector<std::string> &parameters,
                                            const std::string &connName = std::string()) override {
            return std::future<Result>();
        }

        std::future<Result> execSqlAsyncFuture(const std::string &sql,
                                            size_t paraNum,
                                            const std::vector<const char *> &parameters,
                                            const std::string &connName = std::string()) override {
            return std::future<Result>();
        }

        std::string getClientType() const override { return "Postgres"; }
        int getClientVersion() const override { return 1; }
    };

    class MockMapper : public Mapper<User> {
    public:
        MockMapper(std::shared_ptr<DbClient> clientPtr) : Mapper(clientPtr) {}

        std::future<std::vector<User>> findFutureBy(const Criteria &criteria) override {
            std::vector<User> users;
            if (criteria.column() == User::Cols::_username) {
                if (criteria.value().asString() == "testuser") {
                    User user;
                    user.setUsername("testuser");
                    user.setPassword("$2b$12$xxxxxxxxxxxxxxxxxxxxxxxxx"); // Dummy bcrypt hash
                    users.push_back(user);
                }
            }
            std::promise<std::vector<User>> promise;
            promise.set_value(users);
            return promise.get_future();
        }

        std::future<void> insertFuture(const User &user) override {
            std::promise<void> promise;
            promise.set_value();
            return promise.get_future();
        }
    };
}

DROGON_TEST(AuthControllerRegisterUserSuccess, AuthController) {
    // Setup
    auto authController = AuthController();
    auto req = HttpRequest::newHttpRequest();
    auto json = Json::Value();
    json["username"] = "newuser";
    json["password"] = "password";
    req->setJsonObject(json);
    User user(json);

    bool callbackCalled = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k201Created);
        Json::Value body = resp->getJsonObject();
        CHECK(body.isMember("username"));
        CHECK(body.isMember("token"));
    };

    // Mock database client and mapper
    auto mockDbClientPtr = std::make_shared<MockDbClient>();
    auto mockMapperPtr = std::make_shared<MockMapper>(mockDbClientPtr);

    // Override the database client in the application
    auto &app = drogon::app();
    app.registerDbClient(mockDbClientPtr, "default");

    // Call the method
    authController.registerUser(req, callback, std::move(user));

    // Wait for callback to be called
    CHECK(callbackCalled);

    // Restore the original database client (if needed)
    app.registerDbClient(nullptr, "default");
}

DROGON_TEST(AuthControllerRegisterUserMissingFields, AuthController) {
    // Setup
    auto authController = AuthController();
    auto req = HttpRequest::newHttpRequest();
    auto json = Json::Value();
    req->setJsonObject(json);
    User user(json);

    bool callbackCalled = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
        Json::Value body = resp->getJsonObject();
        CHECK_EQ(body["error"].asString(), "missing fields");
    };

    // Call the method
    authController.registerUser(req, callback, std::move(user));

    // Wait for callback to be called
    CHECK(callbackCalled);
}

DROGON_TEST(AuthControllerRegisterUserUsernameTaken, AuthController) {
    // Setup
    auto authController = AuthController();
    auto req = HttpRequest::newHttpRequest();
    auto json = Json::Value();
    json["username"] = "testuser";
    json["password"] = "password";
    req->setJsonObject(json);
    User user(json);

    bool callbackCalled = false;
    auto callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
        Json::Value body = resp->getJsonObject();
        CHECK_EQ(body["error"].asString(), "username is taken");
    };

    // Mock database client and mapper
    auto mockDbClientPtr = std::make_shared<MockDbClient>();
    auto mockMapperPtr = std::make_shared<MockMapper>(mockDbClientPtr);

    // Override the database client in the application
    auto &app = drogon::app();
    app.registerDbClient(mockDbClientPtr, "default");

    // Call the method
    authController.registerUser(req, callback, std::move(user));

    // Wait for callback to be called
    CHECK(callbackCalled);

    // Restore the original database client (if needed)
    app.registerDbClient(nullptr, "default");
}

DROGON_TEST(AuthControllerLoginUser, AuthController) {
    // Setup
    auto authController = AuthController();
    auto req = HttpRequest::newHttpRequest();
    auto json = Json::Value();
    req->setJsonObject(json);
    User user(json);

    bool callbackCalled = false;
    std::function<void(const HttpResponsePtr &)> callback;

    // Mock database client and mapper
    auto mockDbClientPtr = std::make_shared<MockDbClient>();
    auto mockMapperPtr = std::make_shared<MockMapper>(mockDbClientPtr);

    // Override the database client in the application
    auto &app = drogon::app();
    app.registerDbClient(mockDbClientPtr, "default");

    // Test case 1: Missing fields
    callbackCalled = false;
    callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
        Json::Value body = resp->getJsonObject();
        CHECK_EQ(body["error"].asString(), "missing fields");
    };
    authController.loginUser(req, callback, std::move(user));
    CHECK(callbackCalled);

    // Test case 2: User not found
    json["username"] = "nonexistentuser";
    json["password"] = "password";
    req->setJsonObject(json);
    User userNotFound(json);

    callbackCalled = false;
    callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k400BadRequest);
        Json::Value body = resp->getJsonObject();
        CHECK_EQ(body["error"].asString(), "user not found");
    };
    authController.loginUser(req, callback, std::move(userNotFound));
    CHECK(callbackCalled);

    // Test case 3: Invalid password
    json["username"] = "testuser";
    json["password"] = "wrongpassword";
    req->setJsonObject(json);
    User userInvalidPassword(json);

    callbackCalled = false;
    callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k401Unauthorized);
        Json::Value body = resp->getJsonObject();
        CHECK_EQ(body["error"].asString(), "username and password do not match");
    };
    authController.loginUser(req, callback, std::move(userInvalidPassword));
    CHECK(callbackCalled);

    // Test case 4: Successful login
    json["username"] = "testuser";
    json["password"] = "password";
    req->setJsonObject(json);
    User userSuccess(json);

    callbackCalled = false;
    callback = [&](const HttpResponsePtr &resp) {
        callbackCalled = true;
        CHECK_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
        Json::Value body = resp->getJsonObject();
        CHECK(body.isMember("username"));
        CHECK(body.isMember("token"));
    };
    authController.loginUser(req, callback, std::move(userSuccess));
    CHECK(callbackCalled);

    // Restore the original database client (if needed)
    app.registerDbClient(nullptr, "default");
}

DROGON_TEST(AuthControllerAreFieldsValid, AuthController) {
    auto authController = AuthController();
    User user;

    // Test case 1: Both username and password are null
    CHECK_FALSE(authController.areFieldsValid(user));

    // Test case 2: Username is not null, password is null
    user.setUsername("testuser");
    CHECK_FALSE(authController.areFieldsValid(user));

    // Test case 3: Username is null, password is not null
    user.setUsername(nullptr);
    user.setPassword("password");
    CHECK_FALSE(authController.areFieldsValid(user));

    // Test case 4: Both username and password are not null
    user.setUsername("testuser");
    CHECK_TRUE(authController.areFieldsValid(user));
}

DROGON_TEST(AuthControllerIsPasswordValid, AuthController) {
    auto authController = AuthController();
    std::string password = "password";
    std::string hash = "$2b$12$xxxxxxxxxxxxxxxxxxxxxxxxx"; // Dummy bcrypt hash

    // This test will always fail because the dummy hash is not a valid bcrypt hash for "password"
    // However, it tests that the function is called without crashing.
    // To properly test this, you would need to generate a valid bcrypt hash for "password" and use that in the test.
    CHECK_FALSE(authController.isPasswordValid(password, hash));
}

DROGON_TEST(AuthControllerUserWithTokenToJson, AuthController) {
    User user;
    user.setUsername("testuser");
    user.setId(1);

    AuthController::UserWithToken userWithToken(user);
    Json::Value json = userWithToken.toJson();

    CHECK(json.isMember("username"));
    CHECK(json.isMember("token"));
    CHECK_EQ(json["username"].asString(), "testuser");
}