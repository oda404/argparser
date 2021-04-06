// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include<argparser/argparser.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define UI unsigned int

static int is_number(const char *str)
{
    UI i = 0;
    for(; i < strlen(str); ++i)
    {
        if(!isdigit(str[i]))
            return 0;
    }

    return 1;
}

void args_init(Args *args)
{
    args->args_arr = NULL;
    args->argsc = 0;
}

void args_add(
    const char *name,
    const char *arg_short,
    const char *arg_long,
    int is_flag,
    Args *args
)
{
    /* make space for the new arg */
    args->args_arr = 
    realloc(
        args->args_arr, 
        sizeof(Arg) * (args->argsc + 1)
    );

    Arg *head_arg = &args->args_arr[args->argsc];

    head_arg->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(head_arg->name, name);
    head_arg->arg_short = malloc(sizeof(char) * (strlen(arg_short) + 1));
    strcpy(head_arg->arg_short, arg_short);
    head_arg->arg_long = malloc(sizeof(char) * (strlen(arg_long) + 1));
    strcpy(head_arg->arg_long, arg_long);
    head_arg->is_flag = is_flag;
    head_arg->value = NULL;

    ++args->argsc;
}

void args_free(Args *args)
{
    UI i = 0;
    for(; i < args->argsc; ++i)
    {
        free(args->args_arr[i].value);
        free(args->args_arr[i].name);
        free(args->args_arr[i].arg_short);
        free(args->args_arr[i].arg_long);
    }
    free(args->args_arr);
    /* clean up */
    args_init(args);
}

void args_parse(int argc, char **argv, Args *args)
{
    UI argv_i = 0;
    UI args_i = 0;

    for(; args_i < args->argsc; ++args_i)
    {
        for(argv_i = 0; argv_i < (UI)argc; ++argv_i)
        {
            Arg *arg_tmp = &args->args_arr[args_i];
            if(
                strcmp(arg_tmp->arg_short, argv[argv_i]) == 0 ||
                strcmp(arg_tmp->arg_long, argv[argv_i]) == 0
            )
            {
                if(arg_tmp->is_flag)
                {
                    /* we only malloc it, if arg_tmp->value is not NULL, the flag was set */
                    arg_tmp->value = malloc(1);
                }
                else if(argv_i + 1 < (UI)argc)
                {
                    arg_tmp->value =
                    malloc(sizeof(char) * (strlen(argv[argv_i + 1]) + 1));
                    strcpy(arg_tmp->value, argv[argv_i + 1]);
                }
                else
                {
                    printf("[argparser] Warning: Invalid value for arg '%s'\n", argv[argv_i]);
                }
            }
        }
    }
}

int args_was_found(const char *name, Args *args)
{
    UI i = 0;
    for(; i < args->argsc; i++)
    {
        Arg *tmp_arg = &args->args_arr[i];
        if(strcmp(tmp_arg->name, name) == 0)
        {
            if(tmp_arg->value)
            {
                return 1;
            }
            break;
        }
    }

    return 0;
}

size_t args_get_val_len(const char *name, Args *args)
{
    UI i = 0;
    for(; i < args->argsc; i++)
    {
        Arg *tmp_arg = &args->args_arr[i];
        if(strcmp(tmp_arg->name, name) == 0)
        {
            if(tmp_arg->value && !tmp_arg->is_flag)
            {
                return strlen(tmp_arg->value);
            }
            break;
        }
    }

    return 0;
}

int args_out_str(const char *name, char *out, Args *args)
{
    UI i = 0;
    for(; i < args->argsc; ++i)
    {
        Arg *tmp_arg = &args->args_arr[i];
        if(strcmp(tmp_arg->name, name) == 0)
        {
            if(tmp_arg->value)
            {
                strcpy(out, tmp_arg->value);
                return ARGPARSER_ARG_OK;
            }
            break;
        }
    }

    return ARGPARSER_ARG_NOT_FOUND;
}

int args_out_uint(const char *name, UI *out, Args *args)
{
    UI i = 0;
    for(; i < args->argsc; ++i)
    {
        Arg *tmp_arg = &args->args_arr[i];
        if(strcmp(tmp_arg->name, name) == 0)
        {
            if(tmp_arg->value && is_number(tmp_arg->value))
            {
                *out = (UI)atoi(tmp_arg->value);
                return ARGPARSER_ARG_OK;
            }
            break;
        }
    }

    return ARGPARSER_ARG_NOT_FOUND;
}
