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
 @header SCLiveMapSSLSocketServerConnector.h
 @author GeunYoung Lim, interruping@naver.com
 @version    17. 09. 11
 */

#import <Foundation/Foundation.h>
#import "SCLiveMapServerCommunicator.h"

@interface SCLiveMapSSLSocketServerConnector : NSObject <SCLiveMapServerCommunicatorImplementor, SCLiveMapServerCommunicatorDelegate>

/*
 초기화 매서드
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
