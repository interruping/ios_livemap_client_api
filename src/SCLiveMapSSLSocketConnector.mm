/*!
 @file SCLiveMapSSLSocketServerConnector.m
 @brief SCLiveMapSSLSocketServerConnector 클래스 구현
 @author Geun Young Lim
 @version    16. 01. 11
 */
#import "SCLiveMapSSLSocketConnector.h"
#include <memory>
//버퍼 크기 상수

@interface SCLiveMapSSLSocketServerConnector () <NSStreamDelegate> {
     //  프로퍼티 수동 선언
    NSTimeInterval _interval;
    
    char *_writeDataBuffer;
    NSInteger _writeDataLen;
    
    const char *_readDataBuffer;
    NSInteger _readDataLen;
    
    BOOL _ioWrite;
    BOOL _ioRead;
}
/*!
  연결요청시간이 만료됬을 시 호출되는 매서드
 @param timer 타이머
 */
- (void) timeout: (NSTimer *) timer;
 //  타이머 프로퍼티
@property (nonatomic, strong) NSTimer *timeoutTimer;

/*
  일정한 간격을 두고 스트림에 읽고 쓰는 작업을 시행하는 타이머 매서드
 @param timer 타이머
 */

- (void) timeToIO: (NSTimer *) timer;
 //  스트림 IO 타이머
@property (nonatomic, strong) NSTimer *ioTimer;

//  타이머 정리 매서드


- (void)clearAllTimer;

 //  NSStreamDelegate 매서드
