//
// Created by Willi on 8/20/2020.
//
#include "../include/sound_database.h"

SoundDatabase SoundDatabase::instance;

void SoundDatabase::init() {

    LOG(INFO) << "Initializing Sound Database ...";

    for (const auto &file : fs::directory_iterator(fs::current_path().string() + "/resources/sounds/")) {
        LOG(INFO) << "Processing " + file.path().filename().string() + " for sound data.";

        instance.sounds[file.path().filename().string()] = std::shared_ptr<irrklang::ISoundSource>(
                instance.soundEngine->addSoundSourceFromFile(file.path().string().c_str()));

        LOG(INFO) << "Finished processing " + file.path().filename().string() + ".";
    }
}

void SoundDatabase::playSoundByName(const std::string &name, bool looped) {

    if (instance.sounds.find(name) == instance.sounds.end()) {
        LOG(INFO) << "Did not find sound by name " << name;
        return;
    } else {
        instance.soundEngine->play2D(instance.sounds[name].get(), looped);
    }
}

std::optional<std::shared_ptr<irrklang::ISoundSource>> SoundDatabase::getSoundSourceByName(const std::string &name) {
    if (instance.sounds.find(name) == instance.sounds.end()) {
        return {};
    } else {
        return {instance.sounds[name]};
    }
}

SoundDatabase::SoundDatabase() {
    LOG(INFO) << "Creating irrKLang device";
    this->soundEngine = std::unique_ptr<irrklang::ISoundEngine>(irrklang::createIrrKlangDevice());
}

SoundDatabase::~SoundDatabase() {
    for (auto &pair : sounds) {
        pair.second->drop();
    }
}
