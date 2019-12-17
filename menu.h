#ifndef MENU_H
#define MENU_H


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>

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


#endif // MENU_H
