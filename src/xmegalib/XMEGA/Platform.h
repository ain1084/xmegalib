/*
 * Platform.h
 *
 * Created: 2012/01/17 1:48:04
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stddef.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#if APP_SECTION_SIZE >= 65536
#define ROM_READ_INT16_T(address)	static_cast<int16_t>(pgm_read_word_far(&address))
#define ROM_READ_UINT16_T(address)	static_cast<uint16_t>(pgm_read_word_far(&address))
#define ROM_READ_UINT8_T(address)	static_cast<uint8_t>(pgm_read_byte_far(&address))
#else
#define ROM_READ_INT16_T(address)	static_cast<int16_t>(pgm_read_word(address))
#define ROM_READ_UINT16_T(address)	static_cast<uint16_t>(pgm_read_word(address))
#define ROM_READ_UINT8_T(address)	static_cast<uint8_t>(pgm_read_byte(address))
#endif

typedef const int16_t PROGMEM PROGMEM_INT16_T;
typedef const uint16_t PROGMEM PROGMEM_UINT16_T;

#if defined(__cplusplus)

#include "CriticalSection.h"
#include "SPIMaster.h"
#include "SPIChip.h"
#include "I2CMaster.h"
#include "OutputPin.h"
#include "SystemClockTimer.h"
#include "OutputPin.h"
#include "InputPin.h"
#include "SystemClockCounter.h"
#include "SystemClockTimer.h"
#include "Endian.h"

using namespace XMEGA;

#endif
