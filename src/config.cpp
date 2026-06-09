#include "config.hpp"
#include "filesystem"
#include "fstream"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

bool ConfigHandler::init() {
    const char* homeDir = std::getenv("HOME");
    if (homeDir == nullptr) { return false; }

    std::string path = std::string(homeDir) + "/.config/" + CONFIG_FOLDER;
    std::filesystem::create_directory(path);

    configPath = path + CONFIG_NAME;

    if (std::filesystem::exists(configPath)) {
        this->load();
        return true;
    }

    std::ofstream stream(configPath);

    nlohmann::json j = defaultConfig;

    stream << j;
    stream.close();

    config = defaultConfig;

    return true;
}

bool ConfigHandler::save() {
    if (configPath.empty()) {
        return false;
    }

    std::ofstream stream(configPath);

    nlohmann::json j = config;

    stream << j;
    stream.close();

    return true;
}

bool ConfigHandler::load() {
    if (configPath.empty()) {
        return false;
    }

    std::ifstream stream(configPath);
    std::ostringstream contents;

    contents << stream.rdbuf();

    nlohmann::json j = nlohmann::json::parse(contents.str());
    Config loaded = j.get<Config>();

    config = loaded;

    return true;
}