#pragma once
#include <string>

class Downloader {
    public:
        bool init(std::string directory);
        bool downloadPlaylist(std::string url, std::string directory);
};