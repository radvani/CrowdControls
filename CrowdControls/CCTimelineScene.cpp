//
//  CCTimelineScene.cpp
//  CrowdControls
//
//  Created by Raj Advani on 8/18/18.
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

#include "CCTimelineScene.h"

static const int kNumTimelineRows = 15;
static const int kNumTimelineColumns = 57;

CCTimelineScene::CCTimelineScene() {
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"Proposal" withExtension:@"csv"];
    NSString *fileContents = [NSString stringWithContentsOfURL:url encoding:NSUTF8StringEncoding error:nil];
    NSArray *rows = [fileContents componentsSeparatedByString:@"\r"];
    
    for (int i = 1; i < [rows count]; i++) {
        NSString *row = [rows objectAtIndex:i];
        NSArray *columns = [row componentsSeparatedByString:@","];
        
        std::shared_ptr<CCTimelinePoint> point = std::make_shared<CCTimelinePoint>(i - 1,
                                                                                   parseDate(columns[0]),
                                                                                   parseCategory(columns[1]),
                                                                                   parseCompany(columns[2]));
        _pointsByRow[(i - 1) / kNumTimelineColumns].push_back(point);
        _pointsByCategory[point->category].push_back(point);
        _pointsByCompany[point->company].push_back(point);
    }
    
    _sequenceIndex = 0;
}

CCTimelineScene::~CCTimelineScene() {
    
}

// 112 x 180 screen
// 15 x 57

void CCTimelineScene::build(id <VROView> view,
                            std::shared_ptr<VRORenderer> renderer,
                            std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                            std::shared_ptr<VRODriver> driver) {
    
    _view = (VROViewScene *) view;
    _driver = driver;
    
    _sceneController = std::make_shared<VROSceneController>();
    std::shared_ptr<VROScene> scene = _sceneController->getScene();
    std::shared_ptr<VROPortal> rootNode = scene->getRootNode();
    
    float pointRadius = 0.035;
    float y = 1.35f;
    
    std::shared_ptr<VROLight> light = std::make_shared<VROLight>(VROLightType::Directional);
    light->setDirection({ 0, 0, -1 });
    
    rootNode->addLight(light);

    for (int i = 0; i < _pointsByRow.size(); i++) {
        std::vector<std::shared_ptr<CCTimelinePoint>> &points = _pointsByRow[i];
        
        float x = -2.8;
        
        for (int j = 0; j < points.size(); j++) {
            std::shared_ptr<CCTimelinePoint> point = points[j];
            std::shared_ptr<VROSphere> sphere = VROSphere::createSphere(pointRadius, 10, 10, true);
            sphere->getMaterials().front()->getDiffuse().setColor(getColorForCategory(point->category));
            sphere->getMaterials().front()->setLightingModel(VROLightingModel::PhysicallyBased);
            sphere->getMaterials().front()->getMetalness().setColor({ 0.2, 0.2, 0.2, 0.2 });
            sphere->getMaterials().front()->getRoughness().setColor({ 0.7, 0.7, 0.7, 0.7 });
            
            std::shared_ptr<VRONode> sphereNode = std::make_shared<VRONode>();
            sphereNode->setGeometry(sphere);
            sphereNode->setPosition({ x, y, -3 });
            rootNode->addChildNode(sphereNode);

            x += 0.10;
        }
        
        y -= 0.20;
    }

    frameSynchronizer->addFrameListener(shared_from_this());
}

void CCTimelineScene::startSequence(float seconds, std::function<void(CCScene *)> onFinished) {
    
    [_view queueRendererTask:[this] {
     //_quadNode->setPosition({ 0, kYStart, kZDepth });
     }];
}

void CCTimelineScene::onFrameWillRender(const VRORenderContext &context) {
    
}

void CCTimelineScene::onFrameDidRender(const VRORenderContext &context) {
    
}

NSDate *CCTimelineScene::parseDate(NSString *date) {
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setLocale:[[NSLocale alloc] initWithLocaleIdentifier:@"en_US"]];
    [dateFormatter setDateFormat:@"MM/dd/yyyy"];
    return [dateFormatter dateFromString:date];
}

