#ifndef GAMEOVER_H
#define GAMEOVER_H


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>

class GameOver: public sf::Drawable
{
public:
    GameOver(int width, int height);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void nextFrame();
    void dropFrames();
    void finalScore(int final_score);

    void setFrame(int frame);
    int getFrame();
    bool finished();
    void addLetter(char l);
    void delLetter();

    std::string getName();
    int getScore();

private:
    std::string name = "";

    int width;
    int height;
    int outline_thick = 10;
    int max_frames;

    int final_score=0;

    sf::Font gameover_font;
    sf::Font additional_font;

    int frame = 0;
};


#endif // GAMEOVER_H
