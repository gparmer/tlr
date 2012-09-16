#include <tlr.h>

/* 
 * Define our program.  This could be stored in a file and read if we
 * wished.  This would be like storing an executable file, and then
 * having the OS load it in.  Instead, we define it statically here.
 * Defined with static structure initialization:
 */
struct tlr_instruction prog[] = {
	/* basic testing */
	TLR_INST_NOOP(), 
	TLR_INST_NOOP(), 
	TLR_INST_PRINT(TLR_REG_R0), 	
	TLR_INST_PRINT(TLR_REG_PC), 
	TLR_INST_SET(TLR_REG_R0, 37337),
	TLR_INST_PRINT(TLR_REG_R0),
	TLR_INST_STORE(TLR_REG_R0, 0),
	TLR_INST_LOAD(TLR_REG_R1, 0),
	TLR_INST_PRINT(TLR_REG_R1),

	/* Look at us, we have a turing machine! */
	/* A for loop using memory... */
	TLR_INST_SET(TLR_REG_R2, 5),
	/* instruction 10: */
	TLR_INST_LOAD(TLR_REG_R0, 1),
	TLR_INST_PRINT(TLR_REG_R0),
	TLR_INST_SET(TLR_REG_R1, 1),
	TLR_INST_ADD(TLR_REG_R1, TLR_REG_R0),
	TLR_INST_STORE(TLR_REG_R0, 1),
	TLR_INST_JGT(TLR_REG_R2, TLR_REG_R0, 10), /* go back to inst 10 */

	/* A for loop using only registers... */
	TLR_INST_SET(TLR_REG_R0, 0),
	/* instruction 17: */
	TLR_INST_PRINT(TLR_REG_R0),
	TLR_INST_SET(TLR_REG_R1, 1),
	TLR_INST_ADD(TLR_REG_R1, TLR_REG_R0),
	TLR_INST_JGT(TLR_REG_R2, TLR_REG_R0, 17), /* go back to inst 10 */

 	/* infinite loop! */
//	TLR_INST_JMP(0),
};

/* 
 * Things to do: Add a stack pointer, and provide some macros for
 * providing function calls!  Write the Fibonacci sequence!  Provide a
 * sane way to refer to other instructions such as labels (e.g. in
 * jump instructions).
 */

int 
main(void)
{
	struct tlr_context c;

	tlr_init(&c, prog, sizeof(prog)/sizeof(struct tlr_instruction), 2);
	tlr_exec(&c);
	tlr_dealloc(&c);

	return 0;
}
