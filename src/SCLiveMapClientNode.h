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

typedef struct coord {
    double latitude;
    double longitude;
} Coordinate;

@interface SCLiveMapClientNode : NSObject
@property (nonatomic, assign) NSInteger id;
@property (nonatomic, assign) Coordinate coordinate;
@end

#endif /* SCLiveMapClientNode_h */
