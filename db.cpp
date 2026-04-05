#include "db.h"
#include <iostream>
using namespace std;

string DB_PASSWORD = "1234567890";

sql::Connection* getConnection(){
    sql::mysql::MySQL_Driver *driver;
    driver = sql::mysql::get_mysql_driver_instance();

    sql::Connection *con = driver->connect(
        "tcp://127.0.0.1:3306",
        "root",
        DB_PASSWORD
    );

    con->setSchema("BANKING_SERVER");
    return con;
}