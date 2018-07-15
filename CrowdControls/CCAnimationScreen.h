//
//  CCAnimationScreen.h
//  CrowdControls
//
//  Created by Raj Advani on 5/13/18.
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

#import <ViroKit/ViroKit.h>
#import "CCDanceController.h"

@class CCAnimationScreen;

class CCRenderToTextureDelegate : public VRORenderToTextureDelegate {
public:
    CCRenderToTextureDelegate(CCAnimationScreen *renderer);
    virtual ~CCRenderToTextureDelegate();
    
    void didRenderFrame(std::shared_ptr<VRORenderTarget> renderedTarget,
                        std::shared_ptr<VRODriver> driver);
private:
    __weak CCAnimationScreen *_renderer;
};

@interface CCAnimationScreen : NSObject <VRORenderDelegate>

@property (readwrite, nonatomic) IBOutlet id <VROView> view;
@property (readwrite, nonatomic) VRORendererTestType test;

- (id)initWithName:(NSString *)name;
- (void)publishSyphonFrame:(std::shared_ptr<VRORenderTarget>)target;
- (void)setBodyPart:(CCBodyPart)bodyPart toColor:(VROVector4f)color;

@end


