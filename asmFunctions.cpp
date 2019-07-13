#include "asmFunctions.h"


// ADD commands in assembler
int16 asmADD8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]    // Read address of first argument in EBX register
		mov ECX, DWORD PTR[arg2]    // Read address of the second argument in ECX register
		mov EDX, DWORD PTR[result]  // Read address of the result in EDX register
		mov AL, BYTE PTR[EBX]       // Load first operand into the AX register
		mov BL, BYTE PTR[ECX]       // Load second operand into the BX register
		add AL, BL                  // Do the operation
		pushf                       // save flags
		mov BYTE PTR[EDX], AL       // Store the result (in EDX, couse we assigned addres of result to EDX register)
		pop AX                      // Return flags in AX
	}
}

int16 asmADD16(uint16 * arg1, uint16 * arg2, uint16 * result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		add AX, BX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

int16 asmADD32(uint32 * arg1, uint32 * arg2, uint32 * result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		add EAX, EBX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

int16 asmADD64(uint64 * arg1, uint64 * arg2, uint64 * result) {
	__asm {
		mov ESI, DWORD PTR[arg1]	// Read address of the first argument
		mov EDI, DWORD PTR[arg2]	// Read address of the second argument

		mov EAX, DWORD PTR[ESI]		// Load lower half of the first argument into the AX register
		mov EDX, DWORD PTR[ESI + 4]	// Load upper half of the first argument into the DX register

		mov EBX, DWORD PTR[EDI]		// Load lower half of the second argument into the BX register
		mov ECX, DWORD PTR[EDI + 4]	// Load upper half of the second operand into the CX register

		add EAX, EBX				// add lower halfs
		pushf						// Save first intermediatre flags
		adc EDX, ECX				// add upper halfs
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read results address
		mov DWORD PTR[EDI], EAX		// Store lower half of the result
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// return flags in AX
	}
}

// SUB commands in assembler
uint16 asmSUB8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AL, BYTE PTR[EBX]
		mov BL, BYTE PTR[ECX]
		sub AL, BL
		pushf
		mov BYTE PTR[EDX], AL
		pop AX
	}
}

uint16 asmSUB16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		sub AX, BX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

uint16 asmSUB32(uint32* arg1, uint32* arg2, uint32* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		sub EAX, EBX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

uint16 asmSUB64(uint64* arg1, uint64* arg2, uint64* result) {
	__asm {
		mov ESI, DWORD PTR[arg1]	// Read address of the first argument
		mov EDI, DWORD PTR[arg2]	// Read address of the second argument

		mov EAX, DWORD PTR[ESI]		// Load lower half of the first argument into the AX register
		mov EDX, DWORD PTR[ESI + 4]	// Load upper half of the first argument into the DX register

		mov EBX, DWORD PTR[EDI]		// Load lower half of the second argument into the BX register
		mov ECX, DWORD PTR[EDI + 4]	// Load upper half of the second argument into the CX register

		sub EAX, EBX				// sub lower halfs
		pushf						// Save first intermediatre flags
		sbb EDX, ECX				// sub upper halfs
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read address of the result
		mov DWORD PTR[EDI], EAX		// Store lower half of the result
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// return flags in AX
	}
}

//mull command in assembly
uint16 asmMUL8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]	// Read address of the first argument
		mov ECX, DWORD PTR[arg2]	// Read address of the second argument
		mov AL, BYTE PTR[EBX]		// Load first operand into the AL register
		mov BL, BYTE PTR[ECX]		// Load second operand into the BL register
		mul BL						// move in assembly means AX <- AL * BL
		pushf						// Save flags 
		mov EDX, DWORD PTR[result]	// Read address of the result in DX register
		mov BYTE PTR[EDX], AL		// Store the result in AL
		CMP AX, 0					// Set zero flags since mul doesn't sets it
		pushf
		pop BX
		pop AX
		and AX, 0xFF3F
		and BX, 0x00C0
		or AX, BX					// Return flags in AX
	}
}

uint16 asmMUL16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		mul BX						// DX:AX <- AX * BX
		pushf
		mov EDX, DWORD PTR[result]
		mov WORD PTR[EDX], AX
		CMP AX, 0
		pushf
		pop BX
		pop AX
		and AX, 0xFF3F
		and BX, 0x00C0
		or AX, BX
	}
}

uint16 asmMUL32(uint32* arg1, uint32* arg2, uint32* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		mul EBX						// EDX:EAX <- EAX * EBX
		pushf
		mov ECX, DWORD PTR[result]
		mov DWORD PTR[ECX], EAX
		CMP EAX, 0
		pushf
		pop BX
		pop AX
		and AX, 0xFF3F
		and BX, 0x00C0
		or AX, BX
	}
}

