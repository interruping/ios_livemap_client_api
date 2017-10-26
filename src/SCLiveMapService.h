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
 @file SCLiveMapService.h
 @author Geun Young Lim, interruping@naver.com
 @date 2017. 10. 1.
 */
#import <Foundation/Foundation.h>
#import "SCLiveMapClientNode.h"

#ifndef SCLiveMapService_h
#define SCLiveMapService_h

@class  SCLiveMapService;

/*!
 @brief 라이브맵 서비스 Delegate
 @details SCLiveMapService 클래스 객체가 호출하는 콜백매서드들의 모임.
          대부분의 매서들은 서버의 응답관련 매서드, 서비스 이벤트등이 발생할때 호출된다.
 */
@protocol SCLiveMapServiceDelegate
/*!
 @brief LiveMapServer로부터 연결이 실패하였을 때 호출됨.
 @details LiveMapServer로부터 일정 시간 이상 연결 요청에 대한 응답이 없거나 연결 도중 문제가 발생했을 경우 호출된다.
 @param liveMapService 해당 매서드를 호출한 SCLiveMapService 객체
 @param error 에러 정보
 */
-(void) connectionFailToLiveMapServer: (SCLiveMapService * )livemapService error: (NSError *) error;
/*!
 @brief LiveMapServer로부터 통신도중 연결이 끊어졌을 때 호출됨.
 @details 데이터 송수신이 일어나는 도중에 네트워크 연결 끊김 등으로 통신이 끊어졌을 때 호출된다.
 @param liveMapService 해당 매서드를 호출한 SCLiveMapService 객체
 @param error 에러 정보
 */
-(void) connectionLostFromLiveMapServer: (SCLiveMapService *) livemapService error: (NSError *) error;
/*!
 @brief LiveMapService 객체를 사용할 준비가 되었을때 호출됨.
 @details LiveMapService 객체의 asyncStart 매서드를 호출하면 서버로부터 ID를 발급요청을 보낸다. ID발급이 완료되면
          호출된다. 이 메서드가 호출 된 이후로 LiveMapService 객체에 서비스를 요청해야 한다.
 @param livemapService 해당 매서드를 호출한 SCLiveMapService 객체
 @param livemapClientNode 발급받은 ID정보가 담긴 SCLiveMapClientNode 객체
 */
-(void) onServiceReady: (SCLiveMapService*) livemapService createdNode: (SCLiveMapClientNode *) livemapClientNode;
/*!
 @brief 사용자의 주변 위치에 클라이언트가 존재할 떄 호출됨.
 @details 사용자가 SCLiveMapService객체에 updateUserNode 매서드로 자신의 위치를 업데이트 하였을때 LiveMapServer가 주변 클라이언트가
          있는지 없는지 보고 알려준다. LiveMapServer로부터 응답이 욌을 때 호출된다.
 @param livemapService SCLiveMapService 해당 메서드를 호출한 서비스 객체.
 @param nearNodes 주변 클라이언트.
 */
-(void) nearNodesFromLiveMapServer: (SCLiveMapService*) livemapService nearNodes: (NSArray<SCLiveMapClientNode*> *) nearNodes;
/*!
 @brief 사용자가 다른 클라이언트로부터 메시지를 수신받았을 때 호출됨.
 @details 다른 클라이언트가 메시지를 보냈을 떄 호출됩니다. 이 떄 메시지를 보낸 클라이언트는 사용자 주변에 없는 클라이언트 일 수 있습니다.
 @param senderId 메시지 송신 클라이언트 id.
 @param message 수신한 메시지.
 */
-(void) receivedMessageSender: (NSInteger) senderId Msg: (NSString *) message;
@end

/*!
 @brief 라이브맵 서비스 클래스
 @details LiveMapServer와 통신하고 사용가능한 서비스를 사용할 수 있는 클래스
 */
@interface SCLiveMapService : NSObject
/*!
 @brief 초기화 매서드
 */
-(instancetype)init;
/*!
 @brief 비동기 서비스 시작 매서드.
 @details LiveMapServer와 연결을 요청하고 연결 완료 후 ID발급을 요청을 한다.
          이 초기화 매서드를 호출한 다음 이 매서드를 호출하고 이 객체가 참조하고 있는 Delegate의
          onServiceReady:createdNode: 가 호출되었을 때 이 클래스 객체가 사용 준비상태가 된다.
          따라서 onServiceReady:createdNode: 호출 이전에 delegate 프로퍼티, init매서드 이외의 매서드는 사용할 수 없다.
*/
-(void)asyncStart;
/*!
 @brief 라이브맵 서비스 종료 매서드.
 @details LiveMapServer와 연결을 종료하고 서비스를 종료합니다.
 */
-(void)stop;
/*!
 @brief 라이브맵 사용자의 위치를 업데이트 한다.
 @details LiveMapServer가 사용자의 위치를 이 메서드로 받고 실시간으로 업데이트 한다.
          LiveMapServer가 업데이트된 위치를 보고 주변 클라이언트의 정보를 사용자에게 전송한다.
 @param recentUpdateNode 사용자의 최신 위치 정보가 담긴 SCLiveMapClientNode 객체
 */
-(void)updateUserNode: (SCLiveMapClientNode *)recentUpdateNode;
/*!
 @brief 라이브맵 사용자의 위치와 사용자의  시점를 업데이트 한다.
 @details LiveMapServer가 사용자의 위치를 이 메서드로 받고 실시간으로 업데이트 한다.
          LiveMapServer가 사용자의 시점 위치를 보고 시점 주변 클라이언트의 정보를 사용자에게 전송한다.
 @param recentUpdateNode 사용자의 최신 위치 정보가 담긴 SCLiveMapClientNode 객체
 @param latitude 시점 위도
 @param longitude 시점 경도
 */
-(void)updateUserNode: (SCLiveMapClientNode *)recentUpdateNode AndViewPointlat: (double) latitutude lon: (double) longitude;
/*!
 @brief LiveMapServer에 연결되어있는 다른 클라이언트에게 메시지 전송
 @details 다른 클라이언트에게 메시지를 전송한다. 메시지를 전송하기 위해선 수신자 클라이언트의 id를 알아야 한다.
 @param sendNodeId 송신자 Id.
 @param recvId 수신자 Id.
 @param str 메시지.
 */
-(void)sendMessageSender: (NSInteger) sendNodeId Recver: (NSInteger) recvId Msg: (NSString *) str;

/*!
 @brief 대기중인 서비스 요청 갯수.
 @details 사용자가 서비스 요청을 할 경우 요청이 네트워크 큐에서 전송될때까지 대기한다.
          대기하고 있는 서비스 요청의 수를 반환하는 매서드.
 @return NSInteger 대기중인 서비스 요청의 수
 */
-(NSInteger)getWaitingCommandCount;

/*!
 @brief SCLiveMapServiceDelegate 프로퍼티
 */
@property (nonatomic, strong) id<SCLiveMapServiceDelegate> delegate;

/*!
 @brief Host 프로퍼티
 @details LiveMapServer의 도메인 or IPv4 주소.
 */
@property (nonatomic, strong) NSString *host;
@end

#endif /* SCLiveMapService_h */
