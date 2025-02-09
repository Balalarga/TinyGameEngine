#include "BitArray.h"

BitArray::BitArray(uint32_t size): _chunks(size) {
	memset(_chunks.data(), 0, _chunks.size() * sizeof(TChunk));
}

bool BitArray::Get(uint32_t index) const {
	return _chunks[index / 8] & (1 << (index % 8));
}

void BitArray::Set(uint32_t index, bool val) {
}