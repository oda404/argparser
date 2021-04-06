// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include<stddef.h>
#define ARGPARSER_ARG_OK        0
#define ARGPARSER_ARG_NOT_FOUND 1

typedef struct
{
    /* the name of the arg */
	char *name;
    /* short cli arg */
	char *arg_short;
    /* long cli arg */
	char *arg_long;
    char *value;
    /* 
    set to 1 if the arg isn't succedeed by a value, a.k.a a flag
    ex: '-h' opposed to '-n 2'.
    Note that args that have this set to 1
    should only be interfaced with using the 
    args_was_found function.
    */
	int is_flag;
} Arg;

typedef struct 
{
    Arg *args_arr;
    unsigned int argsc;
} Args;

#ifdef __cplusplus
extern "C" {
#endif

/* init the args struct */
void args_init(Args *args);
/* free the args struct and reinit */
void args_free(Args *args);
/* add a new arg to the args struct for later parsing */
void args_add(
    const char *name,
    const char *arg_short,
    const char *arg_long,
    int is_flag,
    Args *args
);

/* try to parse all of the added args */
void args_parse(int argc, char **argv, Args *args);
/* note that "out" should be allocated before calling this function */
int args_out_str(const char *name, char *out, Args *args);
int args_out_uint(const char *name, unsigned int *out, Args *args);
/* returns the length of the given's arg value, returns 0 if there were any errors (passing it a flag will be considered an error) */
size_t args_get_val_len(const char *name, Args *args);
/* returns 1 if the arg was found when parsing, 0 otherwise. Also used for flags*/
int args_was_found(const char *name, Args *args);

#ifdef __cplusplus
}
#endif

#endif // ARGPARSER_H
