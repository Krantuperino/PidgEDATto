#ifndef APPTWEET_H
#define APPTWEET_H

#include <stdio.h>
#include "odbc.h"

short apptweet_new(SQLCHAR* screenName, SQLCHAR* text);
short apptweet_remove(SQLINTEGER tweet_id);
short apptweet_retweet(SQLCHAR* screenName, SQLINTEGER tweet_id);

#endif
