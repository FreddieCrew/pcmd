/*
# impl.cpp

As with the header, this is the actual implementation of the plugin's
functionality with no AMX specific code or includes.

Including `common.hpp` for access to `logprintf` is useful for debugging but for
production debug logging, it's best to use a dedicated logging library such as
log-core by maddinat0r.
*/

#include "impl.hpp"

void Impl::Murmur3(const char* key, size_t len, int* hash) {
	const uint32_t c1 = 0xcc9e2d51;
	const uint32_t c2 = 0x1b873593;
	const int r1 = 15;
	const int r2 = 13;
	const int m = 5;
	const uint32_t n = 0xe6546b64;

	*hash = 0;

	const int nblocks = len / 4;
	const uint32_t* blocks = reinterpret_cast<const uint32_t*>(key);

	for (int i = 0; i < nblocks; ++i) {
		uint32_t k = blocks[i];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		*hash ^= k;
		*hash = (*hash << r2) | (*hash >> (32 - r2));
		*hash = *hash * m + n;
	}

	const uint8_t* tail = reinterpret_cast<const uint8_t*>(key + nblocks * 4);
	uint32_t k1 = 0;

	switch (len & 3) {
	case 3:
		k1 ^= static_cast<uint32_t>(tail[2]) << 16;
	case 2:
		k1 ^= static_cast<uint32_t>(tail[1]) << 8;
	case 1:
		k1 ^= static_cast<uint32_t>(tail[0]);
		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		*hash ^= k1;
	}

	*hash ^= len;
	*hash ^= (*hash >> 16);
	*hash *= 0x85ebca6b;
	*hash ^= (*hash >> 13);
	*hash *= 0xc2b2ae35;
	*hash ^= (*hash >> 16);
}

