#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "driverlib.h"
#include "device.h"



#define SAMPLING_RATE 8000     // Sampling rate in Hz (for generating the composite sin waves)
#define NUM_SAMPLES 1024        // Number of samples for Cooley-Tukey FFT Algorithms

//define struct for complex number
typedef struct{
    int32_t real;
    int32_t img;
}Complex;

// Global variables
float signal_samples[NUM_SAMPLES];     // Array of pointers that store generated sample values
float magnitudes[NUM_SAMPLES];  // Stores magnitudes calculated by the Cooley-Tukey FFT Algorithms
uint32_t magnitudes_Fixed[NUM_SAMPLES];
int32_t signal_samples_Fixed[NUM_SAMPLES];
//Complex result[NUM_SAMPLES];



// Function prototypes
Complex* FFT(int32_t* samples, int numSamples);
Complex complex_mul(Complex complex1,Complex complex2);
Complex complex_add(Complex complex1,Complex complex2);
void GenerateSignals(void);
void Magnitude(void);

// Main function
void main() {

        printf("Generate signal\n");
        // Generate tones based on the pressed key
        GenerateSignals();

        printf("Compute FFT\n");
        // compute FFT and Calculate magnitude
        Magnitude();

        printf("After FFT\n");
        printf("Calculate magnitudes\n");


    while (1) {
        //do something here

    }
}
//Calculate Magnitude
void Magnitude(void){

    int i;

    //FFT
    Complex* result = FFT(signal_samples_Fixed,NUM_SAMPLES);

//    //for checking
//    for(i=0; i<NUM_SAMPLES; ++i){
//        result[i].real= temp[i].real;
//        result[i].img = temp[i].img;
//    }


    for(i=0; i<NUM_SAMPLES; ++i){
        /*
        float temp_real = (float)(result[i].real/32768);
        float temp_img = (float)(result[i].img/32768);
        magnitudes[i] = sqrt(pow(temp_real,2)+pow(temp_img,2));
        magnitudes_Fixed[i] = (uint32_t) (magnitudes[i]*1024); //use Q10
        */

        int64_t temp_real = ((int64_t)result[i].real*(int64_t)result[i].real)>>15;
        int64_t temp_img =  ((int64_t)result[i].img*(int64_t)result[i].img)>>15;// (square)->Q30->(root)->Q15 ; normalize?
        uint32_t sum = (uint32_t) (temp_real+temp_img);

        magnitudes[i] = sqrt((float)sum/32768);
        magnitudes_Fixed[i] = (uint32_t) (magnitudes[i]*32768); //Q15

    }

    //free result
    free(result);

}


// Function to perform the the Cooley-Tukey FFT Algorithms (recursive)
Complex* FFT(int32_t* samples, int numSamples) {



    int i;
    int j;

    Complex* Y = (Complex*) malloc(numSamples*sizeof(Complex));
    Complex* Y_even;
    Complex* Y_odd;

    if(numSamples==1){
        //assign samples value to Y
        Y[0].real = samples[0];
        Y[0].img = 0;

    }else{
        //initialize twiddle factor
        Complex WN;
        WN.real = (int32_t) (32768*cos(2*M_PI/numSamples));
        WN.img = (int32_t) (32768*(-sin(2*M_PI/numSamples)));

        Complex W;
        W.real =(int32_t)(1*32768);
        W.img = 0;


        //initialize odd and even samples
        int32_t* samples_even=(int32_t*) malloc((numSamples/2)*sizeof(int32_t));
        int32_t* samples_odd=(int32_t*) malloc((numSamples/2)*sizeof(int32_t));

        //split samples
        for(i=0; i<numSamples/2; ++i){
            samples_even[i] = samples[2*i];
            samples_odd[i] = samples[2*i+1];
        }

        //call recursively
        Y_even = FFT(samples_even,numSamples/2);
        Y_odd = FFT(samples_odd, numSamples/2);



        for(j=0; j<numSamples/2; ++j){
            //for 0~N/2-1
            Y[j] = complex_mul(W,Y_odd[j]);
            Y[j] = complex_add(Y[j],Y_even[j]);

            //for N/2~N-1
            Y[j+numSamples/2] = complex_mul(W,Y_odd[j]);
            //multiply by -1
            Y[j+numSamples/2].real = -(Y[j+numSamples/2].real);
            Y[j+numSamples/2].img = -(Y[j+numSamples/2].img);
            Y[j+numSamples/2] = complex_add(Y_even[j],Y[j+numSamples/2]);

            //update WN
            W = complex_mul(W,WN);
        }
        //free Y even and Y odd
        free(Y_odd);
        free(Y_even);
        //free samples even and odd
        free(samples_even);
        free(samples_odd);
    }

    //printf("FFT-%d\n",numSamples);
    return Y;
}

//(x+jy)*(u+jv)
Complex complex_mul(Complex complex1,Complex complex2){
    Complex result;


    /*
    //multiplication here
    result.real = ((complex1.real)*(complex2.real)>>15)-((complex1.img)*(complex2.img)>>15);//xu-yv
    result.img  = ((complex1.real)*(complex2.img)>>15)+((complex1.img)*(complex2.real)>>15);//j(xv+uy
     */


    int64_t temp1=0;
    int64_t temp2=0;

    temp1 = ((int64_t)complex1.real)*((int64_t)complex2.real)-((int64_t)complex1.img)*((int64_t)complex2.img);
    result.real = (int32_t)(temp1>>15);
    temp2 = ((int64_t)complex1.real)*((int64_t)complex2.img)+((int64_t)complex1.img)*((int64_t)complex2.real);
    result.img = (int32_t)(temp2>>15);


    return result;
}

//(x+jy)+(u+jv)
Complex complex_add(Complex complex1,Complex complex2){
    Complex result;

    //multiplication here
    result.real = (complex1.real)+(complex2.real);//x+u
    result.img  = (complex1.img)+(complex2.img);//j(y+v)

    return result;
}




// Function to generate DTMF tones for the corresponding key pressed
void GenerateSignals(void) {
    int i = 0, f1 = 200, f2 = 600;

    // Generate the combined tones for the pressed key
    for (i = 0; i < NUM_SAMPLES; ++i) {
        signal_samples[i] = sin(2.0 * M_PI * f1 * i / SAMPLING_RATE) + sin(2.0 * M_PI * f2 * i / SAMPLING_RATE);
        signal_samples_Fixed[i] = ((int32_t)(32768 * signal_samples[i])); //fixed point expression Q10 = 1024 Q15 = 32768 Q8.8 10+6 Q16.16 65536
        //printf("signal[%d] = %f\n",i,signal_samples[i]);
    }
}





