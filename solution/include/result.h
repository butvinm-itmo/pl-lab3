#ifndef RESULT_H
#define RESULT_H

/* Define result structure for type. "*/
#define RESULT(value_t, status_t)                                              \
    typedef struct {                                                           \
        status_t status;                                                       \
        value_t value;                                                         \
    } value_t##_result;

#endif
