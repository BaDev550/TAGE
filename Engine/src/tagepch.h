#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "TAGE/Core/Utilities/Logger.h"
#include "TAGE/Core/Defines.h"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#if (LOG_GRAPICHS_ERROR)
#define DEBUG_BUFFER
#endif