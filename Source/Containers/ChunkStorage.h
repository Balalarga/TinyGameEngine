#pragma once
#include <map>
#include <optional>
#include <set>
#include <vector>


class ChunkStorage {
public:
	explicit ChunkStorage(int64_t chunkSize = 2048);
	using Ptr = uint8_t*;


private:
	struct ChunkMemory {
		int64_t chunkIdx;
		Ptr ptr;
	};

	struct MemoryInfo {
		int64_t chunkIdx;
		Ptr ptr;
		int64_t size;
	};

	using FreePtrs = std::multimap<int64_t, MemoryInfo>;
	using FreePtrsIt = FreePtrs::iterator;
	using ConstFreePtrsIt = FreePtrs::iterator;


public:
	int64_t GetChunkSize() const;

	Ptr ReserveMemory(int64_t size);
	bool DeleteMemory(Ptr ptr);
	void Clear();

	const std::vector<std::vector<uint8_t>>& GetChunks() const;
	const FreePtrs& GetFreeMemory() const;


protected:
	int64_t AddChunk();

	void AddFreeMemory(const MemoryInfo& memory);
	void RemoveFreeMemory(ConstFreePtrsIt&& it);

	bool MergeFree(const MemoryInfo& info);
	std::optional<MemoryInfo> MergeFreeLeft(const MemoryInfo& info);
	std::optional<MemoryInfo> MergeFreeRight(const MemoryInfo& info);


private:
	int64_t _chunkSize;
	std::vector<std::vector<uint8_t>> _chunks;

	struct EngagedInfo {
		int64_t chunkIdx;
		int64_t size;
	};

	std::map<Ptr, EngagedInfo> _engagedMemory;

	FreePtrs _freeMemInfo;
	std::map<Ptr, FreePtrsIt> _freePtrs;
};