#ifndef SHADER_DEFINES_H
#define SHADER_DEFINES_H

#define MSAA_SAMPLES 4

#define MAX_LIGHTS_POWER 12
#define MAX_LIGHTS (1<<MAX_LIGHTS_POWER)

// This determines the tile size for light binning and associated tradeoffs
#define COMPUTE_SHADER_TILE_GROUP_DIM 16
#define COMPUTE_SHADER_TILE_GROUP_SIZE (COMPUTE_SHADER_TILE_GROUP_DIM*COMPUTE_SHADER_TILE_GROUP_DIM)

// If enabled, defers scheduling of per-sample-shaded pixels until after sample 0
// has been shaded across the whole tile. This allows better SIMD packing and scheduling.
// This should basically always be left enabled in practice since it's faster everywhere
// but we maintain the legacy path for benckmarking comparison for now.
#define DEFER_PER_SAMPLE 1

#endif