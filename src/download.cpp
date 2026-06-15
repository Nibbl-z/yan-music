#include "download.hpp"
#include "filesystem"
#include "fstream"
#include <string>
#include <iostream>
#include "globals.hpp"
#include "boost/process.hpp"
#include "algorithm"


#define SONGDB_FILE "song_db.json"

bool DownloadHandler::init(std::string directory) {
    if (std::filesystem::exists(directory)) { return true; }
    return std::filesystem::create_directory(directory);
}

bool DownloadHandler::initSongDbFile() {
    const char* homeDir = std::getenv("HOME");
    if (homeDir == nullptr) { return false; }

    std::string path = std::string(homeDir) + "/.config/" + CONFIG_FOLDER;
    std::filesystem::create_directory(path);

    dbPath = path + SONGDB_FILE;

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
    SongDB loaded = j.get<SongDB>();

    db = loaded;

    return true;
}

bool DownloadHandler::download() {
    std::unordered_map<std::string, int> removeCounts;
    std::vector<std::string> removedFromAll;

    for (std::string playlist : gConfig.config.playlists) {
        std::vector<std::string> removed;
        this->downloadPlaylist(playlist, gConfig.config.musicDirectory, &removed);

        for (std::string id : removed) {
            if (removeCounts.find(id) == removeCounts.end()) { removeCounts[id] = 0; }
            removeCounts[id]++;
        }
    }

    for (std::pair<std::string, int> entry : removeCounts) {
        if (entry.second == gConfig.config.playlists.size()) {
            removedFromAll.push_back(entry.first);
        }
    }

    for (std::filesystem::directory_entry const& file : std::filesystem::directory_iterator(gConfig.config.musicDirectory)) {
        std::string fileName = file.path().filename();
        for (std::string id : removedFromAll) {
            if (fileName.find(id) != std::string::npos) { 
                std::filesystem::remove(file.path());
                db.ids.erase(std::remove(db.ids.begin(), db.ids.end(), id), db.ids.end());

                break;
            }
        }
    }

    return true;
}

bool DownloadHandler::downloadPlaylist(std::string playlist, std::string directory, std::vector<std::string>* r) {
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

    std::string command = "yt-dlp";

    std::vector<std::string> oldSongs = db.ids;
    std::vector<std::string> added;
    std::vector<std::string> removed;

    // check new fetched ids with stored ids, and download individual videos
    for (std::string id : newSongs) {
        if(std::find(oldSongs.begin(), oldSongs.end(), id) == oldSongs.end()) {
            added.push_back(id);
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
        *r = removed;
    } else {
        std::cout << "no videos removed from playlist!" << "\n";
    }
    
    if (added.empty()) {
        std::cout << "no new videos found in playlist!" << "\n";
        return true;
    } else {
        for (std::string id : added) {
            db.ids.push_back(id);
        }
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