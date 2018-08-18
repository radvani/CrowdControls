//
//  CCModelUtil.cpp
//  CrowdControls
//
//  Created by Raj Advani on 8/8/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
//

#include "CCModelUtil.h"
#import <Foundation/Foundation.h>
#include "CCDanceController.h"

std::shared_ptr<VRONode> CCModelUtil::loadFBXModel(std::string model, VROVector3f position, VROVector3f scale,
                                                   std::shared_ptr<CCAnimationWeights> weights,
                                                   std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> *animationCache,
                                                   std::shared_ptr<VRODriver> driver,
                                                   std::function<void()> onModelLoaded) {
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
                                      [scale, position, weights, animationCache, onModelLoaded](std::shared_ptr<VRONode> node, bool success) {
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
                                          
                                          //preloadAnimations(node, weights, animationCache);
                                          onModelLoaded();
                                      });
    return node;
}

void CCModelUtil::preloadAnimations(std::shared_ptr<VRONode> node, std::shared_ptr<CCAnimationWeights> weights,
                                    std::map<std::map<CCSkeletonWeights, std::vector<std::string>>, std::shared_ptr<VROExecutableAnimation>> *cache) {
    
    std::map<CCSkeletonWeights, std::vector<std::string>> animations;
    
    animations.insert({ CCSkeletonAll, { "Dance_B_rest" } });
    cache->insert({ animations, preloadAnimation(node, weights, animations) });
    animations.clear();
    
    std::vector<CCColor> colors = { CCColorBlue, CCColorWhite, CCColorGreen, CCColorYellow, CCColorRed };
    for (CCColor headColor : colors) {
        NSString *head = [NSString stringWithFormat:@"Head_%@", [CCDanceController letterForColor:headColor]];

        for (CCColor leftArmColor : colors) {
            NSString *leftArm = [NSString stringWithFormat:@"Lhand_%@", [CCDanceController letterForColor:leftArmColor]];

            for (CCColor rightArmColor : colors) {
                NSString *rightArm = [NSString stringWithFormat:@"Rhand_%@", [CCDanceController letterForColor:rightArmColor]];

                for (CCColor leftLegColor : colors) {
                    NSString *leftLeg = [NSString stringWithFormat:@"Lfoot_%@_withBody", [CCDanceController letterForColor:leftLegColor]];

                    for (CCColor rightLegColor : colors) {
                        NSString *rightLeg = [NSString stringWithFormat:@"Rfoot_%@_withBody01", [CCDanceController letterForColor:rightLegColor]];

                        animations[CCSkeletonHead].push_back(std::string([head UTF8String]));
                        animations[CCSkeletonLeftArm].push_back(std::string([leftArm UTF8String]));
                        animations[CCSkeletonRightArm].push_back(std::string([rightArm UTF8String]));
                        animations[CCSkeletonLeftLeg].push_back(std::string([leftLeg UTF8String]));
                        animations[CCSkeletonRightLeg].push_back(std::string([rightLeg UTF8String]));

                        cache->insert({ animations, preloadAnimation(node, weights, animations) });
                        animations.clear();
                        
                        NSLog(@"Loaded animation %d", cache->size());
                    }
                }
            }
        }
    }
}

std::shared_ptr<VROExecutableAnimation> CCModelUtil::preloadAnimation(std::shared_ptr<VRONode> node,
                                                                      std::shared_ptr<CCAnimationWeights> weights,
                                                                      std::map<CCSkeletonWeights, std::vector<std::string>> animationList) {
    std::vector<std::shared_ptr<VROSkeletalAnimationLayer>> layers;
    for (auto body_animations : animationList) {
        CCSkeletonWeights skeletonWeights = body_animations.first;
        std::vector<std::string> animations = body_animations.second;
        
        for (std::string animationName : animations) {
            if (animationName.length() > 0) {
                std::shared_ptr<VROSkeletalAnimationLayer> layer = std::make_shared<VROSkeletalAnimationLayer>(animationName, 1.0);
                
                if (skeletonWeights == CCSkeletonLeftArm) {
                    layer->setBoneWeights(weights->getLeftArmWeights());
                } else if (skeletonWeights == CCSkeletonRightArm) {
                    layer->setBoneWeights(weights->getRightArmWeights());
                } else if (skeletonWeights == CCSkeletonLeftLeg) {
                    layer->setBoneWeights(weights->getLeftLegWeights());
                } else if (skeletonWeights == CCSkeletonRightLeg) {
                    layer->setBoneWeights(weights->getRightLegWeights());
                } else if (skeletonWeights == CCSkeletonHead) {
                    layer->setBoneWeights(weights->getHeadWeights());
                }
                layers.push_back(layer);
            }
        }
    }
    std::shared_ptr<VROExecutableAnimation> animation = node->getLayeredAnimation(layers, true);
    animation->preload();
    
    return animation;
}


std::shared_ptr<VROTexture> CCModelUtil::loadRadianceHDRTexture(std::string texture) {
    std::string path;
    NSString *fbxPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:texture.c_str()]
                                                        ofType:@"hdr"];
    path = std::string([fbxPath UTF8String]);
    return VROHDRLoader::loadRadianceHDRTexture(path);
}
