//
//  CCTreeAnimationWeights.cpp
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

#include "CCTreeAnimationWeights.h"

CCTreeAnimationWeights::CCTreeAnimationWeights() {
    
}

std::map<int, float> CCTreeAnimationWeights::getHeadWeights() const {
    return {};
}

std::map<int, float> CCTreeAnimationWeights::getLeftLegWeights() const {
    std::map<int, float> weights;
    
    return weights;
}

std::map<int, float> CCTreeAnimationWeights::getRightLegWeights() const {
    std::map<int, float> weights;
    
    return weights;
}

std::map<int, float> CCTreeAnimationWeights::getLeftArmWeights() const {
    float heavy = 0.95;
    
    std::map<int, float> weights;
    weights[34] = heavy;
    weights[35] = heavy;
    weights[36] = heavy;
    weights[37] = heavy;
    weights[38] = heavy;
    weights[39] = heavy;
    
    return weights;
}

std::map<int, float> CCTreeAnimationWeights::getRightArmWeights() const {
    float heavy = 0.95;
    
    std::map<int, float> weights;
    weights[10] = heavy;
    weights[11] = heavy;
    weights[12] = heavy;
    weights[13] = heavy;
    weights[14] = heavy;
    weights[15] = heavy;
    
    weights[17] = heavy;
    weights[18] = heavy;
    weights[19] = heavy;
    
    weights[21] = heavy;
    weights[22] = heavy;
    weights[23] = heavy;
    
    weights[25] = heavy;
    weights[26] = heavy;
    weights[27] = heavy;
    
    weights[29] = heavy;
    weights[30] = heavy;
    weights[31] = heavy;
    
    return weights;
}
