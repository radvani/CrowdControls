//
//  CCTreeAnimationWeights.hpp
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

#ifndef CCTreeAnimationWeights_hpp
#define CCTreeAnimationWeights_hpp

#include "CCAnimationWeights.h"
#include <vector>
#include <map>

class CCTreeAnimationWeights : public CCAnimationWeights {
public:
    
    CCTreeAnimationWeights();
    virtual ~CCTreeAnimationWeights() {}
    
    virtual std::map<int, float> getHeadWeights() const;
    virtual std::map<int, float> getLeftArmWeights() const;
    virtual std::map<int, float> getRightArmWeights() const;
    virtual std::map<int, float> getLeftLegWeights() const;
    virtual std::map<int, float> getRightLegWeights() const;
    
private:
    
    std::vector<int> _leftLegOnly;
    std::vector<int> _rightLegOnly;
    std::vector<int> _leftArmOnly;
    std::vector<int> _rightArmOnly;
    std::vector<int> _headOnly;
    std::vector<int> _body;
    
    std::map<int, float> _headWeights;
    std::map<int, float> _leftArmWeights;
    std::map<int, float> _rightArmWeights;
    std::map<int, float> _leftLegWeights;
    std::map<int, float> _rightLegWeights;
    
};

#endif /* CCTreeAnimationWeights_hpp */
