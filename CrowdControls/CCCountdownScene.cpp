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

static const float kZDepth = -2;

CCCountdownScene::CCCountdownScene() : VRORendererTest(VRORendererTestType::FBX) {
    
}

CCCountdownScene::~CCCountdownScene() {
    
}

void CCCountdownScene::build(std::shared_ptr<VRORenderer> renderer,
                             std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                             std::shared_ptr<VRODriver> driver) {
    
    _driver = driver;
    
    _sceneController = std::make_shared<VROSceneController>();
    std::shared_ptr<VROScene> scene = _sceneController->getScene();
    
    
    _box = VROBox::createBox(10, 0.25, 0.01);
    _box->getMaterials().front()->getDiffuse().setColor({1, 1, 1, 1});
    _boxNode = std::make_shared<VRONode>();
    _boxNode->setPosition({ 0, 0, kZDepth });
    _boxNode->setGeometry(_box);

    std::shared_ptr<VROPortal> rootNode = scene->getRootNode();
    rootNode->addChildNode(_boxNode);
    
    
}

void CCCountdownScene::setDuration(float seconds) {
    _duration = seconds;
}
