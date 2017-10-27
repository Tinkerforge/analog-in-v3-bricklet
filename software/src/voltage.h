/* analog-in-v3-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * voltage.h Calculate voltage from ADC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
 
#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <stdbool.h>
#include <stdint.h>

#define VOLTAGE_CALIBRATION_PAGE           1
#define VOLTAGE_CALIBRATION_MAGIC_POS      0
#define VOLTAGE_CALIBRATION_OFFSET_POS     1
#define VOLTAGE_CALIBRATION_MULTIPLIER_POS 2
#define VOLTAGE_CALIBRATION_DIVISOR_POS    3

#define VOLTAGE_CALIBRATION_MAGIC          0x12345678

typedef struct {
	bool oversampling_new;
	uint8_t oversampling;

	bool calibration_new;
	int16_t calibration_offset;
	uint16_t calibration_multiplier;
	uint16_t calibration_divisor;
} Voltage;

void voltage_init(void);
void voltage_tick(void);
uint16_t voltage_get_voltage(void);

#endif 
