#include <algorithm>
#include "game_elements.h"



int ColorTable::getColorID(const sf::Color &color) const{
    for(int i = 0; i < 9; i++)
        if(color == table[i])
            return i;
    return 0;
}


sf::Color ColorTable::getColor(int id) const{
    if(id > 0 && id < 9)
        return table[id];
    return table[0];
}


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


Block::Block(int cell_size, int outline_thick, int pos_x, int pos_y, Type type, sf::Color color):
    type(type), color(color), x(pos_x), y(pos_y), cell_size(cell_size), outline_thick(outline_thick)
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
                Cell cell(cell_size, outline_thick, x+j, y+i, color);
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


void Block::reset(int cell_size, int outline_thick, int pos_x, int pos_y, Type type, sf::Color color){
    this->type = type;
    this->cell_size = cell_size;
    this->outline_thick = outline_thick;
    this->x = pos_x;
    this->y = pos_y;
    this->color = color;
    
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


void Block::operator=(const Block &equal){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            figure[i][j] = equal.figure[i][j];
    
    type = equal.type;
    color = equal.color;
    x = equal.x;
    y = equal.y;
    cell_size = equal.cell_size;
    outline_thick = equal.outline_thick;
}


Pit::Pit(int cell_size, int outline_thick, unsigned int width, unsigned int height):
    cell_size(cell_size), thick(outline_thick), width(int(width)), height(int(height))
{
    field = new int*[height];
    for(size_t i = 0; i < height; i++){
        field[i] = new int[width];
        for(size_t j = 0; j < width; j++)
            field[i][j] = 0;
    }
}


Pit::~Pit(){
    for(int i = 0; i < height; i++)
        delete [] field[i];
    delete field;
}


void Pit::draw(sf::RenderTarget &target, sf::RenderStates) const{
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++){
            sf::Color fillColor(90, 90, 90, 255);
            sf::Color lineColor(0, 0, 0, 255);

            switch(field[i][j]){
            case 1:
                fillColor = color_table.getColor(1);
                break;
            case 2:
                fillColor = color_table.getColor(2);
                break;
            case 3:
                fillColor = color_table.getColor(3);
                break;
            case 4:
                fillColor = color_table.getColor(4);
                break;
            case 5:
                fillColor = color_table.getColor(5);
                break;
            case 6:
                fillColor = color_table.getColor(6);
                break;
            case 7:
                fillColor = color_table.getColor(7);
                break;
            case 8:
                fillColor = color_table.getColor(8);
                break;
            default:
                lineColor = sf::Color(45, 45, 45, 255);
                field[i][j] = 0;
                break;
            }

            Cell cell(cell_size, thick, j, i, fillColor, lineColor);
            target.draw(cell);
        }

}


void Pit::addBlock(const Block &block){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            if(block.figure[i][j] && i+block.y >= 0 && i+block.y < height && j+block.x >= 0 && j+block.x < width){
                field[i+block.y][j+block.x] = color_table.getColorID(block.color);
            }
        }
}


void Pit::checkLines(){
    for(int i = 0; i < height; i++){
        int full_line = 1;

        for(int j = 0; j < width; j++)
            if(field[i][j] == 0) full_line = 0;

        if(full_line){
            int line_marked = 1;
            for(int j = 0; j < width/2; j++){
                if(field[i][j] != 8){
                    line_marked = 0;
                    field[i][j] = 8;
                    field[i][width - 1 - j] = 8;
                    break;
                }
            }

            if(line_marked)
                for(int r = i; r > 0; r--)
                    for(int c = 0; c < width; c++)
                        field[r][c] = field[r-1][c];
        }
    }

}


bool Pit::checkOverflow(){
    bool overflowed = false;
    for(int i = 0; i < width && !overflowed; i++)
        if(field[0][i]) overflowed = true;
    return overflowed;
}


void Pit::clear(){
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            field[i][j] = 0;
}















