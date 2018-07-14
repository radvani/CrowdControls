//
//  CCFBXTest.h
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

#ifndef CCFBXTest_h
#define CCFBXTest_h

#include <string>
#include <ViroKit/ViroKit.h>

class CCFBXTest;

class CCFBXEventDelegate : public VROEventDelegate {
public:
    CCFBXEventDelegate(CCFBXTest *test) : _test(test) {};
    virtual ~CCFBXEventDelegate() {};
    void onClick(int source, std::shared_ptr<VRONode> node, ClickState clickState, std::vector<float> position);
    
private:
    CCFBXTest *_test;
};

class CCFBXModel {
public:
    std::string name;
    VROVector3f position;
    VROVector3f scale;
    int lightMask;
    std::string animation;
    
    CCFBXModel(std::string name, VROVector3f position, VROVector3f scale, std::string animation) :
        name(name), position(position), scale(scale), animation(animation) {}
    ~CCFBXModel() {}
};

class CCFBXTest : public VRORendererTest {
public:
    
    CCFBXTest();
    virtual ~CCFBXTest();
    
    void build(std::shared_ptr<VRORenderer> renderer,
               std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
               std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VRONode> getPointOfView() {
        return _pointOfView;
    }
    std::shared_ptr<VROSceneController> getSceneController() {
        return _sceneController;
    }
    
    void rotateColor();
    void rotateFBX();
    
private:

    std::shared_ptr<VRODriver> _driver;
    std::shared_ptr<VRONode> _pointOfView;
    std::shared_ptr<VROSceneController> _sceneController;
    std::shared_ptr<VRONode> _fbxContainerNode;
    std::shared_ptr<VROEventDelegate> _eventDelegate;
    int _fbxIndex;
    float _angle;
    std::vector<std::vector<CCFBXModel>> _models;
    
    static std::shared_ptr<VRONode> loadFBXModel(std::string model, VROVector3f position, VROVector3f scale,
                                                 std::string animation, std::shared_ptr<VRODriver> driver);
    static void animateTake(std::weak_ptr<VRONode> node_w, std::string name);
    static void setColor(std::shared_ptr<VRONode> node, VROVector4f color);
    static std::shared_ptr<VROTexture> loadRadianceHDRTexture(std::string texture);
    
};

#endif /* CCFBXTest_h */