-  (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
 //
/*!
 @breifTCP 소켓을 생성하여 NSStream 객체에 설정한다 (비공개 매서드 )
 @param inputStream 입력 스트림
 @param outputStream 출력 스트림
 @param host 호스트 이름
 @param port 포트
 */
- (void) attachCFSocketToInputStream: (NSInputStream **) inputStream
                        OutputStream: (NSOutputStream **) outputStream
                            WithHost: (NSString *) host
                                port: (NSInteger) port;

 //  입력 스트림 프로퍼티
@property (nonatomic, strong) NSInputStream *inputStream;
 //  출력 스트림 프로퍼티
@property (nonatomic, strong) NSOutputStream *outputStream;

@end

@implementation SCLiveMapSSLSocketServerConnector
#pragma mark - 
#pragma mark  SCLiveMapSSLSocketServerConnector 메서드 구현 (private)

- (void) attachCFSocketToInputStream: (NSInputStream **) inputStream
                        OutputStream: (NSOutputStream **) outputStream
                            WithHost: (NSString *) host
                                port: (int) port {
    CFReadStreamRef     readStream;
    CFWriteStreamRef    writeStream;
    
    assert(host != nil);
    assert( (port > 0) && (port < 65536) );
    assert( (inputStream != NULL) || (outputStream != NULL) );
    
    readStream = NULL;
    writeStream = NULL;
    
    CFStreamCreatePairWithSocketToHost(
                                       NULL,
                                       (__bridge CFStringRef) host,
                                       port,
                                       ((inputStream  != NULL) ? &readStream : NULL),
                                       ((outputStream != NULL) ? &writeStream : NULL));
                                   

    NSDictionary *settings = [[NSDictionary alloc] initWithObjectsAndKeys:
                              [NSNumber numberWithBool:NO], kCFStreamSSLValidatesCertificateChain,
                              kCFNull,kCFStreamSSLPeerName,
                              nil];
//    NSDictionary *settings = [NSDictionary dictionary];

    
    CFReadStreamSetProperty((CFReadStreamRef)readStream, kCFStreamPropertySSLSettings, (CFTypeRef)settings);
    CFWriteStreamSetProperty((CFWriteStreamRef)writeStream, kCFStreamPropertySSLSettings, (CFTypeRef)settings);
    
    
    if (inputStream != NULL) {
        *inputStream  = CFBridgingRelease(readStream);
    }
    if (outputStream != NULL) {
        *outputStream = CFBridgingRelease(writeStream);
    }
}


 //  타임아웃 프로퍼티 생성
@synthesize timeoutTimer = _timeoutTimer;
 //  타임아웃 매서드 구현
- (void) timeout: (NSTimer *) timer {
    [self.timeoutTimer invalidate];
    self.timeoutTimer = nil;
    [self.inputStream close];
    [self.outputStream close];
    //타임 아웃 알리기
    [self timedoutRequestWithHost:self.host port:self.port];
}

 //  IO 타이머 프로퍼티 생성
@synthesize ioTimer = _ioTimer;
 //  IO 동작 타이머 매서드 구현
- (void) timeToIO:(NSTimer *)timer {
    
    if (self.interval == 0) return; //io 인터벌이 0일 경우 NSStream delegate 메소드에 읽고 쓰는 작업을 한다.


    
    if( self.outputStream.hasSpaceAvailable  ) {

        

        
    } else {
        return;
    }
    return;
}

-(void)updateData
{
    NSInteger dataLen = [self readyToWriteToLiveMapServer:&_writeDataBuffer];
    _writeDataLen = dataLen;
}

-(void)servData
{
    [self readyToReadFromLiveMapServer:_readDataBuffer bufferLenght:_readDataLen];
}

 //  타이머 정리 매서드 구현
- (void) clearAllTimer {
    if ( self.timeoutTimer != nil ){
        [self.timeoutTimer invalidate];
        self.timeoutTimer = nil;
    }
    
    if ( self.ioTimer ){
        [self.ioTimer invalidate];
        self.ioTimer = nil;
    }

    
}
#pragma mark -
#pragma mark  SCLiveMapSSLSocketServerConnector 메서드 구현 (public)
 //  초기화 매서드 구현
-(instancetype) init {
    self = [super init];
    
    if ( self != nil ) {
        //초기화 작업.
        _interval = 0.2f;

    
    }
    
    return self;
}
#pragma mark -
#pragma mark SCLiveMapServerCommunicatorImplementor method for pimple (public)
 //  서버 정보 프로퍼티
@synthesize host = _host, port = _port;
 //  타임아웃 프로퍼티
@synthesize untilTimeout;
 //  인터벌 수동 구현
@dynamic interval;
 //  서버 통신 간격 프로퍼티 세터 구현
-(void)setInterval: (NSTimeInterval) ti {
    _interval = ti;
}

-(NSTimeInterval)interval {
    return _interval;
}
 //  구현
-(BOOL)open {
    
    //setenv("CFNETWORK_DIAGNOSTICS","3",1);
    NSInputStream *inputStream = nil;
    NSOutputStream *outputStream = nil;
    
    [self attachCFSocketToInputStream:&inputStream OutputStream:&outputStream WithHost:self.host port:self.port];
    
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];
     
     
    __block BOOL isStreamBusy = YES;
    NSCondition *streamCondition = [[NSCondition alloc] init];

    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0);
    dispatch_async(queue, ^{
        [streamCondition lock];

        [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        self.inputStream = inputStream;
        self.outputStream = outputStream;
        // your background code
        [self.inputStream open];
        [self.outputStream open];
        
        
        
        isStreamBusy = NO;
        [streamCondition signal];
        [streamCondition unlock];
        
        //end of your code
        
        [[NSRunLoop currentRunLoop] run]; // start a run loop, look at the next point
    });

    
    //타임아웃 설정.
    //디폴트 타임아웃 시간. (프로퍼티 설정이 안되었을 때 사용)
    static NSTimeInterval defaultTimeoutVal = 20.0f;
    
    if ( self.timeoutTimer == nil ) {
        
        
        //타임 아웃 타이머 시작
        self.timeoutTimer = [NSTimer scheduledTimerWithTimeInterval:self.untilTimeout > 0.0f ? self.untilTimeout : defaultTimeoutVal // 타임아웃 설정 여부 검사
                                                        target:self
                                                      selector:@selector(timeout:)
                                                      userInfo:nil
                                                       repeats:NO];

    } else if ( self.timeoutTimer != nil ) {
        return NO; //open 매서드를 호출하였는데 타임아웃 타이머가 열려있으면 열지 않는다.
    }
    
    [streamCondition lock];
    while ( isStreamBusy ) {
        [streamCondition wait];
    }
    
    if( self.inputStream.streamStatus  == NSStreamStatusOpen    ||
        self.inputStream.streamStatus  == NSStreamStatusOpening ||
        self.outputStream.streamStatus == NSStreamStatusOpen    ||
        self.outputStream.streamStatus == NSStreamStatusOpening   ) {
        return NO; // 한 스트림이라도 열려있거나 열려있는 상태면 열지 않는다.
    }
    [streamCondition unlock];

    return YES;
}

 //   구현
