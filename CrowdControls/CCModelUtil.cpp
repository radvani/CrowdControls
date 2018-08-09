//
//  CCModelUtil.cpp
//  CrowdControls
//
//  Created by Raj Advani on 8/8/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
//

#include "CCModelUtil.h"
#import <Foundation/Foundation.h>

std::shared_ptr<VRONode> CCModelUtil::loadFBXModel(std::string model, VROVector3f position, VROVector3f scale,
                                                   std::shared_ptr<VRODriver> driver) {
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
                                      [scale, position](std::shared_ptr<VRONode> node, bool success) {
                                          if (!success) {
                                              return;
                                          }
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
                                      });
    return node;
}

std::shared_ptr<VROTexture> CCModelUtil::loadRadianceHDRTexture(std::string texture) {
    std::string path;
    NSString *fbxPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:texture.c_str()]
                                                        ofType:@"hdr"];
    path = std::string([fbxPath UTF8String]);
    return VROHDRLoader::loadRadianceHDRTexture(path);
}
