#include "tile.h"

glm::vec2 Tile::getOffset(TileAtlas atlas)
{
    return glm::vec2(((int)type % (int)atlas.atlasSize.x), (int)type / (int)atlas.atlasSize.y);
}