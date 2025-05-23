//#include "F28x_Project.h"
#include "driverlib.h"
#include "device.h"

// DMA data sections
#pragma DATA_SECTION(sData, "ramgs0")
#pragma DATA_SECTION(rData, "ramgs1")
// Defines
#define BURST_SIZE   3      // Transfers 3 words per burst -> 16 bit data #4
#define rDataLength  1500
#define TRANSFER_SIZE    153  // for burst mode: 153 bursts per transfer 3*51
#define SRC_BURST_STEP 0 // sData[0], sData[30], sData[60] 0,3,6
#define DST_BURST_STEP 1 //dst_step 1

#define SRC_WRAP_SIZE 51 //51 bursts
#define DST_WRAP_SIZE 51 //51 bursts

#define SRC_WRAP_STEP 30 //sData[0], sData[30], sData[60]
#define DST_WRAP_STEP 153 // 3*51


// Globals
uint16_t sData[rDataLength];
uint16_t rData[rDataLength];
uint16_t *srcAddr;
uint16_t *destAddr;
uint32_t sysClockFreq = 0; // System clock frequency
uint16_t cpuTimer0IntCount=0;
uint16_t dmaCh1IntCount=0;


//volatile uint16_t done;
// FILE:   Lab4_cpu01.c


// Function Prototypes
void initDMA();
void initCPUTimers(void);
__interrupt void dmaCh1ISR(void);
__interrupt void cpuTimer0ISR(void);
void configCPUTimer(uint32_t, float, float);


// Main function
void main(void)
{

    int i;
    // Initialize DMA addresses
    srcAddr = &sData;
    destAddr = &rData;

    // Initialize device clock and peripherals
    Device_init();

    // Initialize DMA
    initDMA();

    // Initialize CPU Timers
    initCPUTimers();
    sysClockFreq = SysCtl_getClock(DEVICE_OSCSRC_FREQ);
    configCPUTimer(CPUTIMER0_BASE, sysClockFreq, 8000); // Timer 0: 8 kHz

    // Initialize data buffers

    for (i = 0; i < rDataLength; i++){
        sData[i] = i;
        rData[i] = 0x00AA;
    }

    // Initialize interrupt module
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // Register DMA ISR
    Interrupt_register(INT_DMA_CH1, &dmaCh1ISR);

    // Ensure DMA is connected to Peripheral Frame 2 bridge
    SysCtl_selectSecMaster(0, SYSCTL_SEC_MASTER_DMA);

    // Register ISRs for CPU Timers
    Interrupt_register(INT_TIMER0, &cpuTimer0ISR);

    // Enable interrupts
    Interrupt_enable(INT_DMA_CH1);

    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    Interrupt_enable(INT_TIMER0);
    CPUTimer_startTimer(CPUTIMER0_BASE);

    // Enable global interrupts
    EINT;
    ERTM;

    // Start DMA channel
    DMA_startChannel(DMA_CH1_BASE);
    while (1)
    {
    // Main loop: Application can add other tasks here
    }
}


// DMA Initialisation
void initDMA()
{
    // Initialize DMA controller
    DMA_initController();

    // Configure DMA Channel 1
    DMA_configAddresses(DMA_CH1_BASE, destAddr, srcAddr);

    DMA_configTransfer(DMA_CH1_BASE, TRANSFER_SIZE, SRC_BURST_STEP, DST_BURST_STEP);
    DMA_configBurst(DMA_CH1_BASE,BURST_SIZE,SRC_BURST_STEP, DST_BURST_STEP);
    DMA_configMode(DMA_CH1_BASE, DMA_TRIGGER_TINT0, DMA_CFG_ONESHOT_DISABLE | DMA_CFG_CONTINUOUS_ENABLE | DMA_CFG_SIZE_16BIT);
    DMA_configWrap(DMA_CH1_BASE,SRC_WRAP_SIZE,SRC_WRAP_STEP,DST_WRAP_SIZE,DST_WRAP_STEP);

    DMA_setInterruptMode(DMA_CH1_BASE, DMA_INT_AT_END); // ping pong at beginning

    DMA_enableTrigger(DMA_CH1_BASE);

    DMA_enableInterrupt(DMA_CH1_BASE);
}
// Initialize CPU Timers
void initCPUTimers(void){
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);

    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);

    CPUTimer_stopTimer(CPUTIMER0_BASE);

    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
}
void configCPUTimer(uint32_t cpuTimer, float freq, float fs)
{
    uint32_t temp = (uint32_t)(freq / fs);

    // Initialize timer period
    CPUTimer_setPeriod(cpuTimer, temp);

    // Set pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(cpuTimer, 0);

    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);

}

// DMA Channel1 ISR
__interrupt void dmaCh1ISR(void)
{
    // to be completed
    switch(dmaCh1IntCount){
        case 0:
            dmaCh1IntCount++;//to another block
            destAddr =  &(*(rData+TRANSFER_SIZE*BURST_SIZE));
            break;
        case 1:
            dmaCh1IntCount++;//to another block
            destAddr =  &(*(rData+2*TRANSFER_SIZE*BURST_SIZE));
            break;
        case 2:
            dmaCh1IntCount=0;//to another block
            destAddr =  &rData;
            break;
        default:
            dmaCh1IntCount=0;//to another block
            destAddr =  &rData;
            break;
    };

    //config beginning address
    DMA_configAddresses(DMA_CH1_BASE, destAddr, srcAddr);

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
    return;
}
// cpuTimer0ISR - ISR for CPU Timer 0
__interrupt void cpuTimer0ISR(void)
{
    cpuTimer0IntCount++;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

 // end of file
