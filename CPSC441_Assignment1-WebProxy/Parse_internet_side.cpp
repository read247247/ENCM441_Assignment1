//
//  Parse_internet_side.cpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-29.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Parse_internet_side.hpp"

#define PERCENT_ERRORS 0.05

static int porxy_the_message(std::string& message, int length){
    int errors_made = 0;
    int starting_position;
    char* cstr;
    bool can_edit = true;
    bool edit_now = false;
    if(message.empty()){
        std::cout << "You tried to porxy a nonexistant message\n";
        return -1;
    }
    starting_position = (int)message.length() - length;
    
    cstr = new char[message.length() + 1];
    strcpy(cstr, message.c_str());
    
    for(int i = starting_position; i < message.length(); i++){
        if( rand() % message.length() < message.length() * PERCENT_ERRORS){
            edit_now = true;
        }
        if(cstr[i] == '<'){
            can_edit = false;
            continue;
        } else if(cstr[i] == '>'){
            can_edit = true;
            continue;
        } else if(cstr[i] != ' ' && cstr[i] != '\n' && cstr[i] != '\r' && cstr[i] != '\t' && can_edit == true){
            if(edit_now){
                edit(&cstr[i]);
                edit_now = false;
                errors_made++;
    }   }   }

    message = cstr;
    delete [] cstr;
    return 0;
}

static int get_status(const char* message){
    if(message == NULL){
        return -1;
    }
    char* cursor = (char*)message;
    cursor += 9;
    std::string status(cursor, cursor + 3);
    std::string::size_type sz;
    
    int status_as_int = std::stoi (status,&sz);
    return status_as_int;
}

static int get_length(const std::string& message){
    int index = (int)message.find("Content-Length:");
    index += 16;
    int newline_postition = (int)message.find('\n', index - 1);
    if(newline_postition == -1){
        std::cout << "There is no line break in the message after 'Content-Length:'\n";
        exit(1);
    }
    std::string s_length = message.substr(index, newline_postition);
    std::string::size_type sz;
    int length = std::stoi(s_length, &sz);
    return length;
}

static void edit(char* input){
    char c;
    int r;
    r = rand() % 26;
    c = 'a' + r;
    *input = c;
}
