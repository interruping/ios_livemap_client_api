// Copyright 2017 GeunYoung Lim <interruping4dev@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 @file SCLiveMapServerCommunicator.h
 @author GeunYoung Lim
 @date   2017. 09. 19.
  */
#import "SCLiveMapServerCommunicator.h"
#import "SCLiveMapSSLSocketConnector.h"

@interface SCLiveMapServerCommunicator ()
- (instancetype) init;
@end

 //서버 타입 파라미터 저장 변수 (default value is SCSecureSocketConnector )
static SCLiveMapServerType _instanceType = SSLSocketServer;

@implementation SCLiveMapServerCommunicator
#pragma mark -
#pragma mark SCLiveMapServerCommunicator 매서드 구현 (public)
- (instancetype) initWithServerType:(SCLiveMapServerType)serverType {
    self = [self init];
    if ( self != nil ){
        switch (serverType) {
            case SSLSocketServer:
                _pimpl = [[SCLiveMapSSLSocketServerConnector alloc] init];
                self.pimpl.delegate = self;
                break;
            default:
                break;
        }
    }
    
    return self;
}

#pragma mark -
#pragma mark SCLiveMapServerCommunicator 매서드 구현 (private)
 //초기화 매서드
- (instancetype) init {
    self = [super init];
    
    if (self != nil ){
    }
    
    return self;
}

#pragma mark -
#pragma mark SCLiveMapServerCommunicatorImplementor method for pimpl (public)
 //SCLiveMapServerCommunicatorImplementor optional pimpl 프로퍼티 (optional) 구현
@synthesize pimpl = _pimpl;


 //interval 프로퍼티 pimpl 구현을 위해 수동으로 구현
@dynamic interval;
 //interval 프로퍼티 세터 pimpl 구현
- (void)setInterval: (NSTimeInterval) ti {
    [self.pimpl setInterval:ti];
    
}
 //interval 프로퍼티 게터 pimpl 구현
- (NSTimeInterval)interval {
    return self.pimpl.interval;
}



 //host 프로퍼티 pimpl 구현을 위해 수동으로 구현
@dynamic host;
 //host 프로퍼티 세터 pimpl 구현
- (void)setHost: (NSString *) host {
    self.pimpl.host = host;
}
 //host 프로퍼티 게터 pimpl 구현
- (NSString *)host {
    return self.pimpl.host;
}


 //port 프로퍼티 pimpl 구현을 위해 수동으로 구현
@dynamic port;
 //host 프로퍼티 세터 pimpl 구현
- (void)setPort: (NSInteger) port {
    self.pimpl.port = port;
}
 //port 프로퍼티 게터 pimpl 구현
- (NSInteger) port {
    return self.pimpl.port;
}

 //untilTimeout 프로퍼티 pimpl 구현을 위해 수동으로 구현
@dynamic untilTimeout;

 //untilTimeout 프로퍼티 세터 pimpl 구현
- (void)setUntilTimeout:(NSTimeInterval)untilTimeout {
    self.pimpl.untilTimeout = untilTimeout;
}
 //untilTimeout 프로퍼티 게터 pimpl 구현
- (NSTimeInterval)untilTimeout {
    return self.pimpl.untilTimeout;
}


 //pimpl 구현
-(BOOL)open {
    return [self.pimpl open];
}
 //pimpl 구현
-(BOOL)close {
    return [self.pimpl close];
}
#pragma mark -
#pragma mark SCLiveSCLiveMapServerCommunicatorDelegate method

//delegate 객체 프로퍼티 생성
@synthesize delegate = _delegate;

//pimpl 구현
-(NSInteger)readyToWriteToLiveMapServer: (char **) buffer {
    return [self.delegate readyToWriteToLiveMapServer:buffer];
}
 //pimpl 구현
-(void)readyToReadFromLiveMapServer: (const char *) buffer bufferLenght: (NSInteger) len {
    [self.delegate readyToReadFromLiveMapServer:buffer bufferLenght:len];
}

 //pimpl 구현
- (void)timedoutRequestWithHost:(NSString *)host port:(NSInteger)port {
    [self.delegate timedoutRequestWithHost:host port:port];
}

 //pimpl 구현
- (void)serverCommunitactionError:(NSError *)error {
    [self.delegate serverCommunitactionError:error];
}
@end
