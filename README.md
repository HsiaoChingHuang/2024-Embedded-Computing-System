# Embedded Systems Coursework(Group 12) & Digital Signal Processing (Group 10)

This repository contains embedded system tasks developed using the TI TMS320F2837xD MCU as part of ECS and DSP courseworks. It demonstrates skills in GPIO handling, timers, DMA, IPC communication, fixed-point FFT implementation, digital signal processing, and real-time applications. The primary objective of these coursework is to develop a comprehensive understanding of embedded systems, particularly in the areas of real-time DSP processing, FIR filtering, FFT analysis, inter-processor communication, and peripheral control.

## ECS CW

### Task 1 – Integer and Floating-Point Multiplication
- Multiply signed and unsigned integers.
- Display results using `printf`.
- Adjusted linker file to increase heap for floating-point accuracy.

### Task 2 – FIR Filter Implementation
- FIR filter implemented using convolution.
- Uses circular buffer for real-time signal processing.
- Output verified against expected waveform.

### Task 3 – Goertzel Algorithm
- Detects signal frequency components.
- Uses iterative form of Goertzel’s difference equations.
- Heap and `.cio` configured via linker command file.

### Task 4 – IPC Communication (CPU1 to CPU2)
- Transfers array using shared RAM.
- Communicates array size and base address through IPC.
- Handled via `IpcRegs` and shared section: `SHARERAMGS0`.

### Task 5 – DMA Data Transfer
- Array-to-array data transfer via DMA Channel
- Uses RAMGS0 and RAMGS1 regions.
- Configured with PERINTFRC to simulate periodic DMA trigger.

### Task 6 Question 5 – Button Interrupt with Debounce
- GPIO32 = Button (input with pull-up)
- GPIO31 = LED (output)
- Debouncing using `GPIO_setQualificationMode()` (3/6 sample)

### Task 6 Question 6 – 1kHz Square Wave Generation
- GPIO31 toggled every 0.5ms using CPU Timer.
- Frequency too high to observe on LED, stored output in array for inspection.

### Task 6 Question 7 – Real-Time Clock
- 200MHz CPU using 1ms Timer0 interrupt.
- `updateTime()` and `displayTime()` handle logic and display.
- Debugging via GPIO65 toggle output.

---
## DSP CW

### Group10_DMA – Ping-Pong DMA on TMS320F2837xD

This task demonstrates the use of a **ping-pong Direct Memory Access (DMA)** mechanism triggered by Timer0 on the TI **TMS320F2837xD** microcontroller. DMA allows for high-speed, CPU-independent data transfers, making it ideal for real-time embedded systems.

Configure DMA in **continuous mode** to transfer data from `sData` to `rData` in memory using a **ping-pong buffer mechanism**, triggered by **Timer0 at 8 kHz**.
- Efficient background memory transfer
- Double-buffering with ping-pong technique
- ISR-based buffer switching
- Timer0 periodic triggering

####  Implementation Details

- `sData` and `rData` placed in `ramgs0` and `ramgs1` using `#pragma`
- DMA configured with `BURST_SIZE`, `TRANSFER_SIZE`, and `step size`
- Timer0 ISR increments `cpuTimer0IntCount`
- DMA Ch1 ISR alternates between 3 destination buffers using `dmaCh1IntCount`
- Graph view in Code Composer Studio shows expected stepped waveform in `rData[]`

### Group10_FFT – Fixed-Point FFT on TMS320F2837xD

This project implements a **fixed-point Cooley-Tukey FFT** algorithm on the TI **TMS320F2837xD** MCU to analyze a composite signal of 200 Hz and 600 Hz.

Perform a 1024-point FFT on a composite signal (200 Hz + 600 Hz), using **Q17.15 fixed-point format**.
- Cooley-Tukey radix-2 FFT algorithm
- Manual complex number handling in C
- Fixed-point math: scaling and overflow prevention
- Dynamic memory allocation for FFT buffers

#### Implementation Details

- Signal generated using sin(2πf₁t) + sin(2πf₂t), sampled at 8 kHz
- Converted to fixed-point using `x * 32768` (Q15)
- Custom `complex_add()` and `complex_mul()` functions
- Recursive FFT function with twiddle factor calculations
- Frequency domain magnitude calculated as √(real² + imag²)

#### Challenges

- Managing fixed-point scaling to avoid overflow
- Manual memory allocation (`malloc`) and `free()`
- Adjusted heap size in `.cmd` file to fit large arrays

---

## Tools Used

- **MCU**: TI TMS320F2837xD (C28x + CLA)
- **IDE**: Code Composer Studio (CCS)
- **Language**: C
- **Drivers**: C2000 Peripheral Driver Library

## How to Build & Run

1. Open Code Composer Studio.
2. Import project and set correct target configuration.
3. Ensure linker `.cmd` file has correct RAM/heap settings.
4. Build the project and flash to the target MCU.

---

## References

- [exas Instuments, "TMS320C28x Extended Instruction Sets - Technical Reference Manual," 2019.](https://www.ti.com/lit/ug/spruhs1c)
- [Texas Instruments, "TMS320F2837xD Dual-Core Real-Time Microcontrollers Technical Reference Manual," 2024.](https://www.ti.com/lit/ug/spruhm8k)
- [M. Bhardwaj, "Digital Control of Two Phase Interleaved PFC and Motor Drive Using MCU With CLA," 2013.](https://www.ti.com/lit/an/sprabs5)
- [Massachusetts Institute of Technology, Department of Electrical Engineering and Computer Science, "Lecture 20 The Goertzel Algorithm and the Chirp Transform," 2006.](https://ocw.mit.edu/courses/6-341-discrete-time-signal-processing-fall-2005/c5b65e06d8d1221fa68b23b7f0d23b41_lec20.pdf)
- D. G. Manolakis, V. K. Ingle, Applied Digital Signal Processing, Cambridge University Press, 2011.

---

> 📝 *This project is part of the 2024 ECS and DSP modules coursework at the University of Bristol – MSc in Advanced Microelectronic Systems Engineering.*




