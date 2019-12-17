#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>

enum class Direction{
    Left = 0, Right = 1, Down = 2, Up
};

class Block: public sf::Drawable
{
    friend class Statistic;
    friend class Pit;
public:
    enum Type{ I=1, S=2, Z=3, L=4, J=5, T=6, O=7 };

    Block(int cell_size, int outline_thick, int pos_x, int pos_y, Type type, sf::Color fill_color, sf::Color line_color = sf::Color::Black);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void move(int x_off, int y_off);
    void move(Direction dir);
    bool checkCollision(const Pit &pit);
    void reset(int cell_size, int outline_thick, int pos_x, int pos_y, Type type, sf::Color fill_color, sf::Color line_color = sf::Color::Black);

    void rotateRight();
    void rotateLeft();

    Type getType();

    void setFillColor(const sf::Color &color);
    void setLineColor(const sf::Color &color);

    void setX(int x);
    void setY(int y);

    int getX();
    int getY();

    void operator=(const Block &equal);


private:
    Type type;
    int figure[4][4];
    sf::Color fill_color;
    sf::Color line_color;
    int x, y;
    int cell_size;
    int outline_thick;
};


#endif // BLOCK_H
