//
//  cocSoundMixer.cpp
//  PwcCore
//
//  Created by Lukasz Karluk on 3/05/2016.
//
//

#include "cocSoundMixer.h"

namespace coc {

//--------------------------------------------------------------
SoundMixer::SoundMixer() {
    masterVolume = 1.0;
    masterPanning = 1.0;
}

SoundMixer::~SoundMixer() {
    //
}

//--------------------------------------------------------------
const SoundItem & SoundMixer::addSound(std::string soundPath, std::string soundID) {
    sounds.push_back(initSound());
    SoundItem & sound = *sounds.back();
    sound.soundPath = soundPath;
    sound.soundID = soundID;
    if(sound.soundID.length() == 0) {
        sound.soundID = sound.soundPath;
    }
    return sound;
}

void SoundMixer::removeSound(std::string soundID) {
    for(int i=0; i<sounds.size(); i++) {
        if(sounds[i]->soundID != soundID) {
            continue;
        }
        killSound(sounds[i]);
        sounds.erase(sounds.begin() + i);
        break;
    }
}

//--------------------------------------------------------------
void SoundMixer::load(std::string soundID) {
    // override
}

void SoundMixer::unload(std::string soundID) {
    // override
}

//--------------------------------------------------------------
void SoundMixer::setMasterVolume(float value) {
    masterVolume = value;
}

void SoundMixer::setMasterPanning(float value) {
    masterPanning = value;
}

//--------------------------------------------------------------
void SoundMixer::setVolume(std::string soundID, float value) {
    SoundItem * sound = getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }
    sound->volumeShape.clear();
    sound->volumeShape.push_back(SoundPoint(value));
}

void SoundMixer::setPanning(std::string soundID, float value) {
    SoundItem * sound = getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }
    sound->panningShape.clear();
    sound->panningShape.push_back(SoundPoint(value));
}

void SoundMixer::setNumOfPlays(std::string soundID, int numOfPlays) {
    SoundItem * sound = getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }
    sound->numOfTimesToPlay = numOfPlays;
}

//--------------------------------------------------------------
void SoundMixer::play(std::string soundID) {
    SoundItem * sound = getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }
    sound->bPlay = true;
}

void SoundMixer::stop(std::string soundID) {
    SoundItem * sound = getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }
    sound->bPlay = false;
}

void SoundMixer::pause(std::string soundID) {
    SoundItem * sound = getSoundPtr(soundID);
    if(sound == NULL) {
        return;
    }
    sound->bPause = true;
}

//--------------------------------------------------------------
void SoundMixer::update(float timeDelta) {

    for(int i=0; i<sounds.size(); i++) {
        SoundItem & sound = (SoundItem &)*sounds[i];
        
        if(sound.bLoaded == false) {
            continue;
        }
        
        sound.bPlay.update();
        sound.bLoop.update();
        
        float volume = masterVolume;
        float panning = masterPanning;
    
        sound.timeCurrent.update();
        if(sound.timeCurrent.hasChanged()) {
        
            sound.progress = coc::map(sound.timeCurrent, 0.0, sound.timeDuration, 0.0, 1.0, true);
            sound.progress.update();
            
            int numOfVolumePoints = sound.volumeShape.size();
            for(int i=0; i<numOfVolumePoints; i++) {
                if(numOfVolumePoints == 1) {
                    volume *= sound.volumeShape[0].value;
                    break;
                }
                
                if(i == numOfVolumePoints-1) {
                    break;
                }
                
                SoundPoint & p0 = sound.volumeShape[i+0];
                SoundPoint & p1 = sound.volumeShape[i+1];
                
                bool bInRange = true;
                bInRange = bInRange && (sound.progress >= p0.position);
                bInRange = bInRange && (sound.progress <= p1.position);
                if(bInRange == false) {
                    continue;
                }
                
                volume *= coc::map(sound.progress, p0.position, p1.position, p0.value, p1.value);
                
                break;
            }
            
            int numOfPanningPoints = sound.panningShape.size();
            for(int i=0; i<numOfPanningPoints; i++) {
                if(numOfPanningPoints == 1) {
                    panning *= sound.panningShape[0].value;
                    break;
                }
                
                if(i == numOfPanningPoints-1) {
                    break;
                }
                
                SoundPoint & p0 = sound.panningShape[i+0];
                SoundPoint & p1 = sound.panningShape[i+1];
                
                bool bInRange = true;
                bInRange = bInRange && (sound.progress >= p0.position);
                bInRange = bInRange && (sound.progress <= p1.position);
                if(bInRange == false) {
                    continue;
                }
                
                panning *= coc::map(sound.progress, p0.position, p1.position, p0.value, p1.value);
            }
        }
        
        sound.volume = volume;
        sound.volume.update();
        
        sound.panning = panning;
        sound.panning.update();
    }
}

//--------------------------------------------------------------
const SoundItem * SoundMixer::getSound(std::string soundID) {
    return getSoundPtr(soundID);
}

//--------------------------------------------------------------
SoundItem * SoundMixer::initSound() {
    return new SoundItem();
}

void SoundMixer::killSound(SoundItem * sound) {
    delete sound;
}

//--------------------------------------------------------------
SoundItem * SoundMixer::getSoundPtr(std::string soundID) {
    for(int i=0; i<sounds.size(); i++) {
        if(sounds[i]->soundID == soundID) {
            return sounds[i];
        }
    }
    return NULL;
}

};