CCTimelineCategory CCTimelineScene::parseCategory(NSString *category) {
    if ([category length] == 0) {
        return CCTimelineCategory::Unknown;
    }
    if ([category caseInsensitiveCompare:@"Drinks"] == NSOrderedSame) {
        return CCTimelineCategory::Drinks;
    }
    if ([category caseInsensitiveCompare:@"Dinner"] == NSOrderedSame) {
        return CCTimelineCategory::Dinner;
    }
    if ([category caseInsensitiveCompare:@"Party"] == NSOrderedSame) {
        return CCTimelineCategory::Party;
    }
    if ([category caseInsensitiveCompare:@"Theater"] == NSOrderedSame) {
        return CCTimelineCategory::Theater;
    }
    if ([category caseInsensitiveCompare:@"Movie"] == NSOrderedSame) {
        return CCTimelineCategory::Theater;
    }
    if ([category caseInsensitiveCompare:@"Park"] == NSOrderedSame) {
        return CCTimelineCategory::Outdoors;
    }
    if ([category caseInsensitiveCompare:@"Beach"] == NSOrderedSame) {
        return CCTimelineCategory::Outdoors;
    }
    if ([category caseInsensitiveCompare:@"Travel"] == NSOrderedSame) {
        return CCTimelineCategory::Travel;
    }
    if ([category caseInsensitiveCompare:@"Apart"] == NSOrderedSame) {
        return CCTimelineCategory::Apart;
    }
    if ([category caseInsensitiveCompare:@"Comedy"] == NSOrderedSame) {
        return CCTimelineCategory::Comedy;
    }
    if ([category caseInsensitiveCompare:@"Wedding"] == NSOrderedSame) {
        return CCTimelineCategory::Wedding;
    }
    if ([category caseInsensitiveCompare:@"Bachelor"] == NSOrderedSame) {
        return CCTimelineCategory::Bachelor;
    }
    if ([category caseInsensitiveCompare:@"Dance"] == NSOrderedSame) {
        return CCTimelineCategory::Dance;
    }
    if ([category caseInsensitiveCompare:@"Volunteer"] == NSOrderedSame) {
        return CCTimelineCategory::Volunteer;
    }
    else {
        NSLog(@"Unknown category parsed: %@", category);
        return CCTimelineCategory::Unknown;
    }
}

CCTimelineCompany CCTimelineScene::parseCompany(NSString *company) {
    if ([company length] == 0) {
        return CCTimelineCompany::Unknown;
    }
    if ([company caseInsensitiveCompare:@"Us"] == NSOrderedSame) {
        return CCTimelineCompany::Us;
    }
    if ([company caseInsensitiveCompare:@"Friends"] == NSOrderedSame) {
        return CCTimelineCompany::Friends;
    }
    if ([company caseInsensitiveCompare:@"Family"] == NSOrderedSame) {
        return CCTimelineCompany::Family;
    }
    else {
        NSLog(@"Unknown company parsed: %@", company);
        return CCTimelineCompany::Unknown;
    }
}

int CCTimelineScene::getDaysBetween(NSDate *fromDateTime, NSDate *toDateTime) {
    NSDate *fromDate;
    NSDate *toDate;
    
    NSCalendar *calendar = [NSCalendar currentCalendar];
    
    [calendar rangeOfUnit:NSCalendarUnitDay startDate:&fromDate
                 interval:NULL forDate:fromDateTime];
    [calendar rangeOfUnit:NSCalendarUnitDay startDate:&toDate
                 interval:NULL forDate:toDateTime];
    
    NSDateComponents *difference = [calendar components:NSCalendarUnitDay
                                               fromDate:fromDate toDate:toDate options:0];
    return [difference day];
}

VROVector4f CCTimelineScene::getColorForCategory(CCTimelineCategory category) {
    switch (category) {
        case CCTimelineCategory::Drinks:
            return { 0.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Dinner:
            return { 1.0, 0.0, 0.0, 1.0 };
        case CCTimelineCategory::Party:
            return { 0.0, 1.0, 1.0, 1.0 };
        case CCTimelineCategory::Theater:
            return { 1.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Outdoors:
            return { 0.0, 1.0, 0.0, 1.0 };
        case CCTimelineCategory::Travel:
            return { 0.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Apart:
            return { 0.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Comedy:
            return { 0.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Wedding:
            return { 0.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Bachelor:
            return { 0.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Dance:
            return { 0.0, 0.0, 1.0, 1.0 };
        case CCTimelineCategory::Volunteer:
            return { 0.0, 0.0, 1.0, 1.0 };
        default:
            return { 0.3, 0.3, 0.3, 1.0 };
    }
}

