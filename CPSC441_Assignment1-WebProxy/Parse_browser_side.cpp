//
//  Parse_HTTP.cpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-24.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#include <iostream>
#include <netdb.h>
#include <string>

#include "Parse_browser_side.hpp"

static int get_domain(const char* input, hostent& host_ent){
    if(input == NULL){
        std::cout << "We have called get_domain with null values";
        return -1;
    }
    
    int startIndex, hostIndex, endIndex;
    std::string host;
    std::string toFind("Host: ");
    std::string message(input);
    
    startIndex = (int)message.find(toFind);
    hostIndex = startIndex + 6;
    endIndex = (int)message.find("\r\n", startIndex);
    
    host = message.substr(hostIndex, (endIndex - hostIndex) );
#if DEBUG
    std::cout << "Host is: " << host << "\n";
    std::cout << "Start and end are: " << hostIndex << " " << endIndex << "\n";
#endif
    if(!host.empty()){
        if(host.find(":") != std::string::npos){
            std::cout << "The browser attempted a SSH/HTTPS connection\n";
            exit(1);
        }
        host_ent = *gethostbyname(host.c_str());
    } else {
        std::cout << "We were unable to find a domain in the request from the browser\n";
        return -1;
    }
    return 0;
}
