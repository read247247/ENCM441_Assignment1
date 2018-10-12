//
//  Server.cpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-22.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#include "Parse_browser_side.cpp"
#include "Browser_side.hpp"

#define BUFFER_SIZE 1500000
#define GET_FIRST_TWO_LINES 0

static sockaddr_in& s_initialize_address(sockaddr_in& server, const int PORTNUMBER){
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORTNUMBER);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
#if DEBUG
    std::cout << "Completed s_initialize_address\n";
#endif
    return server;
}

static int s_create_socket(){
    int the_socket_s_create_socket;
    the_socket_s_create_socket = socket(AF_INET, SOCK_STREAM, 0);
#if DEBUG
    std::cout << "Completed s_create_socket.\n";
#endif
    return the_socket_s_create_socket;
}

static int s_bind_socket(const int the_socket, sockaddr_in& server){
//Makes the socket reusable if the program crashes
    int status_placeholder = 1;
    if (setsockopt(the_socket, SOL_SOCKET, SO_REUSEADDR, &status_placeholder, sizeof(status_placeholder)) == -1) {
        std::cerr << "setsockopt failed!!";
        return -1;
    }
//Binds the socket
    status_placeholder = bind(the_socket, (struct sockaddr *) &server, sizeof(struct sockaddr_in));
#if DEBUG
    std::cout << "Completed s_bind_socket.\n";
#endif
    return status_placeholder;
}

static int s_socket_listen(const int the_socket){
    int status;
    status = listen(the_socket, 5);
#if DEBUG
    std::cout << "Completed listen. Now waiting for browser\n";
#endif
    return status;
}

static int s_accept_connection(const int the_socket){
    int status;
    status = accept(the_socket, NULL, NULL);
#if DEBUG
    std::cout << "Completed s_accept_connection: Connection with browser established.\n";
#endif
    return status;
}

static long s_send_data(const int connected_sock, const char* msg_from_server){
    ssize_t check_s_send_data;
    check_s_send_data = send(connected_sock, msg_from_server, 100000, 0);
    if (check_s_send_data < 0) {
        std::cout << "Error in send()\n";
        exit(1);
    }
    std::cout << "Completed s_send_data: data from internet has been sent to browser\n";
    return check_s_send_data;
}

static int s_receive_data(const int connected_sock, char* rcv_message, hostent& host_ent){
    ssize_t bytes_recvd;
    int status;
#if DEBUG
    std::cout << "About to recv\n";
#endif
    bytes_recvd = recv(connected_sock, rcv_message, BUFFER_SIZE, 0);
#if DEBUG
    std::cout << "Completed recv\n";
    std::cout << "Browser says:" << rcv_message << "\n";
#endif
    if(bytes_recvd <= 0){
        return (int)bytes_recvd;
    }
    
    status = get_domain(rcv_message, host_ent);
    if(status == -1){
        return -2; //-1 return value is used above for recv faliure
    }
#if GET_FIRST_TWO_LINES
    int bytes_traversed = 0;
    while(*rcv_message != '\n'){
        rcv_message++;
        bytes_traversed++;
    }
    rcv_message++;
    while(*rcv_message != '\n'){
        rcv_message++;
        bytes_traversed++;
    }
    rcv_message++;
    memset(rcv_message, 0, BUFFER_SIZE - bytes_traversed);
    *rcv_message = '\n';
    rcv_message = rcv_message - 2;
    rcv_message = rcv_message - bytes_traversed;
    std::cout << "rcv_message after removing lines is: \n" << rcv_message << '\n';
#endif
    //else {
#if DEBUG
        std::cout << "Finished s_recieve_data: " << bytes_recvd << "\n";
#endif
        return (int)bytes_recvd;
    //}
}
