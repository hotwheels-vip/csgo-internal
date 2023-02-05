#pragma once
#include <Windows.h>

/* hiding functions from the imports table */
#include "../../dependencies/lazy_importer/lazy_importer.h"

/* string hashing */
#include "../../dependencies/fnv1a/fnv1a.h"

/* global utilities e.g. CreateThread(); */
#include "../../utilities/utilities.h"

/* console logging */
#include "../../utilities/console/console.h"

/* module handle shart */
#include "../../utilities/modules/modules.h"

/* game interfaces */
#include "../../globals/interfaces/interfaces.h"

/* convar stuff */
#include "../../globals/convars/convars.h"

/* hooks */
#include "../hooks/hooks.h"

/* hooking dependencies */
#include "../../dependencies/minhook/include/MinHook.h"

/* formatting strings */
#include <format>