uint16 asmMUL64(uint64* arg1, uint64* arg2, uint64* result) {
	uint64 lval = *arg1;
	uint64 rval = *arg2;
	uint64 res = lval * rval;
	__asm pushf	
	*result = res;
	__asm pop AX
}

//div command in assembly
uint16 asmDIV8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]	// Read address of the first argument
		mov ECX, DWORD PTR[arg2]	// Read address of the second argument
		xor AX, AX					// Reset AX to 0
		mov AL, BYTE PTR[EBX]		// Load first argument into the AX register (dividend)
		mov BL, BYTE PTR[ECX]		// Load second argument into the BX register (divisor)
		div BL						// AH (reminder) : AL (quotient) <- AL / BL
		pushf						// Save flags
		mov EDX, DWORD PTR[result]	// Read address of the result
		mov WORD PTR[EDX], AX		// Store the result
		pop AX						// Return flags in AX
	}
}

uint16 asmDIV16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]	// Read address of the first argument
		mov ECX, DWORD PTR[arg2]	// Read address of the second argument
		xor DX, DX					// Reset AX to 0
		mov AX, WORD PTR[EBX]		// Load first argument into the AX register (dividend)
		mov BX, WORD PTR[ECX]		// Load second argument into the BX register (divisor)
		div BX						// DX (reminder) : AX (quotient) <- AX / BX
		pushf						// Save flags
		mov EBX, DWORD PTR[result]	// Read address of the result 
		mov WORD PTR[EDX], AX		// Store the result
		mov WORD PTR[EDX + 2], DX		// Store the result(Reminder)
		pop AX						// Return flags in AX
	}
}

uint16 asmDIV32(uint32* arg1, uint32* arg2, uint32* result)
{
	__asm {
		mov EBX, DWORD PTR[arg1]	// Read address of the first argument
		mov ECX, DWORD PTR[arg2]	// Read address of the second argument
		xor EDX, EDX				// Reset AX to 0
		mov EAX, DWORD PTR[EBX]		// Load first argument into the AX register (dividend)
		mov EBX, DWORD PTR[ECX]		// Load second argument into the BX register (divisor)
		div BX						// EDX (reminder) : EAX (quotient) <- EAX / EBX
		pushf						// Save flags
		mov EBX, DWORD PTR[result]	// Read address of the destination 
		mov DWORD PTR[EDX], EAX		// Store the result into the destination (Quotient)
		mov DWORD PTR[EDX + 4], EDX	// Store the result into the destination (Reminder)
		pop AX						// Return flags in AX
	}
}

uint16 asmDIV64(uint64* arg1, uint64* arg2, uint64* result) {
	uint64 lval = *arg1;
	uint64 rval = *arg2;
	uint64 res = lval / rval;
	__asm pushf // Doesn't makes sense, dummy flags
	*result = res;
	__asm pop AX
}

// increment command in assembly
uint16  asmINC8(uint8* result) {
	__asm {
		mov EDX, DWORD PTR[result]	// Read address of the operand
		mov AL, BYTE PTR[EDX]		// Load operand into the AX register
		inc AL						// Increment by 1
		pushf						// Save flags
		mov BYTE PTR[EDX], AL		// Store the result into the destination
		pop AX						// Return flags in AX
	}
}

uint16  asmINC16(uint16* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EDX]
		inc AX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

uint16  asmINC32(uint32* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EDX]
		inc EAX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

uint16 asmINC64(uint64* result) {
	__asm {
		mov EDX, DWORD PTR[result]	// Read sddress of the operand
		mov EAX, DWORD PTR[EDX]		// Load lower half of the operand into the AX register
		mov EBX, DWORD PTR[EDX + 4]	// Load upper half of the operand into the BX register

		inc EAX						// Increment lower half
		pushf
		adc EBX, 0					// Add carry bit to upper half
		pushf

		mov DWORD PTR[EDX], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDX + 4], EBX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
	}
}

//decrement command in assembly
uint16 asmDEC8(uint8* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov AL, BYTE PTR[EDX]
		dec AL
		pushf
		mov BYTE PTR[EDX], AL
		pop AX
	}
}

uint16 asmDEC16(uint16* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EDX]
		dec AX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

uint16 asmDEC32(uint32* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EDX]
		dec EAX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

uint16 asmDEC64(uint64* result) {
	__asm {
		mov EDX, DWORD PTR[result]	// Read sddress of the operand
		mov EAX, DWORD PTR[EDX]		// Load lower half of the operand into the AX register
		mov EBX, DWORD PTR[EDX + 4]	// Load upper half of the operand into the BX register

		dec EAX						// Increment lower half
		pushf
		sbb EBX, 0					// Add carry bit to upper half
		pushf

		mov DWORD PTR[EDX], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDX + 4], EBX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
	}
}

//logical command

