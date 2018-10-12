//
//  main.cpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-22.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#include <unistd.h>
#include "Browser_side.cpp"
#include "Internet_side.cpp"

#define SERVERPORTNUMBER 8124
#define CLIENTPORTNUMBER 80
#define DEBUG 0 //Turn on verbose debugging

int main(int argc, const char * argv[]) {
    srand( (unsigned int)time(NULL) );
    int status = -1;
    int the_browser_socket = -1;
    int the_internet_socket = -1;
    int s_connected_sock = -1;
    char message_from_server[BUFFER_SIZE];
    char message_from_browser[BUFFER_SIZE];
    hostent *host_ent = new hostent;
    struct sockaddr_in browser_sockaddr_in;
    struct sockaddr_in internet_sockaddr_in;
    
    s_initialize_address(browser_sockaddr_in, SERVERPORTNUMBER);  //Initialize address
    
    the_browser_socket = s_create_socket();  //Create socket
    
    status = s_bind_socket(the_browser_socket, browser_sockaddr_in);  //Bind socket to address and port
    if(status < 0){
        cout << "The socket failed to bind\n";
        exit(1);
    }
    
    status = s_socket_listen(the_browser_socket);
    if(status < 0){
        cout << "The socket refused to enter listening state\n";
        exit(1);
    }

    while(1){       //Main loop for listening for connection and communcating
        std::cout << "Starting while loop: \n";
        memset(message_from_browser, 0, sizeof message_from_browser);
        memset(message_from_server, 0, sizeof message_from_server);
        
        s_connected_sock = s_accept_connection(the_browser_socket);  //Connect to Browser
        
        status = s_receive_data(s_connected_sock, message_from_browser, *host_ent);  //Receive data
        if(status == 0){ //Nothing recieved
            std::cout << "We did not recieve anything from the browser!\n";
            continue;
        } else if (status == -1) { //ERROR
            std::cout << "Error in recieving from browser\n";
            continue;
        }
        else if(status == -2){  //Get_domain failed
            cout << "Error in getting domain to forward message to\n";
            continue;
        }
        
        status = c_create_initialize_address(CLIENTPORTNUMBER, *host_ent, internet_sockaddr_in);  //Initialize client socket
        if(status < 0){
            cout << "Could not set up connection to the internet (c_create_initialize_address failed)\n";
            break;
        }
    
        the_internet_socket = c_create_socket();  //Create client side of socket
        if(the_internet_socket == -1){
            cout << "Error in while creating server side socket\n";
            break;
        }

        status = c_connect_to_server(internet_sockaddr_in, the_internet_socket);  //Connecting to server
        if(status < 0){
            cout << "Error in connecting to internet socket\n";
            break;
        }
        
        status = c_talk(the_internet_socket, message_from_browser, message_from_server);  //Talk to server
        if(status < 0){continue;}       //Error in sending
        else if (status == 1){continue;} //No data in buffer to send
        close(the_internet_socket);
        the_internet_socket = -1;
        if(status == 200 || status == 301 || status == 302 || status == 403 || status == 404){
            s_send_data(s_connected_sock, message_from_server);  //Send data to browser
        }
    } //End while
    
//Close the sockets
    if(the_internet_socket != -1){close(the_internet_socket);}
    if(s_connected_sock != -1){close(s_connected_sock);}
    if(the_browser_socket != -1){close(the_browser_socket);}
    
    return 0;
} //end main
