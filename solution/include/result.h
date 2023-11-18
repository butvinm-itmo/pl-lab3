#ifndef RESULT_H
#define RESULT_H

#include <stdbool.h>

/* Result with value and status.

Usage:
```
typedef enum {
    CMD_ARGS_CORRECT = 0,
    CMD_ARGS_INCORRECT,
    CMD_ARGS_BAD_ANGLE,
} ArgsParseStatus;

Result(ArgsParseResult, CmdArgs, ArgsParseStatus);

ArgsParseResult parsed_args = do_stuff();

if (parsed_args.status != CMD_ARGS_CORRECT) {
    exit(1);
}
print_args(parsed_args._);
```
*/
#define Result(name, value_t, status_t)                                        \
    typedef struct {                                                           \
        status_t status;                                                       \
        value_t _;                                                             \
    }(name)

/* Maybe result.

Maybe is value tagged with validness flag.

Usage:

header.h
```
Maybe(Image);
```

source.c
```
MaybeImpl(Image);

MaybeImage make_image() {
    // ...
    if (some_condition) {
        return SomeImage(img);
    }
    return NoneImage;
}
```
*/
#define Maybe(value_t)                                                         \
    Result(Maybe##value_t, value_t, bool);                                     \
                                                                               \
    Maybe##value_t Some##value_t(value_t value);                               \
                                                                               \
    extern const Maybe##value_t None##value_t

#define MaybeImpl(value_t)                                                     \
    Maybe##value_t Some##value_t(value_t value) {                              \
        return (Maybe##value_t){true, value};                                  \
    }                                                                          \
                                                                               \
    const Maybe##value_t None##value_t = {false}

#endif
