#ifndef DB_H
#define DB_H

#include <mysql/jdbc.h>
using namespace std;

sql::Connection* getConnection();

#endif