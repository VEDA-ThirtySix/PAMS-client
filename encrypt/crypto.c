#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>    //RSA 암호화
#include <openssl/pem.h>    //PEM 파일
#include <openssl/aes.h>    //AES 암호화
#include <openssl/rand.h>   //난수 생성
#include <openssl/evp.h>    //암호화 인터페이스

#define AES_KEY_SIZE 256    //AES KEY: 256bit
#define AES_BLOCK_SIZE 16   //AES BLOCK: 16byte

// AES 키 생성 함수
unsigned char* generate_aes_key(int *len) {
    unsigned char *aes_key = (unsigned char*)malloc(AES_KEY_SIZE/8);    
    if (RAND_bytes(aes_key, AES_KEY_SIZE/8) != 1) { //난수로 AES 키 생성
        free(aes_key);
        return NULL;
    }
    *len = AES_KEY_SIZE/8;
    return aes_key;
}

// RSA로 AES 키 암호화
unsigned char* rsa_encrypt_key(RSA *rsa, unsigned char *aes_key, int aes_key_len, int *encrypted_len) {
    unsigned char *encrypted = (unsigned char*)malloc(RSA_size(rsa));
    //OAEP 패딩 사용
    *encrypted_len = RSA_public_encrypt(aes_key_len, aes_key, encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    return encrypted;
}

// AES로 데이터 암호화
unsigned char* aes_encrypt_data(unsigned char *aes_key, unsigned char *data, int data_len, int *encrypted_len) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new(); //암호화 된 컨텍스트(ctx) 생성
    unsigned char *encrypted = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);   //암호화된 데이터 저장할 메모리 할당
    unsigned char iv[AES_BLOCK_SIZE];   //초기화 벡터(IV) 배열 선언
    int len;

    // IV 생성
    RAND_bytes(iv, AES_BLOCK_SIZE);

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, iv);
    EVP_EncryptUpdate(ctx, encrypted + AES_BLOCK_SIZE, &len, data, data_len);
    *encrypted_len = len;
    
    EVP_EncryptFinal_ex(ctx, encrypted + AES_BLOCK_SIZE + len, &len);
    *encrypted_len += len;

    // IV를 암호문 앞에 추가
    memcpy(encrypted, iv, AES_BLOCK_SIZE);
    *encrypted_len += AES_BLOCK_SIZE;

    EVP_CIPHER_CTX_free(ctx);
    return encrypted;
}

// 메인 함수
int main() {
    // RSA 키 쌍 생성
    RSA *rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if (!rsa) {
        printf("RSA 키 생성 실패\n");
        return -1;
    }

    // AES 키 생성
    int aes_key_len;
    unsigned char *aes_key = generate_aes_key(&aes_key_len);
    if (!aes_key) {
        printf("AES 키 생성 실패\n");
        RSA_free(rsa);
        return -1;
    }

    // 테스트 데이터
    const char *data = "Hello, Hybrid Encryption!";
    int data_len = strlen(data);

    // AES 키를 RSA로 암호화
    int encrypted_key_len;
    unsigned char *encrypted_key = rsa_encrypt_key(rsa, aes_key, aes_key_len, &encrypted_key_len);
    if (encrypted_key_len == -1) {
        printf("AES 키 암호화 실패\n");
        free(aes_key);
        RSA_free(rsa);
        return -1;
    }

    // 데이터를 AES로 암호화
    int encrypted_data_len;
    unsigned char *encrypted_data = aes_encrypt_data(aes_key, (unsigned char*)data, data_len, &encrypted_data_len);

    // 결과 출력
    printf("원본 데이터: %s\n", data);
    printf("암호화된 AES 키 길이: %d\n", encrypted_key_len);
    printf("암호화된 데이터 길이: %d\n", encrypted_data_len);

    // 메모리 해제
    free(aes_key);
    free(encrypted_key);
    free(encrypted_data);
    RSA_free(rsa);

    return 0;
}
