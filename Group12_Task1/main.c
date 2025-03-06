
#include <stdio.h>
#include "F28x_Project.h" // Includes the necessary device headers and libraries

// Function prototypes
long long multiply_int(int x, int y);
unsigned long long multiply_uint(unsigned int x, unsigned int y);
float multiply_float(float x, float y);


int main(void) {


    puts("This is the numerical Chapter!");

    fflush(stdout);

// Define some variables
    int a = 0x3039;          // Example 16-bit signed integer (12345)
    unsigned int ua = 0x3039; // Example 16-bit unsigned integer (12345)
    float b = 0.8f;           // Example 32-bit floating-point number
    int c = 0x3039;          // Another 16-bit signed integer (12345)

    // Perform multiplications
    long int_result = multiply_int(a, c);//multiply two 16-bit signed int
    long long int_result2 = multiply_int(a, c)<<1;
    unsigned long long uint_result = multiply_uint(ua, ua);
    float float_result = multiply_float(b, b);


    // Print results
    printf("[C28xx_CPU1] Multiplication of int: %d * %d = %ld\n", a, c, int_result);
    printf("[C28xx_CPU1] Multiplication of unsigned int: %u * %u = %llu\n", ua, ua, uint_result);
    char precise[5];//build a array to store string
    sprintf(precise, "%.2f", float_result);//store with two decimal place
    printf("[C28xx_CPU1] Multiplication of float: %.2f * %.2f = %s\n", b, b, precise); // does not work for the CCS
    //get the spring of float
    fflush(stdout);



    return 0;
}

// Function to multiply two 16-bit signed integers and return a 64-bit result
long long multiply_int(int x, int y) {
    return (long long)x * (long long)y;
}

// Function to multiply two 16-bit unsigned integers and return a 64-bit result
unsigned long long multiply_uint(unsigned int x, unsigned int y) {
    return (unsigned long long)x * (unsigned long long)y;
}

// Function to multiply two 32-bit floating-point numbers
float multiply_float(float x, float y) {
    return x * y;
}
