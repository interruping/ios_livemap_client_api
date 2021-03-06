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
 @file SCLiveMapService.m
 @author GeunYoung Lim, interruping@naver.com
 @date 2017. 10. 1.
 */


#import <Foundation/Foundation.h>
#import "SCLiveMapService.h"
#import "service_command.hpp"
#import "SCLiveMapServerCommunicator.h"

#import <memory>
#import <queue>

template <typename CMDTYPE>
static std::pair<int, std::shared_ptr<solarcode::livemap::command_form_base>>
transCmdToPair(std::shared_ptr<CMDTYPE> cmd) {
    using namespace solarcode::livemap;
    
    std::size_t size = cmd->get_entire_size();
    char *buffer = new char[size];
    cmd->serialize(buffer);
    
    std::shared_ptr<command_form_base> passCmd = std::make_shared<command_form_base>(buffer, size);
    
    int type = CMDTYPE::type;
    
    std::pair<int, std::shared_ptr<command_form_base>> retPair
    = std::make_pair(type, passCmd);
    
    return retPair;
}

@interface SCLiveMapService () <SCLiveMapServerCommunicatorDelegate>
{

    std::queue<
    std::pair<int,std::shared_ptr<solarcode::livemap::command_form_base>>
    > _commandQueue;
}

@property (nonatomic, strong) SCLiveMapServerCommunicator *serverCommunicator;


- (NSInteger) readyToWriteToLiveMapServer: (char **) buffer;

- (void) readyToReadFromLiveMapServer: (const char *) buffer bufferLenght: (NSInteger) len;

- (void) timedoutRequestWithHost: (NSString *) host port: (NSInteger) port;

- (void) serverCommunitactionError: (NSError *) error;

@end

@implementation SCLiveMapService


@synthesize delegate = _delegate;
@synthesize serverCommunicator = _serverCommunicator;
@synthesize host = _host;
-(instancetype)init
{
    self = [super init];
    
    if (self != nil ){
        _delegate = nil;
        _serverCommunicator = [[SCLiveMapServerCommunicator alloc]initWithServerType: SSLSocketServer];
        _serverCommunicator.delegate = self;
        _host = nil;
        
    }
    
    return self;
}

-(void)asyncStart
{
    if ( self.host == nil ){
        self.serverCommunicator.host = @"localhost";
    } else {
        self.serverCommunicator.host = self.host;
    }
    self.serverCommunicator.port = 1212;
    

    std::shared_ptr<solarcode::livemap::request_user_info> requestUserInfo
    = std::make_shared<solarcode::livemap::request_user_info>();
    _commandQueue.push(transCmdToPair(requestUserInfo));
    [self.serverCommunicator open];
}

-(void)stop
{
    [self.serverCommunicator close];
}

-(void)updateUserNode: (SCLiveMapClientNode *)recentUpdateNode
{
    using namespace solarcode::livemap;
    std::shared_ptr<user_update_node> command = std::make_shared<user_update_node>(recentUpdateNode);
    _commandQueue.push(transCmdToPair(command));
}

-(void)updateUserNode: (SCLiveMapClientNode *)recentUpdateNode AndViewPointlat: (double) latitutude lon: (double) longitude
{
    using namespace solarcode::livemap;
    Coordinate coord;
    coord.latitude = latitutude;
    coord.longitude = longitude;
    std::shared_ptr<user_viewpoint_update> command = std::make_shared<user_viewpoint_update>(recentUpdateNode, coord);
    
    _commandQueue.push(transCmdToPair(command));
    
    
    
}
-(void)sendMessageSender: (NSInteger) sendNodeId Recver: (NSInteger) recvId Msg: (NSString *) str {
    using namespace solarcode::livemap;
    std::shared_ptr<utf8_message_send> command = std::make_shared<utf8_message_send>(sendNodeId, recvId, str);
    _commandQueue.push(transCmdToPair(command));
}

- (NSInteger) readyToWriteToLiveMapServer: (char **) buffer
{
    using namespace solarcode::livemap;
    
    if ( _commandQueue.empty() == true ){
        
        return 0;
    }

    std::pair<int,std::shared_ptr<command_form_base>> typeAndCmd =  _commandQueue.front();
    _commandQueue.pop();
    
    int type = typeAndCmd.first;
    std::shared_ptr<command_form_base> cmd = typeAndCmd.second;
    
    NSInteger sizeOfCmd = cmd->get_entire_size();

    NSInteger allocedSize = sizeOfCmd + 4;
    
    *buffer = new char[allocedSize];
    
    std::memcpy(*buffer, &type, 4);
    
    cmd->serialize(*buffer + 4);
    
    return allocedSize;
}

- (void) readyToReadFromLiveMapServer: (const char *) buffer bufferLenght: (NSInteger) len
{
    using namespace solarcode::livemap;
    int type = 0;
    
    std::memcpy(&type, buffer, 4);
    
    const char *content = buffer + 4;
    int contentLength = (int)len - 4;
    
    switch (type) {
        case SETUSERINFO: {
            std::shared_ptr<set_user_info> rawCmd = std::make_shared<set_user_info>(content, contentLength);
            SCLiveMapClientNode *createdNode = [[SCLiveMapClientNode alloc]init];
            createdNode.id  = rawCmd->get_id();
            [self.delegate onServiceReady:self createdNode:createdNode];
            break;
        }
        case NEARNODEINFO: {
            std::shared_ptr<near_node_info> rawCmd = std::make_shared<near_node_info>(content, contentLength);
            NSArray<SCLiveMapClientNode *>  *nearNodes = rawCmd->get_near_nodes();
            
            [self.delegate nearNodesFromLiveMapServer:self nearNodes:nearNodes];
            break;
        }
            
        case UTF8MESSAGESEND: {
            std::shared_ptr<utf8_message_send> rawCmd = std::make_shared<utf8_message_send>(content, contentLength);
            [self.delegate receivedMessageSender:rawCmd->sender_id() Msg:rawCmd->get_msg()];
            break;
        }
            
        default: {
            break;
            
        }
    }
    
}

- (void) timedoutRequestWithHost: (NSString *) host port: (NSInteger) port
{
    NSError* error = [[NSError alloc]initWithDomain:NSStreamSocketSSLErrorDomain code:0 userInfo:nil];

    [self.delegate connectionFailToLiveMapServer:self error:error];
}

- (void) serverCommunitactionError: (NSError *) error
{
    [self.delegate connectionLostFromLiveMapServer:self error:error];
}

- (NSInteger) getWaitingCommandCount
{
    return _commandQueue.size();
}
@end
