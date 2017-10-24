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
 @file service_type.hpp
 @author GeunYoung Lim, interruping@naver.com
 @date 2017. 9. 26.
 */
#include "service_type.hpp"
#include "command_form.hpp"
#import "SCLiveMapClientNode.h"


namespace solarcode {
namespace livemap {
    /*!
    @class update_node
    @brief 노드 업데이트 명령 클래
    @detail n/a.
    @namespace solarcode::livemap::update_node;
     */
    template <int TYPE_ID>
    class update_node_base : public command_form_base_t<TYPE_ID> {
    public:
    	/*
    	@brief 생성자
    	@param update_info_node 업데이트할 노드 정보를 담은 노드 객체
    	 */
        update_node_base(SCLiveMapClientNode* update_info):
    	command_form_base_t<TYPE_ID>(),
        _id_seg_info(0,4),
        _lat_seg_info(4,8),
        _lon_seg_info(12,8)
    	{
            auto id = update_info.id;
            command_form_base_t<TYPE_ID>::add_segment(&id, 4);
            auto coordinate = update_info.coordinate;
            
            double latitude = coordinate.latitude;
            command_form_base_t<TYPE_ID>::add_segment(&latitude, 8);
            double longitude = coordinate.longitude;
            command_form_base_t<TYPE_ID>::add_segment(&longitude, 8);
            
    	}
        
        update_node_base(const char * const input_data, const std::size_t input_data_size)
        :command_form_base_t<TYPE_ID>(input_data, input_data_size),
        _id_seg_info(0,4),
        _lat_seg_info(4,8),
        _lon_seg_info(12,8)
        {
            
        }
        
        SCLiveMapClientNode * client_node_for_update()  {
            
            int read_id = 0;
            command_form_base_t<TYPE_ID>::read_segment(&read_id, _id_seg_info);
            SCLiveMapClientNode * client_node_for_update = [[SCLiveMapClientNode alloc]init];
            client_node_for_update.id = read_id;
            
            double read_lat = 0.0f;
            command_form_base_t<TYPE_ID>::read_segment(&read_lat, _lat_seg_info);
            
            double read_lon = 0.0f;
            command_form_base_t<TYPE_ID>::read_segment(&read_lon, _lon_seg_info);
            
            client_node_for_update.coordinate = {
                read_lat, read_lon
            };
            
            return client_node_for_update;
        }
    	virtual ~update_node_base(){}
        
        
        

    private:
        typename command_form_base_t<TYPE_ID>::segment_info _id_seg_info;
        typename command_form_base_t<TYPE_ID>::segment_info _lat_seg_info;
        typename command_form_base_t<TYPE_ID>::segment_info _lon_seg_info;
    };
    
    class user_update_node : public update_node_base<USERNODEUPDATE>
    {
    public:
        user_update_node(SCLiveMapClientNode * user_update_info)
        :update_node_base(user_update_info)
        {
            
        }
        
        user_update_node(const char * const input_data, const std::size_t input_data_size)
        :update_node_base(input_data, input_data_size)
        {
            
        }
        
        virtual ~user_update_node () {}
    };
    
    
    class request_user_info : public command_form_base_t<REQUESTUSERINFO>
    {
    public:
        request_user_info():
        command_form_base_t () {
            
        }
        
    };
    
    class set_user_info : public command_form_base_t<SETUSERINFO>
    {
    public:
        set_user_info(NSInteger new_id) :
        command_form_base_t (),
        _new_id_segment(0,4) {
            _new_id_segment = add_segment(&new_id, sizeof(new_id));
        }
        
        set_user_info(const char * const input_data, const std::size_t input_data_size)
        :command_form_base_t(input_data, input_data_size),
        _new_id_segment(0,4) {
            
        }
        
        NSInteger get_id() {
            NSInteger return_id = 0;
            
            read_segment(&return_id, _new_id_segment);
            
            return return_id;
        }
        
    private:
        segment_info _new_id_segment;
        
    };
    
    class near_node_info : public command_form_base_t<NEARNODEINFO>
    {
    public:
        near_node_info()
        :command_form_base_t(),
        _num_of_near_node_info(0,4)
        {
            
        }
        
        near_node_info(const char * const input_data, const std::size_t input_data_size)
        :command_form_base_t(input_data, input_data_size),
        _num_of_near_node_info(0,4)
        {
            
        }
        
        void set_near_node( NSArray<SCLiveMapClientNode *> *near_nodes ) {
            if ( get_entire_size() != 0 )
                return;
            
            int count_num_of_near_node = (int)[near_nodes count];
            
            add_segment(&count_num_of_near_node, 4);
            
            for ( SCLiveMapClientNode *nearNode in near_nodes ){
                int id = (int)nearNode.id;
                double lat = nearNode.coordinate.latitude;
                double lon = nearNode.coordinate.longitude;
                
                add_segment(&id, 4);
                add_segment(&lat, 8);
                add_segment(&lon, 8);
                
            }

        }
        
