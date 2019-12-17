#ifndef PIT_H
#define PIT_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include "colortable.h"

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


#endif // PIT_H
