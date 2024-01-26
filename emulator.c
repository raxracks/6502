#include <stdio.h>
#include <slibs/slibs.h>
#include <stdint.h>
#include <assert.h>

#define NEGATIVE 	1 << 7
#define OVERFLOW 	1 << 6
#define BREAK 		1 << 4
#define DECIMAL 	1 << 3
#define INTERRUPT 	1 << 2
#define ZERO 		1 << 1
#define CARRY 		1 << 0

typedef struct {
	uint8_t A;
	uint8_t X;
	uint8_t Y;
	uint8_t SP;
	uint8_t P;
	uint16_t PC;
} CPU;

CPU cpu = { 0 };
uint8_t* memory; 

int main(int argc, char** argv) {
	assert(argc > 1);

	sl_string code = { 0 };
	sl_read_file(argv[1], &code);

	memory = malloc(sizeof(uint8_t) * 0xffff);
	memcpy(memory + 0x8000, code.data, code.size);

	for(cpu.PC = 0; cpu.PC < code.size; cpu.PC++) {
		uint8_t opcode = code.data[cpu.PC];
		uint8_t one = code.data[cpu.PC + 1];
		uint8_t two = code.data[cpu.PC + 2];
		uint16_t word = (two << 8) | one;

		uint8_t h_nib = (opcode >> 4) & 0xF;
		uint8_t l_nib = opcode & 0xF;

		printf("%d: 0x%02X (h_nib: 0x%X, l_nib: 0x%X)\n", cpu.PC, opcode, h_nib, l_nib);

		switch(h_nib) {
		case 0x8:
			switch(l_nib) {
			case 0x1:
				printf(" ∟ STA ($%02X, X)\n", one);
				memory[memory[one + cpu.X]] = cpu.A;
				cpu.PC++;
				break;
			case 0x5:
				printf(" ∟ STA $%02X\n", one);
				memory[one] = cpu.A;
				cpu.PC++;
				break;
			case 0x6:
				printf(" ∟ STX $%02X\n", one);
				memory[one] = cpu.X;
				cpu.PC++;
				break;
			case 0xD:
				printf(" ∟ STA $%04X\n", word);
				memory[word] = cpu.A;
				cpu.PC += 2;
				break;
			case 0xE:
				printf(" ∟ STX $%04X\n", word);
				memory[word] = cpu.X;
				cpu.PC += 2;
				break;
			}
			break;
		case 0x9:
			switch(l_nib) {
			case 0x1:
				printf(" ∟ STA ($%02X), Y\n", one);
				memory[memory[one] + cpu.Y] = cpu.A;
				cpu.PC++;
				break;

			case 0x5:
				printf(" ∟ STA $%02X, X\n", word);
				memory[one + cpu.X] = cpu.A;
				cpu.PC++;
				break;
			case 0x9:
				printf(" ∟ STA $%04X, Y\n", word);
				memory[word + cpu.Y] = cpu.A;
				cpu.PC += 2;
				break;
			case 0xD:
				printf(" ∟ STA $%04X, X\n", word);
				memory[word + cpu.X] = cpu.A;
				cpu.PC += 2;
				break;
			}
			break;
		case 0xA:
			switch(l_nib) {
			case 0x0:
				printf(" ∟ LDY #$%02X\n", one);
				cpu.Y = one;
				cpu.PC++;
				break;
			case 0x1:
				printf(" ∟ LDA ($%02X, X)\n", one);
				cpu.A = memory[memory[one + cpu.X]];
				printf("addr: %x val: %x\n", memory[one + cpu.X], cpu.A);
				cpu.PC++;
				break;

			case 0x2:
				printf(" ∟ LDX #$%02X\n", one);
				cpu.X = one;
				cpu.PC++;
				break;
			case 0x4:
				printf(" ∟ LDY $%02X\n", one);
				cpu.Y = memory[one];
				cpu.PC++;
				break;
			case 0x5:
				printf(" ∟ LDA $%02X\n", one);
				cpu.A = memory[one];
				cpu.PC++;
				break;
			case 0x6:
				printf(" ∟ LDX $%02X\n", one);
				cpu.X = memory[one];
				cpu.PC++;
				break;
			case 0x9:
				printf(" ∟ LDA #$%02X\n", one);
				cpu.A = one;
				cpu.PC++;
				break;
			case 0xC:
				printf(" ∟ LDY $%04X\n", word);
				cpu.Y = memory[word];
				cpu.PC += 2;
				break;
			case 0xD:
				printf(" ∟ LDA $%04X\n", word);
				cpu.A = memory[word];
				cpu.PC += 2;
				break;
			case 0xE:
				printf(" ∟ LDX $%04X\n", word);
				cpu.X = memory[word];
				cpu.PC += 2;
				break;
			}
			break;
		case 0xB:
			switch(l_nib) {
			case 0x1:
				printf(" ∟ LDA ($%02X), Y\n", one);
				cpu.A = memory[memory[one] + cpu.Y];
				printf("addr: %x val: %x\n", memory[one] + cpu.Y, cpu.A);
				cpu.PC++;
				break;
			case 0x4:
				printf(" ∟ LDY $%02X, X\n", one);
				cpu.Y = memory[one + cpu.X];
				printf("addr: %x val: %x\n", one + cpu.X, cpu.Y);
				cpu.PC++;
				break;
			case 0x5:
				printf(" ∟ LDA $%02X, X\n", one);
				cpu.A = memory[one + cpu.X];
				printf("addr: %x val: %x\n", one + cpu.X, cpu.A);
				cpu.PC++;
				break;
			case 0x6:
				printf(" ∟ LDX $%02X, Y\n", one);
				cpu.X = memory[one + cpu.Y];
				printf("addr: %x val: %x\n", one + cpu.Y, cpu.X);
				cpu.PC++;
				break;
			case 0x9:
				printf(" ∟ LDA $%04X, Y\n", word);
				cpu.A = memory[word + cpu.Y];
				printf("addr: %x val: %x\n", word + cpu.Y, cpu.A);
				cpu.PC++;
				break;
			case 0xC:
				printf(" ∟ LDY $%04X, X\n", word);
				cpu.Y = memory[word + cpu.X];
				printf("addr: %x val: %x\n", word + cpu.X, cpu.Y);
				cpu.PC += 2;
				break;
			case 0xD:
				printf(" ∟ LDA $%04X, X\n", word);
				cpu.A = memory[word + cpu.X];
				printf("addr: %x val: %x\n", word + cpu.X, cpu.A);
				cpu.PC += 2;
				break;
			case 0xE:
				printf(" ∟ LDX $%04X, Y\n", word);
				cpu.X = memory[word + cpu.Y];
				printf("addr: %x val: %x\n", word + cpu.Y, cpu.X);
				cpu.PC += 2;
				break;
			}
			break;
		case 0xC:
			switch(l_nib) {
			case 0x8:
				printf(" ∟ INY\n");
				cpu.Y++;
				break;
			}
			break;
		case 0xE:
			switch(l_nib) {
			case 0x6:
				printf(" ∟ INC $%02X\n", one);
				memory[one]++;
				cpu.PC++;
				break;
			case 0x8:
				printf(" ∟ INX\n");
				cpu.X++;
				break;
			case 0xE:
				printf(" ∟ INC $%04X\n", word);
				memory[word]++;
				cpu.PC += 2;
				break;
			}
			break;
		case 0xF:
			switch(l_nib) {
			case 0x6:
				printf(" ∟ INC $%02X, X\n", one);
				memory[one + cpu.X]++;
				cpu.PC++;
				break;
			case 0xE:
				printf(" ∟ INC $%04X, X\n", word);
				memory[word + cpu.X]++;
				cpu.PC += 2;
				break;
			}
			break;
		default:
			printf(" ∟ Unimplemented\n"); 
			break;
		}
	}

	printf("CPU State:\n"
			"A\t:\t0x%08X\n"
			"X\t:\t0x%08X\n"
			"Y\t:\t0x%08X\n"
			"SP\t:\t0x%08X\n"
			"P\t:\t0x%08X\n"
			"PC\t:\t0x%016X\n\n"
		, cpu.A, cpu.X, cpu.Y, cpu.SP, cpu.P, cpu.PC);

	printf("Memory State:\n");
	for(int i = 0; i < 0xffff; i++) {
		if(memory[i] != 0x0) {
			printf("0x%04X: 0x%02X (0b%08b)\n", i, memory[i], memory[i]);
		}
	}

	return 0;
}
