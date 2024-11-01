#pragma once
class BitmapImage;


class Steganography
{
private:

	static void encodeNbChar(uint8_t* data, int nbChar);
	static void encodeChar(uint8_t* data, char Char, int place);
	static int getNbChar(uint8_t* data);
	static char getChar(uint8_t* data, int place);

public:

	Steganography();
	~Steganography();

	static void encode(BitmapImage* input, const char* sentence);
	static char* decode(BitmapImage* input);
};

