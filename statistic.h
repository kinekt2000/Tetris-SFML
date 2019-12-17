#ifndef STATISTIC_H
#define STATISTIC_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>

#include "block.h"

class Statistic: public sf::Drawable{
public:
    Statistic(int width, int height, int x_offset);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void upScore(int score);
    void countBlock(Block::Type type);
    void showBlock(const Block &block);
    int getScore();

    void reset();

private:
    int x_offset;

    int width;
    int height;
    sf::Font font;

    Block *showing_block = nullptr;
    int blocks_count[7] = {0};
    int score = 0;
};

#endif // STATISTIC_H
