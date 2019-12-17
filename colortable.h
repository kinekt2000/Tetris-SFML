#ifndef COLORTABLE_H
#define COLORTABLE_H

#include <SFML/Graphics/Color.hpp>

class ColorTable
{
public:
    int getColorID(const sf::Color &color) const;
    sf::Color getColor(int id) const;

private:
    sf::Color table[9] = {sf::Color(0x5A, 0x5A, 0x5A, 0xFF),
                          sf::Color(0xFF, 0x66, 0x66, 0xFF),
                          sf::Color(0xFF, 0xB2, 0x66, 0xFF),
                          sf::Color(0xFF, 0xFF, 0x66, 0xFF),
                          sf::Color(0x66, 0xFF, 0x66, 0xFF),
                          sf::Color(0x66, 0xFF, 0xFF, 0xFF),
                          sf::Color(0x66, 0x66, 0xFF, 0xFF),
                          sf::Color(0xFF, 0x66, 0xFF, 0xFF),
                          sf::Color(0xFF, 0xCC, 0xE5, 0xFF)};
};

#endif // COLORTABLE_H
