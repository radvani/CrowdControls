//
//  CCAnimationScreen.m
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

#import "CCAnimationScreen.h"
#import "CCFBXTest.h"
#import <Syphon/Syphon.h>

@interface CCAnimationScreen ()

@property (readwrite, nonatomic) NSString *name;
@property (readwrite, nonatomic) std::shared_ptr<VRODriver> driver;
@property (readwrite, nonatomic) std::shared_ptr<CCRenderToTextureDelegate> renderToTextureDelegate;
@property (readwrite, nonatomic) std::shared_ptr<VROImagePostProcess> blitPostProcess;
@property (readwrite, nonatomic) std::shared_ptr<CCFBXTest> fbxTest;
@property (readwrite, nonatomic) SyphonServer *syphon;

@end

@implementation CCAnimationScreen

- (id)initWithName:(NSString *)name {
    self = [super init];
    if (self) {
        self.name = name;
    }
    return self;
}

- (void)userDidRequestExitVR {}

- (void)setupRendererWithDriver:(std::shared_ptr<VRODriver>)driver {
    self.driver = driver;
    self.fbxTest = std::make_shared<CCFBXTest>();
    self.fbxTest->build(self.view.renderer, self.view.frameSynchronizer, driver);
    
    self.view.sceneController = self.fbxTest->getSceneController();
    if (self.fbxTest->getPointOfView()) {
        [self.view setPointOfView:self.fbxTest->getPointOfView()];
    }
    
    NSOpenGLContext *oglContext = (__bridge NSOpenGLContext *)driver->getGraphicsContext();
    CGLContextObj context = [oglContext CGLContextObj];
    self.syphon = [[SyphonServer alloc] initWithName:self.name context:context options:nil];
    
    self.renderToTextureDelegate = std::make_shared<CCRenderToTextureDelegate>(self);
    self.view.choreographer->setRenderToTextureDelegate(self.renderToTextureDelegate);
    
    std::vector<std::string> blitSamplers = { "source_texture" };
    std::vector<std::string> blitCode = {
        "uniform sampler2D source_texture;",
        "const highp float gamma = 2.2;",
        "highp vec3 source_color = texture(source_texture, v_texcoord).rgb;",
        "highp vec3 gamma_corrected = pow(source_color, vec3(1.0 / gamma));",
        "frag_color = vec4(gamma_corrected, 1.0);",
    };
    std::shared_ptr<VROShaderProgram> blitShader = VROImageShaderProgram::create(blitSamplers, blitCode, driver);
    _blitPostProcess = driver->newImagePostProcess(blitShader);
}

- (void)setBodyPart:(CCBodyPart)bodyPart toColor:(VROVector4f)color {
    // TODO Find the nodes corresponding to the body part
    self.fbxTest->setColor(color);
}

- (void)publishSyphonFrame:(std::shared_ptr<VRORenderTarget>)target {
    VROViewScene *view = (VROViewScene *) self.view;
    std::shared_ptr<VRODriver> driver = self.driver;

    [self.syphon bindToDrawFrameOfSize:view.bounds.size];
    self.blitPostProcess->blit({ target->getTexture(0) }, driver);
    [self.syphon unbindAndPublish];
}

@end

CCRenderToTextureDelegate::CCRenderToTextureDelegate(CCAnimationScreen *renderer) :
    _renderer(renderer) {
    
}

CCRenderToTextureDelegate::~CCRenderToTextureDelegate() {
    
}

void CCRenderToTextureDelegate::didRenderFrame(std::shared_ptr<VRORenderTarget> renderedTarget,
                                               std::shared_ptr<VRODriver> driver) {
    CCAnimationScreen *renderer = _renderer;
    if (!renderer) {
        return;
    }
    [renderer publishSyphonFrame:renderedTarget];
}
