//
//  Server.hpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-22.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#ifndef Browser_side_hpp
#define Browser_side_hpp

static sockaddr_in& s_initialize_address(sockaddr_in& server, const int PORTNUMBER);
static int s_create_socket();
static int s_bind_socket(const int the_socket, sockaddr_in& server);
static int s_socket_listen(const int the_socket);
static int s_accept_connection(const int the_socket);
static long s_send_data(const int connected_sock, const char* msg_from_server);
static int s_receive_data(const int connected_sock, char* rcv_message, hostent& host_ent);

#endif /* Browser_side_hpp */
