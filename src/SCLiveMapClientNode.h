//
//  SCLiveMapClientNode.h
//  LiveMapClient
//
//  Created by Geon young Lim on 2017. 10. 1..
//  Copyright © 2017년 interruping. All rights reserved.
//

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
