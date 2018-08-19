//
//  CCTimelineScene.hpp
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

#ifndef CCTimelineScene_h
#define CCTimelineScene_h

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <ViroKit/ViroKit.h>
#include "CCScene.h"

enum class CCTimelineCategory {
    Drinks,
    Dinner,
    Party,
    Theater, //Movie
    Outdoors,  // Beach
    Travel,
    Apart,
    Comedy,
    Wedding,
    Bachelor,
    Dance,
    Volunteer,
    Unknown
};

enum class CCTimelineCompany {
    Us,
    Family,
    Friends,
    Unknown
};

class CCTimelinePoint {
public:
    
    int index;
    NSDate *date;
    CCTimelineCategory category;
    CCTimelineCompany company;
    
    CCTimelinePoint(int index, NSDate *date, CCTimelineCategory category, CCTimelineCompany company) :
        index(index), date(date), category(category), company(company) {}
    virtual ~CCTimelinePoint() {}
    
private:
    
};

class CCTimelineScene : public CCScene, public VROFrameListener, public std::enable_shared_from_this<CCTimelineScene> {
public:
    
    CCTimelineScene();
    virtual ~CCTimelineScene();
    
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
    
    void onFrameWillRender(const VRORenderContext &context);
    void onFrameDidRender(const VRORenderContext &context);
    
private:
    
    int _sequenceIndex = 0;
    
    VROViewScene *_view;
    std::shared_ptr<VRODriver> _driver;
    std::shared_ptr<VRONode> _pointOfView;
    std::shared_ptr<VROSceneController> _sceneController;
    float _duration;
    
    // Organized by row
    std::map<int, std::vector<std::shared_ptr<CCTimelinePoint>>> _pointsByRow;
    
    // Points organized by category
    std::map<CCTimelineCategory, std::vector<std::shared_ptr<CCTimelinePoint>>> _pointsByCategory;
    
    // Points organized by company
    std::map<CCTimelineCompany, std::vector<std::shared_ptr<CCTimelinePoint>>> _pointsByCompany;
        
    NSDate *parseDate(NSString *date);
    CCTimelineCategory parseCategory(NSString *category);
    CCTimelineCompany parseCompany(NSString *company);
    int getDaysBetween(NSDate *fromDateTime, NSDate *toDateTime);
    
    VROVector4f getColorForCategory(CCTimelineCategory category);
    
};


#endif /* CCTimelineScene_hpp */
