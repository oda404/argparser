// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include<argx/argx.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define UI unsigned int

static int is_number(const char *str)
{
    for(size_t i = 0; i < strlen(str); ++i)
    {
        if(!isdigit(str[i]))
            return 0;
    }

    return 1;
}

static ArgxArgument *argx_get_arg_by_name(const char *name, Argx *argx)
{
    for(size_t i = 0; i < argx->args_cnt; i++)
    {
        ArgxArgument *tmp = &argx->args[i];
        if(strcmp(tmp->name, name) == 0)
            return tmp;
    }
    return NULL;
}

void argx_init(Argx *argx)
{
    argx->args = NULL;
    argx->args_cnt = 0;
    argx->help_msg = NULL;
}

void argx_destroy(Argx *argx)
{
    for(size_t i = 0; i < argx->args_cnt; ++i)
    {
        free(argx->args[i].value);
        free(argx->args[i].name);
        free(argx->args[i].arg_short);
        free(argx->args[i].arg_long);
    }
    free(argx->args);
    free(argx->help_msg);
    argx_init(argx);
}

ArgxAddStatus argx_arg_add(
    const char *name,
    const char *arg_short,
    const char *arg_long,
    const char *description,
    int is_flag,
    Argx *argx
)
{
    if(!arg_short && !arg_long)
        return ARGX_ADD_INVALID_ARGS;
    
    for(size_t i = 0; i < argx->args_cnt; ++i)
    {
        ArgxArgument *tmp = &argx->args[i];
        if(strcmp(name, tmp->name) == 0)
            return ARGX_ADD_NAME_DUP;
        if(arg_short && strcmp(arg_short, tmp->arg_short) == 0)
            return ARGX_ADD_SHORT_DUP;
        if(arg_long && strcmp(arg_long, tmp->arg_long) == 0)
            return ARGX_ADD_LONG_DUP;
    }

    /* make space for the new arg */
    argx->args = realloc(
        argx->args, 
        sizeof(ArgxArgument) * (argx->args_cnt + 1)
    );

    ArgxArgument *head = &argx->args[argx->args_cnt];

    head->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(head->name, name);

    if(arg_short)
    {
        head->arg_short = malloc(sizeof(char) * (strlen(arg_short) + 1));
        strcpy(head->arg_short, arg_short);
    }
    else
    {
        head->arg_short = NULL;
    }
    
    if(arg_long)
    {
        head->arg_long = malloc(sizeof(char) * (strlen(arg_long) + 1));
        strcpy(head->arg_long, arg_long);
    }
    else
    {
        head->arg_long = NULL;
    }

    if(description)
    {
        head->description = malloc(sizeof(char) * (strlen(description) + 1));
        strcpy(head->description, description);
    }
    else
    {
        head->description = NULL;
    }

    head->is_flag = is_flag;
    head->value = NULL;

    ++argx->args_cnt;

    return ARGX_ADD_OK;
}

void argx_args_parse(char **argv, int argc, Argx *argx)
{
    for(size_t args_i = 0; args_i < (size_t)argc; ++args_i)
    {
        for(size_t argx_i = 0; argx_i < argx->args_cnt; ++argx_i)
        {
            ArgxArgument *tmp = &argx->args[argx_i];

            if(
                (tmp->arg_short && strcmp(tmp->arg_short, argv[args_i]) == 0) ||
                (tmp->arg_long && strcmp(tmp->arg_long, argv[args_i]) == 0)
            )
            {
                if(tmp->is_flag)
                {
                    /* Argument is a flag and isn't followed by a value. */
                    tmp->value = malloc(1);
                }
                else if(args_i + 1 < (size_t)argc)
                {
                    /* 
                     * Arg is supposed to have a following value.
                     */
                    tmp->value = malloc(sizeof(char) *  (strlen(argv[args_i + 1]) + 1));
                    strcpy(tmp->value, argv[args_i + 1]);

                    /* maybe increment args_i ? */
                    // ++args_i;
                }
                else
                {
                    printf("[argx] Non flag argument: \"%s\" has no following value.\n", argv[args_i]);
                }
                break;
            }
        }
    }
}

