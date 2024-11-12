#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <assert.h>
#include <stdio.h>

#define LFINFO 0
#define LDEBUG 1
#define LINFO 2
#define LERROR 3

extern int dtype;

#define DPI_DEBUG_NONE              0x0
#define DPI_DEBUG_MAIN              0x1
#define DPI_DEBUG_SERVER            0x2
#define DPI_DEBUG_CLIENT            0x3
#define DPI_DEBUG_ALL               DPI_DEBUG_MAIN|DPI_DEBUG_SERVER|DPI_DEBUG_CLIENT

#if DEBUG_LEVEL <= LFINFO
  #define fstart(format, ...) printf("[INVERTER/FINFO] Start: %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
  #define ffinish(format, ...) printf("[INVERTER/FINFO] Finish: %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
  #define ferr(format, ...) printf("[INVERTER/FINFO] Error: %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
#else
  #define fstart(format, ...)
  #define ffinish(format, ...)
  #define ferr(format, ...)
#endif /* LFINFO */

#if DEBUG_LEVEL <= LDEBUG
  #define dmsg(type, format, ...) \
    if (dtype & type) printf("[INVERTER/DEBUG] %s:%s:%d: " format "\n", __FILE__, __func__, __LINE__, ## __VA_ARGS__)
  #define dprint(type, msg, buf, start, end, interval) \
    if (dtype & type) { \
      do { \
        int i; \
          printf("[INVERTER/DEBUG] %s:%s: %s (%d bytes)\n", __FILE__, __func__, msg, end - start); \
          for (i = start; i < end; i++) \
          { \
            printf("%02X ", buf[i]); \
            if (i % interval == (interval - 1)) \
            { \
              printf("\n"); \
            } \
          } \
          printf("\n"); \
      } while (0); \
    }
#else
  #define dmsg(type, format, ...)
  #define dprint(type, msg, buf, start, end, interval)
#endif /* DEBUG */

#if DEBUG_LEVEL <= LINFO
  #define imsg(type, format, ...) if (dtype & type) printf("[INVERTER/INFO] %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
  #define iprint(type, msg, buf, start, end, interval) \
    if (dtype & type) { \
      do { \
        int i; \
        printf("[INVERTER/INFO] %s:%s: %s (%d bytes)\n", __FILE__, __func__, msg, end - start); \
        for (i = start; i < end; i++) \
        { \
          printf("%02X ", buf[i]); \
          if (i % interval == (interval - 1)) \
          { \
            printf("\n"); \
          } \
        } \
        printf("\n"); \
      } while (0); \
    }
  #define ikprint(type, msg, buf, start, end, interval) \
    if (dtype & type) { \
      do { \
        int i; \
        printf("[INVERTER/INFO] %s:%s: %s (%d bytes)\n", __FILE__, __func__, msg, end - start); \
        for (i = start; i < end; i++) \
        { \
          printf("%02x", buf[i]); \
          if (i % interval == (interval - 1)) \
          { \
            printf("\n"); \
          } \
        } \
        printf("\n"); \
      } while (0); \
    }
#else
  #define imsg(type, format, ...)
  #define iprint(type, msg, buf, start, end, interval)
  #define ikprint(type, msg, buf, start, end, interval)
#endif /* INFO */

#if DEBUG_LEVEL <= LERROR
  #define emsg(format, ...) printf("[INVERTER/ERROR] " format "\n", ## __VA_ARGS__)
#else
  #define emsg(format, ...)
#endif /* ERROR */

#endif /* __DEBUG_H__ */
