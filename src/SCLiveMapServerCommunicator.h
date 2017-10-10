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
 @brief SCLiveMapServerCommunicator 클래스 헤더파일
 @author GeunYoung Lim, interruping@naver.com
 @version 2017. 09. 19
 */
#import <Foundation/Foundation.h>

/*! @brief SCLiveMapServerCommunicator의 인스턴스를 생성할때 사용할 기반이 된는 서버 타입을 설정하기 위한 열거값들. */
typedef NS_ENUM( NSInteger, SCLiveMapServerType ) {
    SSLSocketServer, // SSL로 연결하는 TCP 서버
};

/*!
 @brief 라이브맵 서버와 통신시 발생하는 이벤트 델리게이트
*/
@protocol SCLiveMapServerCommunicatorDelegate <NSObject>
@required @property (nonatomic, weak) id <SCLiveMapServerCommunicatorDelegate> delegate;
@optional
/*!
 @brief 서버로 데이터를 보낼 준비가 됐을 때 호출됨, 서버로 데이터를 보내기위하여 버퍼에 데이터를 쓴다.
 @param buffer 버퍼
 @param len 버퍼 길이
 @return BOOL YES이면 서버에 데이터를 쓴다. NO이면 쓰지 않는다.
 */
- (NSInteger) readyToWriteToLiveMapServer: (char **) buffer;

/*!
 @brief 서버로 부터 데이터를 받아 읽을 준비가 됐을 때 호출됨, 서버로 데이터를 받아 작업 한다.
 @param buffer 서버로 부터 받은 데이터를 쓴 버퍼
 @param len 버퍼 길이
 */
- (void) readyToReadFromLiveMapServer: (const char *) buffer bufferLenght: (NSInteger) len;

/*!
 @brief 서버로 부터 요청이 단위 시간이상 응답하지 않을 때 호출 됨.
 @param host 호스트
 @param port 포트
*/
- (void) timedoutRequestWithHost: (NSString *) host port: (NSInteger) port;
/*!
 @brief 서버와 통신 중 에러 발생시 호출 됨.
 @param error 에러정보
*/
- (void) serverCommunitactionError: (NSError *) error;
@end



/*!
 @brief 구현부 인터페이스 프로토콜
 @details pimpl 관용구를 사용, 상세 클래스 구현부 클래스들의 인터페이스 역할.
*/
@protocol SCLiveMapServerCommunicatorImplementor <NSObject>
@optional
/*! @brief pimpl 관용구를 사용하기 위해 존재하는 포인터 (선택사항)*/
@property (nonatomic, strong) id <SCLiveMapServerCommunicatorImplementor, SCLiveMapServerCommunicatorDelegate> pimpl;
@required
/*! @brief livemap 서버와 읽고 쓰는 간격 시간 프로퍼티 */
@property (nonatomic, assign) NSTimeInterval interval;
/*! @brief host 프로퍼티 */
@property (nonatomic, strong) NSString *host;
/*! @brief port 프로퍼티 */
@property (nonatomic, assign) NSInteger port;
/*! @brief 연결 요청 대기 시간 */
@property (nonatomic, assign) NSTimeInterval untilTimeout;
/*! @brief 서버와 통신을 시작한다. */
-(BOOL)open;
/*! @brief 서버와 통신을 중단한다. */
-(BOOL)close;

@end

/*!
 @brief SCLiveMapServerCommunicator 클래스
 @details 라이브맵 서버에 접속하는 매커니즘을 구현한 클래스.
*/
@interface SCLiveMapServerCommunicator : NSObject <SCLiveMapServerCommunicatorImplementor, SCLiveMapServerCommunicatorDelegate>
- (instancetype)initWithServerType: (SCLiveMapServerType) serverType;
//SCLiveMapServerCommunicatorImplementor 프로토콜 메서드들.
- (void)setInterval: (NSTimeInterval) ti;
- (BOOL)open;
- (BOOL)close;
//SCLiveMapServerCommunicatorDelegate 프로토콜 매서드들.
- (NSInteger) readyToWriteToLiveMapServer: (char **) buffer;
- (void) readyToReadFromLiveMapServer: (const char *) buffer bufferLenght: (NSInteger) len;

@end
