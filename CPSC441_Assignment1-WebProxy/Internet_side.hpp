//
//  Client.hpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-22.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#ifndef Internet_side_hpp
#define Internet_side_hpp

static int c_create_initialize_address(const int MYPORTNUMBER, const hostent host_ent, sockaddr_in& server);
static int c_create_socket();
static int c_connect_to_server(sockaddr_in& server, const int sock);
static int c_talk(const int sock, char* msg_to_snd, char* rcv_msg);

#endif /* Internet_side_hpp */
