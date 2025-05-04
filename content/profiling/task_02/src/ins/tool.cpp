#include <dlfcn.h>      /* lookup dladdr         */
#include <cxxabi.h>     /* c++ symbol demangling */

#include "util.h"

/******************************************************************************
 * Instrumentation callbacks & helpers
 ******************************************************************************/

#define DEMANGLE(name) abi::__cxa_demangle(name, nullptr, nullptr, nullptr)

static char * __attribute__((no_instrument_function))
resolve_symbol(void *addr)
{
    Dl_info sym_info;
    char    *sym_name;
    int32_t ans;

    /* find containing function of address */
    ans = dladdr(addr, &sym_info);

    /* __cxa_demangle may return nullptr if symbol name is not mangled *
     * print original if that is the case                              */
    sym_name = (char *) (DEMANGLE(sym_info.dli_sname) ? : sym_info.dli_sname);

    return sym_name;
}

extern "C" void __attribute__((no_instrument_function))
__cyg_profile_func_enter(void *this_fn, void *call_site)
{
    char *caller_name = resolve_symbol(call_site);
    char *callee_name = resolve_symbol(this_fn);

    INFO("Enter: %s --> %s", caller_name, callee_name);
}

extern "C" void __attribute__((no_instrument_function))
__cyg_profile_func_exit(void *this_fn, void *call_site)
{
    char *caller_name = resolve_symbol(call_site);
    char *callee_name = resolve_symbol(this_fn);

    INFO("Exit : %s --> %s", callee_name, caller_name);
}

