#include <iostream>
#include "Text.hpp"
#include "concat.hpp"

namespace pse
{
    std::unordered_map<std::string, sf::Font *>    Text::fonts;

    Text::Text(const sf::String &str, const sf::Vector2f &pos, const sf::Color &color, unsigned int textSize,
               std::string_view font, const sf::Text::Style &style) noexcept
            :
            _fontFile(font)
    {
        setFont(font);
        setString(str);
        setColor(color);
        setTextSize(textSize);
        setPosition(pos);
        setStyle(style);
    }

    std::unique_ptr<ViewItem> Text::copy() const noexcept
    {
        return std::make_unique<Text>(_str, sf::Vector2f(0, 0), _color, _textSize, _fontFile, _style);
    }

    sf::Vector2f Text::getSize() const noexcept
    {
        sf::FloatRect rect = _text.getLocalBounds();
        return { rect.width, rect.height };
    }

    void Text::setString(const sf::String &str) noexcept
    {
        _str = str;
        _text.setString(_str);
    }

    void Text::setColor(const sf::Color &color) noexcept
    {
        _color = color;
        _text.setFillColor(color);
    }

    void Text::setTextSize(unsigned int textSize) noexcept
    {
        _textSize = textSize;
        _text.setCharacterSize(_textSize);
    }

    void Text::setFont(std::string_view font) noexcept
    {
        if (fonts.find(font.data()) == fonts.end())
        {
            sf::Font *f = new sf::Font();
            if (!(f->loadFromFile(font.data())))
            {
                std::cerr << putils::concat("Error loading font '", font.data(), "'") << std::endl;
                return;
            }
            fonts[font.data()] = f;
        }

        _font = *fonts[font.data()];
        _text.setFont(_font);
    }

    void Text::setStyle(const sf::Text::Style &style) noexcept
    {
        _style = style;
        _text.setStyle(_style);
    }
}
