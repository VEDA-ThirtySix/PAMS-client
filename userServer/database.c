#include "user_server.h"

sqlite3* init_database() {
    sqlite3 *db;
    int rc = sqlite3_open("parking.db", &db);
    
    if (rc) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    
    // 테이블 생성
    const char *sql_create_users = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "plate TEXT UNIQUE NOT NULL,"
        "home TEXT,"
        "phone TEXT);";
        
    char *err_msg = 0;
    rc = sqlite3_exec(db, sql_create_users, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return NULL;
    }
    
    return db;
}
int save_user_data(sqlite3 *db, BasicInfo *basicInfo) {
    const char *sql = "INSERT INTO users (name, plate, home, phone) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("[DB][ERROR] Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, basicInfo->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, basicInfo->plate, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, basicInfo->home, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, basicInfo->phone, -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if(rc != SQLITE_DONE) {
        printf("[DB][ERROR] Failed to insert data: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("[DB][SUCCESS] User data saved successfully\n");
    return 1;
}

int edit_user_data(sqlite3 *db, BasicInfo *basicInfo) {
    const char *sql = "UPDATE users SET name = ?, home = ?, phone = ? WHERE plate = ?;";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("[DB][ERROR] Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, basicInfo->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, basicInfo->home, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, basicInfo->phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, basicInfo->plate, -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if(rc != SQLITE_DONE) {
        printf("[DB][ERROR] Failed to update data: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    if(sqlite3_changes(db) == 0) {
        printf("[DB][ERROR] No matching plate number found\n");
        return 0;
    }
    
    printf("[DB][SUCCESS] User data updated successfully\n");
    return 1;
}
int check_plate_exists(sqlite3 *db, const char* plate) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM users WHERE plate = ?;";
    int count = 0;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("[DB][ERROR] Failed to prepare statement\n");
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, plate, -1, SQLITE_STATIC);
    
    if(sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return count > 0;
}