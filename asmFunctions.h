#ifndef ASM_FUNCTIONS_H
#define ASM_FUNCTIONS_H

#include "base_definitions.h"
#include "processor.h" 

	// ADD
	int16 asmADD8(uint8* arg1, uint8* arg2, uint8* result);
	int16 asmADD16(uint16* arg1, uint16* arg2, uint16* result);
	int16 asmADD32(uint32* arg1, uint32* arg2, uint32* result);
	int16 asmADD64(uint64* arg1, uint64* arg2, uint64* result);

	//SUB
	uint16 asmSUB8(uint8* arg1, uint8* arg2, uint8* result);
	uint16 asmSUB16(uint16* arg1, uint16* arg2, uint16* result);
	uint16 asmSUB32(uint32* arg1, uint32* arg2, uint32* result);
	uint16 asmSUB64(uint64* arg1, uint64* arg2, uint64* result);

	//MUL
	uint16 asmMUL8(uint8* arg1, uint8* arg2, uint8* result);
	uint16 asmMUL16(uint16* arg1, uint16* arg2, uint16* result);
	uint16 asmMUL32(uint32* arg1, uint32* arg2, uint32* result);
	uint16 asmMUL64(uint64* arg1, uint64* arg2, uint64* result);

    //DIV
	uint16 asmDIV8(uint8* arg1, uint8* arg2, uint8* result);
	uint16 asmDIV16(uint16* arg1, uint16* arg2, uint16* result);
	uint16 asmDIV32(uint32* arg1, uint32* arg2, uint32* result);
	uint16 asmDIV64(uint64* arg1, uint64* arg2, uint64* result);

	//INC
	uint16  asmINC8(uint8* result);
	uint16  asmINC16(uint16* result);
	uint16  asmINC32(uint32* result);
	uint16  asmINC64(uint64* result);

	//DEC
	uint16 asmDEC8(uint8* result);
	uint16 asmDEC16(uint16* result);
	uint16 asmDEC32(uint32* result);
	uint16 asmDEC64(uint64* result);

	//logical commands

	//AND
	uint16  asmAND8(uint8* arg1, uint8* arg2, uint8* result);
	uint16  asmAND16(uint16* arg1, uint16* arg2, uint16* result);
	uint16  asmAND32(uint32* arg1, uint32* arg2, uint32* result);
	uint16  asmAND64(uint64* arg1, uint64* arg2, uint64* result);

	//OR
	uint16  asmOR8(uint8* arg1, uint8* arg2, uint8* result);
	uint16  asmOR16(uint16* arg1, uint16* arg2, uint16* result);
	uint16  asmOR32(uint32* arg1, uint32* arg2, uint32* result);
	uint16  asmOR64(uint64* arg1, uint64* arg2, uint64* result);

	//XOR
	uint16  asmXOR8(uint8* arg1, uint8* arg2, uint8* result);
	uint16  asmXOR16(uint16* arg1, uint16* arg2, uint16* result);
	uint16  asmXOR32(uint32* arg1, uint32* arg2, uint32* result);
	uint16  asmXOR64(uint64* arg1, uint64* arg2, uint64* result);

	//NOT
	uint16  asmNOT8(uint8* result);
	uint16  asmNOT16(uint16* result);
	uint16  asmNOT32(uint32* result);
	uint16  asmNOT64(uint64* result);

	//NAND
	uint16  asmNAND8(uint8* arg1, uint8* arg2, uint8* result);
	uint16  asmNAND16(uint16* arg1, uint16* arg2, uint16* result);
	uint16  asmNAND32(uint32* arg1, uint32* arg2, uint32* result);
	uint16  asmNAND64(uint64* arg1, uint64* arg2, uint64* result);
	
	//NOR
	uint16  asmNOR8(uint8* arg1, uint8* arg2, uint8* result);
	uint16  asmNOR16(uint16* arg1, uint16* arg2, uint16* result);
	uint16  asmNOR32(uint32* arg1, uint32* arg2, uint32* result);
	uint16  asmNOR64(uint64* arg1, uint64* arg2, uint64* result);

#endif
