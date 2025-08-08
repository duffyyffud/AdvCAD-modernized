#include "debug_levels.h"
#include <cstdlib>
#include <cstring>

// Global debug level - defaults to SILENT
int g_debugLevel = WH_DEBUG_SILENT;

void WH_InitDebugLevel() {
    const char* env_level = getenv("DEBUG_LEVEL");
    if (env_level) {
        int level = atoi(env_level);
        if (level >= WH_DEBUG_SILENT && level <= WH_DEBUG_TRACE) {
            g_debugLevel = level;
        }
    }
}

void WH_SetDebugLevel(int level) {
    if (level >= WH_DEBUG_SILENT && level <= WH_DEBUG_TRACE) {
        g_debugLevel = level;
    }
}

const char* WH_GetDebugLevelName(int level) {
    switch (level) {
        case WH_DEBUG_SILENT:  return "SILENT";
        case WH_DEBUG_NORMAL:  return "NORMAL";
        case WH_DEBUG_VERBOSE: return "VERBOSE";
        case WH_DEBUG_TRACE:   return "TRACE";
        default:               return "UNKNOWN";
    }
}