#pragma once
#include <Windows.h>

/* macros */
#include "../../globals/macros/macros.h"

/* hiding functions from the imports table */
#include "../../dependencies/lazy_importer/lazy_importer.h"

/* configuration system */
#include "../config/config.h"
#include "../config/variables.h"

#include "../math/math.h"

/* string hashing */
#include "../../dependencies/fnv1a/fnv1a.h"

/* menu shart */
#include "../../hacks/menu/menu.h"

/* global utilities e.g. CreateThread(); */
#include "../../utilities/utilities.h"

/* console logging */
#include "../../utilities/console/console.h"

/* module handling */
#include "../../utilities/modules/modules.h"

/* game interfaces */
#include "../interfaces/interfaces.h"

/* input system */
#include "../../utilities/input/input.h"

/* convar stuff */
#include ".././convars/convars.h"

/* netvar manager */
#include "../netvars/netvars.h"

/* renderer */
#include "../render/render.h"

/* hooks */
#include "../../hooks/hooks.h"

/* hooking dependencies */
#include "../../dependencies/minhook/include/MinHook.h"

/* ImGui */
#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_freetype.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include "../../dependencies/imgui/imgui_internal.h"

/* formatting strings */
#include <format>

/* thread */
#include <thread>

/* globals */
#include "../../globals/globals.h"
