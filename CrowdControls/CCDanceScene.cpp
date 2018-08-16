//
//  CCDanceScene.cpp
//  ViroKit
//
//  Created by Raj Advani on 6/30/18.
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

#include "CCDanceScene.h"
#include "CCModelUtil.h"

CCDanceScene::CCDanceScene() {
}

CCDanceScene::~CCDanceScene() {
    
}

void CCDanceScene::build(id <VROView> view,
                         std::shared_ptr<VRORenderer> renderer,
                         std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                         std::shared_ptr<VRODriver> driver) {

    _view = (VROViewScene *)view;
    _driver = driver;
    
    VROVector3f position = { 0, -1, -3 };
    VROVector3f scale = { 0.2, 0.2, 0.2 };
    std::shared_ptr<CCFBXModel> danceAll = std::make_shared<CCFBXModel>("trap", position, scale);
    _models.insert({ "trap", danceAll });
    
    _sceneController = std::make_shared<VROSceneController>();
    std::shared_ptr<VROScene> scene = _sceneController->getScene();
    
    std::shared_ptr<VROLight> light = std::make_shared<VROLight>(VROLightType::Spot);
    light->setColor({ 1.0, 1.0, 1.0 });
    light->setPosition( { 0, 10, 10 });
    light->setDirection( { 0, -1.0, -1.0 });
    light->setAttenuationStartDistance(25);
    light->setAttenuationEndDistance(50);
    light->setSpotInnerAngle(35);
    light->setSpotOuterAngle(60);
    light->setCastsShadow(true);
    light->setIntensity(1000);
    
    std::shared_ptr<VROLight> ambient = std::make_shared<VROLight>(VROLightType::Ambient);
    ambient->setColor({ 1.0, 1.0, 1.0 });
    ambient->setIntensity(500);
    
    //std::shared_ptr<VROTexture> environment = CCModelUtil::loadRadianceHDRTexture("ibl_mans_outside");
    
    std::shared_ptr<VROPortal> rootNode = scene->getRootNode();
    rootNode->setPosition({0, 0, 0});
    rootNode->addLight(light);
    rootNode->addLight(ambient);
    //rootNode->setLightingEnvironment(environment);
    //rootNode->setBackgroundSphere(environment);
    
    _fbxContainerNode = std::make_shared<VRONode>();
    rootNode->addChildNode(_fbxContainerNode);
    
    /*
     Shadow surface.
     */
    std::shared_ptr<VROSurface> surface = VROSurface::createSurface(80, 80);
    surface->setName("Surface");
    surface->getMaterials().front()->setLightingModel(VROLightingModel::Lambert);
    VROARShadow::apply(surface->getMaterials().front());
    
    std::shared_ptr<VRONode> surfaceNode = std::make_shared<VRONode>();
    surfaceNode->setGeometry(surface);
    surfaceNode->setRotationEuler({ -M_PI_2, 0, 0 });
    surfaceNode->setPosition({0, -6, -6});
    surfaceNode->setLightReceivingBitMask(1);
    rootNode->addChildNode(surfaceNode);
    
    std::shared_ptr<VRONodeCamera> camera = std::make_shared<VRONodeCamera>();
    camera->setRotationType(VROCameraRotationType::Orbit);
    camera->setOrbitFocalPoint({ 0, 0, -3});
    
    std::shared_ptr<VRONode> cameraNode = std::make_shared<VRONode>();
    cameraNode->setCamera(camera);
    cameraNode->setPosition({ 0, 0.5, 1 });
    rootNode->addChildNode(cameraNode);
    
    _pointOfView = cameraNode;
}

void CCDanceScene::addModel(std::string name) {
    auto kv = _models.find(name);
    if (kv == _models.end()) {
        pinfo("Failed to add model %s, not found", name.c_str());
        return;
    }
    
    std::shared_ptr<CCFBXModel> &model = kv->second;
    model->node = CCModelUtil::loadFBXModel(model->file, model->position, model->scale, _driver);
    _fbxContainerNode->addChildNode(model->node);
    
    _activeModels.insert( {name, model });
}

void CCDanceScene::startSequence(float durationSeconds, std::function<void(CCScene *)> onFinished) {
    _animationDurationSeconds = durationSeconds;
    
    for (auto &kv : _activeModels) {
        std::shared_ptr<CCFBXModel> &model = kv.second;
        animateTake(model->node, model->queuedAnimation, durationSeconds, onFinished);
    }
}

void CCDanceScene::clearModels() {
    _fbxContainerNode->removeAllChildren();
}

void CCDanceScene::queueAnimation(std::string name, CCBodyPart bodyPart, std::string animation) {
    auto kv = _models.find(name);
    if (kv == _models.end()) {
        pinfo("Failed to queue animation %s on model %s: model not found", animation.c_str(), name.c_str());
        return;
    }
    
    std::shared_ptr<CCFBXModel> &model = kv->second;
    
    NSLog(@"Queued model %s with animation %s", name.c_str(), animation.c_str());
    model->queuedAnimations[bodyPart] = animation;
}

void CCDanceScene::animateTake(std::shared_ptr<VRONode> node, std::string name, float durationSeconds,
                               std::function<void(CCScene *)> onFinished) {
    std::shared_ptr<VROExecutableAnimation> animation = node->getAnimation(name.c_str(), true)->copy();
    pinfo("Original animation duration %f", animation->getDuration());
    
    animation->setDuration(durationSeconds);
    animation->execute(node, [this, onFinished] {
        onFinished(this);
    });
}

void CCDanceScene::setColor(std::shared_ptr<VRONode> node, VROVector4f color) {
    if (node->getGeometry()) {
        for (std::shared_ptr<VROMaterial> material : node->getGeometry()->getMaterials()) {
            material->getDiffuse().setColor(color);
        }
    }
    for (std::shared_ptr<VRONode> child : node->getChildNodes()) {
        setColor(child, color);
    }
}

void CCDanceScene::setColor(VROVector4f color) {
    if (!_fbxContainerNode) {
        return;
    }
    setColor(_fbxContainerNode, color);
}
