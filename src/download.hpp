#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <nlohmann/detail/macro_scope.hpp>

typedef struct {
    std::unordered_map<std::string, std::vector<std::string>> ids;
} PlaylistDB;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlaylistDB, ids)

const PlaylistDB defaultDB {
    {}
};

class DownloadHandler {
    public:
        bool init(std::string directory);
        bool initPlaylistDbFile();
        bool loadDb();
        bool saveDb();
        bool downloadPlaylist(std::string id, std::string directory);
        PlaylistDB db;
    private:
        std::string dbPath;
};