#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>

class Cell: public sf::Drawable
{
public:
    Cell(int cell_size, int outline_thick,
         int pos_x, int pos_y,
         sf::Color fillColor, sf::Color lineColor = sf::Color::Black);
    void draw(sf::RenderTarget &target, sf::RenderStates) const override;

private:
    int size;
    int thick;
    sf::Color fillColor;
    sf::Color lineColor;
    int x;
    int y;
};

#endif // CELL_H
