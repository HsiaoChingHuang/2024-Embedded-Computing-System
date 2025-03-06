//###########################################################################
//
// FILE:   rtc_timer_cpu01.c
//
// TITLE:  Real-time Clock Implementation using CPU Timer for F2837xD
//
//###########################################################################

#include "F28x_Project.h"

// Time structure definition
typedef struct {
    Uint16 milliseconds;  // 0-999
    Uint16 seconds;       // 0-59
    Uint16 minutes;       // 0-59
    Uint16 hours;         // 0-23
    Uint16 days;          // 1-31
} RTC_Time;

// Initialize time to 00:00:00:000, Day 1
RTC_Time currentTime = {0, 0, 0, 0, 1};
// Time update flag
volatile Uint16 timeUpdated = 0;

// Function prototypes
__interrupt void cpu_timer0_isr(void);
void updateTime(void);
void displayTime(void);

//
// Main
//
void main(void)
{
    // Step 1. Initialize System Control:
    InitSysCtrl();

    // Step 2. Initialize GPIO:
    InitGpio();

    // Configure GPIO for debug output
    GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(65, GPIO_OUTPUT, GPIO_PUSHPULL);

    // Step 3. Disable all interrupts and initialize PIE vector table:
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    // Remap interrupt vectors
    EALLOW;
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    EDIS;

    // Step 4. Initialize CPU Timer
    InitCpuTimers();

    // Configure Timer0 for 1ms interrupt (200MHz CPU frequency)
    // 1ms = 1000£gs
    ConfigCpuTimer(&CpuTimer0, 200, 1000);

    // Initialize timer register
    CpuTimer0Regs.TCR.all = 0x4000;

    // Step 5. Enable interrupts
    IER |= M_INT1;                     // Enable CPU INT1
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // Enable TINT0 in PIE
    EINT;                              // Enable Global interrupts
    ERTM;                              // Enable Real-time debug interrupts

    // Main loop
    while(1)
    {
        // Check if time has been updated
        if(timeUpdated)
        {
            displayTime();
            timeUpdated = 0;  // Clear update flag
        }
        // Add other tasks here
    }
}

//
// Timer0 Interrupt Service Routine - Executes every millisecond
//
__interrupt void cpu_timer0_isr(void)
{
    // Update time
    updateTime();

    // Acknowledge interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// Update time function
//
void updateTime(void)
{
    currentTime.milliseconds++;

    if(currentTime.milliseconds >= 1000)
    {
        currentTime.milliseconds = 0;
        currentTime.seconds++;

        if(currentTime.seconds >= 60)
        {
            currentTime.seconds = 0;
            currentTime.minutes++;

            if(currentTime.minutes >= 60)
            {
                currentTime.minutes = 0;
                currentTime.hours++;

                if(currentTime.hours >= 24)
                {
                    currentTime.hours = 0;
                    currentTime.days++;
                }
            }
        }
        timeUpdated = 1;  // Update display every second
    }
}

//
// Display time function - Can output via UART or other methods
// Implement specific display logic in actual application
//
void displayTime(void)
{
    // Add specific display logic here
    // For example, send via UART or update LCD display

    // Example: Toggle GPIO65 for debug
    GpioDataRegs.GPATOGGLE.all = (1 << (65 % 32));  // For GPIO65, use GPATOGGLE register
}

//
// End of file
//
