 /*!
 @file SCLiveMapServerCommunicator.h
 @brief SCLiveMapServerCommunicator 클래스 구현
 @author Geun Young Lim
 @copyright  2017 Geun Young Lim
 @version    16. 01. 09
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
