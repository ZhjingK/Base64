#ifndef BASE64_H
#define BASE64_H

const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

//Base64 ����
void Base64_encode(const char* filename);

//Base64 ����
void Base64_decode(const char *filename);

#endif