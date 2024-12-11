//
// Created by cvrain on 24-12-9.
//

#ifndef USER_INFO_HPP
#define USER_INFO_HPP

#include <QObject>

#include "types/result.hpp"

class UserInfo final : public QObject {
    Q_OBJECT

public:
    static UserInfo* getInstance();
private:
    UserInfo();

    QString userName;
    QString userId;
    QString token;
    bool isLogin;
};


#endif //USER_INFO_HPP
