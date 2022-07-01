#pragma once

#include "PlatDef.h"

/**
 * Each keycode header-file has to define the following enum:
 *          enum KeyCode : unsigned int
 */

#ifdef AW_PLATFORM_WINDOWS
#include "platform/Windows/WinKeyCodes.h"
#endif