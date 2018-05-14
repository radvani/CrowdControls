//
//  VROAudioPlayer.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/22/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROAudioPlayer_h
#define VROAudioPlayer_h

#include <string>
#include <memory>
#include "VROSoundDelegateInternal.h"

class VROData;

class VROAudioPlayer {
    
public:
    
    virtual ~VROAudioPlayer() {}
  
    virtual void setup() = 0;
    virtual void setDelegate(std::shared_ptr<VROSoundDelegateInternal> delegate) {
      _delegate = delegate;
    }
  
    virtual void setLoop(bool loop) = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void setVolume(float volume) = 0;
    virtual void setMuted(bool muted) = 0;
    virtual void seekToTime(float seconds) = 0;

protected:
    std::shared_ptr<VROSoundDelegateInternal> _delegate;
};

#endif /* VROAudioPlayer_h */