//AND command in assembly
uint16  asmAND8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]	// Read sddress of the first operand
		mov ECX, DWORD PTR[arg2]	// Read sddress of the second operand
		mov EDX, DWORD PTR[result]	// Read sddress of the destination
		mov AL, BYTE PTR[EBX]		// Load first operand into the AX register
		mov BL, BYTE PTR[ECX]		// Load second operand into the BX register
		and AL, BL					// Do the operation
		pushf						// Save flags 
		mov BYTE PTR[EDX], AL		// Store the result into the destination
		pop AX						// Return flags in AX
	}
}

uint16  asmAND16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		and AX, BX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

uint16  asmAND32(uint32* arg1, uint32* arg2, uint32* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		and EAX, EBX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

uint16  asmAND64(uint64* arg1, uint64* arg2, uint64* result) {
	__asm {
		mov ESI, DWORD PTR[arg1]	// Read sddress of the first operand
		mov EDI, DWORD PTR[arg2]	// Read sddress of the second operand

		mov EAX, DWORD PTR[ESI]		// Load lower half of the first operand into the AX register
		mov EDX, DWORD PTR[ESI + 4]	// Load upper half of the first operand into the DX register

		mov EBX, DWORD PTR[EDI]		// Load lower half of the second operand into the AX register
		mov ECX, DWORD PTR[EDI + 4]	// Load upper half of the second operand into the DX register

		and EAX, EBX				// Do the operation for the lower half
		pushf						// Save first intermediatre flags
		and EDX, ECX				// Do the operation for the upper half
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read sddress of the destination
		mov DWORD PTR[EDI], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
	}
}

//OR command in assembly
uint16  asmOR8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AL, BYTE PTR[EBX]
		mov BL, BYTE PTR[ECX]
		or AL, BL
		pushf
		mov BYTE PTR[EDX], AL
		pop AX
	}
}

uint16  asmOR16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		or AX, BX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

uint16  asmOR32(uint32* arg1, uint32* arg2, uint32* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		or EAX, EBX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

uint16  asmOR64(uint64* arg1, uint64* arg2, uint64* result) {
	__asm {
		mov ESI, DWORD PTR[arg1]	// Read sddress of the first operand
		mov EDI, DWORD PTR[arg2]	// Read sddress of the second operand

		mov EAX, DWORD PTR[ESI]		// Load lower half of the first operand into the AX register
		mov EDX, DWORD PTR[ESI + 4]	// Load upper half of the first operand into the DX register

		mov EBX, DWORD PTR[EDI]		// Load lower half of the second operand into the AX register
		mov ECX, DWORD PTR[EDI + 4]	// Load upper half of the second operand into the DX register

		or EAX, EBX					// Do the operation for the lower half
		pushf						// Save first intermediatre flags
		or EDX, ECX					// Do the operation for the upper half
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read sddress of the destination
		mov DWORD PTR[EDI], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
	}
}


//XOR command in assembly 
uint16  asmXOR8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AL, BYTE PTR[EBX]
		mov BL, BYTE PTR[ECX]
		xor AL, BL
		pushf
		mov BYTE PTR[EDX], AL
		pop AX
	}
}

uint16  asmXOR16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		xor AX, BX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

uint16  asmXOR32(uint32* arg1, uint32* arg2, uint32* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		xor EAX, EBX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

uint16  asmXOR64(uint64* arg1, uint64* arg2, uint64* result) {
	__asm {
		mov ESI, DWORD PTR[arg1]	// Read sddress of the first operand
		mov EDI, DWORD PTR[arg2]	// Read sddress of the second operand

		mov EAX, DWORD PTR[ESI]		// Load lower half of the first operand into the AX register
		mov EDX, DWORD PTR[ESI + 4]	// Load upper half of the first operand into the DX register

		mov EBX, DWORD PTR[ESI]		// Load lower half of the second operand into the AX register
		mov ECX, DWORD PTR[ESI + 4]	// Load upper half of the second operand into the DX register

		xor EAX, EBX				// Do the operation for the lower half
		pushf						// Save first intermediatre flags
		xor EDX, ECX				// Do the operation for the upper half
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read sddress of the destination
		mov DWORD PTR[EDI], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
	}
}

//NOT command in assembly
uint16  asmNOT8(uint8* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov AL, BYTE PTR[EDX]
		not AL
		pushf
		mov BYTE PTR[EDX], AL
		pop AX
	}
}

uint16  asmNOT16(uint16* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EDX]
		not AX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
	}
}

uint16  asmNOT32(uint32* result) {
	__asm {
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EDX]
		not EAX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
	}
}

