#include "download.hpp"
#include "filesystem"
#include "fstream"
#include <string>
#include <iostream>
#include "globals.hpp"
#include "boost/process.hpp"
#include "algorithm"


#define PLAYLISTDB_FILE "playlist_db.json"

bool DownloadHandler::init(std::string directory) {
    if (std::filesystem::exists(directory)) { return true; }
    return std::filesystem::create_directory(directory);
}

bool DownloadHandler::initPlaylistDbFile() {
    const char* homeDir = std::getenv("HOME");
    if (homeDir == nullptr) { return false; }

    std::string path = std::string(homeDir) + "/.config/" + CONFIG_FOLDER;
    std::filesystem::create_directory(path);

    dbPath = path + PLAYLISTDB_FILE;

    if (std::filesystem::exists(dbPath)) {
        this->loadDb();
        return true;
    }

    std::ofstream stream(dbPath);

    nlohmann::json j = defaultDB;

    stream << j;
    stream.close();

    db = defaultDB;
    
    return true;
}

bool DownloadHandler::saveDb() {
    if (dbPath.empty()) {
        return false;
    }

    std::ofstream stream(dbPath);

    nlohmann::json j = db;

    stream << j;
    stream.close();

    return true;
}

bool DownloadHandler::loadDb() {
    if (dbPath.empty()) {
        return false;
    }

    std::ifstream stream(dbPath);
    std::ostringstream contents;

    contents << stream.rdbuf();

    nlohmann::json j = nlohmann::json::parse(contents.str());
    PlaylistDB loaded = j.get<PlaylistDB>();

    db = loaded;

    return true;
}

bool DownloadHandler::downloadPlaylist(std::string playlist, std::string directory) {
    std::string fetchIdsCommand = "yt-dlp --flat-playlist --get-id \"https://www.youtube.com/playlist?list=" + playlist + "\"";
    boost::process::ipstream pipeStream;

    boost::process::child c(fetchIdsCommand, boost::process::std_out > pipeStream);

    std::vector<std::string> newSongs;
    std::string line;

    while (pipeStream && std::getline(pipeStream, line)) {
        newSongs.push_back(line);
    }

    c.wait();

    for (std::string id : newSongs) {
        std::cout << id << "\n";
    }

    std::string command = "yt-dlp \"https://www.youtube.com/playlist?list=" + playlist + "\"";
    

    if (db.ids.find(playlist) == db.ids.end()) {
        db.ids[playlist] = newSongs;
        command = "yt-dlp \"https://www.youtube.com/playlist?list=" + playlist + "\"";
    } else {
        command = "yt-dlp";
        std::vector<std::string> oldSongs = db.ids[playlist];
        std::vector<std::string> removed;
        bool newID = false;

        // check new fetched ids with stored ids, and download individual videos
        for (std::string id : newSongs) {
            if(std::find(oldSongs.begin(), oldSongs.end(), id) == oldSongs.end()) {
                newID = true;
                command.append(" \"https://www.youtube.com/watch?v=" + id + "\"");
            }
        }

        // check if ids have been removed and remove them
        for (std::string id : oldSongs) {
            if(std::find(newSongs.begin(), newSongs.end(), id) == newSongs.end()) {
                removed.push_back(id);
            }
        }

        if (!removed.empty()) {
            for (std::filesystem::directory_entry const& file : std::filesystem::directory_iterator(directory)) {
                std::string fileName = file.path().filename();
                for (std::string id : removed) {
                    if (fileName.find(id) != std::string::npos) { 
                        std::filesystem::remove(file.path());
                        break;
                    }
                }
            }
        }

        if (!newID) {
            std::cout << "no new videos found in playlist!" << "\n";
            return true;
        }

        db.ids[playlist] = newSongs;
    }

    std::string args[] = {
        "-P \"" + directory + "\"",
    };

    
    for (std::string arg : args) {
        command.append(" ");
        command.append(arg);
    }

    for (std::string arg : gConfig.config.ytdlpFlags) {
        command.append(" ");
        command.append(arg);
    }

    std::cout << command << "\n";

    int result = std::system(command.c_str());
    return result == 0;
}