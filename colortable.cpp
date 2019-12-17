#include "colortable.h"

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
