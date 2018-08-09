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

class CCModelUtil {
public:
    
    /*
     Load the given bundled FBX model and position it accordingly.
     */
    static std::shared_ptr<VRONode> loadFBXModel(std::string model, VROVector3f position, VROVector3f scale,
                                                 std::shared_ptr<VRODriver> driver);
    
    /*
     Load the given bundled Radiance HDR texture.
     */
    static std::shared_ptr<VROTexture> loadRadianceHDRTexture(std::string texture);
    
};

#endif /* CCModelUtil_hpp */
