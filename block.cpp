#include "block.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "cell.h"
#include "pit.h"

Block::Block(int cell_size, int outline_thick, int pos_x, int pos_y, Type type, sf::Color fill_color, sf::Color line_color):
    type(type), fill_color(fill_color), line_color(line_color), x(pos_x), y(pos_y), cell_size(cell_size), outline_thick(outline_thick)
{
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            figure[i][j] = 0;

    switch (type){
    case Type::I:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % ~ ~ ~
        figure[1][0] = 1; // 1 % ~ ~ ~
        figure[2][0] = 1; // 2 % ~ ~ ~
        figure[3][0] = 1; // 3 % ~ ~ ~
        break;

    case Type::S:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % ~ ~ ~
        figure[1][0] = 1; // 1 % % ~ ~
        figure[1][1] = 1; // 2 ~ % ~ ~
        figure[2][1] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::Z:
                          //   0 1 2 3
        figure[0][1] = 1; // 0 ~ % ~ ~
        figure[1][1] = 1; // 1 % % ~ ~
        figure[1][0] = 1; // 2 % ~ ~ ~
        figure[2][0] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::L:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % ~ ~ ~
        figure[1][0] = 1; // 1 % ~ ~ ~
        figure[2][0] = 1; // 2 % % ~ ~
        figure[2][1] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::J:
                          //   0 1 2 3
        figure[0][1] = 1; // 0 ~ % ~ ~
        figure[1][1] = 1; // 1 ~ % ~ ~
        figure[2][1] = 1; // 2 % % ~ ~
        figure[2][0] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::T:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 %  ~ ~
        figure[1][0] = 1; // 1 % % ~ ~
        figure[1][1] = 1; // 2 % ~ ~ ~
        figure[2][0] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::O:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % % ~ ~
        figure[0][1] = 1; // 1 % % ~ ~
        figure[1][0] = 1; // 2 ~ ~ ~ ~
        figure[1][1] = 1; // 3 ~ ~ ~ ~
        break;
    }
}


void Block::draw(sf::RenderTarget &target, sf::RenderStates) const{
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(figure[i][j]){
                Cell cell(cell_size, outline_thick, x+j, y+i, fill_color, line_color);
                target.draw(cell);
            }
        }
    }
}


void Block::move(int x_off, int y_off){
    x += x_off;
    y += y_off;
}

void Block::move(Direction dir){
    switch (dir) {
    case Direction::Left:
        x--;
        break;
    case Direction::Right:
        x++;
        break;
    case Direction::Down:
        y++;
        break;
    case Direction::Up:
        y--;
        break;
    }
}


bool Block::checkCollision(const Pit &pit){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(figure[i][j]){
                if(j+x<0 || j+x >= pit.width || i+y >= pit.height)
                    return true;
                if(i+y >= 0)
                    if(pit.field[i+y][j+x])
                        return true;
            }
    return false;
}


void Block::reset(int cell_size, int outline_thick, int pos_x, int pos_y, Type type, sf::Color fill_color, sf::Color line_color){
    this->type = type;
    this->cell_size = cell_size;
    this->outline_thick = outline_thick;
    this->x = pos_x;
    this->y = pos_y;
    this->fill_color = fill_color;
    this->line_color = line_color;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            figure[i][j] = 0;

    switch (type){
    case Type::I:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % ~ ~ ~
        figure[1][0] = 1; // 1 % ~ ~ ~
        figure[2][0] = 1; // 2 % ~ ~ ~
        figure[3][0] = 1; // 3 % ~ ~ ~
        break;

    case Type::S:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % ~ ~ ~
        figure[1][0] = 1; // 1 % % ~ ~
        figure[1][1] = 1; // 2 ~ % ~ ~
        figure[2][1] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::Z:
                          //   0 1 2 3
        figure[0][1] = 1; // 0 ~ % ~ ~
        figure[1][1] = 1; // 1 % % ~ ~
        figure[1][0] = 1; // 2 % ~ ~ ~
        figure[2][0] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::L:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % ~ ~ ~
        figure[1][0] = 1; // 1 % ~ ~ ~
        figure[2][0] = 1; // 2 % % ~ ~
        figure[2][1] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::J:
                          //   0 1 2 3
        figure[0][1] = 1; // 0 ~ % ~ ~
        figure[1][1] = 1; // 1 ~ % ~ ~
        figure[2][1] = 1; // 2 % % ~ ~
        figure[2][0] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::T:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 %  ~ ~
        figure[1][0] = 1; // 1 % % ~ ~
        figure[1][1] = 1; // 2 % ~ ~ ~
        figure[2][0] = 1; // 3 ~ ~ ~ ~
        break;

    case Type::O:
                          //   0 1 2 3
        figure[0][0] = 1; // 0 % % ~ ~
        figure[0][1] = 1; // 1 % % ~ ~
        figure[1][0] = 1; // 2 ~ ~ ~ ~
        figure[1][1] = 1; // 3 ~ ~ ~ ~
        break;
    }
}


void Block::rotateRight(){
    int minor_size = 0;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(figure[i][j])
                minor_size = std::max(std::max(i, j)+1, minor_size);

    int rotated[4][4] = {{0},{0},{0},{0}};
    for(int i = 0 ; i < minor_size; i++)
        for(int j = 0; j < minor_size; j++)
            if(figure[i][j])
                rotated[j][minor_size-1-i] = 1;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            figure[i][j]=rotated[i][j];
}


void Block::rotateLeft(){
    int minor_size = 0;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(figure[i][j])
                minor_size = std::max(std::max(i, j)+1, minor_size);

    int rotated[4][4] = {{0},{0},{0},{0}};
    for(int i = 0; i < minor_size; i++)
        for(int j = 0; j < minor_size; j++)
            if(figure[i][j])
                rotated[minor_size-1-j][i] = 1;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            figure[i][j] = rotated[i][j];
}


Block::Type Block::getType(){
    return type;
}


void Block::setFillColor(const sf::Color &color){
    fill_color = color;
}


void Block::setLineColor(const sf::Color &color){
    line_color = color;
}


void Block::setX(int x){
    this->x = x;
}


void Block::setY(int y){
    this->y = y;
}


int Block::getX(){
    return x;
}


int Block::getY(){
    return y;
}


void Block::operator=(const Block &equal){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            figure[i][j] = equal.figure[i][j];

    type = equal.type;
    fill_color = equal.fill_color;
    line_color = equal.line_color;
    x = equal.x;
    y = equal.y;
    cell_size = equal.cell_size;
    outline_thick = equal.outline_thick;
}
