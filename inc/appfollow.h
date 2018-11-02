#ifndef APPFOLLOW_H
#define APPFOLLOW_H

#include <stdio.h>
#include "odbc.h"

short appfollow_new(SQLCHAR* screenName_follower, SQLCHAR* screenName_followeed);
short appfollow_remove(SQLCHAR* screenName_follower, SQLCHAR* screenName_followeed);

#endif
