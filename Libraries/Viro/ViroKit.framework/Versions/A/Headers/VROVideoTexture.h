//
//  VROVideoTexture.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoTexture_h
#define VROVideoTexture_h

#include "VROFrameListener.h"
#include "VROTexture.h"
#include <memory>
#include "VROVideoDelegateInternal.h"
#include "VROTime.h"
#include <math.h>       /* floor */
#include <cmath>

class VRORenderContext;
class VROFrameSynchronizer;
class VRODriver;

class VROVideoTexture : public VROTexture, public VROFrameListener {
    
public:
    
    VROVideoTexture(VROTextureType type, VROStereoMode state = VROStereoMode::None) :
        VROTexture(type, VROTextureInternalFormat::RGBA8, state) {}
    virtual ~VROVideoTexture() {}
    
    /*
     Delete any rendering resources. Invoked prior to destruction, on the
     rendering thread.
     */
    virtual void deleteGL() {}

    /*
     Use this video texture to display the contents of the given URL. The video
     will not run until play() is invoked.
     */
    virtual void loadVideo(std::string url,
                           std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                           std::shared_ptr<VRODriver> driver) = 0;
    
    /*
     Perform video initialization (which causes a stutter) early.
     */
    virtual void prewarm() = 0;
    
    virtual void onFrameWillRender(const VRORenderContext &context) {}
    virtual void onFrameDidRender(const VRORenderContext &context) {}
    
    virtual void pause() = 0;
    virtual void play() = 0;
    virtual bool isPaused() = 0;

    virtual void seekToTime(float seconds) = 0;
    virtual float getCurrentTimeInSeconds() = 0;
    virtual float getVideoDurationInSeconds() = 0;

    virtual void setMuted(bool muted) = 0;
    virtual void setVolume(float volume) = 0;
    virtual void setLoop(bool loop) = 0;
    
    virtual void setDelegate(std::shared_ptr<VROVideoDelegateInternal> delegate) {
        _delegate = delegate;
    }
    
protected:
  
    std::weak_ptr<VROVideoDelegateInternal> _delegate;

    /*
     Notifies delegates about the video player's current time, per second.
     */
    void updateVideoTime(){
        std::shared_ptr<VROVideoDelegateInternal> delegate = _delegate.lock();
        if (!delegate) {
            return;
        }

        /*
         Reduce the amount of JNI Calls to getCurrentTimeInSeconds() to
         a per-second basis - the lowest unit of time currently used by
         video players. Thus, _lastVideoTimeGetAttempt is used to filter
         the amount of calls made.
         */
        double currentRenderTime = floor(VROTimeCurrentSeconds());
        if (_lastVideoTimeGetAttempt == currentRenderTime) {
            return;
        }
        _lastVideoTimeGetAttempt = currentRenderTime;

        /*
         Only notify delegates if the last known CurrentVideoTime returned
         from the AVPlayer has changed. Also, we're doing it by integer seconds
         */
        int currentVideoTimeInSeconds = getCurrentTimeInSeconds();
        if (_lastCurrentVideoTimeInSeconds != currentVideoTimeInSeconds) {
            // if a video just starts, then getVideoDurationInSeconds returns NaN, do nothing in this case.
            float duration = getVideoDurationInSeconds();
            if (!isnan(duration)) {
                delegate->onVideoUpdatedTime(currentVideoTimeInSeconds, duration);
                _lastCurrentVideoTimeInSeconds = currentVideoTimeInSeconds;
            }
        }
    }

private:
    
    /*
     Last time stamp at which we attempted a JNI call getCurrentTimeInSeconds() from the
     video player.
     */
    double _lastVideoTimeGetAttempt  = -1;

    /*
     Last known current video time that was retrieved from the player.
     */
    int _lastCurrentVideoTimeInSeconds = -1;
    
};

#endif /* VROVideoTexture_h */
