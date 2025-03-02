#pragma once
#include <map>
#include <optional>
#include <set>
#include <vector>


class ChunkStorage {
public:
	using Chunk = std::vector<uint8_t>;
	using Ptr = Chunk::value_type*;
	using MemSize = int64_t;
	using ChunkIdx = int64_t;


private:
	struct ChunkMemory {
		ChunkIdx chunkIdx;
		Ptr ptr;
	};

	struct MemoryInfo {
		ChunkIdx chunkIdx;
		Ptr ptr;
		MemSize size;
	};

	using FreePtrs = std::multimap<MemSize, MemoryInfo>;
	using FreePtrsIt = FreePtrs::iterator;
	using ConstFreePtrsIt = FreePtrs::iterator;


public:
	explicit ChunkStorage(MemSize chunkSize = 2048);

	MemSize GetChunkSize() const;

	Ptr ReserveMemory(MemSize size);
	bool DeleteMemory(Ptr ptr);
	void Clear();

	const std::vector<Chunk>& GetChunks() const;
	const FreePtrs& GetFreeMemory() const;


protected:
	ChunkIdx AddChunk();

	void AddFreeMemory(const MemoryInfo& memory);
	void RemoveFreeMemory(ConstFreePtrsIt&& it);

	bool MergeFree(const MemoryInfo& info);
	std::optional<MemoryInfo> MergeFreeLeft(const MemoryInfo& info);
	std::optional<MemoryInfo> MergeFreeRight(const MemoryInfo& info);


private:
	MemSize _chunkSize;
	std::vector<Chunk> _chunks;

	struct EngagedInfo {
		ChunkIdx chunkIdx;
		MemSize size;
	};

	std::map<Ptr, EngagedInfo> _engagedMemory;

	FreePtrs _freeMemInfo;
	std::map<Ptr, FreePtrsIt> _freePtrs;
};