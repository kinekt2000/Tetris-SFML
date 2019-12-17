#include "cell.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Cell::Cell(int cell_size, int outline_thick,
           int pos_x, int pos_y,
           sf::Color fillColor, sf::Color lineColor):
    thick(outline_thick), fillColor(fillColor), lineColor(lineColor), x(pos_x), y(pos_y)
{
    size = cell_size - thick;
}


void Cell::draw(sf::RenderTarget &target, sf::RenderStates) const{
    sf::RectangleShape shape(sf::Vector2f(size, size));
    shape.setFillColor(fillColor);
    shape.setOutlineThickness(thick);
    shape.setOutlineColor(lineColor);
    shape.setPosition(thick + x*(size+thick), thick + y*(size+thick));
    target.draw(shape);
}
