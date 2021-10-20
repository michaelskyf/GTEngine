#ifndef GTE_MACROS_H
#define GTE_MACROS_H

#include <stdio.h>

/* output macros */
#define LOG(fmt, ...) fprintf(stdout, fmt "\n", ##__VA_ARGS__)
#define LOGI(fmt, ...) fprintf(stdout, "[INF] " fmt "\n", ##__VA_ARGS__)
#define LOGW(fmt, ...) fprintf(stdout, "[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOGD(fmt, ...) fprintf(stdout, "[DBG] (%s()->%s:%d) " fmt "\n", __func__, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGE(fmt, ...) fprintf(stderr, "[ERR] (%s()->%s:%d) " fmt "\n", __func__, __FILE__, __LINE__, ##__VA_ARGS__)

#endif
