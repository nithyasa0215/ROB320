#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// TODO: Declare any necessary helper functions

void dec_to_hex(char* decimal_str, char* hex_str) {
    // TODO: Convert decimal string to hexadecimal string
    // Example: decimal_str = "255", hex_str = "FF"
    //          decimal_str = "4096", hex_str = "1000"
    int decimal = atoi(decimal_str);
    sprintf(hex_str,decimal);
}

int main() {
    // Read decimal value as string from STDIN
    char decimal_str[8] = {0};
    fgets(decimal_str, sizeof(decimal_str), stdin);

    // Convert decimal string to hexadecimal string
    char hex_str[8] = {0};
    dec_to_hex(decimal_str, hex_str);

    // Write hexadecimal value to STDOUT
    printf("%s", hex_str);
}