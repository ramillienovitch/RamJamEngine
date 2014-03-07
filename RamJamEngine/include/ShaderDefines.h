#ifndef SHADER_DEFINES_H
#define SHADER_DEFINES_H

#define SHADER_DEBUG 1

#define MSAA_SAMPLES 4
#define SHADOWAA_SAMPLES 4

#define MAX_LIGHTS_POWER 12
#define MAX_LIGHTS (1<<MAX_LIGHTS_POWER)


//////////////////////////////////////////////////////////////////////////
// -------   Tiled Deferred   -------
// This determines the tile size for light binning and associated tradeoffs
#define COMPUTE_SHADER_TILE_GROUP_DIM 16
#define COMPUTE_SHADER_TILE_GROUP_SIZE (COMPUTE_SHADER_TILE_GROUP_DIM*COMPUTE_SHADER_TILE_GROUP_DIM)

#define DEFER_PER_SAMPLE 1


//////////////////////////////////////////////////////////////////////////
// -------   Sample Distribution Shadow Mapping   -------
#define SHADOW_MAP_SIZE 1024
#define PARTITIONS 4

#define REDUCE_ZBOUNDS_BLOCK_DIM 16
#define REDUCE_ZBOUNDS_BLOCK_SIZE (REDUCE_ZBOUNDS_BLOCK_DIM*REDUCE_ZBOUNDS_BLOCK_DIM)

// TODO: This actually needs to be modified to ensure that it fits in local memory for any number of partitions!
// Needs to be based on max local memory size and max partition count.
// Currently set to 128 threads which works with up to 10 partitions (given 32KB local memory)
#define REDUCE_BOUNDS_BLOCK_X 16
#define REDUCE_BOUNDS_BLOCK_Y 8
#define REDUCE_BOUNDS_BLOCK_SIZE (REDUCE_BOUNDS_BLOCK_X*REDUCE_BOUNDS_BLOCK_Y)
#define REDUCE_BOUNDS_SHARED_MEMORY_ARRAY_SIZE (PARTITIONS * REDUCE_BOUNDS_BLOCK_SIZE)

#define SCATTER_BLOCK_DIM 32
#define SCATTER_BLOCK_SIZE (SCATTER_BLOCK_DIM*SCATTER_BLOCK_DIM)

#endif