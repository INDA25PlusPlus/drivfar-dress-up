#ifndef ASSET_H
#define ASSET_H

#include <stddef.h>
#include <stdint.h>

// Represents a raw file.
typedef struct {
	// Numbers of bytes in `data`.
	size_t size;
	// The raw asset file data. May be null, indicating that there isn't an
	// asset.
	const uint8_t *data;
} asset_data_t;

#endif
