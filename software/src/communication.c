/* analog-in-v3-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/utility/callback_value.h"

#include "voltage.h"

extern Voltage voltage;

CallbackValue_uint16_t callback_value_voltage;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_VOLTAGE: return get_callback_value_uint16_t(message, response, &callback_value_voltage);
		case FID_SET_VOLTAGE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_uint16_t(message, &callback_value_voltage);
		case FID_GET_VOLTAGE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_uint16_t(message, response, &callback_value_voltage);
		case FID_SET_OVERSAMPLING: return set_oversampling(message);
		case FID_GET_OVERSAMPLING: return get_oversampling(message, response);
		case FID_SET_CALIBRATION: return set_calibration(message);
		case FID_GET_CALIBRATION: return get_calibration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_oversampling(const SetOversampling *data) {
	if(data->oversampling > ANALOG_IN_V3_OVERSAMPLING_16384) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	voltage.oversampling     = data->oversampling;
	voltage.oversampling_new = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_oversampling(const GetOversampling *data, GetOversampling_Response *response) {
	response->header.length = sizeof(GetOversampling_Response);
	response->oversampling  = voltage.oversampling;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_calibration(const SetCalibration *data) {
	if(data->divisor == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	voltage.calibration_offset     = data->offset;
	voltage.calibration_multiplier = data->multiplier;
	voltage.calibration_divisor    = data->divisor;
	voltage.calibration_new        = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_calibration(const GetCalibration *data, GetCalibration_Response *response) {
	response->header.length = sizeof(GetCalibration_Response);
	response->offset        = voltage.calibration_offset;
	response->multiplier    = voltage.calibration_multiplier;
	response->divisor       = voltage.calibration_divisor;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_voltage_callback(void) {
	return handle_callback_value_callback_uint16_t(&callback_value_voltage, FID_CALLBACK_VOLTAGE);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init_uint16_t(&callback_value_voltage, voltage_get_voltage);

	communication_callback_init();
}
