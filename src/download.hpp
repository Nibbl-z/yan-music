#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <nlohmann/detail/macro_scope.hpp>

typedef struct {
    std::vector<std::string> ids;
} SongDB;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SongDB, ids)

const SongDB defaultDB {
    {}
};

class DownloadHandler {
    public:
        bool init(std::string directory);
        bool initSongDbFile();
        bool loadDb();
        bool saveDb();
        bool download();
        bool downloadPlaylist(std::string id, std::string directory, std::vector<std::string>* r);
        SongDB db;
    private:
        std::string dbPath;
};