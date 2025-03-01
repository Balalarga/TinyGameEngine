#include <gtest/gtest.h>
#include "ChunkStorage.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/ranges.h"

void PrintFreeMemory(const ChunkStorage& s) {
	for (auto& [size, info]: s.GetFreeMemory()) {
		spdlog::info("Offset: {}. Size: {}", int32_t(info.ptr - s.GetChunks()[info.chunkIdx].data()), size);
	}
};

void PrintChunks(const ChunkStorage& s) {
	spdlog::info("{}", fmt::join(s.GetChunks().begin(), s.GetChunks().end(), ""));
};

TEST(ChunkStorage, SimpleMerging) {
	ChunkStorage storage(20);
	auto mem = storage.ReserveMemory(5);
	memset(mem, 1, 5);
	auto mem1 = storage.ReserveMemory(5);
	memset(mem1, 2, 5);
	auto mem2 = storage.ReserveMemory(5);
	memset(mem2, 3, 5);
	PrintChunks(storage);
	PrintFreeMemory(storage);
	bool res = storage.DeleteMemory(mem1);
	spdlog::info("deleted: {}", res);
	PrintChunks(storage);
	PrintFreeMemory(storage);
	res = storage.DeleteMemory(mem2);
	spdlog::info("deleted: {}", res);
	PrintChunks(storage);
	PrintFreeMemory(storage);
}

TEST(ChunkStorage, RandomNumBools) {
	srand(time(0));
	spdlog::info("Random Bool Test");
	ChunkStorage storage(20 * 1024);
	struct MemHandle {
		MemHandle(ChunkStorage& storage, int64_t size): storage(storage), ptr(storage.ReserveMemory(size)) {
		}

		~MemHandle() {
			Reset();
		}

		void Reset() {
			if (ptr) {
				storage.DeleteMemory(ptr);
				ptr = nullptr;
			}
		}

		ChunkStorage& storage;
		ChunkStorage::Ptr ptr;
	};

	auto testNum = rand() % storage.GetChunkSize();
	std::vector<MemHandle> handles;
	handles.reserve(testNum);
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	for (int i = 0; i < testNum; i++) {
		handles.emplace_back(storage, 1);
	}
	spdlog::info(
		"Init vector of {} bools in {}us",
		testNum,
		std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
	start = std::chrono::steady_clock::now();
	for (auto& h: handles) {
		h.Reset();
	}
	spdlog::info(
		"Reset data in {}us",
		std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
}