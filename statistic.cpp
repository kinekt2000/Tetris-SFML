#include "statistic.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include "colortable.h"

Statistic::Statistic(int width, int height, int x_offset):
    x_offset(x_offset), width(width), height(height)
{
    font.loadFromFile("whitestone.otf");
}


void Statistic::draw(sf::RenderTarget &target, sf::RenderStates) const{
    ColorTable color_table;

    sf::RectangleShape area(sf::Vector2f(width, height));
    area.setFillColor(sf::Color(45, 45, 45, 0xFF));
    area.setPosition(x_offset, 0);
    target.draw(area);

    sf::RectangleShape block(sf::Vector2f(140, 140));
    float left_field =  x_offset + (width - block.getLocalBounds().width)/2;
    block.setFillColor(sf::Color(90, 90, 90, 255));
    block.setPosition(left_field, 110);

    sf::Text score_label("Score:", font, 20);
    score_label.setPosition(left_field, 20);

    sf::Text score_line(std::to_string(score), font, 25);
    score_line.setPosition(left_field, 40);

    sf::Text next_block("Next block:", font, 20);
    next_block.setPosition(left_field, 85);

    target.draw(score_label);
    target.draw(score_line);
    target.draw(next_block);
    target.draw(block);
    if(showing_block) target.draw(*showing_block);

    const int tetromino_table[8][4] = {{0, 1, 2, 3},  // I
                                       {1, 2, 4, 5},  // S
                                       {0, 1, 5, 6},  // Z
                                       {0, 1, 2, 4},  // L
                                       {0, 4, 5, 6},  // J
                                       {1, 4, 5, 6},  // T
                                       {0, 1, 4, 5},  // O
                                       {8, 8, 8, 8}}; // STOPPER

    sf::Text block_count("", font, 20);
    sf::RectangleShape demo_cell(sf::Vector2f(16, 16));
    demo_cell.setOutlineThickness(2);
    demo_cell.setOutlineColor(sf::Color::Black);
    int cell_y_offset = 260;

    for(int i = 0; i < 8; i++){
        demo_cell.setFillColor(color_table.getColor(i+1));
        int j = 0;
        for(int y = 0; y < 2; y++)
            for(int x = 0; x < 4; x++)
                if(x + y*4 == tetromino_table[i][j]){
                    demo_cell.setPosition(left_field + 2 + x*18, cell_y_offset + y*18);
                    target.draw(demo_cell);
                    j += 1;
                }

        block_count.setString(std::to_string(blocks_count[i]));
        block_count.setPosition(left_field + 85, i ? cell_y_offset+8 : cell_y_offset - 2);
        target.draw(block_count);

        cell_y_offset+= 50;
        if(i == 0) cell_y_offset -= 20;
    }
}


void Statistic::upScore(int score){
    this->score += score;
}


void Statistic::countBlock(Block::Type type){
    blocks_count[static_cast<int>(type)-1]++;
}


void Statistic::showBlock(const Block &block){
    if(showing_block) delete showing_block;

    showing_block = new Block(block.cell_size, block.outline_thick, 0, 0, block.type, block.fill_color);
    showing_block->x = 11;
    showing_block->y = 4;
}


int Statistic::getScore(){
    return score;
}


void Statistic::reset(){
    score = 0;
    for(int i = 0; i < 7; i++)
        blocks_count[i] = 0;
}
