//
//  CCViewController.m
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

#import "CCViewController.h"

enum class VROTestSceneType {
    Scene,
};

static const VRORendererTestType kRendererTest = VRORendererTestType::FBX;

@interface CCViewController ()

@end

@implementation CCViewController

- (void)loadView {
    VRORendererConfiguration config;
    config.enableHDR = YES;
    config.enablePBR = YES;
    config.enableBloom = YES;
    
    VROViewScene *view = [[VROViewScene alloc] initWithFrame:NSMakeRect(20, 20, 600, 600)
                                                      config:config
                                                shareContext:nil];
    view.renderDelegate = self.renderDelegate;
    
    self.renderDelegate.view = view;
    self.renderDelegate.test = kRendererTest;
    
    self.view = view;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

@end
