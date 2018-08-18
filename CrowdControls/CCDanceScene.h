//
//  CCDanceScene.h
//  CrowdControls
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

#ifndef CCDanceScene_h
#define CCDanceScene_h

#include <string>
#include <memory>
#include <functional>
#include <map>
#include <ViroKit/ViroKit.h>
#include "CCScene.h"
#include "CCDanceController.h"

class CCAnimationWeights;

class CCFBXModel {
public:
    std::string file;
    VROVector3f position;
    VROVector3f scale;
    std::map<CCSkeletonWeights, std::vector<std::string>> queuedAnimations;
    std::shared_ptr<VRONode> node;
    std::shared_ptr<CCAnimationWeights> weights;
    std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> cachedAnimations;
    
    CCFBXModel(std::string file, VROVector3f position, VROVector3f scale,
               std::shared_ptr<CCAnimationWeights> weights,
               std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> cache) :
        file(file), position(position), scale(scale), weights(weights), cachedAnimations(cache) {}
    ~CCFBXModel() {}
};

class CCDanceScene : public CCScene {
public:
    
    CCDanceScene();
    virtual ~CCDanceScene();
    
    void build(id <VROView> view,
               std::shared_ptr<VRORenderer> renderer,
               std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
               std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VRONode> getPointOfView() {
        return _pointOfView;
    }
    std::shared_ptr<VROSceneController> getSceneController() {
        return _sceneController;
    }
    
    void startSequence(float durationSeconds, std::function<void(CCScene *)> onFinished);
    
    /*
     Add the model with the given name to the dance scene.
     */
    void addModel(std::string name, std::function<void()> onModelLoaded);
    
    /*
     Clear all models from the dance scene.
     */
    void clearModels();
    
    /*
     Queue the animations with the given names for all models, to be run at
     the next startSequence call.
     */
    void queueAnimations(std::map<CCSkeletonWeights, std::vector<std::string>> animations);
    
    /*
     Set the color of the given body part.
     */
    void setColor(CCBodyPart bodyPart, VROVector4f color);
    
private:

    VROViewScene *_view;
    std::shared_ptr<VRODriver> _driver;
    std::shared_ptr<VRONode> _pointOfView;
    std::shared_ptr<VROSceneController> _sceneController;
    std::shared_ptr<VRONode> _fbxContainerNode;
    
    std::map<std::string, std::shared_ptr<CCFBXModel>> _models;
    std::map<std::string, std::shared_ptr<CCFBXModel>> _activeModels;
    float _animationDurationSeconds;
    
    std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> _jamsCachedAnimations;
    std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> _treeCachedAnimations;

    static void setColor(std::shared_ptr<VRONode> node, VROVector4f color, NSString *textureName);
    
};

#endif /* CCDanceScene_h */
