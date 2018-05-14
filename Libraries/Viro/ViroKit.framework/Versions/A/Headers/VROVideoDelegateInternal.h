//
//  VROVideoDelegateInternal.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/10/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoDelegateInternal_h
#define VROVideoDelegateInternal_h

#include <string>

class VROVideoDelegateInternal {
    
public:
    
    VROVideoDelegateInternal() {}
    virtual ~VROVideoDelegateInternal() {}

    virtual void videoWillBuffer() = 0;
    virtual void videoDidBuffer() = 0;
    virtual void videoDidFinish() = 0;
    virtual void onVideoUpdatedTime(float currentTimeInSeconds, float totalTimeInSeconds) = 0;
    virtual void videoDidFail(std::string error) = 0;
};

#endif /* VROVideoDelegateInternal_h */
