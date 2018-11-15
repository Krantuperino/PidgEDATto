#ifndef APPREQ_H
#define APPREQ_H

#include <stdio.h>
#include "odbc.h"

short appreq_user(SQLCHAR* screenName);
short appreq_tweets(SQLCHAR* screenName);
short appreq_retweets(SQLCHAR* tweet_id);
short appreq_maxrt();
short appreq_maxfw();


#endif
