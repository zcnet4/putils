#include <iostream>
#include "concat.hpp"
#include "Sprite.hpp"

namespace pse {
    std::unordered_map<std::string, sf::Texture>    Sprite::textures;

    Sprite::Sprite(std::string_view texture, const sf::Vector2f & pos, const sf::Vector2f & size)
            :
            _textureFile(texture),
            _size(size) {
        Sprite::setTexture(texture);
        setPosition(pos);
        setSize(_size);
    }

    std::unique_ptr<ViewItem> Sprite::copy() const noexcept {
        return std::make_unique<Sprite>(_textureFile, _size, sf::Vector2f(0, 0));
    }

    void Sprite::setTexture(std::string_view texture) {
        if (textures.find(texture.data()) == textures.end()) {
            sf::Texture t;

            if (!t.loadFromFile(texture.data()))
                throw std::runtime_error(
                        putils::concat("Error loading texture '", texture, "'")
                );

            t.setSmooth(true);

            textures[texture.data()] = std::move(t);
        }

        _sprite.setTexture(textures[texture.data()]);
    }
}