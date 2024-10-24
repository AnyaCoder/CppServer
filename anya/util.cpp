#include "util.h"

namespace anya {
	uint32_t GetThreadId() {
		return std::hash<std::thread::id>{}(std::this_thread::get_id());
	}

	uint32_t GetFiberId() {
		return 0;
	}
}