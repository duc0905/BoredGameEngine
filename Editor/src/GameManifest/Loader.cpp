#include "Loader.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>

#include "Manifest.hpp"

using nlohmann::json;

std::vector<std::string> splitString(std::string s, std::string delim){
    std::vector<std::string> res;
    int pos = 0;

    // while(pos < s.size()){
    while((pos = s.find(delim)) != std::string::npos){
        res.push_back(s.substr(0,pos));
        s.erase(0,pos + delim.length());
    }
    res.push_back(s);
    return res;
}

Manifest::Version ParseVersion(json data) {
    if (!data.contains("version")) {
        throw new std::runtime_error("Field 'version' is missing from Bored.json");
    }

    if (!data["version"].is_string()) {
        throw new std::runtime_error("Field 'version' must be of type string");
    }

    std::string version = data["version"];
    auto vs = splitString(version, ".");
    Manifest::Version v;
    if (vs.size() == 0) {
        throw new std::format_error(std::format("Invalid version number: %s", version).c_str());
    }

    v.major = std::stoi(vs[0]);
    if (vs.size() > 1)  v.minor = std::stoi(vs[1]);
    else v.minor = 0;
    if (vs.size() > 2)  v.patch = std::stoi(vs[2]);
    else v.patch = 0;

    return v;
}

std::string ParseName(json data) {
    if (!data.contains("name")) {
        throw new std::runtime_error("Field 'name' is missing from Bored.json");
    }

    if (!data["name"].is_string()) {
        throw new std::runtime_error("Field 'name' must be of type string");
    }

    return data["name"];
}

std::string ParseDescription(json data) {
    if (!data.contains("description")) {
        throw new std::runtime_error("Field 'description' is missing from Bored.json");
    }

    if (!data["description"].is_string()) {
        throw new std::runtime_error("Field 'description' must be of type string");
    }

    return data["description"];
}

std::string ParsePath(const std::string& file) {
    // TODO:
    return "";
}

/* TODO: Implement */
Manifest LoadManifestFile(const std::string& file)
{
    Manifest m;

    std::ifstream f(file);

    std::cout << "Filename: " << file << std::endl;

    if (!f.is_open()) {
        std::cout << "file dne\n";
        return {};
    }

    json data;
    try {
        data = json::parse(f);
    } catch(std::exception e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw std::runtime_error(std::format("Cannot parse manifest file: %s", file));
    }

    m.name = ParseName(data);
    m.description = ParseDescription(data);
    m.version = ParseVersion(data);
    m.path = ParsePath(file);

    return m;
}

void PrintManifest(const Manifest& game) {
    std::cout << "Name: " << game.name << std::endl
            << "Description: " << game.description << std::endl
            << "Version: " << game.version.major
                    << "." << game.version.minor
                    << "." << game.version.patch
                    << std::endl;
    std::cout << "Core modules: \n";
    for (auto x : game.core_modules)
        std::cout << x.first << std::endl;

    std::cout << "Custom modules: \n";
    for (auto x : game.custom_modules)
        std::cout << x.first << std::endl;

    std::cout << "Path: " << game.path << std::endl;

}
