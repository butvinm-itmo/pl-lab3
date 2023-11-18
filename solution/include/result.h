#ifndef RESULT_H
#define RESULT_H

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

#endif
