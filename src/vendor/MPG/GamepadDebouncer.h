/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once
#include <ctime>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include "GamepadState.h"

// Implement this wrapper function for your platform
// TODO: Make this a pure virtual member instead.

class GamepadDebouncer
{
	public:
		GamepadDebouncer(const uint8_t debounceMS = 5) : debounceMS(debounceMS) { }

		void debounce(GamepadState *state);
		uint32_t getMillis() {return std::time(nullptr);}

		const uint8_t debounceMS;
		GamepadState debounceState;
		uint32_t dpadTime[4] = { 0 };
		uint32_t buttonTime[GAMEPAD_BUTTON_COUNT] = { 0 };
};
