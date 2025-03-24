#include <cstdio>       /* fprintf                  */
#include <cstdint>      /* [u]int{8,16,32,64}_t     */
#include <string>       /* string                   */
#include "pin.H"        /* Intel Pin API            */
#include "util.h"       /* DIE, ANSI codes          */

using namespace std;

/******************************************************************************
 ************************* INSTRUMENTATION CALLBACKS **************************
 ******************************************************************************/

/*
 *  ins_instrum - instruction level instrumentation callback routine
 *      @ins    : Intel Pin instruction object (non-persistent)
 * 
 *      @return : N/A
 * 
 * This routine is invoked ONCE for every newly encountered instruction.
 * Persistent instrumented code is registered via INS_InsertCall().
 */
void ins_instrum(INS ins, void *)
{
    /* fetch references to increasingly larger units that contain ins */
    RTN rtn = INS_Rtn(ins);
    SEC sec = RTN_Valid(rtn) ? RTN_Sec(rtn) : SEC_Invalid();
    IMG img = SEC_Valid(sec) ? SEC_Img(sec) : IMG_Invalid();

    /* extract the names & ins disassembly */
    uint64_t ins_addr   = INS_Address(ins);
    string   ins_disass = INS_Disassemble(ins);
    string   rtn_name   = RTN_Valid(rtn) ? RTN_Name(rtn) : "?????";
    string   sec_name   = SEC_Valid(sec) ? SEC_Name(sec) : "?????";
    string   img_name   = IMG_Valid(img) ? IMG_Name(img) : "?????";

    /* output info for each instruction */
    fprintf(stderr,
        CYAN   TOOL_TAG CLEAR " "
        RED    "%s"     CLEAR ":"
        GREEN  "%s"     CLEAR ":"
        BLUE   "%s"     CLEAR ":"
        YELLOW "%lx"    CLEAR " -- "
               "%s\n",
        img_name.c_str(), sec_name.c_str(), rtn_name.c_str(),
        ins_addr, ins_disass.c_str());
}

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

    /* rgister instruction-level instrumentation function */
    INS_AddInstrumentFunction(ins_instrum, NULL);

    /* begin program execution */
    PIN_StartProgram();

    /* this point will never be reached */
    return 0;
}