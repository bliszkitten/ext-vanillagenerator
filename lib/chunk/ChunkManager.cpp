#include <lib/MortonHelper.h>
#include <lib/chunk/ChunkManager.h>

Chunk *ChunkManager::GetChunk(int64_t chunkX, int64_t chunkZ) {
  uint_fast64_t location = morton2d_encode(chunkX, chunkZ);

  auto searchResult = chunks.find(location);
  if (searchResult == chunks.end()) {
    return nullptr;
  }

  return searchResult->second;
}

void ChunkManager::SetChunk(int_fast64_t chunkX, int_fast64_t chunkZ, Chunk *chunk) {
  chunks.insert({morton2d_encode(chunkX, chunkZ), chunk});
}

MinecraftBlock ChunkManager::GetBlockAt(int_fast64_t x, int_fast16_t y, int_fast64_t z) {
  Chunk *chunk;
  if (IsInWorld(x, y, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return MinecraftBlock(chunk->GetFullBlock(static_cast<int_fast8_t>(x & 0xf), y, static_cast<int_fast8_t>(z & 0xf)));
  }

  return MinecraftBlock((Block) 0);
}

void ChunkManager::SetBlockAt(int_fast64_t x, int_fast16_t y, int_fast64_t z, MinecraftBlock block) {
  Chunk *chunk;

  if ((chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    chunk->SetFullBlock(static_cast<int_fast8_t>(x & 0xf), y, static_cast<int_fast8_t>(z & 0xf), block.getFullId());
  } else {
    throw std::invalid_argument("Cannot set block at coordinates x="
                                    + std::to_string(x) + ", y=" + std::to_string(y) + ", z=" + std::to_string(z)
                                    + ", terrain is not loaded or out of bounds");
  }
}

bool ChunkManager::IsInWorld(int_fast64_t x, int_fast16_t y, int_fast64_t z) const {
  return x <= INT32_MAX && x >= INT32_MIN && y < maxY && y >= minY && z <= INT32_MAX && z >= INT32_MIN;
}

int_fast16_t ChunkManager::GetMinY() const {
  return minY;
}

int_fast16_t ChunkManager::GetMaxY() const {
  return maxY;
}

ChunkManager::~ChunkManager() {
  for (auto data : chunks) {
    data.second->DestroyObjects();

    delete data.second;
  }

  chunks.clear();
}

std::map<uint_fast64_t, Chunk *> ChunkManager::GetChunks() const {
  return chunks;
}

MinecraftBlock ChunkManager::GetHighestBlockAt(int_fast64_t x, int_fast64_t z) {
  Chunk *chunk;
  if (IsInWorld(x, 0, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return MinecraftBlock(chunk->GetFullBlock(static_cast<int_fast8_t>(x & 0xf), chunk->GetHighestBlockAt(x & 0xf, z & 0xf), static_cast<int_fast8_t>(z & 0xf)));
  }

  return MinecraftBlock((Block) 0);
}

int_fast16_t ChunkManager::GetHighestElevationAt(int_fast64_t x, int_fast64_t z) {
  Chunk *chunk;
  if (IsInWorld(x, 0, z) && (chunk = GetChunk(x >> 4, z >> 4)) != nullptr) {
    return chunk->GetHighestBlockAt(x & 0xf, z & 0xf);
  }

  return 0;
}
