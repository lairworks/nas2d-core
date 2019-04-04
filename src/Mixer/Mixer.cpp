// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "NAS2D/Mixer/Mixer.h"

#include "NAS2D/Resources/Sound.h"
#include "NAS2D/Resources/Music.h"

namespace NAS2D {

    Mixer::Mixer(const std::string& name) : mName(name) {}

    void Mixer::playSound(Sound& /*sound*/) {}
    void Mixer::stopSound() {}
    void Mixer::pauseSound() {}
    void Mixer::resumeSound() {}
    void Mixer::playMusic(Music& music, int loops /*= Mixer::CONTINUOUS*/) {
        fadeInMusic(music, loops, 0);
    }
    void Mixer::stopMusic() {}
    void Mixer::pauseMusic() {}
    void Mixer::resumeMusic() {}
    void Mixer::fadeInMusic(Music& /*music*/, int /*loops*/ /*= Mixer::CONTINUOUS*/, int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/) {}
    void Mixer::fadeOutMusic(int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/) {}
    bool Mixer::musicPlaying() const {
        return false;
    }
    void Mixer::mute() {}
    void Mixer::unmute() {}
    void Mixer::stopAllAudio() {
        stopMusic();
        stopSound();
    }
    void Mixer::pauseAllAudio() {
        pauseMusic();
        pauseSound();
    }
    void Mixer::resumeAllAudio() {
        resumeMusic(); resumeSound();
    }
    void Mixer::soundVolume(int /*level*/) {}
    void Mixer::musicVolume(int /*level*/) {}
    const std::string& Mixer::name() const {
        return mName;
    }
    NAS2D::Signals::Signal0<void>& Mixer::musicComplete() {
        return _music_complete;
    }

} // namespace
