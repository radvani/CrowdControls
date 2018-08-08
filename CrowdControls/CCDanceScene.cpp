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

CCDanceScene::CCDanceScene() :
    VRORendererTest(VRORendererTestType::FBX) {
    _angle = 0;
}

CCDanceScene::~CCDanceScene() {
    
}

void CCDanceScene::build(std::shared_ptr<VRORenderer> renderer,
                      std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                      std::shared_ptr<VRODriver> driver) {

    _driver = driver;
    
    CCFBXModel danceAll("trap", { 0, -1, -3 }, { 0.2, 0.2, 0.2 }, "Trap" );
    _models.push_back({ danceAll });
    
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
    
    std::shared_ptr<VROTexture> environment = loadRadianceHDRTexture("ibl_mans_outside");
    
    std::shared_ptr<VROPortal> rootNode = scene->getRootNode();
    rootNode->setPosition({0, 0, 0});
    rootNode->addLight(light);
    rootNode->addLight(ambient);
    //rootNode->setLightingEnvironment(environment);
    //rootNode->setBackgroundSphere(environment);
    
    _fbxContainerNode = std::make_shared<VRONode>();
    rootNode->addChildNode(_fbxContainerNode);

    _fbxIndex = 0;
    rotateFBX();
    
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

void CCDanceScene::rotateFBX() {
    _fbxContainerNode->removeAllChildren();

    std::vector<CCFBXModel> models = _models[_fbxIndex];
    for (CCFBXModel &model : models) {
        std::shared_ptr<VRONode> fbxNode = loadFBXModel(model.name, model.position, model.scale,
                                                        model.animation, _driver);
        _fbxContainerNode->addChildNode(fbxNode);
    }
    _fbxIndex = (_fbxIndex + 1) % _models.size();
}

std::shared_ptr<VRONode> CCDanceScene::loadFBXModel(std::string model, VROVector3f position, VROVector3f scale,
                                                 std::string animation, std::shared_ptr<VRODriver> driver) {
    std::string url;
    std::string base;
    VROResourceType resourceType = VROResourceType::URL;
    
    NSString *fbxPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:model.c_str()]
                                                        ofType:@"vrx"];
    NSURL *fbxURL = [NSURL fileURLWithPath:fbxPath];
    url = std::string([[fbxURL description] UTF8String]);
    
    NSString *basePath = [fbxPath stringByDeletingLastPathComponent];
    NSURL *baseURL = [NSURL fileURLWithPath:basePath];
    base = std::string([[baseURL description] UTF8String]);
    
    std::shared_ptr<VRONode> node = std::make_shared<VRONode>();
    VROFBXLoader::loadFBXFromResource(url, resourceType, node, driver,
                                      [scale, position, animation](std::shared_ptr<VRONode> node, bool success) {
                                          if (!success) {
                                              return;
                                          }
                                          
                                          float red = arc4random() % 255 / 255.0;
                                          float green = arc4random() % 255 / 255.0;
                                          float blue = arc4random() % 255 / 255.0;
                                          setColor(node, {red, green, blue, 1.0});
                                          
                                          node->setScale(scale);
                                          node->setPosition(position);
                                          
                                          if (node->getGeometry()) {
                                              node->getGeometry()->setName("FBX Root Geometry");
                                          }
                                          for (std::shared_ptr<VRONode> &child : node->getChildNodes()) {
                                              if (child->getGeometry()) {
                                                  child->getGeometry()->setName("FBX Geometry");
                                              }
                                              for (std::shared_ptr<VRONode> &grandchild : child->getChildNodes()) {
                                                  if (grandchild->getGeometry()) {
                                                      grandchild->getGeometry()->setName("FBX Geometry");
                                                      
                                                      for (const std::shared_ptr<VROMaterial> &material : grandchild->getGeometry()->getMaterials()) {
                                                          material->setTransparency(1.0);
                                                      }
                                                  }
                                              }
                                          }
                                          
                                          std::set<std::string> animations = node->getAnimationKeys(true);
                                          for (std::string animation : animations) {
                                              pinfo("Loaded animation [%s]", animation.c_str());
                                          }
                                          animateTake(node, animation);
                                          
                                          pinfo("FBX HAS LOADED into node %p", node.get());
                                      });
    return node;
}

void CCDanceScene::animateTake(std::weak_ptr<VRONode> node_w, std::string name) {
    std::shared_ptr<VRONode> node = node_w.lock();
    if (!node) {
        return;
    }
    node->getAnimation(name.c_str(), true)->execute(node, [node_w, name] {
        animateTake(node_w, name);
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

std::shared_ptr<VROTexture> CCDanceScene::loadRadianceHDRTexture(std::string texture) {
    std::string path;
    NSString *fbxPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:texture.c_str()]
                                                        ofType:@"hdr"];
    path = std::string([fbxPath UTF8String]);
    return VROHDRLoader::loadRadianceHDRTexture(path);
}
