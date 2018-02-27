# xmegalib
C++ class library for Atmel ATxmega.

## Implemented functions

### ATxmega classes

- SPI master (MSPI)
- I2C master
- I2C master (GPIO)
- Timer
- I2S/LPCM audio sample encoder (using SPI)
- S/PDIF audio sample encoder (using SPI)
- AT020QVS LCD module

### External device classes

- AT45DBxx (DataFlash)
- CS2200 (PLL Clock Synthesizer)
- CS8406 (Digital audio transmitter)
- LM49450 (DAC)
- MAX9850 (DAC)
- PCM1770A (DAC)
- XRA1200 (I2C I/O expander)
- SRAM23K (Serial SRAM)
- SSD1322 (Display controller)
- IL9325 (Display controller)
- Push button
- FAT file system (Read access only)
- Microsd card accessor

### audio classes

- sample renderer
- smaple buffer
- mixier
- windows wave file parser

### utility classes

- String
- Collections (vector, stack)
- smart pointer

![BOARD](https://github.com/ain1084/xmegalib/blob/master/images/board.png)
