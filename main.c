/**
 * @file main.c
 * @author Nguyễn Quang Minh mssv:20234024
 * @brief
 * Lập trình thuật toán mã hoá Playfair
 * Input: Bản rõ 
 * Key: Tự định nghĩa 
 * Output: Bản mã 
 * Note: Khi có ký tự giống nhau trong cùng 1 cặp, đổi thành X. Sử dụng I thay cho J.
 * @version 0.1
 * * @date 2026-02-28
 * * @copyright Copyright (c) 2026
 * */

#include <stdio.h>

#define SIZE 5

// tính độ dài chuỗi
int my_strlen(const char *str) {
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

// sao chép chuỗi src sang dest
void my_strcpy(char *dest, const char *src) {
    int i = 0;
    while(src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// chuyển ký tự thường -> hoa
char to_upper(char c) {
    if(c >= 'a' && c <= 'z') {
        return c - ('a' - 'A'); // c - 32
    }
    return c;
}

// kiểm tra c có nằm trong mảng str (độ dài len) hay chưa
int isExist(char c, char *str, int len) {
    int i;
    for(i = 0; i < len; i++){
        if(str[i] == c) {
            return 1;
        }
    }
    return 0;
}

// Xây dựng ma trận 5x5 từ key (đổi 'J' thành 'I')
void buildMatrix(char *key, char matrix[SIZE][SIZE]) {
    char temp[26]; // chứa tạm các ký tự đã dùng
    int idx = 0;   // số ký tự đã đưa vào temp
    
    // Khởi tạo temp
    for(int i = 0; i < 26; i++){
        temp[i] = '\0';
    }

    // Bước 1: duyệt key, chuyển hoa, đổi 'J' thành 'I', bỏ trùng
    int lenKey = my_strlen(key);
    for(int i = 0; i < lenKey; i++){
        char c = to_upper(key[i]);
        
        // chỉ lấy A-Z
        if(c < 'A' || c > 'Z') {
            continue;
        }
        // Đổi 'J' thành 'I' thay vì bỏ qua hoàn toàn
        if(c == 'J') {
            c = 'I';
        }
        // chưa có thì thêm
        if(!isExist(c, temp, idx)){
            temp[idx++] = c;
        }
    }

    // Bước 2: điền các ký tự còn thiếu (A-Z, bỏ J vì đã gộp vào I)
    for(char c = 'A'; c <= 'Z'; c++){
        if(c == 'J') {
            continue;
        }
        if(!isExist(c, temp, idx)) {
            temp[idx++] = c;
        }
        if(idx == 25) {
            break;
        }
    }

    // Bước 3: đổ temp vào ma trận 5x5
    int k = 0;
    for(int r = 0; r < SIZE; r++){
        for(int c = 0; c < SIZE; c++){
            matrix[r][c] = temp[k++];
        }
    }
}

// Tìm vị trí (row, col) của ký tự c trong matrix
void findPosition(char c, char matrix[SIZE][SIZE], int *row, int *col){
    for(int r = 0; r < SIZE; r++){
        for(int cc = 0; cc < SIZE; cc++){
            if(matrix[r][cc] == c){
                *row = r;
                *col = cc;
                return;
            }
        }
    }
}

// Mã hoá 2 ký tự a, b -> out (gồm 2 ký tự)
void encryptPair(char a, char b, char matrix[SIZE][SIZE], char *out){
    int r1, c1, r2, c2;
    findPosition(a, matrix, &r1, &c1);
    findPosition(b, matrix, &r2, &c2);

    // 1. Cùng hàng
    if(r1 == r2){
        c1 = (c1 + 1) % SIZE;
        c2 = (c2 + 1) % SIZE;
    }
    // 2. Cùng cột
    else if(c1 == c2){
        r1 = (r1 + 1) % SIZE;
        r2 = (r2 + 1) % SIZE;
    }
    // 3. Khác hàng, khác cột
    else {
        // đổi cột
        int temp = c1;
        c1 = c2;
        c2 = temp;
    }

    out[0] = matrix[r1][c1];
    out[1] = matrix[r2][c2];
}

// Chuẩn hoá plaintext theo đúng quy tắc cặp của Playfair
int preparePlaintext(const char *input, char *output){
    int i;
    char temp[1024];
    int lenTemp = 0;

    // 1. Loại bỏ ký tự không hợp lệ, chuyển hoa, đổi 'J' thành 'I'
    for(i = 0; input[i] != '\0'; i++){
        char c = to_upper(input[i]);
        if(c >= 'A' && c <= 'Z'){
            if(c == 'J') {
                c = 'I';
            }
            temp[lenTemp++] = c;
        }
    }
    temp[lenTemp] = '\0';

    // 2. Chia cặp và chèn 'X' đúng quy tắc
    int newLen = 0;
    i = 0;
    while(i < lenTemp){
        // Ký tự đầu tiên của cặp
        output[newLen++] = temp[i];

        // Nếu còn ký tự thứ 2
        if(i + 1 < lenTemp){
            if(temp[i] == temp[i+1]){
                // Trùng nhau trong cùng 1 cặp -> chèn 'X'
                output[newLen++] = 'X';
                i++; // Chỉ tiến 1 bước
            } else {
                // Khác nhau -> lấy làm ký tự thứ 2 của cặp
                output[newLen++] = temp[i+1];
                i += 2; // Đã xong 1 cặp, tiến 2 bước
            }
        } else {
            // Lẻ ký tự cuối cùng -> thêm 'X'
            output[newLen++] = 'X';
            i++;
        }
    }
    output[newLen] = '\0';

    return newLen;
}

int main(){
    // Key cho trước
    char key[] = "MORNACHY";
    // Bản rõ cho trước
    char plaintext[] = "Do you like to study a cryptography course";

    // Mảng matrix 5x5
    char matrix[SIZE][SIZE];
    buildMatrix(key, matrix);

    printf("Ma tran Playfair:\n");
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("--------------------\n");

    // Chuẩn hoá plaintext
    char cleanText[1024];
    int newLen;
    
    // Gọi hàm prepare
    newLen = preparePlaintext(plaintext, cleanText);

    // Mảng chứa kết quả mã hoá
    char ciphertext[1024];
    int k = 0;

    // Mỗi 2 ký tự -> encryptPair
    for(int i = 0; i < newLen; i += 2){
        encryptPair(cleanText[i], cleanText[i+1], matrix, &ciphertext[k]);
        k += 2;
    }
    ciphertext[k] = '\0';

    // In kết quả
    printf("Plaintext goc      : %s\n", plaintext);
    printf("Plaintext chia cap : ");
    for(int i = 0; i < newLen; i+=2) {
        printf("%c%c ", cleanText[i], cleanText[i+1]);
    }
    printf("\n");
    printf("Key                : %s\n", key);
    printf("Ciphertext         : %s\n", ciphertext);

    return 0;
}
