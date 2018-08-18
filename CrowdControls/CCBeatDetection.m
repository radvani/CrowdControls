//
//  CCBeatDetection.m
//  CrowdControls
//
//  Created by Raj Advani on 8/16/18.
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

#import "CCBeatDetection.h"

@interface CCBeatDetection ()

@property (readwrite, nonatomic) int windowSize;
@property (readwrite, nonatomic) int sampleRate;
@property (readwrite, nonatomic) int hopSize;
@property (readwrite, nonatomic) int silenceThresholdDb;
@property (readwrite, nonatomic) aubio_tempo_t *tempo;

@end


@implementation CCBeatDetection {
    fvec_t *_samples;
}

- (id)initWithSampleRate:(int)sampleRate {
    self = [super init];
    if (self) {
        _windowSize = 1024;
        _sampleRate = sampleRate;
        _hopSize = _windowSize / 4;
        _tempo = new_aubio_tempo("default", _windowSize, _hopSize, _sampleRate);
        
        _silenceThresholdDb = 10;
        //aubio_tempo_set_silence(_tempo, _silenceThresholdDb);
        
        _samples = new_fvec(_windowSize);
    }
    return self;
}

- (void)dealloc {
    del_aubio_tempo(_tempo);
    del_fvec(_samples);
}

- (void)processMicrophoneBuffer:(AudioBufferList *)inputDataList frameCount:(uint32_t)frameCount {
    fvec_t *samplesOut = new_fvec(2);
    int sampleCount = 0;
    
    int count = frameCount;
    AudioBuffer buffer = inputDataList->mBuffers[0];
    
    void *bufferPointer = buffer.mData;
    if (bufferPointer) {
        float *dataArray = (float *) bufferPointer;
        for (int i = 0; i < count / 2; i++) {
            float x = dataArray[i * 2 + 0];   // copy left  channel sample
            float y = dataArray[i * 2 + 1];   // copy right channel sample
            
            fvec_set_sample(_samples, x * x + y * y, sampleCount);
            sampleCount += 1;
            
            if (sampleCount == _windowSize || i == count / 2 - 1) {
                aubio_tempo_do(_tempo, _samples, samplesOut);
                if (fvec_get_sample(samplesOut, 0) != 0) {
                    _beatsPerMinute  =aubio_tempo_get_bpm(_tempo);
                    NSLog(@"[BPM %f]", _beatsPerMinute);
                    break;
                }
                sampleCount = 0;
            }
        }
    }
    
    del_fvec(samplesOut);
}

@end
