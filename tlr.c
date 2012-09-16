/**
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 *
 * Copyright 2012 by Gabriel Parmer.
 * Author: Gabriel Parmer, gparmer@gwu.edu, 2012
 *
 * The Tiny Language Runtime (TLR).  A C tutorial for f12 3411,
 * Operating Systems.
 */

#include <tlr.h>
#include <assert.h>

int
tlr_exec_onestep(struct tlr_context *c)
{
	struct tlr_instruction *i, *is;
	struct tlr_program *p;
	int pcval;

	/* int ints[10]; */
	/* &ints[5] is the same as (ints + 5) */
	assert(c);
	pcval = c->registers[TLR_REG_PC];
	p     = &(c->program);
	is    = p->instructions;
	/* i  = is + pcval; ...is the same as... */
	i     = &(is[pcval]);
	/* 
	 * The above is the same as:
	 * i = &c->program.instructions[c->registers[TLR_REG_PC]];
	 * 
	 * However, this long-form statement is very difficult to
	 * debug.  The previous, though more verbose allows you to be
	 * very explicit about your types, and let the compiler tell
	 * you where the types you think each part has don't match the
	 * reality.  The single statement is very difficult to debug
	 * because an error on that line is not going to tell you
	 * much.  Breaking it up into multiple lines with explicitly
	 * typed variables helps debugging and your understanding.
	 */
	switch (i->operation) {
	case TLR_NOOP:
		/* 
		 * Because we defined NOOP to be 0, any instructions
		 * in uninitialized memory (that is zeroed out) is
		 * interpreted as NOOPs for free!
		 */
		break;
	case TLR_PRINT:
	{
		int regval;

		regval = c->registers[i->reg_from];
		printf("%d\n", regval);
		break;
	}
	case TLR_SET:
		c->registers[i->reg_to] = i->scalar;
		break;
	case TLR_LOAD:
	{
		int offset;

		offset = i->mem_addr;
		if (offset >= c->mem_sz) {
			printf("TLR: segfault\n");
			return -1;
		}
		c->registers[i->reg_to] = c->memory[offset];
		break;
	}
	case TLR_STORE:
	{
		int offset;

		offset = i->mem_addr;
		if (offset >= c->mem_sz) {
			printf("TLR: segfault @ %d\n", c->registers[TLR_REG_PC]);
			return -1;
		}
		c->memory[offset] = c->registers[i->reg_from];
		break;
	}
	case TLR_SUB:
	{
		int from = c->registers[i->reg_from];
		int to   = c->registers[i->reg_to];
		c->registers[i->reg_to] = from - to;
		break;
	}
	case TLR_ADD:
		c->registers[i->reg_to] += c->registers[i->reg_from];
		break;
	case TLR_JGT:
		if (i->scalar >= c->program.size) {
			printf("TLR: segfault @ %d\n", c->registers[TLR_REG_PC]);
			return -1;
		}
		if (c->registers[i->reg_from] > c->registers[i->reg_to]) {
			c->registers[TLR_REG_PC] = i->scalar;
			return 0;
		}
		break;
	case TLR_JEQ:
		if (i->scalar >= c->program.size) {
			printf("TLR: segfault @ %d\n", c->registers[TLR_REG_PC]);
			return -1;
		}
		if (c->registers[i->reg_to] == c->registers[i->reg_from]) {
			c->registers[TLR_REG_PC] = i->scalar;
			return 0;
		}
		break;
	case TLR_JI:
		if (c->registers[i->reg_to] >= c->program.size) {
			printf("TLR: segfault @ %d\n", c->registers[TLR_REG_PC]);
			return -1;
		}
		c->registers[TLR_REG_PC] = c->registers[i->reg_to];
		return 0;
	default:
		printf("TLR:  Undefined opcode %d!\n", i->operation);
		return -1;
	}
	c->registers[TLR_REG_PC]++;

	return 0;
}

void
tlr_exec(struct tlr_context *c)
{
	/* keep stepping the program till the program completes execution */
	while (!tlr_exec_onestep(c)) {
		if (c->registers[TLR_REG_PC] == 
		    c->program.size) {
			/* we're done!!! exit. */
			return;
		}
	}
}

int
tlr_init(struct tlr_context *c, struct tlr_instruction *instructions, int prog_sz, int mem_sz)
{
	memset(c, 0, sizeof(struct tlr_context));
	/* (*c).program is the same as c->program */
	c->program.instructions = instructions;
	c->program.size         = prog_sz;

	if (mem_sz > 0) {
		c->memory = malloc(sizeof(int) * mem_sz);
		assert(c->memory);
		memset(c->memory, 0, sizeof(int) * mem_sz);
		c->mem_sz = mem_sz;
	}

	return 0;
}

struct tlr_context *
tlr_create(struct tlr_instruction *instructions, int prog_sz, int mem_sz)
{
	struct tlr_context *c;

	assert(instructions && prog_sz > 0 && mem_sz >= 0);
	c = malloc(sizeof(struct tlr_context));
	assert(c);
	tlr_init(c, instructions, prog_sz, mem_sz);

	return c;
}

/**
 * Assumes that the context is tlr_inited.  Will _not_ deallocate the
 * context itself.
 */
void
tlr_dealloc(struct tlr_context *c)
{
	assert(c);

	if (c->mem_sz > 0) {
		assert(c->memory);
		free(c->memory);
	}
}

/* Assumes that the context is dynamically allocated */
void
tlr_free(struct tlr_context *c)
{
	tlr_dealloc(c);
	free(c);
}

