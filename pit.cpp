#include "pit.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "block.h"
#include "cell.h"

Pit::Pit(int cell_size, int outline_thick, unsigned int width, unsigned int height):
    cell_size(cell_size), thick(outline_thick), width(int(width)), height(int(height))
{
    line_buffer.loadFromFile("line.wav");
    line.setBuffer(line_buffer);
    line.setVolume(100.f);

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
                field[i+block.y][j+block.x] = color_table.getColorID(block.fill_color);
            }
        }
}


int Pit::checkLines(){
    bool first_frame = 0;
    int count = 0;

    for(int i = 0; i < height; i++){
        int full_line = 1;

        for(int j = 0; j < width; j++)
            if(field[i][j] == 0) full_line = 0;

        if(full_line){
            int line_marked = 1;
            int j;
            for(j = 0; j < width/2; j++){
                if(field[i][j] != 8){
                    line_marked = 0;
                    field[i][j] = 8;
                    field[i][width - 1 - j] = 8;
                    break;
                }
            }
            if(j == 0) first_frame = 1;

            if(line_marked){
                for(int r = i; r > 0; r--)
                    for(int c = 0; c < width; c++)
                        field[r][c] = field[r-1][c];
                count++;
            }
        }
    }
    if(first_frame) line.play();
    return count;
}


bool Pit::checkOverflow(){
    bool overflowed = false;
    for(int i = 0; i < width && !overflowed; i++)
        if(field[0][i]) overflowed = true;
    return overflowed;
}


void Pit::changeLineSound(float volume){
    line.setVolume(volume);
}


int Pit::projectionY(const Block &block){
    int y = -1;

    bool figure[4][4] = {{0},{0},{0},{0}};
    for(int i = 0; i < 4; i++)
        for(int j = 0; j <4; j++)
            if(block.figure[i][j])
                figure[i][j] = 1;

    bool collided = 0;
    while(!collided){
        y++;
        for(int k = 0; k < 16; k++){
            int i = k/4;
            int j = k%4;

            if(figure[i][j]){
                if(i + y >= height || field[i+y][j+block.x]){
                    collided = 1;
                    y--;
                    break;
                }
            }
        }
    }
    return y;
}


void Pit::clear(){
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            field[i][j] = 0;
}
