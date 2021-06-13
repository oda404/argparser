
#include<argx/argx.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<limits.h>

#define HELP_EXPECT   1
#define STRING_EXPECT "itiomorbovina"
#define UINT_EXPECT   2976579765U

int main(int argc, char **argv)
{
    Argx argx;

    argx_init(&argx);

    printf("Running validity tests... ");

    int status;

    status = argx_arg_add("help",   "-h", "--help", "show this message and exit",  1, &argx);
    assert(status == ARGX_ADD_OK);

    status = argx_arg_add("string", "-s", "--string", "test string", 0, &argx);
    assert(status == ARGX_ADD_OK);

    status = argx_arg_add("uint",   "-u", "--uint", "test uint",    0, &argx);
    assert(status == ARGX_ADD_OK);

    argx_args_parse(argv, argc, &argx);

    /* check if "help" exists */
    assert(argx_arg_present("help", &argx) == HELP_EXPECT);
    /* "help" is a flag and doesn't have a value. */
    assert(argx_arg_get_str_len("help", NULL, &argx) == ARGX_GET_TYPE_MISSMATCH);

    /* check if "string" exists */
    assert(argx_arg_present("string", &argx) == 1);

    char *str_out = NULL;
    size_t str_out_len = 0;

    /* get "string"'s length */
    status = argx_arg_get_str_len("string", &str_out_len, &argx);
    assert(str_out_len == strlen(STRING_EXPECT));

    /* get "string"'s value */
    str_out = malloc(sizeof(char) * (str_out_len + 1));
    status = argx_arg_get_str("string", str_out, &argx);
    assert(status == ARGX_GET_OK);
    assert(strcmp(str_out, STRING_EXPECT) == 0);
    free(str_out);

    /* check if "uint" exists */
    assert(argx_arg_present("uint", &argx) == 1);

    unsigned int uint_out;

    status = argx_arg_get_uint("uint", &uint_out, &argx);
    assert(status == ARGX_GET_OK);
    assert(uint_out == UINT_EXPECT);

    printf("passed.\n");
    printf("Running invalidity tests... ");

    assert(argx_arg_present("invalid", &argx) == 0);

    str_out_len = 0;
    status = argx_arg_get_str_len("invalid", &str_out_len, &argx);
    assert(status == ARGX_GET_NOT_FOUND);
    assert(str_out_len == 0);

    str_out = NULL;
    status = argx_arg_get_str("invalid", str_out, &argx);
    assert(status == ARGX_GET_NOT_FOUND);
    assert(str_out == NULL);

    uint_out = 0;
    status = argx_arg_get_uint("invalid", &uint_out, &argx);
    assert(status == ARGX_GET_NOT_FOUND);
    assert(uint_out == 0);

    printf("passed.\n");

    argx_help_msg_gen(
        "Usage: whore",
        "damn",
        &argx
    );
    printf("%s\n", argx_help_msg_get(&argx));

    argx_destroy(&argx);
    return 0;
}