uint16  asmNOT64(uint64* result) {
	__asm {
		mov EDI, DWORD PTR[result]	// Read sddress of the only operand

		mov EAX, DWORD PTR[EDI]		// Load lower half of the operand into the AX register
		mov EDX, DWORD PTR[EDI + 4]	// Load upper half of the operand into the DX register

		not EAX						// Do the operation for the lower half
		pushf						// Save first intermediatre flags
		not EDX						// Do the operation for the upper half
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read sddress of the destination
		mov DWORD PTR[EDI], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
	}
}

//NAND command in assembly
uint16  asmNAND8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AL, BYTE PTR[EBX]
		mov BL, BYTE PTR[ECX]
		and AL, BL
		not AL
		pushf
		mov BYTE PTR[EDX], AL
		pop AX
		xor AX, 0x00C0	// toggle ZF & SF
	}
}

uint16  asmNAND16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		and AX, BX
		not AX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
		xor AX, 0x00C0	// toggle ZF & SF
	}
}

uint16  asmNAND32(uint32* arg1, uint32* arg2, uint32* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		and EAX, EBX
		not EAX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
		xor AX, 0x00C0	// toggle ZF & SF
	}
}

uint16  asmNAND64(uint64* arg1, uint64* arg2, uint64* result) {
	__asm {
		mov ESI, DWORD PTR[arg1]	// Read sddress of the first operand
		mov EDI, DWORD PTR[arg2]	// Read sddress of the second operand

		mov EAX, DWORD PTR[ESI]		// Load lower half of the first operand into the AX register
		mov EDX, DWORD PTR[ESI + 4]	// Load upper half of the first operand into the DX register

		mov EBX, DWORD PTR[EDI]		// Load lower half of the second operand into the AX register
		mov ECX, DWORD PTR[EDI + 4]	// Load upper half of the second operand into the DX register

		and EAX, EBX				// Do the operation for the lower half
		not EAX
		pushf						// Save first intermediatre flags
		and EDX, ECX				// Do the operation for the upper half
		not EDX
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read sddress of the destination
		mov DWORD PTR[EDI], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
		xor AX, 0x00C0				// toggle ZF & SF
	}
}

//NOR command in assembly
uint16  asmNOR8(uint8* arg1, uint8* arg2, uint8* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AL, BYTE PTR[EBX]
		mov BL, BYTE PTR[ECX]
		or AL, BL
		not AL
		pushf
		mov BYTE PTR[EDX], AL
		pop AX
		xor AX, 0x00C0	// toggle ZF & SF
	}
}

uint16  asmNOR16(uint16* arg1, uint16* arg2, uint16* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov AX, WORD PTR[EBX]
		mov BX, WORD PTR[ECX]
		or AX, BX
		not AX
		pushf
		mov WORD PTR[EDX], AX
		pop AX
		xor AX, 0x00C0	// toggle ZF & SF
	}
}

uint16  asmNOR32(uint32* arg1, uint32* arg2, uint32* result) {
	__asm {
		mov EBX, DWORD PTR[arg1]
		mov ECX, DWORD PTR[arg2]
		mov EDX, DWORD PTR[result]
		mov EAX, DWORD PTR[EBX]
		mov EBX, DWORD PTR[ECX]
		or EAX, EBX
		not EAX
		pushf
		mov DWORD PTR[EDX], EAX
		pop AX
		xor AX, 0x00C0	// toggle ZF & SF
	}
}

uint16  asmNOR64(uint64* arg1, uint64* arg2, uint64* result) {
	__asm {
		mov ESI, DWORD PTR[arg1]	// Read sddress of the first operand
		mov EDI, DWORD PTR[arg2]	// Read sddress of the second operand

		mov EAX, DWORD PTR[ESI]		// Load lower half of the first operand into the AX register
		mov EDX, DWORD PTR[ESI + 4]	// Load upper half of the first operand into the DX register

		mov EBX, DWORD PTR[EDI]		// Load lower half of the second operand into the AX register
		mov ECX, DWORD PTR[EDI + 4]	// Load upper half of the second operand into the DX register

		or EAX, EBX					// Do the operation for the lower half
		not EAX
		pushf						// Save first intermediatre flags
		or EDX, ECX					// Do the operation for the upper half
		not EDX
		pushf						// Save the second intermediatre flags

		mov EDI, DWORD PTR[result]	// Read sddress of the destination
		mov DWORD PTR[EDI], EAX		// Store lower half of the result into the destination
		mov DWORD PTR[EDI + 4], EDX	// Store upper half of the result into the destination

		pop BX						// Combine flags into single flags
		pop AX						// CF=CF2 PF=PF1 ZF=ZF1&ZF2, SF=SF2 OF=OF2

		mov CX, BX					// PF = PF1
		and CX, 0x0004
		and AX, 0xFFFD
		or AX, CX

		or BX, 0xFFBF				// ZF=ZF1&ZF2
		and AX, BX					// and also return flags in AX
		xor AX, 0x00C0				// toggle ZF & SF
	}
}