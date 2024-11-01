#include <iostream>
#include "BitmapImage.h"
#include "Steganography.h"
#define NB_BITS_RESERVED 30
#define END_CHAR 0x1F
Steganography::Steganography()
{
};

Steganography::~Steganography() { };

void Steganography::encodeNbChar(uint8_t* data, int nbChar)
{
	for (int i = 0; i < NB_BITS_RESERVED; i++)
	{
		if (data[i] & 1 ^ nbChar >> i & 1)
		{
			data[i] ^= 1;
		}
	}
}

void Steganography::encodeChar(uint8_t* data, char Char, int place)
{
	//Mise en place du mot de 7 bits au sein de deux pixels :
	//Deux bits pour le rouge du premier pixel et deux autres pour le rouge du deuxième pixel
	//Un bit pour le bleu du premier pixel et un autre pour le bleu du second pixel
	//Un bit pour le vert du premier pixel
	//Comparaison Bit In / Bit Out s'ils ne sont pas les mêmes alors Bit Out = Bit In

	if (Char >> 0 & 1 ^ data[(place * 6)] >> 0 & 1)
		data[(place * 6)] ^= (1 << 0);

	if (Char >> 1 & 1 ^ data[(place * 6)] >> 1 & 1)
		data[(place * 6)] ^= (1 << 1);

	if (Char >> 2 & 1 ^ data[(place * 6) + 3] >> 0 & 1)
		data[(place * 6) + 3] ^= (1 << 0);

	if (Char >> 3 & 1 ^ data[(place * 6) + 3] >> 1 & 1)
		data[(place * 6) + 3] ^= (1 << 1);

	if (Char >> 4 & 1 ^ data[(place * 6) + 1] >> 0 & 1)
		data[(place * 6) + 1] ^= (1 << 0);

	if (Char >> 5 & 1 ^ data[(place * 6) + 4] >> 0 & 1)
		data[(place * 6) + 4] ^= (1 << 0);

	if (Char >> 6 & 1 ^ data[(place * 6) + 2] >> 0 & 1)
		data[(place * 6) + 2] ^= (1 << 0);
}

int Steganography::getNbChar(uint8_t* data)
{
	int result = 0;
	for (int i = 0; i < NB_BITS_RESERVED; i++)
		if (data[i] & 1 ^ result >> i & 1)
			result ^= (1 << i);
	return result;
}

char Steganography::getChar(uint8_t* data, int place)
{
	char result = NULL;
	if (result >> 0 & 1 ^ data[(place * 6)] >> 0 & 1)
		result ^= (1 << 0);

	if (result >> 1 & 1 ^ data[(place * 6)] >> 1 & 1)
		result ^= (1 << 1);

	if (result >> 2 & 1 ^ data[(place * 6) + 3] >> 0 & 1)
		result ^= (1 << 2);

	if (result >> 3 & 1 ^ data[(place * 6) + 3] >> 1 & 1)
		result ^= (1 << 3);

	if (result >> 4 & 1 ^ data[(place * 6) + 1] >> 0 & 1)
		result ^= (1 << 4);

	if (result >> 5 & 1 ^ data[(place * 6) + 4] >> 0 & 1)
		result ^= (1 << 5);

	if (result >> 6 & 1 ^ data[(place * 6) + 2] >> 0 & 1)
		result ^= (1 << 6);
	
	return result;
}

void Steganography::encode(BitmapImage* input, const char* sentence)
{
	int iterator = 0;
	uint8_t* buffer = input->m_pixels;
	while (sentence[iterator] != NULL)
	{
		encodeChar(buffer, sentence[iterator], iterator + NB_BITS_RESERVED);
		iterator++;
	}
	encodeNbChar(buffer,iterator);
	//Putting an end character == 0x1F to know where end the secret message
	encodeChar(buffer, END_CHAR, iterator + NB_BITS_RESERVED);
}

char* Steganography::decode(BitmapImage* input) {
	uint8_t* buffer = input->m_pixels;

	if (getNbChar(buffer) > input->m_fileHeader->bfSize - NB_BITS_RESERVED)
		return NULL;

	if (!(getChar(buffer, getNbChar(buffer) + NB_BITS_RESERVED) == END_CHAR))
		return NULL;

	int iterator = 0;
	char* result = new char[getNbChar(buffer) + 1];
	result[iterator++] = getChar(buffer, NB_BITS_RESERVED + iterator);

	while (result[iterator - 1] != END_CHAR) {
		result[iterator++] = getChar(buffer, NB_BITS_RESERVED + iterator);
	}

	result[iterator] = '\0';

	return result;
}