-(BOOL)close {
    [self.inputStream close];
    [self.outputStream close];
    [self.inputStream removeFromRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    [self.outputStream removeFromRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    [self clearAllTimer];
    return YES;
}
#pragma mark -
#pragma mark SCLiveMapServerCommunicatorDelegate method

 //  delegate 객체 프로퍼티 생성
@synthesize delegate = _delegate;

 //  Write 동작 델리게이트 호출 구현
-(NSInteger)readyToWriteToLiveMapServer: (char **) buffer {
    return [self.delegate readyToWriteToLiveMapServer:buffer];
}
 //  Read 동작 델리게이트 호출 구현
-(void)readyToReadFromLiveMapServer: (const char *) buffer bufferLenght: (NSInteger) len {
    [self.delegate readyToReadFromLiveMapServer:buffer bufferLenght:len];
}
 //  타임아웃 동작 델리게이트 호출 구현
- (void)timedoutRequestWithHost:(NSString *)host port:(NSInteger)port {
    [self.delegate timedoutRequestWithHost:host port:port];
}
 //  에러 동작 델레게이트 호출 구현
- (void)serverCommunitactionError:(NSError *)error {
    [self.delegate serverCommunitactionError:error];
}
#pragma mark -
#pragma mark NSStreamDelegate protocol method (private)
 //  NSStreamDelegate 매서드 구현
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode {
    
    switch(eventCode) {
        case NSStreamEventNone:{
            
            break;
        }
        case NSStreamEventOpenCompleted :{
            //타임아웃 타이머 끄기
            [self.timeoutTimer invalidate];

            break;
        }
        case NSStreamEventHasBytesAvailable:
        case NSStreamEventHasSpaceAvailable:
        {

            if ( self.outputStream.hasSpaceAvailable ){
                NSLog(@"write stream");
                
                [self performSelectorOnMainThread:@selector(updateData) withObject:nil waitUntilDone:YES];
                
                if ( _writeDataLen > 0 ) {
                    
                    char headerData[4];
                    
                    int dataLen = (int)_writeDataLen;
                    
                    memcpy(headerData, &dataLen, 4);
                    [self.outputStream write:(uint8_t*)headerData maxLength:4];
                    [self.outputStream write:(uint8_t*)_writeDataBuffer maxLength:_writeDataLen];
                    
                    delete [] _writeDataBuffer;
                } else {
                    
                    int header = 4;
                    int type = 0;
                    
                    [self.outputStream write:(const uint8_t *)&header maxLength:4];
                    [self.outputStream write:(const uint8_t *)&type maxLength:4];
                    
                   
                }
                [NSThread sleepForTimeInterval:self.interval];
            }
            
            
            if ( self.inputStream.hasBytesAvailable ){
                NSLog(@"read stream");
                char headerData[4];
                
                [self.inputStream read:(uint8_t*)headerData maxLength:4];
                
                int32_t lengthOfContent = 0;
                
                memcpy(&lengthOfContent, headerData, 4);
                
                NSMutableData *contentData = [[NSMutableData alloc]initWithCapacity:lengthOfContent];
                while( !self.inputStream.hasBytesAvailable ){
                    [NSThread sleepForTimeInterval:0.1];
                }
                [self.inputStream read:(uint8_t *)[contentData mutableBytes] maxLength:lengthOfContent];
                _readDataLen = lengthOfContent;
                _readDataBuffer = (const char *)[contentData bytes];
                
                [self performSelectorOnMainThread:@selector(servData) withObject:nil waitUntilDone:YES];
                [NSThread sleepForTimeInterval:self.interval];
                
            }
            break;
        }
        case NSStreamEventErrorOccurred: {
            [self serverCommunitactionError:aStream.streamError]; //델리게이트로 에러 전달 후
            [aStream close]; //스트림 닫기.
            [self clearAllTimer];
            break;
        }
        case NSStreamEventEndEncountered: {
            [aStream close];
            [self clearAllTimer];
            break;
        }
    }
}
@end
