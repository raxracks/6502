#include <stdio.h>
#include <slibs/slibs.h>
#include <stdint.h>
#include <assert.h>

uint8_t N = 1 << 7;
uint8_t V = 1 << 6;
uint8_t B = 1 << 4;
uint8_t D = 1 << 3;
uint8_t I = 1 << 2;
uint8_t Z = 1 << 1;
uint8_t C = 1 << 0;

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

void add_flag(uint8_t flag) {
	cpu.P |= flag;
}

void clear_flag(uint8_t flag) {
	cpu.P &= ~flag;
}

void set_flag(uint8_t flag, int condition) {
	if(condition == 1) {
		add_flag(flag);
	} else if(condition == 0) {
		clear_flag(flag);
	}
}

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
			case 0x4:
				printf(" ∟ STY $%02X\n", one);
				memory[one] = cpu.Y;
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
			case 0xA:
				printf(" ∟ TXA\n");
				cpu.A = cpu.X;

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				break;
			case 0xC:
				printf(" ∟ STY $%04X\n", word);
				memory[word] = cpu.Y;
				cpu.PC += 2;
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
			case 0x4:
				printf(" ∟ STY $%02X, X\n", one);
				memory[one + cpu.X] = cpu.Y;
				cpu.PC++;
				break;

			case 0x5:
				printf(" ∟ STA $%02X, X\n", word);
				memory[one + cpu.X] = cpu.A;
				cpu.PC++;
				break;
			case 0x6:
				printf(" ∟ STX $%02X, Y\n", one);
				memory[one + cpu.Y] = cpu.X;
				cpu.PC++;
				break;
			case 0x8:
				printf(" ∟ TYA\n");
				cpu.A = cpu.Y;

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				break;
			case 0x9:
				printf(" ∟ STA $%04X, Y\n", word);
				memory[word + cpu.Y] = cpu.A;
				cpu.PC += 2;
				break;
			case 0xA:
				printf("TXS\n");
				cpu.SP = cpu.X;
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

				set_flag(N, cpu.Y & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				cpu.PC++;
				break;
			case 0x1:
				printf(" ∟ LDA ($%02X, X)\n", one);
				cpu.A = memory[memory[one + cpu.X]];

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC++;
				break;

			case 0x2:
				printf(" ∟ LDX #$%02X\n", one);
				cpu.X = one;

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				cpu.PC++;
				break;
			case 0x4:
				printf(" ∟ LDY $%02X\n", one);
				cpu.Y = memory[one];

				set_flag(N, cpu.Y & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				cpu.PC++;
				break;
			case 0x5:
				printf(" ∟ LDA $%02X\n", one);
				cpu.A = memory[one];

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC++;
				break;
			case 0x6:
				printf(" ∟ LDX $%02X\n", one);
				cpu.X = memory[one];

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				cpu.PC++;
				break;
			case 0x8:
				printf(" ∟ TAY\n");
				cpu.Y = cpu.A;

				set_flag(N, cpu.Y & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				break;
			case 0x9:
				printf(" ∟ LDA #$%02X\n", one);
				cpu.A = one;

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC++;
				break;
			case 0xA:
				printf(" ∟ TAX\n");
				cpu.X = cpu.A;

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				break;
			case 0xC:
				printf(" ∟ LDY $%04X\n", word);
				cpu.Y = memory[word];

				set_flag(N, cpu.Y & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				cpu.PC += 2;
				break;
			case 0xD:
				printf(" ∟ LDA $%04X\n", word);
				cpu.A = memory[word];

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC += 2;
				break;
			case 0xE:
				printf(" ∟ LDX $%04X\n", word);
				cpu.X = memory[word];

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				cpu.PC += 2;
				break;
			}
			break;
		case 0xB:
			switch(l_nib) {
			case 0x1:
				printf(" ∟ LDA ($%02X), Y\n", one);
				cpu.A = memory[memory[one] + cpu.Y];

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC++;
				break;
			case 0x4:
				printf(" ∟ LDY $%02X, X\n", one);
				cpu.Y = memory[one + cpu.X];

				set_flag(N, cpu.Y & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				cpu.PC++;
				break;
			case 0x5:
				printf(" ∟ LDA $%02X, X\n", one);
				cpu.A = memory[one + cpu.X];

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC++;
				break;
			case 0x6:
				printf(" ∟ LDX $%02X, Y\n", one);
				cpu.X = memory[one + cpu.Y];

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				cpu.PC++;
				break;
			case 0x9:
				printf(" ∟ LDA $%04X, Y\n", word);
				cpu.A = memory[word + cpu.Y];

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC++;
				break;
			case 0xA:
				printf("TSX\n");
				cpu.X = cpu.SP;

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				break;
			case 0xC:
				printf(" ∟ LDY $%04X, X\n", word);
				cpu.Y = memory[word + cpu.X];

				set_flag(N, cpu.Y & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				cpu.PC += 2;
				break;
			case 0xD:
				printf(" ∟ LDA $%04X, X\n", word);
				cpu.A = memory[word + cpu.X];

				set_flag(N, cpu.A & 0x80 != 0);	
				set_flag(Z, cpu.A == 0);

				cpu.PC += 2;
				break;
			case 0xE:
				printf(" ∟ LDX $%04X, Y\n", word);
				cpu.X = memory[word + cpu.Y];

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				cpu.PC += 2;
				break;
			}
			break;
		case 0xC:
			switch(l_nib) {
			case 0x8:
				printf(" ∟ INY\n");
				cpu.Y++;
				
				set_flag(N, cpu.Y & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				break;
			}
			break;
		case 0xE:
			switch(l_nib) {
			case 0x6:
				printf(" ∟ INC $%02X\n", one);
				memory[one]++;

				set_flag(N, memory[one] & 0x80 != 0);	
				set_flag(Z, memory[one] == 0);

				cpu.PC++;
				break;
			case 0x8:
				printf(" ∟ INX\n");
				cpu.X++;

				set_flag(N, cpu.X & 0x80 != 0);	
				set_flag(Z, cpu.X == 0);

				break;
			case 0xA:
				printf(" ∟ NOP\n");
				break;
			case 0xE:
				printf(" ∟ INC $%04X\n", word);
				memory[word]++;

				set_flag(N, memory[word] & 0x80 != 0);	
				set_flag(Z, memory[word] == 0);

				cpu.PC += 2;
				break;
			}
			break;
		case 0xF:
			switch(l_nib) {
			case 0x6:
				printf(" ∟ INC $%02X, X\n", one);
				uint8_t addr = one + cpu.X;
				memory[addr]++;

				set_flag(N, memory[addr] & 0x80 != 0);	
				set_flag(Z, memory[addr] == 0);

				cpu.PC++;
				break;
			case 0xE:
				printf(" ∟ INC $%04X, X\n", word);
				uint16_t addr = word + cpu.X;
				memory[addr]++;
				
				set_flag(N, memory[addr] & 0x80 != 0);	
				set_flag(Z, cpu.Y == 0);

				cpu.PC += 2;
				break;
			}
			break;
		default:
			printf(" ∟ Unimplemented\n"); 
			break;
		}
	}

	printf("\nCPU State:\n"
		"A=$%02X "
		"X=$%02X "
		"Y=$%02X\n"
		"SP=$%02X "
		"PC=$%04X\n"
		"NV-BDIZC\n"
		"%08b\n\n",
	cpu.A, cpu.X, cpu.Y, cpu.SP, cpu.PC, cpu.P);

	printf("Memory State:\n");
	for(int i = 0; i < 0xffff - 0x8000; i++) {
		if(memory[i] != 0x0) {
			printf("$%04X: $%02X (%%%08b)\n", i, memory[i], memory[i]);
		}
	}

	return 0;
}
