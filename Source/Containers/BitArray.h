#pragma once
#include <cstdint>
#include <vector>


class BitArray {
public:
	using TChunk = uint64_t;
	BitArray() = default;
	explicit BitArray(uint32_t size);


	bool Get(uint32_t index) const;
	void Set(uint32_t index, bool val);


private:
	std::vector<TChunk> _chunks;
};