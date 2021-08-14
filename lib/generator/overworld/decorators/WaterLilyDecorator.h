#ifndef EXT_NOISELIB_LIB_GENERATOR_OVERWORLD_DECORATORS_WATERLILYDECORATOR_H_
#define EXT_NOISELIB_LIB_GENERATOR_OVERWORLD_DECORATORS_WATERLILYDECORATOR_H_

#include "Decorator.h"

class WaterLilyDecorator : public Decorator {
 public:
  void Decorate(ChunkManager &world, Random &random, int_fast32_t chunkX, int_fast32_t chunkZ) override;
};

#endif //EXT_NOISELIB_LIB_GENERATOR_OVERWORLD_DECORATORS_WATERLILYDECORATOR_H_
