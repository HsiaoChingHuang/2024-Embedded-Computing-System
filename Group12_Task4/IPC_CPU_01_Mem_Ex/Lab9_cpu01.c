// FILE: Lab9_cpu01.c

#include "F28x_Project.h"

// Function Prototypes
interrupt void ipc1_isr(void);

// Variables
//Uint16 dacbOutput;
//Uint16 AdcaResult;
uint32_t shared_array_C1[20]= {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30};
#pragma DATA_SECTION(shared_array_C1, "SHARERAMGS0"); // define in .cmd that section (SHARERAMGS0      : > RAMGS15,    PAGE = 1)
uint32_t base_address = 0x01B000;//for RAMGS15

void main(void)
{
    // Initialize System Control for CPU1
    InitSysCtrl();

    // Disable interrupts and clear PIE control registers
    DINT;
    InitPieCtrl();

    // Disable CPU interrupts and clear interrupt flags
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table
    InitPieVectTable();

    // Map the IPC interrupt service routine
    EALLOW;
    PieVectTable.IPC0_INT = &ipc1_isr; // Map IPC1 interrupt to ISR function
    EDIS;

    /* Clear all IPC flags for a clean start, ensuring no leftover signals
       interfere with subsequent IPC operations */
    IpcRegs.IPCCLR.all = 0xFFFFFFFF;

    // Enable IPC interrupt in PIE
    PieCtrlRegs.PIEIER1.bit.INTx13 = 1; // Enable IPC1 ISR in PIE group 1

    // Enable global interrupts and real-time interrupt events
    IER |= M_INT1;  // Enable CPU interrupt group 1
    EINT;           // Enable global interrupts (INTM)
    ERTM;           // Enable real-time debug interrupt (DBGM)

    // Wait until CPU2 signals readiness
    while (IpcRegs.IPCSTS.bit.IPC17 == 0); // Wait for CPU2 to set IPC17
    IpcRegs.IPCACK.bit.IPC17 = 1;          // Acknowledge and clear IPC17


    IpcRegs.IPCSENDDATA = 20;
    IpcRegs.IPCSENDADDR= base_address;

    IpcRegs.IPCSET.bit.IPC1 = 1;  // Set IPC1 flag to notify CPU2

    while(1)
    {
        // Main loop: additional processing or tasks can be added here
    }
}


interrupt void ipc1_isr(void)
{
    // Read data from IPC receive register
    //unsigned int receivedata = (Uint16) IpcRegs.IPCRECVDATA; // Read data on IPC receive register
    //unsigned int transmitdata = receivedata + 1;             // Increment received data

    // Write the incremented data to IPC send register
    //IpcRegs.IPCSENDDATA = transmitdata;                      // Write data to IPC send register
    IpcRegs.IPCSET.bit.IPC1 = 1;                             // Set IPC1 flag to notify CPU2 ---- check if this comment is alright

    // Wait if received data is acknowledged
    while (IpcRegs.IPCSET.bit.IPC0 == 0);                    //---- check if this comment is alright

    // Clear interrupt flags
    IpcRegs.IPCACK.bit.IPC0 = 1;                             // Clear IPC0 flag to acknowledge CPU1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;                  // Acknowledge PIE group 1 interrupt
}



 // end of file










