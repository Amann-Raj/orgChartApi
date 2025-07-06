#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

DROGON_TEST(ConfigFileLoading)
{
    // Create a dummy config file
    std::ofstream configFile("../test_config.json");
    configFile << "{\n";
    configFile << "  \"listeners\": [{\"address\": \"0.0.0.0\", \"port\": 8081}],\n";
    configFile << "  \"app\": {\n";
    configFile << "    \"name\": \"TestApp\",\n";
    configFile << "    \"threads\": 4,\n";
    configFile << "    \"document_root\": \"./\"\n";
    configFile << "  }\n";
    configFile << "}\n";
    configFile.close();

    // Load the config file
    drogon::app().loadConfigFile("../test_config.json");

    // Check if the config is loaded correctly
    CHECK_EQ(drogon::app().getName(), "TestApp");
    CHECK_EQ(drogon::app().getThreadNum(), 4);
    CHECK_EQ(drogon::app().getDocumentRoot(), "./");

    // Clean up the dummy config file
    std::remove("../test_config.json");
}

DROGON_TEST(AppConfiguration)
{
    // Test setting and getting app configurations
    drogon::app().setName("TestApp2");
    CHECK_EQ(drogon::app().getName(), "TestApp2");

    drogon::app().setThreadNum(8);
    CHECK_EQ(drogon::app().getThreadNum(), 8);

    drogon::app().setDocumentRoot("/tmp");
    CHECK_EQ(drogon::app().getDocumentRoot(), "/tmp");
}

DROGON_TEST(InvalidConfigFile)
{
    // Create an invalid config file
    std::ofstream configFile("../invalid_config.json");
    configFile << "{\n";
    configFile << "  \"listeners\": [{\"address\": \"0.0.0.0\", \"port\": \"invalid\"}]\n"; // Invalid port
    configFile << "}\n";
    configFile.close();

    // Try to load the invalid config file
    // Drogon should log an error message, but the app should not crash.
    drogon::app().loadConfigFile("../invalid_config.json");

    // Clean up the dummy config file
    std::remove("../invalid_config.json");
}

DROGON_TEST(DefaultConfigFile)
{
    // Test loading the default config file (config.json)
    // This test assumes that config.json does not exist or is empty.
    drogon::app().loadConfigFile("config.json");
    // Check if the app name is empty (default value)
    CHECK_EQ(drogon::app().getName(), "");
}

DROGON_TEST(JsonConfigLoading)
{
    // Create a dummy config file with JSON format
    std::ofstream configFile("../test_config.json");
    configFile << "{\n";
    configFile << "  \"app\": {\n";
    configFile << "    \"json_config\": {\n";
    configFile << "      \"key1\": \"value1\",\n";
    configFile << "      \"key2\": 123\n";
    configFile << "    }\n";
    configFile << "  }\n";
    configFile << "}\n";
    configFile.close();

    // Load the config file
    drogon::app().loadConfigFile("../test_config.json");

    // Check if the JSON config is loaded correctly
    auto jsonConfig = drogon::app().getCustomConfig();
    CHECK(jsonConfig.isObject());
    CHECK_EQ(jsonConfig["key1"].asString(), "value1");
    CHECK_EQ(jsonConfig["key2"].asInt(), 123);

    // Clean up the dummy config file
    std::remove("../test_config.json");
}

DROGON_TEST(CustomConfig)
{
    // Test setting and getting custom configurations
    Json::Value customConfig;
    customConfig["key1"] = "value1";
    customConfig["key2"] = 456;

    drogon::app().setCustomConfig(customConfig);
    auto retrievedConfig = drogon::app().getCustomConfig();

    CHECK(retrievedConfig.isObject());
    CHECK_EQ(retrievedConfig["key1"].asString(), "value1");
    CHECK_EQ(retrievedConfig["key2"].asInt(), 456);
}

DROGON_TEST(RunMode)
{
    // Test setting and getting run mode
    drogon::app().setRunMode(drogon::RunMode::kProduction);
    CHECK_EQ(drogon::app().getRunMode(), drogon::RunMode::kProduction);

    drogon::app().setRunMode(drogon::RunMode::kDebug);
    CHECK_EQ(drogon::app().getRunMode(), drogon::RunMode::kDebug);
}

