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
    
    virtual void update(float timeDelta=0) override;
    
protected:

    ciSoundItem * initSound() override;
    void killSound(coc::SoundItem * sound) override;

};

};