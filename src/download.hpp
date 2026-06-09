#pragma once
#include <string>

class DownloadHandler {
    public:
        bool init(std::string directory);
        bool downloadPlaylist(std::string url, std::string directory);
};