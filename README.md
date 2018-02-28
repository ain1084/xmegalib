# xmegalib
C++ class library for Atmel ATxmega.

## Implemented functions

### ATxmega classes

- SPI master (MSPI)
- I2C master
- I2C master (GPIO)
- Timer
- I2S/LPCM audio sample encoder (using MSPI)
- S/PDIF audio sample encoder (using MSPI)

### External device classes

- AT45DBxx (DataFlash)
- CS2200 (PLL Clock Synthesizer)
- XRA1200 (I2C I/O expander)
- SRAM23K (Serial SRAM)
- Push button
- FAT file system (Read access only)
- Microsd card accessor
- Audio
  - LM49450
  - MAX9850
  - PCM1770A
  - CS8406 (Digital audio transmitter)
- Display
  - SSD1322
  - IL9325
  - HD44780

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
