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
#include "cinder/app/App.h"
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
            
                sound.sourceFileRef = ci::audio::load(ci::app::loadAsset(sound.soundPath));
                sound.voiceSamplePlayerNodeRef = ci::audio::Voice::create(sound.sourceFileRef);
                sound.samplePlayerNodeRef = sound.voiceSamplePlayerNodeRef->getSamplePlayerNode();
                
                float duration = sound.samplePlayerNodeRef->getNumSeconds();
                sound.timeDuration = duration;
                sound.timeDuration.update();
            
            } else {
            
                sound.samplePlayerNodeRef = NULL;
                sound.voiceSamplePlayerNodeRef = NULL;
                sound.sourceFileRef = NULL;
                
                sound.bPlay = false;
                sound.bPlay.update();
                
                sound.bPause = false;
                sound.bPause.update();
                
                sound.timeCurrent = 0.0;
                sound.timeCurrent.update();
                
                sound.timeDuration = 0.0;
                sound.timeDuration.update();
                
                sound.progress = 0.0;
                sound.progress.update();
                
                sound.numOfTimesPlayed = 0;
                sound.numOfTimesPlayed.update();
            }
        }
        
        if(sound.bLoad == false) {
            continue;
        }
        
        if(sound.bPlay.hasChanged()) {
            
            if(sound.bPlay) {
            
                sound.voiceSamplePlayerNodeRef->start();
            
            } else {
            
                sound.voiceSamplePlayerNodeRef->stop();
            }
        }
        
        if(sound.bPause.hasChanged()) {
        
            if(sound.bPause) {
            
                sound.voiceSamplePlayerNodeRef->pause();
            
            } else {
            
                sound.voiceSamplePlayerNodeRef->start();
            }
        }
        
        float progress = sound.samplePlayerNodeRef->getReadPosition() / (float)sound.samplePlayerNodeRef->getNumFrames();
        sound.progress = progress;
        sound.progress.update();
        
        if(sound.progress.hasChanged()) {

            sound.timeCurrent = coc::map(sound.progress, 0.0, 1.0, 0.0, sound.timeDuration, true);
            sound.timeCurrent.update();
            
            updateVolume(sound);
            if(sound.volume.hasChanged()) {
                sound.voiceSamplePlayerNodeRef->setVolume(sound.volume);
            }

            updatePanning(sound);
            if(sound.panning.hasChanged()) {
                sound.voiceSamplePlayerNodeRef->setPan(sound.panning);
            }
        }
        
        bool bFinished = true;
        bFinished = bFinished && (sound.bPlay == true);
        bFinished = bFinished && (sound.progress == 1.0f);
        bFinished = bFinished && (sound.progress.hasChanged() == false);
        if(bFinished == true) {
            
            sound.samplePlayerNodeRef->seek(0);
            
            sound.timeCurrent = 0.0;
            sound.timeCurrent.update();
            
            sound.progress = 0.0;
            sound.progress.update();
            
            sound.numOfTimesPlayed = sound.numOfTimesPlayed + 1;
            
            bool bPlayAgain = true;
            bPlayAgain = bPlayAgain && (sound.bLoop == true);
            bPlayAgain = bPlayAgain && (sound.numOfTimesPlayed < sound.numOfTimesToPlay);
            if(bPlayAgain == true) {
            
                sound.voiceSamplePlayerNodeRef->start();
            
            } else {
            
                sound.voiceSamplePlayerNodeRef->stop();
                
                sound.bPlay = false;
                sound.bPlay.update();
                
                sound.numOfTimesPlayed = 0;
                sound.numOfTimesPlayed.update();
            }
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