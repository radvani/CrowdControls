//
//  VROControllerInputType.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROInputType_h
#define VROInputType_h

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <float.h>

/**
 * Header file containing sets of input sources that are grouped to
 * a VR platform.
 */
namespace ViroDayDream{
    enum InputSource{
        Controller = 1,
        TouchPad = 2,
        AppButton = 3,
        HomeButton = 4,
        VolUpButton = 5,
        VolDownButton = 6
    };
}

namespace ViroCardBoard{
    enum InputSource{
        Controller=1,
        ViewerButton = 2
    };
}

namespace ViroOculus{
    enum InputSource{
        Controller = 1,
        TouchPad = 2,
        BackButton = 3
    };
}
#endif
