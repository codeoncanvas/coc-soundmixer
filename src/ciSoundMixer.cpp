//
//  ciSoundMixer.cpp
//  PwcCore
//
//  Created by Lukasz Karluk on 3/05/2016.
//
//

#include "ciSoundMixer.h"
#include "cinder/audio/audio.h"

namespace coc {

//--------------------------------------------------------------
ciSoundMixer::ciSoundMixer() : coc::SoundMixer() {
    //
}

ciSoundMixer::~ciSoundMixer() {
    //
}

//--------------------------------------------------------------
void ciSoundMixer::load(std::string soundID) {
    ciSoundItem * sound = (ciSoundItem *)getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }

    sound->soundFileRef = ci::audio::load(ci::app::loadAsset(sound->soundPath));
    sound->soundRef = ci::audio::Voice::create(sound->soundFileRef);
    
    sound->bLoaded = true;
    sound->timeDuration = sound->soundFileRef->getNumSeconds();
}

void ciSoundMixer::unload(std::string soundID) {
    ciSoundItem * sound = (ciSoundItem *)getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }
    
    // unload cinder audio file.

    sound->bLoaded = false;
    sound->timeDuration = 0.0;
}

//--------------------------------------------------------------
void ciSoundMixer::update(float timeDelta) {

    for(int i=0; i<sounds.size(); i++) {
        ciSoundItem & sound = (ciSoundItem &)*sounds[i];
        sound.timeCurrent = sound.soundFileRef->getReadPositionSeconds();
    }
    
    coc::SoundMixer::update(timeDelta);
    
    for(int i=0; i<sounds.size(); i++) {
        ciSoundItem & sound = (ciSoundItem &)*sounds[i];
        
        if(sound.bLoaded == false) {
            continue;
        }
        
        if(sound.bPlay.hasChanged()) {
        
        }
        
        if(sound.bPause.hasChanged()) {
        
        }
    }
}

//--------------------------------------------------------------
ciSoundItem * ciSoundMixer::initSound() {
    return new ciSoundItem();
}

void ciSoundMixer::killSound(coc::SoundItem * sound) {
    delete (ciSoundItem *)sound;
}

};