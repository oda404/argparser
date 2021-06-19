
#include<argx/argx.hpp>

namespace argx
{

Argx::Argx()
{
    native::argx_init(&m_argx_handle);
}

Argx::~Argx()
{
    native::argx_destroy(&m_argx_handle);
}

native::Argx* Argx::get_native_argx_handle()
{
    return &m_argx_handle;
}

AddStatus Argx::arg_add(
    const std::string &name,
    const std::string &arg_short,
    const std::string &arg_long,
    const std::string &description,
    bool is_flag
)
{
    return native::argx_arg_add(
        name.c_str(),
        arg_short.c_str(),
        arg_long.c_str(),
        description.c_str(),
        is_flag,
        &m_argx_handle
    );
}

void Argx::args_parse(char **argv, int argc)
{
    native::argx_args_parse(argv, argc, &m_argx_handle);
}

bool Argx::arg_present(const std::string &name)
{   
    return native::argx_arg_present(
        name.c_str(),
        &m_argx_handle
    );
}

Argument *Argx::arg_get(const std::string &name)
{
    return native::argx_arg_get(
        name.c_str(),
        &m_argx_handle
    );
}

HelpMsgGenStatus Argx::help_msg_gen(
    const std::string &usage,
    const std::string &description
)
{
    return native::argx_help_msg_gen(
        usage.c_str(),
        description.c_str(),
        &m_argx_handle
    );
}

std::string Argx::help_msg_get()
{
    if(!m_argx_handle.help_msg)
        return "";

    return std::string(m_argx_handle.help_msg);
}


}
