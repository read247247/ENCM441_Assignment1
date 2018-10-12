//
//  Parse_internet_side.hpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-29.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#ifndef Parse_internet_side_hpp
#define Parse_internet_side_hpp

#include <stdio.h>
static int porxy_the_message(std::string& message, int length);
static int get_status(const char* message);
static int get_length(const std::string& message);
static void edit(char* input);
#endif /* Parse_internet_side_hpp */
