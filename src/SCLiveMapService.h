//
//  SCLiveMapService.h
//  LiveMapClient
//
//  Created by Geon young Lim on 2017. 10. 1..
//  Copyright © 2017년 interruping. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "SCLiveMapClientNode.h"

#ifndef SCLiveMapService_h
#define SCLiveMapService_h

@class  SCLiveMapService;

@protocol SCLiveMapServiceDelegate
-(void) connectionFailToLiveMapServer: (SCLiveMapService * )livemapService error: (NSError *) error;
-(void) connectionLostFromLiveMapServer: (SCLiveMapService *) livemapService error: (NSError *) error;

-(void) onServiceReady: (SCLiveMapService*) livemapService createdNode: (SCLiveMapClientNode *) livemapClientNode;
-(void) nearNodesFromLiveMapServer: (SCLiveMapService*) livemapService nearNodes: (NSArray<SCLiveMapClientNode*> *) nearNodes;
-(void) receivedMessageSender: (NSInteger) senderId Msg: (NSString *) message;
@end

@interface SCLiveMapService : NSObject
-(instancetype)init;
-(void)asyncStart;
-(void)stop;
-(void)updateUserNode: (SCLiveMapClientNode *)recentUpdateNode;
-(void)updateUserNode: (SCLiveMapClientNode *)recentUpdateNode AndViewPointlat: (double) latitutude lon: (double) longitude;
-(void)sendMessageSender: (NSInteger) sendNodeId Recver: (NSInteger) recvId Msg: (NSString *) str;

-(NSInteger)getWaitingCommandCount;

@property (nonatomic, strong) id<SCLiveMapServiceDelegate> delegate;

@end

#endif /* SCLiveMapService_h */
