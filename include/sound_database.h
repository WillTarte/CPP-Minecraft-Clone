//
// Created by Willi on 8/20/2020.
//
#pragma once

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include "../libs/easylogging++.h"
#include "../libs/irrKLang/include/irrKlang.h"

#if defined(__unix__) || _MSC_VER >= 1914
namespace fs = std::filesystem;
#else
namespace fs = std::filesystem;
#endif

class SoundDatabase {
private:
    SoundDatabase();

    SoundDatabase(const SoundDatabase &) = delete;

    SoundDatabase &operator=(const SoundDatabase &) = delete;

    static SoundDatabase instance;
    std::unordered_map<std::string, std::shared_ptr<irrklang::ISoundSource>> sounds{};
    std::unique_ptr<irrklang::ISoundEngine> soundEngine;

public:
    ~SoundDatabase();

    static void init();

    static void playSoundByName(const std::string &name, bool looped = false);

    static std::optional<std::shared_ptr<irrklang::ISoundSource>> getSoundSourceByName(const std::string &name);
};
