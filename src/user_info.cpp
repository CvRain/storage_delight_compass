//
// Created by cvrain on 24-12-9.
//

#include "user_info.hpp"

UserInfo* UserInfo::getInstance() {
    static UserInfo instance;
    return &instance;
}

UserInfo::UserInfo() {
}