ArgxGetStatus argx_arg_get_str(const char *name, char *out, Argx *argx)
{
    ArgxArgument *arg = argx_get_arg_by_name(name, argx);
    if(!arg || !arg->value)
        return ARGX_GET_NOT_FOUND;

    strcpy(out, arg->value);
    return ARGX_GET_OK;
}

ArgxGetStatus argx_arg_get_uint(const char *name, UI *out, Argx *argx)
{
    ArgxArgument *arg = argx_get_arg_by_name(name, argx);
    if(!arg || !arg->value)
        return ARGX_GET_NOT_FOUND;

    if(!is_number(arg->value))
        return ARGX_GET_TYPE_MISSMATCH;
        
    *out = (UI)atoll(arg->value);
    return ARGX_GET_OK;
}

ArgxGetStatus argx_arg_get_str_len(const char *name, size_t *out, Argx *argx)
{
    ArgxArgument *arg = argx_get_arg_by_name(name, argx);
    if(!arg || !arg->value)
        return ARGX_GET_NOT_FOUND;

    if(arg->is_flag)
        return ARGX_GET_TYPE_MISSMATCH;

    *out = strlen(arg->value);
    return ARGX_GET_OK;
}

int argx_arg_present(const char *name, Argx *argx)
{
    ArgxArgument *arg = argx_get_arg_by_name(name, argx);
    if(!arg || !arg->value)
        return 0;

    return 1;
}

int argx_help_msg_gen(
    const char *usage, 
    const char *description,
    Argx *argx
)
{
    if(!argx->args_cnt)
        return 1;

    size_t longest_arg_len = 0;
    size_t help_msg_len = 0;

    help_msg_len += strlen(usage) + strlen(description) + 3;

    for(size_t i = 0; i < argx->args_cnt; ++i)
    {
        const ArgxArgument *tmp = &argx->args[i];

        size_t len = 0;
        if(tmp->arg_short)
            len += strlen(tmp->arg_short);
        if(tmp->arg_long)
            len += strlen(tmp->arg_long);

        if(len > longest_arg_len)
            longest_arg_len = len;

        /* 
         * 2 - the 2 starting whitespaces 
         * 2 - the comma and the whitespace between args
         * 1 - \n
         */
        len += 2 + 2 + 1 + strlen(tmp->description);
        help_msg_len += len;
    }

    /* 
     * We still need to calculate the space between the CLI args 
     * and the arg description. We do that based on the longest arg
     * so it looks nicer.
     */

    for(size_t i = 0; i < argx->args_cnt; ++i)
    {
        const ArgxArgument *tmp = &argx->args[i];
        size_t len = strlen(tmp->arg_short) + strlen(tmp->arg_long);
        help_msg_len += longest_arg_len - len + 3;
    }

    argx->help_msg = malloc(sizeof(char) * (help_msg_len + 1));
    size_t written = 0;
    written = sprintf(argx->help_msg, "%s\n%s\n\n", usage, description);

    for(size_t i = 0; i < argx->args_cnt; ++i)
    {
        const ArgxArgument *tmp = &argx->args[i];

        size_t len = strlen(tmp->arg_short) + strlen(tmp->arg_long);
        written += sprintf(argx->help_msg + written, "  %s, %s   ", tmp->arg_short, tmp->arg_long);

        for(size_t k = 0; k < longest_arg_len - len; ++k)
        {
            strcat(argx->help_msg, " ");
            ++written;
        }

        written += sprintf(argx->help_msg + written, "%s\n", tmp->description);
    }

    return 0;
}

const char *argx_help_msg_get(Argx *argx)
{
    return argx->help_msg;
}
