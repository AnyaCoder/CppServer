#ifndef __ANYA_UTIL_H__
#define __ANYA_UTIL_H__
#include <stdint.h>
#include <thread>
#include <functional>

namespace anya {

	uint32_t GetThreadId();
	uint32_t GetFiberId();
}
#endif // !__ANYA_UTIL_H__
