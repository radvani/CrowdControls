//
//  CCModelUtil.hpp
//  CrowdControls
//
//  Created by Raj Advani on 8/8/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
//

#ifndef CCModelUtil_hpp
#define CCModelUtil_hpp

#include <ViroKit/ViroKit.h>
#include "CCAnimationWeights.h"
#include "CCTypes.h"

class CCModelUtil {
public:
    
    /*
     Load the given bundled FBX model and position it accordingly.
     */
    static std::shared_ptr<VRONode> loadFBXModel(std::string model, VROVector3f position, VROVector3f scale,
                                                 std::shared_ptr<CCAnimationWeights> weights,
                                                 std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> *animationCache,
                                                 std::shared_ptr<VRODriver> driver,
                                                 std::function<void()> onModelLoaded);
    
    /*
     Preload *all* animation combinations for the given model.
     */
    static void preloadAnimations(std::shared_ptr<VRONode> model, std::shared_ptr<CCAnimationWeights> weights,
                                  std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> *cache);
    
    /*
     Preload the given animation and return it.
     */
    static std::shared_ptr<VROExecutableAnimation> preloadAnimation(std::shared_ptr<VRONode> node, std::shared_ptr<CCAnimationWeights> weights,
                                                                    std::map<CCSkeletonWeights, std::vector<std::string>> animation);
    
    /*
     Load the given bundled Radiance HDR texture.
     */
    static std::shared_ptr<VROTexture> loadRadianceHDRTexture(std::string texture);
    
};

#endif /* CCModelUtil_hpp */
