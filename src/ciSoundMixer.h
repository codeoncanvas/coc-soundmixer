//
//  ciSoundMixer.hpp
//  PwcCore
//
//  Created by Lukasz Karluk on 3/05/2016.
//
//

#pragma once

#include "cocSoundMixer.h"
#include "cinder/audio/audio.h"

namespace coc {

//--------------------------------------------------------------
class ciSoundItem : public coc::SoundItem {

public:

    ciSoundItem() : coc::SoundItem() {
            //
    }
    
    ci::audio::SourceFileRef soundFileRef;
    ci::audio::VoiceRef soundRef;
};

//--------------------------------------------------------------
class ciSoundMixer : public coc::SoundMixer {

public:

    ciSoundMixer();
    ~ciSoundMixer();
    
    void load(std::string soundID) override;
    void unload(std::string soundID) override;
    
    void update(float timeDelta=0) override;
    
protected:

    ciSoundItem * initSound() override;
    void killSound(coc::SoundItem * sound) override;

};

};