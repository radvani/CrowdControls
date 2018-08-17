//
//  CCAudioRecorder.m
//  CrowdControls
//
//  Created by Raj Advani on 8/17/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
//
//  This file is modified from the gist available at http://pulkitgoyal.in/audio-processing-on-ios-using-aubio/
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

#import "CCAudioRecorder.h"

@interface CCAudioRecorder ()

@property (readwrite, nonatomic) AudioUnit audioUnit;

@end
    
@implementation CCAudioRecorder {
    
    BOOL _isRecording;
    
    AudioDeviceID _inputDevice;
    double _sampleRate;
    int _circBufferSize;
    float *_circBuffer;
    
    int _circInIdx;
    float _audioLevel;
    
    int _hwSRate;   // Microphone (hardware) sample rate
    int _micPermissionDispatchToken;
    BOOL _interrupted;    // For restart from audio interruption notification
    
    int _numberOfChannels;
    uint32_t _outputBus;
    uint32_t _inputBus;
}

- (id)init {
    self = [super init];
    if (self) {
        _isRecording = NO;
        
        // Setup AudioUnit for recording
        _inputDevice = 0;
        _sampleRate = 44100.0;
        _circBufferSize = 32768;
        _circBuffer = (float *) malloc(sizeof(float) * _circBufferSize);
        _circInIdx = 0;
        _audioLevel = 0;
        
        _hwSRate = 48000.0;
        _micPermissionDispatchToken = 0;
        _interrupted = false;
        
        _numberOfChannels = 2;
        _outputBus = 0;
        _inputBus = 1;
    }
    return self;
}

- (void) dealloc {
    free (_circBuffer);
}

- (void)startRecording {
    if (_isRecording) {
        return;
    }
    [self startAudioUnit];
}

- (void)startAudioUnit {
    OSStatus err = noErr;
    if (_audioUnit == nil) {
        [self setupAudioUnit];
    }
    
    AudioUnit au = _audioUnit;
    if (!au) {
        NSLog(@"Failed to setup AudioUnit for microphone recording");
        return;
    }
    
    err = AudioUnitInitialize(au);
    if (err != noErr) {
        NSLog(@"Failed to initialize AudioUnit for microphone recording");
        return;
    }
    err = AudioOutputUnitStart(au);
    if (err != noErr) {
        NSLog(@"Failed to start AudioUnit for microphone recording");
        return;
    }
    _isRecording = true;
}
    
