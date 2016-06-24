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

#include "cocCore.h"

namespace coc {

//--------------------------------------------------------------
class SoundPoint {

public:

    SoundPoint(float value, float position=0) {
        this->value = value;
        this->position = position;
    }

    float value;
    float position;
};

//--------------------------------------------------------------
class SoundItem {

public:

    SoundItem() :
    soundPath(""), soundID(""),
    bLoad(false), bPlay(false), bPause(false), bLoop(false),
    timeCurrent(0.0), timeDuration(0.0), progress(0.0),
    numOfTimesPlayed(0), numOfTimesToPlay(1),
    volume(1.0), panning(0.5) {
        
        volumeShape.push_back(SoundPoint(volume));
        panningShape.push_back(SoundPoint(panning));
    }
    
    std::string soundPath;
    std::string soundID;
    
    Value<bool> bLoad;
    Value<bool> bPlay;
    Value<bool> bPause;
    Value<bool> bLoop;
    
    Value<float> timeCurrent;
    Value<float> timeDuration;
    Value<float> progress;

    Value<int> numOfTimesPlayed;
    Value<int> numOfTimesToPlay;

    Value<float> volume;
    std::vector<SoundPoint> volumeShape;
    
    Value<float> panning;
    std::vector<SoundPoint> panningShape;
};

//--------------------------------------------------------------
class SoundMixer {
    
public:
    
    SoundMixer();
    ~SoundMixer();
    
    virtual const SoundItem & addSound(std::string soundPath, std::string soundID="");
    virtual void removeSound(std::string soundID);
    
    virtual void load(std::string soundID);
    virtual void unload(std::string soundID);
    
    void setMasterVolume(float value);
    void setMasterPanning(float value);
    
    virtual void setVolume(std::string soundID, float value);
    virtual void setPanning(std::string soundID, float value);
    virtual void setNumOfPlays(std::string soundID, int numOfPlays);
    
    virtual void play(std::string soundID);
    virtual void stop(std::string soundID);
    virtual void pause(std::string soundID);
    
    virtual void update(float timeDelta=0);
    
    const SoundItem * getSound(std::string soundID);
    
protected:

    virtual SoundItem * initSound();
    virtual void killSound(SoundItem * sound);
    
    virtual void updateVolume(SoundItem & sound);
    virtual void updatePanning(SoundItem & sound);
    
    SoundItem * getSoundPtr(std::string soundID);
    
    Value<float> masterVolume;
    Value<float> masterPanning;
    std::vector<SoundItem *> sounds;
};

};