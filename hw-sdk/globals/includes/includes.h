#pragma once

// libs
#include <Windows.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <d3d9.h>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <intrin.h>
#include <iosfwd>
#include <mutex>
#include <process.h>
#include <sstream>
#include <stdexcept>
#include <tchar.h>
#include <thread>

// string enc/xor float
#include "../../dependencies/hash/hash.h"
#include "../../dependencies/xor/xor.h"

// deps
#include "../../dependencies/lazy_importer/lazy_importer.h"

// formatting
#include "../../dependencies/formatting/tinyformat.h"
#define FMT_HEADER_ONLY
#include "../../dependencies/formatting/fmt/chrono.h"
#include "../../dependencies/formatting/fmt/format.h"

// helpers
#include "../../utils/helpers/bit_flag.h"
#include "../../utils/helpers/primitive_definitions/primitives.h"

// utils
#include "../../utils/console/console.h"
#include "../../utils/utils.h"
#include "../../utils/vfunc/vfunc.h"

// g_csgo global
#include "../../globals/csgo.h"

// hotwheels global
#include "../../globals/hotwheels/hotwheels.h"

// used macros
#include "../../globals/macros/macros.h"

// math
#include "../../utils/math/math.h"
