//
//  CCCountdownScene.m
//  CrowdControls
//
//  Created by Raj Advani on 7/14/18.
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

#import "CCCountdownScene.h"

static const float kYStart = 1.095f;
static const float kWidth = 0.315;
static const float kYDecrement = kWidth;
static const float kZDepth = -2;
static const int kCountdownPositions = 8;

CCCountdownScene::CCCountdownScene() :
    _currentBarIndex(-1) {
    
}

CCCountdownScene::~CCCountdownScene() {
    
}

void CCCountdownScene::build(id <VROView> view,
                             std::shared_ptr<VRORenderer> renderer,
                             std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                             std::shared_ptr<VRODriver> driver) {
    
    _view = (VROViewScene *) view;
    _driver = driver;
    
    _sceneController = std::make_shared<VROSceneController>();
    std::shared_ptr<VROScene> scene = _sceneController->getScene();
    
    _quad = VROSurface::createSurface(10, kWidth);
    _quad->getMaterials().front()->getDiffuse().setColor({1, 1, 1, 1});
    _quadNode = std::make_shared<VRONode>();
    _quadNode->setPosition({ 0, kYStart, kZDepth });
    _quadNode->setGeometry(_quad);

    std::shared_ptr<VROPortal> rootNode = scene->getRootNode();
    rootNode->addChildNode(_quadNode);
    frameSynchronizer->addFrameListener(shared_from_this());
}

void CCCountdownScene::startSequence(float seconds, std::function<void(CCScene *)> onFinished) {
    float start = VROTimeCurrentSeconds();
    _duration = seconds;
    
    _currentBarIndex = 0;
    _barIncrementTime = seconds / (float) kCountdownPositions;
    _nextBarTime = start + _barIncrementTime;
    _onFinished = onFinished;
    
    [_view queueRendererTask:[this] {
        _quadNode->setPosition({ 0, kYStart, kZDepth });
     }];
}

void CCCountdownScene::onFrameWillRender(const VRORenderContext &context) {
    if (_currentBarIndex == -1) {
        return;
    }
    
    double time = VROTimeCurrentSeconds();
    if (time > _nextBarTime) {
        _quadNode->setPosition({ 0, _quadNode->getPosition().y - kYDecrement, kZDepth });
        _nextBarTime += _barIncrementTime;
        _currentBarIndex++;
        
        if (_currentBarIndex == kCountdownPositions) {
            _currentBarIndex = -1;
            _onFinished(this);            
        }
    }
}

void CCCountdownScene::onFrameDidRender(const VRORenderContext &context) {
    
}