DROGON_TEST(DocumentRoot)
{
    // Test setting and getting document root
    drogon::app().setDocumentRoot("/var/www/html");
    CHECK_EQ(drogon::app().getDocumentRoot(), "/var/www/html");
}

DROGON_TEST(UploadPath)
{
    // Test setting and getting upload path
    drogon::app().setUploadPath("/tmp/uploads");
    CHECK_EQ(drogon::app().getUploadPath(), "/tmp/uploads");
}

DROGON_TEST(LogPath)
{
    // Test setting and getting log path
    drogon::app().setLogPath("/var/log/drogon");
    CHECK_EQ(drogon::app().getLogPath(), "/var/log/drogon");
}
// Additional tests for uncovered code
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>
#include <fstream>

DROGON_TEST(AddressFamily)
{
    // Test setting and getting address family
    drogon::app().setAddressFamily(AF_INET6);
    CHECK_EQ(drogon::app().getAddressFamily(), AF_INET6);

    drogon::app().setAddressFamily(AF_INET);
    CHECK_EQ(drogon::app().getAddressFamily(), AF_INET);
}

DROGON_TEST(ClientMaxBodySize)
{
    // Test setting and getting client max body size
    drogon::app().setClientMaxBodySize(1024 * 1024 * 10); // 10MB
    CHECK_EQ(drogon::app().getClientMaxBodySize(), 1024 * 1024 * 10);
}

DROGON_TEST(MaxConnections)
{
    // Test setting and getting max connections
    drogon::app().setMaxConnections(2000);
    CHECK_EQ(drogon::app().getMaxConnections(), 2000);
}

DROGON_TEST(KeepaliveTimeout)
{
    // Test setting and getting keepalive timeout
    drogon::app().setKeepaliveTimeout(60);
    CHECK_EQ(drogon::app().getKeepaliveTimeout(), 60);
}

DROGON_TEST(SendfileThreadNum)
{
    // Test setting and getting sendfile thread num
    drogon::app().setSendfileThreadNum(2);
    CHECK_EQ(drogon::app().getSendfileThreadNum(), 2);
}

DROGON_TEST(SessionTimeout)
{
    // Test setting and getting session timeout
    drogon::app().setSessionTimeout(3600);
    CHECK_EQ(drogon::app().getSessionTimeout(), 3600);
}

DROGON_TEST(Timezone)
{
    // Test setting and getting timezone
    drogon::app().setTimezone("America/Los_Angeles");
    CHECK_EQ(drogon::app().getTimezone(), "America/Los_Angeles");
}

DROGON_TEST(LogLevel)
{
    // Test setting and getting log level
    drogon::app().setLogLevel(trantor::Logger::kWarn);
    CHECK_EQ(drogon::app().getLogLevel(), trantor::Logger::kWarn);

    drogon::app().setLogLevel(trantor::Logger::kDebug);
    CHECK_EQ(drogon::app().getLogLevel(), trantor::Logger::kDebug);
}

DROGON_TEST(StaticFileCache)
{
    // Test enabling and disabling static file cache
    drogon::app().enableStaticFilesCache(true);
    CHECK_EQ(drogon::app().isStaticFilesCacheEnabled(), true);

    drogon::app().enableStaticFilesCache(false);
    CHECK_EQ(drogon::app().isStaticFilesCacheEnabled(), false);
}

DROGON_TEST(SslConfLoading)
{
    // Create a dummy ssl config file
    std::ofstream configFile("../test_ssl_config.json");
    configFile << "{\n";
    configFile << "  \"ssl\": {\n";
    configFile << "    \"cert\": \"/path/to/cert.pem\",\n";
    configFile << "    \"key\": \"/path/to/key.pem\"\n";
    configFile << "  }\n";
    configFile << "}\n";
    configFile.close();

    // Load the config file
    drogon::app().loadConfigFile("../test_ssl_config.json");

    // Clean up the dummy config file
    std::remove("../test_ssl_config.json");
}