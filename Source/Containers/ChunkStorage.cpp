#include "ChunkStorage.h"

#include <cassert>
#include <utility>


#ifndef CHUNK_STORAGE_ZERO_MEMORY
#define CHUNK_STORAGE_ZERO_MEMORY 1
#endif


ChunkStorage::ChunkStorage(int64_t chunkSize): _chunkSize(chunkSize) {
}

bool ChunkStorage::DeleteMemory(Ptr ptr) {
	const auto it = _engagedMemory.find(ptr);
	if (it == _engagedMemory.end()) {
		return false;
	}

	MemoryInfo memInfo{ it->second.chunkIdx, it->first, it->second.size };
	_engagedMemory.erase(it);
	#if CHUNK_STORAGE_ZERO_MEMORY
	memset(memInfo.ptr, 0, memInfo.size);
	#endif

	if (!MergeFree(memInfo))
		AddFreeMemory(memInfo);

	return true;
}

int64_t ChunkStorage::GetChunkSize() const {
	return _chunkSize;
}

ChunkStorage::Ptr ChunkStorage::ReserveMemory(int64_t size) {
	MemoryInfo memInfo;
	if (auto closestIt = _freeMemInfo.upper_bound(size); closestIt != _freeMemInfo.end()) {
		memInfo = closestIt->second;
		RemoveFreeMemory(std::move(closestIt));
	} else {
		const int64_t newChunkIdx = AddChunk();
		memInfo = { newChunkIdx, _chunks[newChunkIdx].data(), _chunkSize };
	}

	assert(memInfo.size >= size);
	if (memInfo.size > size) {
		AddFreeMemory(MemoryInfo{ memInfo.chunkIdx, memInfo.ptr + size, memInfo.size - size });
	}

	_engagedMemory.emplace(memInfo.ptr, EngagedInfo{ memInfo.chunkIdx, size });
	return memInfo.ptr;
}

void ChunkStorage::Clear() {
	_chunks.clear();
}

int64_t ChunkStorage::AddChunk() {
	auto& chunk =
			#if CHUNK_STORAGE_ZERO_MEMORY
			_chunks.emplace_back(_chunkSize, 0);
	#else
			_chunks.emplace_back(_chunkSize);
	#endif
	return static_cast<int64_t>(_chunks.size() - 1);
}

void ChunkStorage::AddFreeMemory(const MemoryInfo& memory) {
	if (memory.size <= 0)
		return;

	const auto it = _freeMemInfo.emplace(memory.size, memory);
	_freePtrs.emplace(memory.ptr, it);
}

void ChunkStorage::RemoveFreeMemory(ConstFreePtrsIt&& it) {
	_freePtrs.erase(it->second.ptr);
	_freeMemInfo.erase(it);
}

bool ChunkStorage::MergeFree(const MemoryInfo& info) {
	auto mergeRight = MergeFreeRight(info);
	auto mergeLeft = MergeFreeLeft(mergeRight ? *mergeRight : info);
	if (mergeLeft.has_value()) {
		return true;
	}

	if (mergeRight) {
		auto freeMemIt = _freePtrs.try_emplace(mergeRight->ptr, _freeMemInfo.emplace(mergeRight->size, *mergeRight));
		assert(freeMemIt.second);
		return true;
	}

	return false;
}

std::optional<ChunkStorage::MemoryInfo> ChunkStorage::MergeFreeLeft(const MemoryInfo& info) {
	auto leftFreeIt = _freePtrs.lower_bound(info.ptr);
	if (leftFreeIt == _freePtrs.begin())
		return std::nullopt;

	if (leftFreeIt == _freePtrs.end())
		--leftFreeIt;

	while (leftFreeIt != _freePtrs.begin() && leftFreeIt->second->second.chunkIdx != info.chunkIdx) {
		--leftFreeIt;
	}

	if (leftFreeIt->second->second.chunkIdx != info.chunkIdx)
		return std::nullopt;

	MemoryInfo freeInfo = leftFreeIt->second->second;
	assert(info.chunkIdx == freeInfo.chunkIdx);

	if (freeInfo.ptr + freeInfo.size != info.ptr)
		return std::nullopt;

	freeInfo.size += info.size;
	assert(freeInfo.size < _chunkSize);

	_freeMemInfo.erase(leftFreeIt->second);
	leftFreeIt->second = _freeMemInfo.emplace(freeInfo.size, freeInfo);

	return freeInfo;
}

std::optional<ChunkStorage::MemoryInfo> ChunkStorage::MergeFreeRight(const MemoryInfo& info) {
	auto rightFreeIt = _freePtrs.upper_bound(info.ptr);
	if (rightFreeIt == _freePtrs.end())
		return std::nullopt;

	MemoryInfo freeInfo = rightFreeIt->second->second;
	if (info.chunkIdx != freeInfo.chunkIdx)
		return std::nullopt;

	if (freeInfo.ptr - info.ptr != info.size)
		return std::nullopt;

	_freeMemInfo.erase(rightFreeIt->second);
	_freePtrs.erase(rightFreeIt);

	return MemoryInfo{ info.chunkIdx, info.ptr, freeInfo.size + info.size };
}

const std::vector<std::vector<uint8_t>>& ChunkStorage::GetChunks() const {
	return _chunks;
}

const ChunkStorage::FreePtrs& ChunkStorage::GetFreeMemory() const {
	return _freeMemInfo;
}