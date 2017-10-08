/*!
 @header SCLiveMapSSLSocketServerConnector.h
 @brief SCLiveMapSSLSocketServerConnector 클래스 헤더파일
 @author Geun Young Lim
 @copyright  2017 Geun Young Lim
 @version    16. 01. 11
 */

#import <Foundation/Foundation.h>
#import "SCLiveMapServerCommunicator.h"

@interface SCLiveMapSSLSocketServerConnector : NSObject <SCLiveMapServerCommunicatorImplementor, SCLiveMapServerCommunicatorDelegate>

/*
 @breif 초기화 매서드
 @return instancetype 인스턴스 객체 
*/
- (instancetype)init;
//SCLiveMapServerCommunicatorImplementor 프로토콜 메서드들.
- (void)setInterval: (NSTimeInterval) ti;
- (BOOL)open;
- (BOOL)close;
//SCLiveMapServerCommunicatorDelegate 프로토콜 매서드들.
- (NSInteger)readyToWriteToLiveMapServer: (char **) buffer;
- (void)readyToReadFromLiveMapServer: (const char *) buffer bufferLenght: (NSInteger) len;

@end
