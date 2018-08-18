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
    // Body only are bones that used by Head, Body, LeftLeg, RightLeg, LeftLegBodyOn, and RightLegBodyOn
    // Not sure about 57 and 106 here. Head requires joint 4
    _headOnly = { 4, 5,
                  8, 9, 10, 11, 12, 13, 14,
                 15, 16, 17, 18, 19, 20, 21,
                 22, 23, 24, 25, 26, 27, 28,
                 29, 30, 31, 32, 33, 34, 35,
                 36, 37, 38, 39, 40, 41, 42,
                 43, 44, 45, 46, 47, 48, 49,
                 50, 51, 52, 53, 54, 55, 56 };
    
    // Bones 108 and 113 are problematic in that they're used by both
    // right leg and left leg but a straight 50/50 blend doesn't work.
    // We use custom weights for those (see at the bottom).
    _body = { 0, 1, 2, 3, 6, 7, 57, 107 };
    _leftLegOnly  = { 109, 110, 111, 112 };
    _rightLegOnly = { 114, 115, 116, 117 };
    
    // The arm weights are used only by the left and right arm, only
    _leftArmOnly = {  82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105 };
    _rightArmOnly = { 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,  76,  77,  78,  79,  80,  81 };
    
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
    for (int boneIndex : _body) {
        _headWeights[boneIndex] = zeroWeight;
        _leftArmWeights[boneIndex] = zeroWeight;
        _rightArmWeights[boneIndex] = zeroWeight;
        _leftLegWeights[boneIndex] = 0.5;
        _rightLegWeights[boneIndex] = 0.5;
    }

    _headWeights[108] = zeroWeight;
    _leftArmWeights[108] = zeroWeight;
    _rightArmWeights[108] = zeroWeight;
    _leftLegWeights[108] = 0.75;
    _rightLegWeights[108] = 0.25;
    
    _headWeights[113] = zeroWeight;
    _leftArmWeights[113] = zeroWeight;
    _rightArmWeights[113] = zeroWeight;
    _leftLegWeights[113] = 0.25;
    _rightLegWeights[113] = 0.75;
}

std::map<int, float> CCTreeAnimationWeights::getHeadWeights() const {
    return _headWeights;
}

std::map<int, float> CCTreeAnimationWeights::getLeftLegWeights() const {
    return _leftLegWeights;
}

std::map<int, float> CCTreeAnimationWeights::getRightLegWeights() const {
    return _rightLegWeights;
}

std::map<int, float> CCTreeAnimationWeights::getLeftArmWeights() const {
    return _leftArmWeights;
}

std::map<int, float> CCTreeAnimationWeights::getRightArmWeights() const {
    return _rightArmWeights;
}