- (void)setupAudioUnit {
    // Configure and open the Audio Unit
    AudioComponentDescription componentDesc;
    componentDesc.componentType = kAudioUnitType_Output;
    componentDesc.componentSubType = kAudioUnitSubType_HALOutput;//kAudioUnitSubType_RemoteIO;
    componentDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    componentDesc.componentFlags = 0;
    componentDesc.componentFlagsMask = 0;
    
    OSStatus osErr = noErr;
    AudioComponent component = AudioComponentFindNext(nil, &componentDesc);
    
    osErr = AudioComponentInstanceNew(component, &_audioUnit);
    
    AudioUnit au = _audioUnit;
    if (!au) {
        return;
    }
    
    uint32_t one_ui32 = 1;
    uint32_t zero_ui32 = 0;
    
    // Enable input on the Audio Unit HAL
    osErr = AudioUnitSetProperty(_audioUnit,
                                 kAudioOutputUnitProperty_EnableIO,
                                 kAudioUnitScope_Input,
                                 _inputBus,
                                 &one_ui32, sizeof(uint32_t));
    if (osErr != noErr) {
        NSLog(@"Error enabling input on audio unit");
    }
    
    // Disable output on the Audio Unit HAL
    osErr = AudioUnitSetProperty(_audioUnit,
                                 kAudioOutputUnitProperty_EnableIO,
                                 kAudioUnitScope_Output,
                                 _outputBus,
                                 &zero_ui32, sizeof(uint32_t));
    if (osErr != noErr) {
        NSLog(@"Error disabling output on audio unit");
    }
    
    // Select the default input device
    uint32_t param = sizeof(AudioDeviceID);
    AudioObjectPropertyAddress theAddress = { kAudioHardwarePropertyDefaultInputDevice,
                                              kAudioObjectPropertyScopeGlobal,
                                              kAudioObjectPropertyElementMaster };
    osErr = AudioObjectGetPropertyData(kAudioObjectSystemObject,
                                       &theAddress,
                                       0,
                                       NULL,
                                       &param,
                                       &_inputDevice);
    if(osErr != noErr) {
        NSLog(@"Failed to get default input device");
    }
    
    // Set the current device to the default input unit
    osErr = AudioUnitSetProperty(au,
                                 kAudioOutputUnitProperty_CurrentDevice,
                                 kAudioUnitScope_Global,
                                 0,
                                 &_inputDevice,
                                 sizeof(AudioDeviceID));
    if(osErr != noErr) {
        NSLog(@"Failed to set audio unit input device");
    }
    
    // Set format to 32-bit Floats, linear PCM
    int nc = 2;  // 2 channel stereo
    AudioStreamBasicDescription streamFormatDesc;
    streamFormatDesc.mSampleRate = _sampleRate;
    streamFormatDesc.mFormatID = kAudioFormatLinearPCM;
    streamFormatDesc.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
    streamFormatDesc.mBytesPerPacket = nc * sizeof(uint32_t);
    streamFormatDesc.mFramesPerPacket = 1;
    streamFormatDesc.mBytesPerFrame = nc * sizeof(uint32_t);
    streamFormatDesc.mChannelsPerFrame = nc;
    streamFormatDesc.mBitsPerChannel = 8 * sizeof(uint32_t);
    streamFormatDesc.mReserved = 0;
    
    osErr = AudioUnitSetProperty(au,
                                 kAudioUnitProperty_StreamFormat,
                                 kAudioUnitScope_Input, _outputBus,
                                 &streamFormatDesc,
                                 sizeof(AudioStreamBasicDescription));
    if (osErr != noErr) {
        NSLog(@"Error disabling output on audio unit");
    }
    
    osErr = AudioUnitSetProperty(au,
                                 kAudioUnitProperty_StreamFormat,
                                 kAudioUnitScope_Output,
                                 _inputBus,
                                 &streamFormatDesc,
                                 sizeof(AudioStreamBasicDescription));
    if (osErr != noErr) {
        NSLog(@"Error initializing beat detector");
    }
    
    // Setup recorder callback
    AURenderCallbackStruct inputCallbackStruct;
    inputCallbackStruct.inputProc = recordingCallback;
    inputCallbackStruct.inputProcRefCon = (__bridge void *)self;
    
    osErr = AudioUnitSetProperty(au,
                                 kAudioOutputUnitProperty_SetInputCallback,
                                 kAudioUnitScope_Global,
                                 _inputBus,
                                 &inputCallbackStruct,
                                 sizeof(AURenderCallbackStruct));
    if (osErr != noErr) {
        NSLog(@"Error setting recorder callback");
    }
    
    // Ask CoreAudio to allocate buffers on render.
    osErr = AudioUnitSetProperty(au,
                                 kAudioUnitProperty_ShouldAllocateBuffer,
                                 kAudioUnitScope_Output,
                                 _inputBus,
                                 &one_ui32,
                                 sizeof(uint32_t));
    if (osErr != noErr) {
        NSLog(@"Error initializing beat detector");
    }
}


OSStatus recordingCallback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) {
 
    CCAudioRecorder *audioObject = (__bridge CCAudioRecorder *) inRefCon;
    
    OSStatus err = noErr;
        // set mData to nil, AudioUnitRender() should be allocating buffers
    AudioBuffer buffer;
    buffer.mNumberChannels = 2;
    buffer.mDataByteSize = 2048;
    buffer.mData = nil;
    
    AudioBufferList bufferList;
    bufferList.mNumberBuffers = 1;
    bufferList.mBuffers[0] = buffer;
                                     
    AudioUnit au = audioObject.audioUnit;
    if (au) {
        err = AudioUnitRender(au, ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames,
                              &bufferList);
    }
    
    if (audioObject.delegate) {
        [audioObject.delegate processMicrophoneBuffer:&bufferList frameCount:inNumberFrames];
    } else {
        // If there's no delegate just print the audio level
        [audioObject processMicrophoneBuffer:&bufferList frameCount:inNumberFrames];
    }
    return 0;
}
    
- (void)processMicrophoneBuffer:(AudioBufferList *)inputDataList frameCount:(uint32_t)frameCount {
    int count = frameCount;
    AudioBuffer buffer = inputDataList->mBuffers[0];
    
    void *bufferPointer = buffer.mData;
    if (bufferPointer) {
        float *dataArray = (float *) bufferPointer;
        float sum = 0;
        
        int j = _circInIdx;
        int m = _circBufferSize;
        
        for (int i = 0; i < count / 2; i++) {
            float x = dataArray[i * 2 + 0];   // copy left  channel sample
            float y = dataArray[i * 2 + 1];   // copy right channel sample
            _circBuffer[j + 0] = x;
            _circBuffer[j + 1] = y;
            j += 2;
            
            if (j >= m) {
                j = 0;
            }
            sum += (x * x + y * y);
        }
        
        _circInIdx = j;              // circular index will always be less than size
        if (sum > 0.0 && count > 0) {
            float tmp = 5.0 * (logf(sum / (float)(count)) + 20.0);
            float r = 0.2;
            _audioLevel = r * tmp + (1.0 - r) * _audioLevel;
            
            NSLog(@"Audio level %f", _audioLevel);
        }
    }
}

- (void)stopRecording {
    AudioUnitUninitialize(self.audioUnit);
    _isRecording = false;
}

@end
