#include "ArgsParser.h"


ArgsParser& ArgsParser::Get() {
	static ArgsParser self;
	return self;
}

void ArgsParser::Init(int argc, char** argv) {
	for (auto i = 0; i < argc; i++) {
		_args.emplace_back(argv[i]);
	}
}