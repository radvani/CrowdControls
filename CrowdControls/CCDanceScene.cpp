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
    
    _models.insert({ "Jams",    std::make_shared<CCFBXModel>("Jams", position, scale) });
    _models.insert({ "Trees",   std::make_shared<CCFBXModel>("Trees", position, scale) });
    _models.insert({ "Punk",    std::make_shared<CCFBXModel>("Punk", position, scale) });
    _models.insert({ "FlatTop", std::make_shared<CCFBXModel>("FlatTop", position, scale) });
    _models.insert({ "Poof",    std::make_shared<CCFBXModel>("Poof", position, scale) });
    _models.insert({ "Ballet",  std::make_shared<CCFBXModel>("Ballet", position, scale) });
    
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
        
        int totalAnimations = 0;
        for (int i = 0; i < 5; i++) {
            CCBodyPart bodyPart = (CCBodyPart) i;
            std::string animationName = model->queuedAnimations[i];
            if (animationName.length() > 0) {
                ++totalAnimations;
            }
        }
        
        float weightPerAnimation = 1.0f/ (float) totalAnimations;
        std::vector<std::shared_ptr<VROSkeletalAnimationLayer>> layers;
        
        for (int i = 0; i < 5; i++) {
            CCBodyPart bodyPart = (CCBodyPart) i;
            std::string animationName = model->queuedAnimations[i];
            if (animationName.length() > 0) {
                std::shared_ptr<VROSkeletalAnimationLayer> layer = std::make_shared<VROSkeletalAnimationLayer>(animationName, weightPerAnimation);
                
                if (bodyPart == CCBodyPartHead) {
                    layer->setBoneWeights(getBodyWeights());
                } else if (bodyPart == CCBodyPartLeftArm) {
                    layer->setBoneWeights(getLeftArmWeights());
                } else if (bodyPart == CCBodyPartRightArm) {
                    layer->setBoneWeights(getRightArmWeights());
                } else if (bodyPart == CCBodyPartLeftLeg) {
                    layer->setBoneWeights(getLeftLegWeights());
                } else {
                    layer->setBoneWeights(getRightLegWeights());
                }
                layers.push_back(layer);
            }
        }
        
        std::shared_ptr<VROExecutableAnimation> animation = model->node->getLayeredAnimation(layers, true);
        pinfo("Original animation duration %f", animation->getDuration());
        
        animation->setDuration(durationSeconds);
        animation->execute(model->node, [this, onFinished] {
            onFinished(this);
        });
    }
}

void CCDanceScene::clearModels() {
    _fbxContainerNode->removeAllChildren();
}

void CCDanceScene::queueAnimation(CCBodyPart bodyPart, std::string animation) {
    for (auto kv : _models) {
        std::shared_ptr<CCFBXModel> &model = kv.second;
        model->queuedAnimations[bodyPart] = animation;
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

std::map<int, float> CCDanceScene::getBodyWeights() const {
    std::map<int, float> weights;
    
    std::map<int, float> map = getLeftLegWeights();
    for (auto kv : map) {
        weights[kv.first] = 1 - kv.second;
    }
    map = getRightArmWeights();
    for (auto kv : map) {
        weights[kv.first] = 1 - kv.second;
    }
    map = getLeftLegWeights();
    for (auto kv : map) {
        weights[kv.first] = 1 - kv.second;
    }
    map = getRightLegWeights();
    for (auto kv : map) {
        weights[kv.first] = 1 - kv.second;
    }
    for (int i = 0; i < 70; i++) {
        if (weights.find(i) == weights.end()) {
            weights[i] = 0.5;
        }
    }
    
    return weights;
}

std::map<int, float> CCDanceScene::getLeftArmWeights() const {
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

std::map<int, float> CCDanceScene::getRightArmWeights() const {
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

std::map<int, float> CCDanceScene::getLeftLegWeights() const {
    float heavy = 0.70;
    
    std::map<int, float> weights;
    weights[59] = heavy;
    weights[60] = heavy;
    weights[61] = heavy;
    weights[62] = heavy;
    
    for (int i = 0; i < 70; i++) {
        if (weights.find(i) == weights.end()) {
            weights[i] = 0.0;
        }
    }
    
    return weights;
}

std::map<int, float> CCDanceScene::getRightLegWeights() const {
    float heavy = 0.70;
    
    std::map<int, float> weights;
    weights[59] = heavy;
    weights[60] = heavy;
    weights[61] = heavy;
    weights[62] = heavy;
    
    for (int i = 0; i < 70; i++) {
        if (weights.find(i) == weights.end()) {
            weights[i] = 0.0;
        }
    }
    
    return weights;
}
