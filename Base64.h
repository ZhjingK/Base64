#ifndef BASE64_H
#define BASE64_H

const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

//Base64 ±àÂë
void Base64_encode(const char* filename);

//Base64 ½âÂë
void Base64_decode(const char *filename);

#endif