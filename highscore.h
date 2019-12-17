#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

class Highscore: public sf::Drawable
{
public:
    Highscore(int width, int height, const char* path);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void addToTable(const std::string &name, int score);
    void save();
    void open();

private:
    std::string token = "HIGHSCORETABLE";
    bool opened = 0;

    int width;
    int height;
    std::string path;

    sf::Font font;

    std::vector<std::pair<std::string, int>> table;
};


#endif // HIGHSCORE_H
