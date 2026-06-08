#include "downloader.hpp"
#include "filesystem"
#include <string>
#include <iostream>

bool Downloader::init(std::string directory) {
    std::filesystem::create_directory(directory);
    return true;
}

bool Downloader::downloadPlaylist(std::string url, std::string directory) {
    std::string args[] = {
        "-4",
        "-P \"" + directory + "\"",
        "--sleep-interval 5",
        "-f \"ba\""
    };

    std::string command = "yt-dlp \"" + url + "\"";
    for (std::string arg : args) {
        command.append(" ");
        command.append(arg);
    }

    std::cout << command << "\n";

    int result = std::system(command.c_str());

    return result == 0;
}