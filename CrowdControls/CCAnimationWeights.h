//
//  CCAnimationWeights.h
//  CrowdControls
//
//  Created by Raj Advani on 8/16/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:

//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef CCAnimationWeights_h
#define CCAnimationWeights_h

#include <map>

class CCAnimationWeights {
public:
    
    CCAnimationWeights() {}
    virtual ~CCAnimationWeights() {}
    
    virtual std::map<int, float> getHeadWeights() const = 0;
    virtual std::map<int, float> getLeftArmWeights() const = 0;
    virtual std::map<int, float> getRightArmWeights() const = 0;
    virtual std::map<int, float> getLeftLegWeights() const = 0;
    virtual std::map<int, float> getRightLegWeights() const = 0;
    
};

#endif /* CCAnimationWeights_h */
