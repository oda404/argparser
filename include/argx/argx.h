// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef ARGX_H
#define ARGX_H

#include<stddef.h>

typedef enum E_ArgxAddStatus
{
    ARGX_ADD_OK        = 0,
    ARGX_ADD_NAME_DUP  = 1,
    ARGX_ADD_SHORT_DUP = 2,
    ARGX_ADD_LONG_DUP  = 3
} ArgxAddStatus;

typedef enum E_ArgxGetStatus
{
    /* Info was gathered successfully. */
    ARGX_GET_OK = 0,
    /* 
     * Argument was either not added by argx_arg_add or 
     * argx_args_parse didn't find it.
     */
    ARGX_GET_NOT_FOUND = 1,
    /* */
    ARGX_GET_TYPE_MISSMATCH = 2
} ArgxGetStatus;

typedef struct S_ArgxArgument
{
	char *name;
	char *arg_short;
	char *arg_long;
    char *value;
	int is_flag;
} ArgxArgument;

typedef struct S_Argx
{
    ArgxArgument *args;
    size_t args_cnt;
    char *help_msg;
} Argx;

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Inits the argx struct.
 * @param argx Argx handle.
 */
void argx_init(Argx *argx);
/* 
 * Destroys the argx struct and frees any associated memory.
 * @param argx Argx handle.
 */
void argx_destroy(Argx *argx);
/* 
 * Adds a new argument to the argx struct for later parsing. 
 * @param name The name of the argument.
 * @param arg_short The short CLI version of the argument.
 * @param arg_long The long CLI version of the argument.
 * @param is_flag Whether the argument is a flag or not the argument.
 * @param argx Argx handle.
 */
ArgxAddStatus argx_arg_add(
    const char *name,
    const char *arg_short,
    const char *arg_long,
    int is_flag,
    Argx *argx
);
/* Parses based on the previously added arguments to the 'argx' struct.
 * @param argv The array of arguments to parse.
 * @param argc The number of arguments to parse from 'argv'.
 * @param argx Argx handle.
 */
void argx_args_parse(char **argv, int argc, Argx *argx);
/*
 * Searches for the argument with name 'name', interprets it's value as a string and stores it into 'out'. 
 * Note that 'out' should be properly allocated before calling thing function (see argx_arg_get_str_len).
 * @param name The name of the argument.
 * @param out Where the value of the argument will be stored.
 * @param argx Argx handle.
 */
ArgxGetStatus argx_arg_get_str(const char *name, char *out, Argx *argx);
/*
 * Searches for the argument with name 'name', interprets it's value as an unsigned int and stores it into 'out'. 
 * If any errors occur, the value of 'out' is not touched.
 * @param name The name of the argument.
 * @param out Where the value of the argument will be stored.
 * @param argx Argx handle.
 */
ArgxGetStatus argx_arg_get_uint(const char *name, unsigned int *out, Argx *argx);
/*
 * Searches for the argument with name 'name', interprets it's value as a string, gets it's length and stores it into 'out'. 
 * If any errors occur, the value of 'out' is not touched.
 * @param name The name of the argument.
 * @param out Where the string's length will be stored.
 * @param argx Argx handle.
 */
ArgxGetStatus argx_arg_get_str_len(const char *name, size_t *out, Argx *argx);
/*
 * Searches for the argument with name 'name'. 
 * If any errors occur, the value of 'out' is not touched.
 * @param name The name of the argument.
 * @param argx Argx handle.
 * @return 1 if the argument was found, 0 otherwise.
 */
int argx_arg_present(const char *name, Argx *argx);
/*
 * Generates an internal formatted help message string based on 
 * already added arguments via argx_arg_add(). 
 * You can get a pointer to this string via argx_help_msg_get().
 * The message has the following format:
 * [2 spaces] <arg_short>, <arg_long> [3 spaces] <description>\n
 * @param argx Argx handle. 
 * @return 0 on success, 1 otherwise.
 */
int argx_help_msg_gen(Argx *argx);
/*
 * Gets a pointer to the help message string generated by argx_help_msg_gen(). 
 * @param argx Argx handle. 
 * @return NULL if no message was generated or argx_help_msg_gen failed, a pointer to the message otherwise.
 */
const char *argx_help_msg_get(Argx *argx);

#ifdef __cplusplus
}
#endif

#endif // ARGX_H
