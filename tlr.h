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

#ifndef TLR_H
#define TLR_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
	TLR_NOOP,
	TLR_SET,   /* regto = scalar */
	TLR_ADD,   /* regto = regfrom + regto */
	TLR_SUB,   /* regto = regfrom - regto */
	TLR_LOAD,  /* load a value from memory into a regto */
	TLR_STORE, /* store a value to memory from a regfrom */
	TLR_JGT,   /* if regfrom > regto, pc = regdest */
	TLR_JEQ,   /* if regfrom == regto, pc = regdest */
	TLR_JI,    /* pc = regto */
	TLR_PRINT, /* print a register */
	TLR_MAX	   /* number of opcodes */
} tlr_opcode_t;

typedef enum {
	TLR_REG_R0,
	TLR_REG_R1,
	TLR_REG_R2,
	TLR_REG_R3,
	TLR_REG_R4,
	TLR_REG_R5,
	TLR_REG_PC,
	TLR_REG_MAX
} tlr_reg_t;

struct tlr_instruction {
	tlr_opcode_t operation;
	tlr_reg_t reg_from, reg_to;
	int mem_addr, scalar;
};

/* 
 * Static definitions of the instruction structs.  These utility
 * definitions simply make writing your program easier!
 */
#define TLR_INST_NOOP() { .operation = TLR_NOOP }
#define TLR_INST_PRINT(r) { .operation = TLR_PRINT, .reg_from = r, }
#define TLR_INST_SET(r, s) { .operation = TLR_SET, .reg_to = r, .scalar = s, }
#define TLR_INST_CPY(rfrom, rto) { .operation = TLR_CPY, .reg_to = rto, .reg_from = rfrom, }
#define TLR_INST_ADD(r0, rto) { .operation = TLR_ADD, .reg_from = r0, .reg_to = rto, }
#define TLR_INST_SUB(rfrom, rto) { .operation = TLR_SUB, .reg_from = rfrom, .reg_to = rto, }
#define TLR_INST_LOAD(r, m) { .operation = TLR_LOAD, .reg_to = r, .mem_addr = m, }
#define TLR_INST_STORE(r, m) { .operation = TLR_STORE, .reg_from = r, .mem_addr = m, }
#define TLR_INST_JEQ(r0, r1, addr) { .operation = TLR_JEQ, .reg_from = r0, .reg_to = r1, .scalar = addr, }
#define TLR_INST_JGT(rfrom, rto, addr) { .operation = TLR_JGT, .reg_from = rfrom, .reg_to = rto, .scalar = addr, }
#define TLR_INST_JI(rto) { .operation = TLR_JI, .reg_to = rto, } 
/* Jump to a fixed address */
#define TLR_INST_JMP(scalar) TLR_INST_SET(TLR_REG_PC, scalar) 
/* Jump to an indirect address */
#define TLR_INST_IJMP(rdest) TLR_INST_JEQ(TLR_REG_R0, TLR_REG_R0, rdest)

struct tlr_program {
	int size;
	struct tlr_instruction *instructions;
};

struct tlr_context {
	int registers[TLR_REG_MAX];
	/* memory */
	int mem_sz;
	int *memory;
	struct tlr_program program;
};

int  tlr_exec_onestep(struct tlr_context *c);
void tlr_exec(struct tlr_context *c);
int  tlr_init(struct tlr_context *c, struct tlr_instruction *instructions, int prog_sz, int mem_sz);
struct tlr_context *tlr_create(struct tlr_instruction *instructions, int prog_sz, int mem_sz);
void tlr_dealloc(struct tlr_context *c);
void tlr_free(struct tlr_context *c);

#endif


