#ifndef APPTWEET_H
#define APPTWEET_H

#include <stdio.h>
#include "odbc.h"

short apptweet_new(SQLCHAR* screenName, SQLCHAR* text);
short apptweet_remove(SQLCHAR* tweet_id);
short apptweet_retweet(SQLCHAR* screenName, SQLCHAR *tweet_id);

#endif
