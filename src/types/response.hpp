//
// Created by cvrain on 24-12-21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <cpprest/json.h>
#include <cpprest/details/web_utilities.h>

class BaseResponse {
public:
    std::string message;
    std::string result;
    int code;
};

class CommonResponse: public BaseResponse {
public:
    web::json::value data;
};

#endif //RESPONSE_HPP
