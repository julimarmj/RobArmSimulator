/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once

#include <stdint.h>

#include "GamepadOptions.h"

#define STORAGE_FIRST_AVAILBLE_INDEX 2048

class GamepadStorage
{
	public:
		GamepadOptions Options;

		virtual void start(); // TODO: Should be pure virtual.
		virtual void save(); // TODO: Should be pure virtual.
		
		inline GamepadOptions getGamepadOptions() { return Options; }
		inline void setGamepadOptions(GamepadOptions options) { Options = options; }
};

static GamepadStorage GamepadStore;
