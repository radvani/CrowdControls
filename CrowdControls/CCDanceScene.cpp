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
#include "CCJamsAnimationWeights.h"
#include "CCTreeAnimationWeights.h"

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
    
    std::shared_ptr<CCAnimationWeights> jamsWeights = std::make_shared<CCJamsAnimationWeights>();
    std::shared_ptr<CCAnimationWeights> treeWeights = std::make_shared<CCTreeAnimationWeights>();
    
    VROVector3f position = { 0, -1, -3 };
    VROVector3f scale = { 0.2, 0.2, 0.2 };
    
    _models.insert({ "Jams",    std::make_shared<CCFBXModel>("Jams",    position, scale, jamsWeights, _jamsCachedAnimations) });
    _models.insert({ "Trees",   std::make_shared<CCFBXModel>("Trees",   position, scale, treeWeights, _treeCachedAnimations) });
    _models.insert({ "FlatTop", std::make_shared<CCFBXModel>("FlatTop", position, scale, jamsWeights, _jamsCachedAnimations) });
    _models.insert({ "Punk",    std::make_shared<CCFBXModel>("Punk",    position, scale, treeWeights, _treeCachedAnimations) });
    _models.insert({ "Poof",    std::make_shared<CCFBXModel>("Poof",    position, scale, jamsWeights, _jamsCachedAnimations) });
    _models.insert({ "Ballet",  std::make_shared<CCFBXModel>("Ballet",  position, scale, treeWeights, _treeCachedAnimations) });
    
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
    
    std::shared_ptr<VROPortal> rootNode = scene->getRootNode();
    rootNode->setPosition({0, 0, 0});
    rootNode->addLight(light);
    rootNode->addLight(ambient);
    
    _fbxContainerNode = std::make_shared<VRONode>();
    rootNode->addChildNode(_fbxContainerNode);
    
    std::shared_ptr<VRONodeCamera> camera = std::make_shared<VRONodeCamera>();
    camera->setRotationType(VROCameraRotationType::Orbit);
    camera->setOrbitFocalPoint({ 0, 0, -3});
    
    std::shared_ptr<VRONode> cameraNode = std::make_shared<VRONode>();
    cameraNode->setCamera(camera);
    cameraNode->setPosition({ 0, 0.5, 1 });
    rootNode->addChildNode(cameraNode);
    
    _pointOfView = cameraNode;
}

void CCDanceScene::addModel(std::string name, std::function<void()> onModelLoaded) {
    auto kv = _models.find(name);
    if (kv == _models.end()) {
        pinfo("Failed to add model %s, not found", name.c_str());
        return;
    }
    
    std::shared_ptr<CCFBXModel> &model = kv->second;
    model->node = CCModelUtil::loadFBXModel(model->file, model->position, model->scale, model->weights, &model->cachedAnimations,
                                            _driver, onModelLoaded);
    _fbxContainerNode->addChildNode(model->node);
    
    _activeModels.insert( {name, model });
}

void CCDanceScene::startSequence(float durationSeconds, std::function<void(CCScene *)> onFinished) {
    _animationDurationSeconds = durationSeconds;
   
    for (auto &kv : _activeModels) {
        std::shared_ptr<CCFBXModel> &model = kv.second;
 
        std::shared_ptr<VROExecutableAnimation> animation;
        auto &cachedAnimations = model->cachedAnimations;
        
        auto it = cachedAnimations.find(model->queuedAnimations);
        if (it != cachedAnimations.end()) {
            pinfo("Using cached animation");
            animation = it->second;
        }
        else {
            std::vector<std::shared_ptr<VROSkeletalAnimationLayer>> layers;
            for (auto body_animations : model->queuedAnimations) {
                CCSkeletonWeights skeletonWeights = body_animations.first;
                std::vector<std::string> animations = body_animations.second;
                
                for (std::string animationName : animations) {
                    if (animationName.length() > 0) {
                        std::shared_ptr<VROSkeletalAnimationLayer> layer;
                        
                        auto cachedLayerIt = model->cachedSkeletalAnimationLayers.find({ skeletonWeights, animationName });
                        if (cachedLayerIt != model->cachedSkeletalAnimationLayers.end()) {
                            layer = cachedLayerIt->second;
                        } else {
                            layer = std::make_shared<VROSkeletalAnimationLayer>(animationName, 1.0);
                            if (skeletonWeights == CCSkeletonLeftArm) {
                                layer->setBoneWeights(model->weights->getLeftArmWeights());
                            } else if (skeletonWeights == CCSkeletonRightArm) {
                                layer->setBoneWeights(model->weights->getRightArmWeights());
                            } else if (skeletonWeights == CCSkeletonLeftLeg) {
                                layer->setBoneWeights(model->weights->getLeftLegWeights());
                            } else if (skeletonWeights == CCSkeletonRightLeg) {
                                layer->setBoneWeights(model->weights->getRightLegWeights());
                            } else if (skeletonWeights == CCSkeletonHead) {
                                layer->setBoneWeights(model->weights->getHeadWeights());
                            }
                            model->cachedSkeletalAnimationLayers[{ skeletonWeights, animationName }] = layer;
                        }
                        layers.push_back(layer);
                    }
                }
            }
        
            animation = model->node->getLayeredAnimation(layers, true);
            pinfo("Created new animation [duration %f]", animation->getDuration());
            
            cachedAnimations[model->queuedAnimations] = animation;
        }
        
        animation->setDuration(durationSeconds);
        
        // Preload to ensure all animations in the chain start at the same time
        // when we execute
        animation->preload();
        animation->execute(model->node, [this, onFinished] {
            onFinished(this);
        });
    }
}

void CCDanceScene::clearModels() {
    _fbxContainerNode->removeAllChildren();
}

void CCDanceScene::queueAnimations(std::map<CCSkeletonWeights, std::vector<std::string>> animations) {
    for (auto kv : _models) {
        kv.second->queuedAnimations = animations;
    }
}

void CCDanceScene::setColor(std::shared_ptr<VRONode> node, VROVector4f color, NSString *textureName) {
    std::shared_ptr<VROGeometry> geometry = node->getGeometry();
    if (geometry) {
        for (std::shared_ptr<VROMaterial> material : node->getGeometry()->getMaterials()) {
            NSString *diffuseName = @"";
            if (material->getDiffuse().getTexture()) {
                diffuseName = [NSString stringWithUTF8String:material->getDiffuse().getTexture()->getName().c_str()];
            }
            if ([diffuseName rangeOfString:textureName options:NSCaseInsensitiveSearch].location != NSNotFound) {
                material->getDiffuse().setColor(color);
            }
        }
    }
    for (std::shared_ptr<VRONode> child : node->getChildNodes()) {
        setColor(child, color, textureName);
    }
}

void CCDanceScene::setColor(CCBodyPart bodyPart, VROVector4f color) {
    if (!_fbxContainerNode) {
        return;
    }
    
    NSString *textureNameFragment = nil;
    switch (bodyPart) {
        case CCBodyPartHead:
            textureNameFragment = @"Body";
            break;
        case CCBodyPartLeftArm:
            textureNameFragment = @"Larm";
            break;
        case CCBodyPartRightArm:
            textureNameFragment = @"Rarm";
            break;
        case CCBodyPartLeftLeg:
            textureNameFragment = @"Lleg";
            break;
        case CCBodyPartRightLeg:
            textureNameFragment = @"Rleg";
            break;
        default:
            break;
    }
    setColor(_fbxContainerNode, color, textureNameFragment);
}
