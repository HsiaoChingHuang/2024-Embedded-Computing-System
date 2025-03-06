#include "F28x_Project.h"
// DMA data sections
#pragma DATA_SECTION(array1, "ramgs0")
#pragma DATA_SECTION(array2, "ramgs1")
// Defines
#define BURST       4       // Transfers 5 values per burst
#define ARRAY_SIZE  100
#define TRANSFER    ((ARRAY_SIZE/(BURST + 1)) - 1)  // Will be 19
// Globals
Uint16 array1[ARRAY_SIZE];
Uint16 array2[ARRAY_SIZE];
volatile Uint16 done;
// Function Prototypes
interrupt void local_D_INTCH6_ISR(void);
void error(void);
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;

// DMA initialization function
void InitDMA(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.DMA = 1;
    DmaRegs.DMACTRL.all = 0x0001;
    EDIS;
}
// DMA Channel 6 configuration
void ConfigureDMAChannel6(void)
{
    EALLOW;
    DmaRegs.CH6.SRC_BEG_ADDR_SHADOW = (Uint32)array1;
    DmaRegs.CH6.SRC_ADDR_SHADOW =     (Uint32)array1;
    DmaRegs.CH6.DST_BEG_ADDR_SHADOW = (Uint32)array2;
    DmaRegs.CH6.DST_ADDR_SHADOW =     (Uint32)array2;
    DmaRegs.CH6.BURST_SIZE.all = BURST;
    DmaRegs.CH6.SRC_BURST_STEP = 1;
    DmaRegs.CH6.DST_BURST_STEP = 1;
    DmaRegs.CH6.TRANSFER_SIZE = TRANSFER;
    DmaRegs.CH6.SRC_TRANSFER_STEP = 1;
    DmaRegs.CH6.DST_TRANSFER_STEP = 1;
    DmaRegs.CH6.MODE.all = (1 << 3)    // PERINTE = 1
                        | (0 << 4)      // ONESHOT = 0
                        | (0 << 5)      // CONTINUOUS = 0
                        | (0 << 7)      // DATASIZE = 0 (16-bit)
                        | (1 << 8)      // CHINTMODE = 1
                        | (1 << 9);     // CHINTE = 1
    DmaRegs.CH6.CONTROL.bit.PERINTCLR = 1;
    EDIS;
}
#ifdef _FLASH
// Copy time critical code and Flash setup code to RAM
// This includes InitFlash(), Flash API functions and any functions that are
// assigned to ramfuncs section.
// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
// symbols are created by the linker. Refer to the device .cmd file.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif
void main(void)
{
    Uint16 i;
    InitSysCtrl();
#ifdef _FLASH
    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();
#endif
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EALLOW;
    PieVectTable.DMA_CH6_INT = &local_D_INTCH6_ISR;
    EDIS;
    // Initialize arrays
    for(i = 0; i < ARRAY_SIZE; i++)
    {
        array1[i] = i + 1;
        array2[i] = 0;
    }
    InitDMA();
    ConfigureDMAChannel6();
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    PieCtrlRegs.PIEIER7.bit.INTx6 = 1;
    IER = M_INT7;
    EINT;
    EALLOW;
    DmaRegs.CH6.CONTROL.bit.RUN = 1;
    done = 0;
    while(!done)
    {
        EALLOW;
        DmaRegs.CH6.CONTROL.bit.PERINTFRC = 1;
        EDIS;
        DELAY_US(1000);
    }
    ESTOP0;
}
interrupt void local_D_INTCH6_ISR(void)
{
    Uint16 i;
    EALLOW;
    DmaRegs.CH6.CONTROL.bit.HALT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
    EDIS;
    for(i = 0; i < ARRAY_SIZE; i++)
    {
        if(array2[i] != (i + 1))
        {
            error();
        }
    }
    done = 1;
}
void error(void)
{
    asm("     ESTOP0");
    for(;;);
}
