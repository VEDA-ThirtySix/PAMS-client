#ifndef DATABASE_H
#define DATABASE_H

#include "types.h"
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

sqlite3* init_database(void);
int save_user_data(sqlite3 *db, BasicInfo *basicInfo);
int edit_user_data(sqlite3 *db, BasicInfo *basicInfo);
//int delete_user_data(sqlite3 *db, BasicInfo *basicInfo);


#endif//DATABASE_H