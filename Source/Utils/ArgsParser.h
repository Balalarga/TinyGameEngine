#pragma once
#include <string>
#include <vector>

#include "NonCopyable.h"


class ArgsParser: public NonMoveCopyable {
public:
	static ArgsParser& Get();
	void Init(int argc, char** argv);


private:
	std::vector<std::string> _args;
};