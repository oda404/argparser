
#pragma once

#include<string>
#include<sstream>

namespace argx
{

namespace native
{
#include<argx/argx.h>
}

typedef struct native::S_ArgxArgument  Argument;
typedef enum   native::E_ArgxAddStatus AddStatus;
typedef enum   native::E_ArgxGetStatus GetStatus;
typedef enum   native::E_ArgxHelpMsgGenStatus HelpMsgGenStatus;

class Argx
{
private:
    native::Argx m_argx_handle;

public:
    Argx();
    ~Argx();

    native::Argx *get_native_argx_handle();

    AddStatus arg_add(
        const std::string &name,
        const std::string &arg_short,
        const std::string &arg_long,
        const std::string &description,
        bool is_flag
    );

    void args_parse(char **argv, int argc);

    template<typename T>
    GetStatus arg_val_get(
        const std::string &name,
        T &out
    )
    {
        Argument *arg = native::argx_arg_get(
            name.c_str(),
            &m_argx_handle
        );
        if(!arg || !arg->value)
            return GetStatus::ARGX_GET_NOT_FOUND;

        if(arg->is_flag || !std::is_standard_layout<T>::value)
            return GetStatus::ARGX_GET_TYPE_MISSMATCH;

        std::stringstream ss(arg->value);
        T tmp;
        if(ss >> tmp)
        {
            out = tmp;
            return GetStatus::ARGX_GET_OK;
        }

        return GetStatus::ARGX_GET_TYPE_MISSMATCH;
    }

    Argument *arg_get(const std::string &name);

    bool arg_present(const std::string &name);

    HelpMsgGenStatus help_msg_gen(
        const std::string &usage,
        const std::string &description
    );

    std::string help_msg_get();
};

}
