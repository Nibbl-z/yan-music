#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#define CONFIG_FOLDER "yan-music/"
#define CONFIG_NAME "yan-music_conf.json"

typedef struct {
    std::vector<std::string> playlists;
    std::vector<std::string> ytdlpFlags;
} Config;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, playlists, ytdlpFlags)

const Config defaultConfig = {
    {

    },
    {
        "-4",
        "--sleep-interval 5",
    }
};

class ConfigHandler {
    public:
        Config config;

        bool init();
        bool load();
        bool save();
    private:
        std::string configPath;
};