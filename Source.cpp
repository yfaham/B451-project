#include <iostream>
#include "AESConstants.h"

using namespace std;

const int M[4][4] = { {2,3,1,1}, {1,2,3,1}, {1,1,2,3}, {3,1,1,2} };  // for mixColumns

// major functions
void subBytes(unsigned char state[4][4]);
void shiftRows(unsigned char state[4][4]);
void mixColumns(unsigned char state[4][4], unsigned char result[4][4]);

// helper functions
unsigned char byteProduct(int n, unsigned char byte);  // used in mixColumns

int main()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << M[i][j] << ' ';
		}
		cout << endl;
	}
	system("pause");
	return 0;
}

void subBytes(unsigned char state[4][4])
{
	int hex1, hex2;  // byte = hex1 * 16^1 + hex2 * 16^0
	for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++) {
			hex2 = state[r][c] % 16;
			hex1 = (state[r][c] / 16) % 16;
			state[r][c] = SBOX[hex1 * 16 + hex2];
		}
}

void shiftRows(unsigned char state[4][4])
{
	//shift first and third rows
	unsigned char state10 = state[1][0],
		state33 = state[3][3];
	for (int i = 0; i < 3; i++) {
		state[1][i] = state[1][i + 1];  // shift first row
		state[3][3 - i] = state[3][2 - i];  // shift third row
	}
	state[1][3] = state10;
	state[3][0] = state33;

	// shift second row
	swap(state[2][0], state[2][2]);
	swap(state[2][1], state[2][3]);
}

void mixColumns(unsigned char state[4][4], unsigned char result[4][4])
{
	for (int c = 0; c < 4; c++)  // c is for columns
		for (int r = 0; r < 4; r++) {  // r is for rows
			result[r][c] = 0;
			for (int i = 0; i < 4; i++)
				result[r][c] = result[r][c] ^ byteProduct(M[r][i], state[i][c]);
		}
}

unsigned char byteProduct(int n, unsigned char byte)
{
	if (n == 2) {
		unsigned char result = byte << 1;
		if (byte >= 128)  // mcb equals 1
			result = result ^ 27;  // result XOR 11011
		return result;
	}
	else if (n == 3) {
		return byteProduct(2, byte) ^ byte;
	}
	else if (n < 1 || 3 < n) {  // wrong value for n
		cout << "\nerror in byteProduct: n is outside [1, 3].\n";
		system("pause");
		exit(1);
	}
	else  // n should be 1
		return byte;
}