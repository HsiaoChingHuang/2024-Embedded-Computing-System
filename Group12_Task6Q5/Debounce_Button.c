// Included Files
#include "driverlib.h"
#include "device.h"
#include "gpio.h"

// Global Variables
uint16_t pressCount;

#define DEVICE_GPIO_PIN_BUTTON 32 //use GPIO32 as the input



// Function Prototypes
__interrupt void buttonISR(void);

//
// Main
//
int main(void)
{

    // Initialize device clock, peripherals, PIE, and interrupt module
    Device_init();
    Interrupt_initModule();
    Interrupt_initVectorTable();

    //Initialize GPIO and configure as push-pull pin
    //set direction to out
    Device_initGPIO();

    //output LED
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    //turn off first
    GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);// #define DEVICE_GPIO_PIN_LED1        31U  // GPIO number for LD2


    //input Button
    GPIO_setPadConfig(DEVICE_GPIO_PIN_BUTTON, GPIO_PIN_TYPE_PULLUP);
    GPIO_setDirectionMode(32, GPIO_DIR_MODE_IN);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_BUTTON, GPIO_QUAL_6SAMPLE); // GPIO_QUAL_SYNC Synchronization to SYSCLKOUT. GPIO_QUAL_3SAMPLE Qualified with 3 samples. GPIO_QUAL_6SAMPLE Qualified with 6 samples. GPIO_QUAL_ASYNC No synchronization.
    GPIO_setQualificationPeriod(DEVICE_GPIO_PIN_BUTTON, 100);//only 1~510 available
    GPIO_setInterruptPin(DEVICE_GPIO_PIN_BUTTON,GPIO_INT_XINT1);
    GPIO_setInterruptType(GPIO_INT_XINT1,GPIO_INT_TYPE_RISING_EDGE); //GPIO_INT_TYPE_FALLING_EDGE sets detection to edge and trigger to falling GPIO_INT_TYPE_RISING_EDGE sets detection to edge and trigger to rising GPIO_INT_TYPE_BOTH_EDGES sets detection to both edges
    GPIO_enableInterrupt(GPIO_INT_XINT1);

    // Register ISRs for GPIO
    //Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
    Interrupt_register(INT_XINT1, &buttonISR);
    Interrupt_enable(INT_XINT1);


    // Enable global and real-time interrupts
    EINT;
    ERTM;

    // Idle loop
    while(1) {
        // Actions based on timer interrupts can be added here

    }
}


// buttonISR - ISR for pressing button
__interrupt void buttonISR(void)
{
    pressCount++;

    GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);
    DEVICE_DELAY_US(50000);
    GPIO_writePin(DEVICE_GPIO_PIN_LED1, 1);

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

// End of File
