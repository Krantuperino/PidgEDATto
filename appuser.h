#ifndef APPUSER_H
#define APPUSER_H

#include <stdio.h>
#include "odbc.h"

short appuser_new(SQLCHAR* screenName, SQLCHAR* location);
short appuser_remove(SQLCHAR* screenName);

#endif
