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
 @file SCLiveMapClientNode.h
 @author GeunYoung Lim, interruping@naver.com
 @date 2017. 10. 1.
 */

#ifndef SCLiveMapClientNode_h
#define SCLiveMapClientNode_h
#import <Foundation/Foundation.h>

/*!
 @brief GPS 위치 구조체
 @details 위도 경도를 갖는 구조체.
 */
typedef struct coord {
    double latitude;
    double longitude;
} Coordinate;

/*!
 @brief LiveMap 클라이언트 노드 클래스
 @details 클라이언트 노드 클래스, LiveMapServer에 존재하는 클라이언트 정보를 담는 클래스
 */
@interface SCLiveMapClientNode : NSObject
/*!
 @brief id 프로퍼티
 @details LiveMapServer에서 유일한 클라이언트 id 값.
 */
@property (nonatomic, assign) NSInteger id;
/*!
 @brief 좌표 프로퍼티
 @details 클라이언트의 현재 GPS 위치 위도, 경도.
 */
@property (nonatomic, assign) Coordinate coordinate;
@end

#endif /* SCLiveMapClientNode_h */
