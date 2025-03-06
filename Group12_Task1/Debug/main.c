
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
    int a = 0x4000;          // Example 16-bit signed integer (16384)
    unsigned int ua = 0x4000; // Example 16-bit unsigned integer (16384)
    float b = 0.5f;           // Example 32-bit floating-point number
    int c = 0x4000;          // Another 16-bit signed integer (4096)

    // Perform multiplications
    long int_result = multiply_int(a, c);
    long long int_result2 = multiply_int(a, c)<<1;
    unsigned long long uint_result = multiply_uint(ua, ua);
    float float_result = multiply_float(b, b);


    // Print results
    printf("[C28xx_CPU1] Multiplication of int: %d * %d = %ld\n", a, c, int_result);
    printf("[C28xx_CPU1] Multiplication of unsigned int: %u * %u = %llu\n", ua, ua, uint_result);
//    printf("[C28xx_CPU1] Multiplication of float: %.2f * %.2f = %.2f\n", b, b, float_result); // does not work for the CCS

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
