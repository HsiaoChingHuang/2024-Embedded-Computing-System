#include <stdio.h>
#include <math.h>

#define SAMPLING_RATE 8000     // Sampling rate in Hz (for generating the tones)
#define NUM_TARGETS 8          // Number of target frequencies (DTMF frequencies)
#define NUM_SAMPLES 205        // Number of samples for Goertzel algorithm

// Global variables
float magnitudes[NUM_TARGETS];  // Stores magnitudes calculated by the Goertzel algorithm
float samples[NUM_SAMPLES];     // Array to store generated sample values
int Flag = 1;                   // Flag to track valid/invalid key presses
char digit;                     // Holds the user input (digit) from the keypad

// Function prototypes
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[]);
void Frequency_detection(float magnitudes[]);
void Generate_tones(char digit);

// Function to perform the Goertzel algorithm for multiple frequencies
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[]) {


    int n;
    int j;


    //iterative function calculating X[k]=y_k[N]
    for(j =0; j<NUM_TARGETS; j++){

       //record v[] -2, -1, 0~N-1
        float V[NUM_SAMPLES+2];
        V[0]=0; //v[-2]
        V[1]=0;//v[-1]

        //start with V[2]=v[0]
        float k=targetFrequencies[j]*NUM_SAMPLES/SAMPLING_RATE;
        float dummy= 2*M_PI*k/NUM_SAMPLES;


       //find v_k[N-1] and v_k[N]
        for(n=2; n<NUM_SAMPLES+2; n++){
            V[n]=2*cos(dummy)*V[n-1]-V[n-2]+samples[n-2];
        }

        //caclulate X[k]=y_k[N]=v_k[N]-W_n^kv_k[N-1]
        //W_n^k to exponential and to cos +j sin for calculating mag.
        //note that v[N]=V[NUM+1]
        float a = pow(V[NUM_SAMPLES+1]-V[NUM_SAMPLES]*cos(dummy), 2);
        float b = pow(V[NUM_SAMPLES]*sin(dummy),2);
        magnitudes[j]=pow(a+b, 0.5);

    }

    //---------------For the program to run, i have to comment out this printf, because it's not working properly----------------
    /*
    int i;
    // Print the magnitudes for each frequency
    for (i = 0; i < NUM_TARGETS; ++i) {
        printf("Magnitude for frequency: %.1f Hz, magnitude: %.2f\n", targetFrequencies[i], magnitudes[i]);
    }
    */

}

// Function to detect the key based on frequency magnitudes
void Frequency_detection(float magnitudes[]) {
    int i;
    char pad[4][4] = {  // 4x4 keypad representation (DTMF)
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };
    char result;
    int max1 = 0, max2 = 0, i1 = -1, i2 = -1;

    // Find the strongest frequency in the low group (697, 770, 852, 941)
    for (i = 0; i < 4; i++) {
        if (magnitudes[i] > max1) {
            max1 = magnitudes[i];
            i1 = i;
        }
        // Find the strongest frequency in the high group (1209, 1336, 1477, 1633)
        if (magnitudes[i + 4] > max2) {
            max2 = magnitudes[i + 4];
            i2 = i;
        }
    }

    // Determine which key was pressed
    result = pad[i1][i2];


    // Print the result based on the flag
    if (Flag == 1) {
        printf("The key you pressed is: %c\n", result);  // Valid key press
    } else {
        printf("Invalid key press detected. Please try again.\n");
        Flag = 1;  // Reset flag
    }

}

// Function to generate DTMF tones for the corresponding key pressed
void Generate_tones(char digit) {
    int i = 0, f1 = 0, f2 = 0;

    // Map the key press to the corresponding DTMF frequencies
    switch (digit) {
        case '0': f1 = 941; f2 = 1335; break;
        case '1': f1 = 697; f2 = 1209; break;
        case '2': f1 = 697; f2 = 1335; break;
        case '3': f1 = 697; f2 = 1477; break;
        case '4': f1 = 770; f2 = 1209; break;
        case '5': f1 = 770; f2 = 1335; break;
        case '6': f1 = 770; f2 = 1477; break;
        case '7': f1 = 852; f2 = 1209; break;
        case '8': f1 = 852; f2 = 1335; break;
        case '9': f1 = 852; f2 = 1477; break;
        case '*': f1 = 941; f2 = 1209; break;
        case '#': f1 = 941; f2 = 1477; break;
        case 'A': f1 = 697; f2 = 1633; break;
        case 'B': f1 = 770; f2 = 1633; break;
        case 'C': f1 = 852; f2 = 1633; break;
        case 'D': f1 = 941; f2 = 1633; break;
        default : f1 = 0; f2 = 0; Flag = 0; break;  // Invalid key
    }

    // Generate the combined tones for the pressed key
    for (i = 0; i < NUM_SAMPLES; ++i) {
        samples[i] = sin(2.0 * M_PI * f1 * i / SAMPLING_RATE) + 0.5 * sin(2.0 * M_PI * f2 * i / SAMPLING_RATE);
    }
}

// Main function
int main() {
    while (1) {
        // Get user input (key press)
        printf("Type a key (0-9, A-D, *, #) and press return:\n");
        scanf("\n%c", &digit);

        // Generate tones based on the pressed key
        Generate_tones(digit);

        // Define the target DTMF frequencies (standard DTMF frequencies)
        float targetFrequencies[NUM_TARGETS] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};

        // Call the Goertzel algorithm to calculate the magnitudes for each target frequency
        goertzel_multi(samples, NUM_SAMPLES, targetFrequencies);

        // Detect the pressed key based on the frequency magnitudes
        Frequency_detection(magnitudes);

    }
}

