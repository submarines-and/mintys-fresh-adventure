#include "tile.h"

glm::vec2 Tile::getOffset()
{
    return glm::vec2(((int)type % (int)atlas.size.x), (int)type / (int)atlas.size.y);
}