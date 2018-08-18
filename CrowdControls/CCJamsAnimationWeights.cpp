//
//  CCJamsAnimationWeights.cpp
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

#include "CCJamsAnimationWeights.h"
#include <vector>

CCJamsAnimationWeights::CCJamsAnimationWeights() {
    // Body only are bones that used by Head, Body, LeftLeg, RightLeg, LeftLegBodyOn, and RightLegBodyOn
    _headOnly = { 1, 2, 3, 4 };
    _leftLegOnly  = { 0, 5, 6, 7, 8, 59, 60, 61, 62, 63 };
    _rightLegOnly = { 0, 5, 6, 7, 8, 64, 65, 66, 67, 68 };

    // The arm weights are used only by the left and right arm, only
    _leftArmOnly = {  33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56 };
    _rightArmOnly = {  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };
    
    // We don't use 0.0 as a weight because if we did, Viro would discard that layer of the animation
    // entirely. By using 0.01, we ensure when two animations are blended, the one with 0.99
    // will have the full impact, but whe only the 0.01 animation is enabled, it is normalized
    // to the full weight (1.0) instead of being discarded.
    float fullWeight = 0.99;
    float zeroWeight = 0.01;
    
    for (int boneIndex : _headOnly) {
        _headWeights[boneIndex] = fullWeight;
        _leftArmWeights[boneIndex] = zeroWeight;
        _rightArmWeights[boneIndex] = zeroWeight;
        _leftLegWeights[boneIndex] = zeroWeight;
        _rightLegWeights[boneIndex] = zeroWeight;
    }
    for (int boneIndex : _leftArmOnly) {
        _headWeights[boneIndex] = zeroWeight;
        _leftArmWeights[boneIndex] = fullWeight;
        _rightArmWeights[boneIndex] = zeroWeight;
        _leftLegWeights[boneIndex] = zeroWeight;
        _rightLegWeights[boneIndex] = zeroWeight;
    }
    for (int boneIndex : _rightArmOnly) {
        _headWeights[boneIndex] = zeroWeight;
        _leftArmWeights[boneIndex] = zeroWeight;
        _rightArmWeights[boneIndex] = fullWeight;
        _leftLegWeights[boneIndex] = zeroWeight;
        _rightLegWeights[boneIndex] = zeroWeight;
    }
    for (int boneIndex : _leftLegOnly) {
        _headWeights[boneIndex] = zeroWeight;
        _leftArmWeights[boneIndex] = zeroWeight;
        _rightArmWeights[boneIndex] = zeroWeight;
        _leftLegWeights[boneIndex] = fullWeight;
        _rightLegWeights[boneIndex] = zeroWeight;
    }
    for (int boneIndex : _rightLegOnly) {
        _headWeights[boneIndex] = zeroWeight;
        _leftArmWeights[boneIndex] = zeroWeight;
        _rightArmWeights[boneIndex] = zeroWeight;
        _leftLegWeights[boneIndex] = zeroWeight;
        _rightLegWeights[boneIndex] = fullWeight;
    }
}

std::map<int, float> CCJamsAnimationWeights::getHeadWeights() const {
    return _headWeights;
}

std::map<int, float> CCJamsAnimationWeights::getLeftLegWeights() const {
    return _leftLegWeights;
}

std::map<int, float> CCJamsAnimationWeights::getRightLegWeights() const {
    return _rightLegWeights;
}

std::map<int, float> CCJamsAnimationWeights::getLeftArmWeights() const {
    return _leftArmWeights;
}

std::map<int, float> CCJamsAnimationWeights::getRightArmWeights() const {
    return _rightArmWeights;
}
