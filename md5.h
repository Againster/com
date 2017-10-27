#pragma once
#include <string>
#include <cstring>
#include <fstream>
#include <stdint.h>

using std::string;
using std::ifstream;

class MD5 {
public:
	explicit MD5();
	explicit MD5(const void *input, size_t length);
	explicit MD5(const string &str);
	explicit MD5(ifstream &in);
	void Update(const void *input, std::streamsize length);
	void Update(const string &str);
	void Update(ifstream &in);
	const unsigned char* Digest();
	string ToString();
	void Reset();
private:
	void Update(const unsigned char *input, std::streamsize length);
	void Final();
	void Transform(const unsigned char block[64]);
	void Encode(const uint32_t *input, unsigned char *output, size_t length);
	void Decode(const unsigned char *input, uint32_t *output, size_t length);
	string UCsToHexString(const unsigned char *input, size_t length);

	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator=(const MD5&);
private:
	uint32_t _state[4]; /* state (ABCD) */
	uint32_t _count[2]; /* number of bits, modulo 2^64 (low-order word first) */
	unsigned char _buffer[64]; /* input buffer */
	unsigned char _Digest[16]; /* message digest */
	bool _finished; /* calculate finished ? */

	static const unsigned char PADDING[64]; /* padding for calculate */
	static const char HEX[16];
	static const size_t BUFFER_SIZE = 1024;
};

