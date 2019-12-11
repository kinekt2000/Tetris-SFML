#ifndef GAME_ELEMS_H
#define GAME_ELEMS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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


class Pit: public sf::Drawable
{
    friend class Block;
public:
    Pit(int cell_size, int outline_thick, unsigned int width, unsigned int height);
    ~Pit() override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void addBlock(const Block &block);
    int checkLines();
    bool checkOverflow();

    void changeLineSound(float volume);

    int projectionY(const Block &block);

    void clear();
    
private:
    ColorTable color_table;
    int **field;
    int cell_size;
    int thick;
    int width;
    int height;

    sf::SoundBuffer line_buffer;
    sf::Sound line;
};

#endif // GAME_ELEMS_H











