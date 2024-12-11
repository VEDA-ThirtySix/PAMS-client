#include "database.h"

/**
 * @brief   데이터베이스를 초기화하고 연결을 설정하는 함수
 * @details SQLite 데이터베이스 파일을 열고, 필요하면 `users` 테이블을 생성합니다.
 *
 * @return  sqlite3* 데이터베이스 연결 객체 포인터. 실패 시 NULL 반환
 *
 * @author 
 * @date 
 */
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

/**
 * @brief   사용자 데이터를 데이터베이스에 저장하는 함수
 * @details `users` 테이블에 새로운 사용자의 정보를 추가합니다.
 *
 * @param[in] db        SQLite 데이터베이스 연결 객체
 * @param[in] basicInfo 저장할 사용자 정보 (이름, 차량 번호판, 주소, 전화번호)
 * @return  int 1: 성공, 0: 데이터 삽입 실패
 *
 * @author 
 * @date 
 */
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

/**
 * @brief   사용자 데이터를 수정하는 함수
 * @details `users` 테이블에서 차량 번호판(plate)을 기준으로 사용자의 이름, 주소 및 전화번호를 업데이트합니다.
 *
 * @param[in] db        SQLite 데이터베이스 연결 객체
 * @param[in] basicInfo 수정할 사용자 정보 (이름, 차량 번호판, 주소, 전화번호)
 * @return  int 1: 성공, 0: 일치하는 번호판 없음 또는 업데이트 실패
 *
 * @author 
 * @date 
 */
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
