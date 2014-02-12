#ifndef SHADER_DEFINES_H
#define SHADER_DEFINES_H

#define SHADER_DEBUG 1

#define MSAA_SAMPLES 4

#define MAX_LIGHTS_POWER 12
#define MAX_LIGHTS (1<<MAX_LIGHTS_POWER)

// This determines the tile size for light binning and associated tradeoffs
#define COMPUTE_SHADER_TILE_GROUP_DIM 16
#define COMPUTE_SHADER_TILE_GROUP_SIZE (COMPUTE_SHADER_TILE_GROUP_DIM*COMPUTE_SHADER_TILE_GROUP_DIM)

#define DEFER_PER_SAMPLE 1

#endif