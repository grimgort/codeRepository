#include "Asteroide.h"
#include <DataTables.h>
#include <Utility.h>

#include <CommandQueue.h>
#include <SoundNode.h>
#include <NetworkNode.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

using namespace std::placeholders;

//namespace
//{
//const std::vector<AsteroideData> Table = initializeAsteroideData();
//}

Asteroide::Asteroide (Type type, const TextureHolder& textures)
    : Projectile (type, textures)
{

}
