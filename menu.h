#ifndef MENU_H
#define MENU_H

#include <cstdint>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "game_elements.h"

class Menu: public sf::Drawable
{
public:
    enum State{
        Main, Settings
    };

    Menu(int width, int height);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void nextOption();
    void prevOption();
    int getOption();
    void finishGame();

    void changeState(State state);
    State getState();
    void changeMusicVolume(int delta);
    void changeSoundsVolume(int delta);
    int getMusicVolume();
    int getSoundsVolume();

    void setDropPlaceState(bool state);
    bool getDropPlaceState();

private:
    State state = Main;
    int music_volume = 100;
    int sounds_volume = 100;
    bool show_drop_place = 1;

    bool game_not_started = 1;
    int height;
    int width;
    sf::Font font;

    int option = 0;
};


class Game_Over: public sf::Drawable
{
public:
    Game_Over(int width, int height);
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

    sf::Font game_over_font;
    sf::Font additional_font;

    int frame = 0;
};


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

#endif // MENU_H
