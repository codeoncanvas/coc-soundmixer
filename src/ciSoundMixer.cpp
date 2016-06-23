/**
 *
 *      ┌─┐╔═╗┌┬┐┌─┐
 *      │  ║ ║ ││├┤
 *      └─┘╚═╝─┴┘└─┘
 *   ┌─┐┌─┐╔╗╔┬  ┬┌─┐┌─┐
 *   │  ├─┤║║║└┐┌┘├─┤└─┐
 *   └─┘┴ ┴╝╚╝ └┘ ┴ ┴└─┘
 *
 * Copyright (c) 2016 Code on Canvas Pty Ltd, http://CodeOnCanvas.cc
 *
 * This software is distributed under the MIT license
 * https://tldrlegal.com/license/mit-license
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code
 *
 **/

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
void ciSoundMixer::update(float timeDelta) {

    for(int i=0; i<sounds.size(); i++) {
        ciSoundItem & sound = (ciSoundItem &)*sounds[i];
        
        sound.bLoad.update();
        sound.bPlay.update();
        sound.bPause.update();
        sound.bLoop.update();
        
        if(sound.bLoad.hasChanged()) {
            
            if(sound.bLoad) {
            
                sound.soundFileRef = ci::audio::load(ci::app::loadAsset(sound.soundPath));
                sound.soundRef = ci::audio::Voice::create(sound.soundFileRef);
                sound.timeDuration = sound.soundFileRef->getNumSeconds();
            
            } else {
            
                sound.soundRef = NULL;
                sound.soundFileRef = NULL;
                sound.bLoad = false;
                sound.bPlay = false;
                sound.bPause = false;
                sound.timeCurrent = 0;
                sound.timeDuration = 0;
                sound.progress = 0;
                sound.numOfTimesPlayed = 0;
            }
        }
        
        if(sound.bLoad == false) {
            continue;
        }
        
        if(sound.bPlay.hasChanged()) {
            
            if(sound.bPlay) {
            
                sound.soundRef->start();
            
            } else {
            
                sound.soundRef->stop();
            }
        }
        
        if(sound.bPause.hasChanged()) {
        
            if(sound.bPause) {
            
                sound.soundRef->pause();
            
            } else {
            
                sound.soundRef->start();
            }
        }
        
        sound.timeCurrent = sound.soundFileRef->getReadPositionSeconds();
        sound.timeCurrent.update();
        if(sound.timeCurrent.hasChanged()) {
        
            sound.progress = coc::map(sound.timeCurrent, 0.0, sound.timeDuration, 0.0, 1.0, true);
            sound.progress.update();
            
            updateVolume(sound);
            updatePanning(sound);
        }
        
        if(sound.volume.hasChanged()) {
        
            sound.soundRef->setVolume(sound.volume);
        }
        
        if(sound.panning.hasChanged()) {
        
            sound.soundRef->setPan(sound.panning);
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