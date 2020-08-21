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

/// The SoundDatabase holds all the processed sound files that can be used at runtime. (singleton)
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

    /// Initializes the database, should only be called once at the start of the application.
    static void init();

    /** Plays a sound by it's given name.
     *
     * @param name the name of the sound file to play
     * @param looped If the sound should be looped. Default = false
     */
    static void playSoundByName(const std::string &name, bool looped = false);

    /** Returns the underlying sound source for a given sound file
     *
     * @param name the name of the sound file
     * @return If found, returns the sound source, otherwise empty.
     */
    static std::optional<std::shared_ptr<irrklang::ISoundSource>> getSoundSourceByName(const std::string &name);
};
