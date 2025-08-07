#ifndef WH_DEBUG_LEVELS_H
#define WH_DEBUG_LEVELS_H

// 3-Level debug printing system for AdvCAD
// Usage: Set environment variable DEBUG_LEVEL or use command line argument --debug=N

enum WH_DebugLevel {
    WH_DEBUG_SILENT = 0,    // No output (for web app, scripting)
    WH_DEBUG_NORMAL = 1,    // Basic progress + errors (DEFAULT)
    WH_DEBUG_VERBOSE = 2,   // Detailed debugging info
    WH_DEBUG_TRACE = 3      // Full trace including loops and arrays
};

// Global debug level - defaults to NORMAL
extern int g_debugLevel;

// Initialization and control functions
void WH_InitDebugLevel();
void WH_SetDebugLevel(int level);
const char* WH_GetDebugLevelName(int level);

// Simple 3-level debug macros
#define WH_PRINT_NORMAL(msg) \
    do { if (g_debugLevel >= WH_DEBUG_NORMAL) { \
        printf("%s\n", msg); \
    }} while(0)

#define WH_PRINT_VERBOSE(msg) \
    do { if (g_debugLevel >= WH_DEBUG_VERBOSE) { \
        printf("DEBUG: %s\n", msg); \
    }} while(0)

#define WH_PRINT_TRACE(msg) \
    do { if (g_debugLevel >= WH_DEBUG_TRACE) { \
        printf("TRACE: %s\n", msg); \
    }} while(0)

// Formatted versions
#define WH_PRINTF_NORMAL(fmt, ...) \
    do { if (g_debugLevel >= WH_DEBUG_NORMAL) { \
        printf(fmt "\n", ##__VA_ARGS__); \
    }} while(0)

#define WH_PRINTF_VERBOSE(fmt, ...) \
    do { if (g_debugLevel >= WH_DEBUG_VERBOSE) { \
        printf("DEBUG: " fmt "\n", ##__VA_ARGS__); \
    }} while(0)

#define WH_PRINTF_TRACE(fmt, ...) \
    do { if (g_debugLevel >= WH_DEBUG_TRACE) { \
        printf("TRACE: " fmt "\n", ##__VA_ARGS__); \
    }} while(0)

// Special cases for errors and warnings (always shown unless SILENT)
#define WH_PRINT_ERROR(msg) \
    do { if (g_debugLevel > WH_DEBUG_SILENT) { \
        printf("ERROR: %s\n", msg); \
    }} while(0)

#define WH_PRINTF_ERROR(fmt, ...) \
    do { if (g_debugLevel > WH_DEBUG_SILENT) { \
        printf("ERROR: " fmt "\n", ##__VA_ARGS__); \
    }} while(0)

#define WH_PRINT_WARNING(msg) \
    do { if (g_debugLevel >= WH_DEBUG_NORMAL) { \
        printf("WARNING: %s\n", msg); \
    }} while(0)

#define WH_PRINTF_WARNING(fmt, ...) \
    do { if (g_debugLevel >= WH_DEBUG_NORMAL) { \
        printf("WARNING: " fmt "\n", ##__VA_ARGS__); \
    }} while(0)

// Legacy macro replacement (for existing code migration)
#define DEBUG_PRINT(msg) WH_PRINT_TRACE(msg)
#define DEBUG_PRINTF(fmt, ...) WH_PRINTF_TRACE(fmt, ##__VA_ARGS__)

#endif // WH_DEBUG_LEVELS_H