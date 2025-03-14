#include <cstdio>       /* fprintf                  */
#include <cstdint>      /* [u]int{8,16,32,64}_t     */
#include <string>       /* string                   */
#include "pin.H"        /* Intel Pin API            */
#include "util.h"       /* DIE, ANSI codes          */

using namespace std;

/******************************************************************************
 ************************************ MAIN ************************************
 ******************************************************************************/

int32_t main(int32_t argc, char *argv[])
{
    /* initialize PIN */
    int32_t ans = PIN_Init(argc, argv);
    DIE(ans, "Error initializing PIN");

    /* load symbols in order to access function names */
    PIN_InitSymbols();

    /* TODO */

    /* begin program execution */
    PIN_StartProgram();

    /* this point will never be reached */
    return 0;
}