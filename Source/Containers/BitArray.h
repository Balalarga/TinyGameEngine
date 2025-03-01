#pragma once
#include <cstdint>
#include <vector>


class BitArray {
public:
	using TStored = uint32_t;
	static constexpr uint8_t BitsSize = sizeof(TStored) * 8;

	~BitArray() = default;

	// If Index > Size returns -1
	int8_t Get(uint64_t Index) const;
	void SetNResize(uint64_t Index, bool Value);
	[[maybe_unused]] bool Set(uint64_t Index, bool Value);

	uint64_t Size() const;
	void Resize(uint64_t NewSize);
	void Clear();

	const std::vector<TStored>& GetBits() const;

	bool operator==(const BitArray& InBa) const;
	int8_t operator[](uint64_t index) const;


private:
	std::vector<TStored> _bits;
};