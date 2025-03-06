// Included Files
#include "driverlib.h"
#include "device.h"
#include "gpio.h"

// Global Variables
//uint16_t cpuTimer0IntCount;
uint16_t cpuTimer1IntCount;
uint32_t sysClockFreq = 0; // System clock frequency
#define LENGTH 200 //record 1000 samples
uint32_t square_wave_result[LENGTH];


// Function Prototypes
__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
void initCPUTimers(void);
void configCPUTimer(uint32_t, float, float);

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
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);

    //turn off first
    GPIO_writePin(DEVICE_GPIO_PIN_LED1, 0);// #define DEVICE_GPIO_PIN_LED1        31U  // GPIO number for LD2

    // Register ISRs for CPU Timers
    //Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
    Interrupt_register(INT_TIMER1, &cpuTimer1ISR);

    // Initialize and configure CPU Timers
    initCPUTimers();
    sysClockFreq = SysCtl_getClock(DEVICE_OSCSRC_FREQ);
    //configCPUTimer(CPUTIMER0_BASE, sysClockFreq, 8000);     // Timer 0: 8 kHz
    configCPUTimer(CPUTIMER1_BASE, sysClockFreq, 0.5);     // Timer 1: 0.5 ms period to toggle (for 1kHz square wave?)

    // Enable and start CPU Timers
    //CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    //Interrupt_enable(INT_TIMER0);
    Interrupt_enable(INT_TIMER1);
    //CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER1_BASE);

    // Enable global and real-time interrupts
    EINT;
    ERTM;

    // Idle loop
    while(1) {
        // Actions based on timer interrupts can be added here

    }
}

// initCPUTimers - Initializes all CPU timers to a known state
void initCPUTimers(void)
{
    // Set maximum period for Timer 0 and Timer 1
    //CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);
    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);

    // Set pre-scaler to divide by 1 (SYSCLKOUT) and stop timers
   // CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
    //CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_stopTimer(CPUTIMER1_BASE);

    // Reload counters and reset interrupt counts
    //CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);
    //cpuTimer0IntCount = 0;
    cpuTimer1IntCount = 0;
}

// configCPUTimer - Configures the selected timer with given frequency (Hz) and period (uSeconds)
void configCPUTimer(uint32_t cpuTimer, float freq, float period)
{
    uint32_t temp = (uint32_t)(freq * period /1000);
    CPUTimer_setPeriod(cpuTimer, temp);
    CPUTimer_setPreScaler(cpuTimer, 0);
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);
    /*
    if (cpuTimer == CPUTIMER0_BASE) {
        cpuTimer0IntCount = 0;
    } else if (cpuTimer == CPUTIMER1_BASE) {
        cpuTimer1IntCount = 0;
    }
    */
    cpuTimer1IntCount = 0;
}

/*
// cpuTimer0ISR - ISR for CPU Timer 0
__interrupt void cpuTimer0ISR(void)
{
    cpuTimer0IntCount++;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}
*/

// cpuTimer1ISR - ISR for CPU Timer 1
__interrupt void cpuTimer1ISR(void)
{
    cpuTimer1IntCount++;
    GPIO_togglePin(DEVICE_GPIO_PIN_LED1);
    uint16_t num = (cpuTimer1IntCount-1)%LENGTH;
    square_wave_result[num]=GPIO_readPin(DEVICE_GPIO_PIN_LED1);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

// End of File