        NSArray<SCLiveMapClientNode *> * get_near_nodes() {
            int read_num_of_near_node_info = 0;
            read_segment(&read_num_of_near_node_info, _num_of_near_node_info);
            
            NSMutableArray<SCLiveMapClientNode *> * near_nodes = [[NSMutableArray alloc]init];
            
            const int sizeof_id = 4;
            const int sizeof_lat = 8;
            const int sizeof_lon = 8;
            
            int start_begin = 4;
            
            for ( int index = 0; index < read_num_of_near_node_info; index++ ) {
                segment_info id_seginfo(start_begin, sizeof_id);
                segment_info lat_seginfo(start_begin + sizeof_id, sizeof_lat);
                segment_info lon_seginfo(start_begin + sizeof_id + sizeof_lat, sizeof_lon);
                
                start_begin += sizeof_id + sizeof_lat + sizeof_lon;
                
                int read_id = 0;
                double read_lat = 0.0f;
                double read_lon = 0.0f;
                
                read_segment(&read_id, id_seginfo);
                read_segment(&read_lat, lat_seginfo);
                read_segment(&read_lon, lon_seginfo);
                
                SCLiveMapClientNode * read_node = [[SCLiveMapClientNode alloc]init];
                
                read_node.id = read_id;
                read_node.coordinate = {
                    read_lat, read_lon
                };
                
                [near_nodes addObject:read_node];
            }
            
            return near_nodes;
        }
        
    private:
        segment_info _num_of_near_node_info;
    };
    
    class user_viewpoint_update : public update_node_base<USERVIEWPOINTUPDATE>
    {
    public:
        user_viewpoint_update(SCLiveMapClientNode * user_update_info, Coordinate view_point)
        : update_node_base(user_update_info)
        , _viewpoint_lat(20, 8)
        , _viewpoint_lon(28, 8)
        {
            double lat = view_point.latitude;
            double lon = view_point.longitude;
            add_segment(&lat, 8);
            add_segment(&lon, 8);
        }
        
        user_viewpoint_update(const char * const input_data, const std::size_t input_data_size)
        : update_node_base(input_data, input_data_size)
        , _viewpoint_lat(20, 8)
        , _viewpoint_lon(28, 8)
        {
            
        }
        
        const Coordinate get_viewpoint() {
            Coordinate viewpoint;
            double lat = 0.0;
            double lon = 0.0;
            read_segment(&lat, _viewpoint_lat);
            read_segment(&lon, _viewpoint_lon);
            
            viewpoint.latitude = lat;
            viewpoint.longitude = lon;
            
            return viewpoint;
        }
    private:
        segment_info _viewpoint_lat;
        segment_info _viewpoint_lon;
    };
    
    class utf8_message_send : public command_form_base_t<UTF8MESSAGESEND>
    {
    public:
        utf8_message_send(int send_node_id, int recv_node, NSString *msg)
        : command_form_base_t()
        , _sender_id_info(0, 4)
        , _recv_id_info(4, 4)
        , _msg_length_info(8, 4)
        {
            add_segment(&send_node_id, 4);
            add_segment(&recv_node, 4);
            int msg_length = (int)([msg dataUsingEncoding:NSUTF8StringEncoding].length);
            add_segment(&msg_length, 4);
            
            const char *tmp = [msg UTF8String];
            add_segment(tmp, msg_length + 1 );
            
        }
        
        utf8_message_send(const char * const input_data, const std::size_t input_data_size)
        : command_form_base_t(input_data, input_data_size)
        , _sender_id_info(0, 4)
        , _recv_id_info(4, 4)
        , _msg_length_info(8, 4)
        {
            
        }
        
        int sender_id() {
            int id = 0;
            read_segment(&id, _sender_id_info);
            
            return id;
        }
        
        int recv_id() {
            int id = 0;
            read_segment(&id, _recv_id_info);
            
            return id;
        }
        
        NSString* get_msg() {
            
            int msg_length = 0;
            read_segment(&msg_length, _msg_length_info);
            
            char *msg = new char[msg_length + 1];
            
            segment_info msg_info(12, msg_length + 1);
            read_segment(msg, msg_info);
            
            msg[msg_length] = '\0';
           
            NSString *ret_str = [NSString stringWithUTF8String:msg];
            return ret_str;
        }
        
        
    private:
        segment_info _sender_id_info;
        segment_info _recv_id_info;
        segment_info _msg_length_info;
    };

}
}
