#ifndef __RESOURCE_IDENTIFIERS_HPP__
#define __RESOURCE_IDENTIFIERS_HPP__

#include <SFML/Graphics/Texture.hpp>

// Forward declaration of SFML classes
namespace sf
{
//	class Texture;
    class Font;
    class Shader;
    class SoundBuffer;
}

namespace Textures
{
    enum ID
    {
        BOARD,
        TRANKS,
        BULLETS,
        BUTTONS
    };
}

namespace Fonts
{
    enum ID
    {
        Sansation
    };
}

// Forward declaration and a few type definitions
template<typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif
