//
//  Parse_HTTP.hpp
//  Assignment1
//
//  Created by Nathan Godard on 2018-09-24.
//  Copyright © 2018 Nathan Godard. All rights reserved.
//

#ifndef Parse_HTTP_hpp
#define Parse_HTTP_hpp

static int get_domain(const char* input, hostent& host_ent);
static void get_host(std::vector<std::string>& http_request, std::string& host);

#endif /* Parse_HTTP_hpp */
