#include "BitArray.h"


int8_t BitArray::Get(uint64_t Index) const {
	const uint64_t BitsIndex = Index / BitsSize;
	if (BitsIndex >= (uint64_t)_bits.size())
		return -1;

	const TStored Module = Index % BitsSize;
	return static_cast<int8_t>((_bits[BitsIndex] & (1 << Module)) >> Module);
}

void BitArray::SetNResize(uint64_t Index, bool Value) {
	const uint64_t BitsIndex = Index / BitsSize;
	if (BitsIndex >= _bits.size()) {
		_bits.resize(BitsIndex + 1, 0);
	}

	const TStored Module = Index % BitsSize;
	_bits[BitsIndex] = (_bits[BitsIndex] & ~(1 << Module)) | static_cast<TStored>(Value << Module);
}

bool BitArray::Set(uint64_t Index, bool Value) {
	const uint64_t BitsIndex = Index / BitsSize;
	if (BitsIndex >= _bits.size()) {
		return false;
	}

	const TStored Module = Index % BitsSize;
	_bits[BitsIndex] = (_bits[BitsIndex] & ~(1 << Module)) | static_cast<TStored>(Value << Module);
	return true;
}

void BitArray::Resize(uint64_t NewSize) {
	uint64_t Capacity = NewSize / BitsSize;
	if (NewSize % BitsSize > 0)
		Capacity += 1;

	_bits.resize(Capacity);
}

void BitArray::Clear() {
	_bits.clear();
}

uint64_t BitArray::Size() const {
	return _bits.size() * BitsSize;
}

const std::vector<BitArray::TStored>& BitArray::GetBits() const {
	return _bits;
}

bool BitArray::operator==(const BitArray& InBa) const {
	return _bits == InBa._bits;
}

int8_t BitArray::operator[](uint64_t index) const {
	return Get(index);
}