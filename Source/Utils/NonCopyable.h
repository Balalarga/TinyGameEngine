#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H


struct NonCopyable {
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

	NonCopyable(NonCopyable&&) = default;
	NonCopyable& operator=(NonCopyable&&) = default;
};

struct NonMoveCopyable {
	~NonMoveCopyable() = default;
	NonMoveCopyable() = default;
	NonMoveCopyable(const NonMoveCopyable&) = delete;
	NonMoveCopyable& operator=(const NonMoveCopyable&) = delete;

	NonMoveCopyable(NonMoveCopyable&&) = delete;
	NonMoveCopyable& operator=(NonMoveCopyable&&) = delete;
};

#endif //NONCOPYABLE_H