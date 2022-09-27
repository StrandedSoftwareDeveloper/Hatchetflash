/* Chunk.h file description:
* Defines the Chunk class, and states all of the functions that it will have as defined in "Chunk.cpp".
*/

#pragma once

#include <vector>

class Chunk
{
public:
	unsigned long int seed = 0;
	int chunkX = 0;
	int chunkY = 0;
	// Should contain an array of 4096 (16^3) vectors, which contain at least 0 blocks, with unsigned ints being used for information for each block.
	std::vector<unsigned int> blockStorage[4096];

	/* A constructor for chunk objects.
	* 
	* Seed specifies the seed the chunk is generated from. This is significant to have in the chunk as it allows for not all chunks to use the same seed.
	* chunkX and chunkY specify the chunk coordinates of the chunk for easier later use in generation and functions.
	*/
	Chunk(unsigned long int input_seed, int input_chunkX, int input_chunkY) {
		seed = input_seed;
		chunkX = input_chunkX;
		chunkY = input_chunkY;
	}

	void MakeChunkFilledWithTestBlocks();

	void UpdateChunkMesh();
};