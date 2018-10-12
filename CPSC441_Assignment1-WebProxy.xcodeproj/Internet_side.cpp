//
//  Client.cpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-22.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//
#include <iostream>
#include <netdb.h>
#include <string>

#include "Internet_side.hpp"
#include "Parse_internet_side.cpp"

#define BUFFER_SIZE 1500000
#define SIZE_TO_SEND_TO_INTERNET 10000

using namespace std;

static int c_create_initialize_address(const int MYPORTNUMBER, const hostent host_ent, sockaddr_in& server){
    if(host_ent.h_name == NULL){
        return -1;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(MYPORTNUMBER);
    bcopy( (char*) host_ent.h_addr, (char*)&server.sin_addr.s_addr, host_ent.h_length );
#if DEBUG
    cout << "Complete c_create_initialize_address\n";
#endif
    return 0;
}

static int c_create_socket(){
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
#if DEBUG
    cout << "Complete c_create_socket\n";
#endif
    return sock;
}

static int c_connect_to_server(sockaddr_in& server, const int sock){
    int status;
#if DEBUG
    cout << "About to connect to internet\n";
#endif
    status = connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in));
#if DEBUG
    cout << "Complete c_connect_to_server: \n";
#endif
    return status;
}

static int c_talk(const int sock, char* msg_to_snd, char* rcv_msg){
    ssize_t status;
    int length = 0;
    if(msg_to_snd[0] == 0){
        cout << "No data to send to server\n";
        return 1;
    }
//Send data
    status = send(sock, msg_to_snd, SIZE_TO_SEND_TO_INTERNET, 0);
#if DEBUG
    cout << "Completed send of data to internet\n";
#endif
    if (status < 0) {
        cout << "Error in sending to server\n";
        return (int)status;
    }
    
//Receive data
    size_t total_recieved = 0;
    while ( 1 ) {
#if DEBUG
        cout << "about to recieve once\n";
#endif
        ssize_t status = recv( sock, rcv_msg, BUFFER_SIZE, 0 );
#if DEBUG
        cout << "recieved once\n" << rcv_msg << "\n";
#endif
        if ( status == -1 ){
            cout << "Error in recving from server\n";
            return (int)status;
        }
        if ( status == 0 ) break; // got end-of-stream
        rcv_msg += status;
        total_recieved += status;
    }
    rcv_msg -= total_recieved;
    rcv_msg[12] = ' ';      //DONT CHANGE THIS IT WILL BREAK 404 and 304 MESSAGES I DONT KNOW WHY
#if DEBUG
    cout << "Completed receive from server\n" << total_recieved;
#endif
    cout << "Server said: \n" << rcv_msg << "\n";
    
    std::string message(rcv_msg);
    int http_status = get_status(rcv_msg);
    if(http_status != 304 && http_status != 404){
        length = get_length(message);
    }
    if( (message.find("Content-Type: text/plain") != std::string::npos) ||
       (message.find("Content-Type: text/html") != std::string::npos)
       )
    {
#if DEBUG
        cout << "Status: " << http_status << "\n";
        cout << "Length: " << length << "\n";
#endif
        porxy_the_message(message, length);
        memset(rcv_msg, 0, BUFFER_SIZE);
        strcpy(rcv_msg, message.c_str());
#if DEBUG
        cout << "Complete c_talk: " << rcv_msg << "\n";
#endif
        return http_status;
    }

#if DEBUG
    cout << "Content is not text or html\n";
    cout << "Complete c_talk, ";
#endif
    return http_status;
}
