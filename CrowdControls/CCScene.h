//
//  CCScene.hpp
//  CrowdControls
//
//  Created by Raj Advani on 8/8/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
//

#ifndef CCScene_hpp
#define CCScene_hpp

#include <memory>
#include <ViroKit/ViroKit.h>

class CCScene {
public:
    
    CCScene() {}
    virtual ~CCScene() {}
    
    /*
     Build the scene.
     */
    virtual void build(id <VROView> view,
                       std::shared_ptr<VRORenderer> renderer,
                       std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                       std::shared_ptr<VRODriver> driver) = 0;
    virtual std::shared_ptr<VRONode> getPointOfView() = 0;
    virtual std::shared_ptr<VROSceneController> getSceneController() = 0;
    
    /*
     Start the next sequence of this scene, and let it run for the given number
     of seconds. The precise sequence started depends on how the subclass has been
     parameterized.
     */
    virtual void startSequence(float durationSeconds, std::function<void(CCScene *)> onFinished) = 0;
    
};

#endif /* CCScene_hpp */
