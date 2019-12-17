#include "menu.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Menu::Menu(int width, int height):
    height(height), width(width)
{
    font.loadFromFile("tetris.ttf");
}


void Menu::draw(sf::RenderTarget &target, sf::RenderStates) const{
    sf::Text game_title("TETRIS", font, 90);
    game_title.setPosition((width-game_title.getLocalBounds().width)/2, height/5);
    game_title.setFillColor(sf::Color::Red);

    sf::Color option_color;

    target.clear();
    target.draw(game_title);
    switch(state){
    case State::Main:{
        sf::Text new_game_option("New game", font, 30);
        new_game_option.setPosition(50, height/2);

        if(option == 0) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        new_game_option.setFillColor(option_color);

        sf::Text resume_option("Resume", font, 30);
        resume_option.setPosition(50, height/2+50);

        if(option == 1) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else
            if (game_not_started) option_color = sf::Color(0xE0, 0xE0, 0xE0, 0x88);
            else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        resume_option.setFillColor(option_color);

        sf::Text settings_option("Settings", font, 30);
        settings_option.setPosition(50, height/2+100);

        if(option == 2) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        settings_option.setFillColor(option_color);

        sf::Text highscore_option("Highscores", font, 30);
        highscore_option.setPosition(50, height/2+150);

        if(option == 3) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        highscore_option.setFillColor(option_color);

        sf::Text quit_option("Quit game", font, 30);
        quit_option.setPosition(50, height/2+200);

        if(option == 4) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        quit_option.setFillColor(option_color);

        target.draw(new_game_option);
        target.draw(resume_option);
        target.draw(settings_option);
        target.draw(highscore_option);
        target.draw(quit_option);
        break;
    }
    case State::Settings:{
        sf::Text music_option("Music:", font, 30);
        music_option.setPosition(50, height/2);

        sf::RectangleShape filled_bar(sf::Vector2f(music_volume*2, 30));
        filled_bar.setPosition(50 + music_option.getLocalBounds().width + 44, height/2+5);
        sf::RectangleShape empty_bar(sf::Vector2f(200 - music_volume*2, 30));
        empty_bar.setPosition(filled_bar.getPosition().x+music_volume*2, height/2+5);

        if(option == 0) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        music_option.setFillColor(option_color);
        filled_bar.setFillColor(option_color);
        option_color.a = 0x80;
        empty_bar.setFillColor(option_color);
        target.draw(filled_bar);
        target.draw(empty_bar);

        sf::Text sounds_option("Sounds:", font, 30);
        sounds_option.setPosition(50, height/2+50);
        filled_bar.setSize(sf::Vector2f(sounds_volume*2, 30));
        filled_bar.setPosition(50 + sounds_option.getLocalBounds().width+20, height/2+50+5);
        empty_bar.setSize(sf::Vector2f(200 - sounds_volume*2, 30));
        empty_bar.setPosition(filled_bar.getPosition().x+sounds_volume*2, height/2+50+5);

        if(option == 1) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        sounds_option.setFillColor(option_color);
        filled_bar.setFillColor(option_color);
        option_color.a = 0x80;
        empty_bar.setFillColor(option_color);
        target.draw(filled_bar);
        target.draw(empty_bar);


        sf::Text drop_place_option("Show drop place", font, 30);
        drop_place_option.setPosition(50, height/2+100);

        if(option == 2) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        drop_place_option.setFillColor(option_color);
        sf::RectangleShape check_box(sf::Vector2f(24, 24));
        check_box.setOutlineThickness(3);
        check_box.setPosition(50+drop_place_option.getLocalBounds().width+23, height/2 + 100 + 7);
        check_box.setOutlineColor(option_color);
        check_box.setFillColor(sf::Color(0, 0, 0, 0));
        target.draw(check_box);
        if(show_drop_place){
            check_box.setSize(sf::Vector2f(16, 16));
            check_box.setOutlineThickness(0);
            check_box.setFillColor(option_color);
            check_box.setPosition(50+drop_place_option.getLocalBounds().width+23+4, height/2+100+7+4);
            target.draw(check_box);
        }


        sf::Text accept_option("OK", font, 30);
        accept_option.setPosition(50, height/2+150);

        if(option == 3) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        accept_option.setFillColor(option_color);

        target.draw(sounds_option);
        target.draw(music_option);
        target.draw(drop_place_option);
        target.draw(accept_option);
        break;
    }
    }

}


void Menu::nextOption(){
    switch(state){
    case State::Main:
        option++;
        if(game_not_started && option == 1) option = 2;
        if(option > 4) option = 0;
        break;
    case State::Settings:
        option++;
        if(option > 3) option = 0;
        break;
    }
}


void Menu::prevOption(){
    switch(state){
    case State::Main:
        option--;
        if(game_not_started && option == 1) option = 0;
        if(option < 0) option = 4;
        break;
    case State::Settings:
        option--;
        if(option < 0) option = 3;
        break;
    }
}


int Menu::getOption(){
    if(option == 0 && state == State::Main) game_not_started = 0;
    return option;
}


void Menu::finishGame(){
    game_not_started = 1;
}


void Menu::changeState(State state){
    if (state == State::Settings) option = 0;
    else option = 2;
    this->state = state;
}


Menu::State Menu::getState(){
    return state;
}


void Menu::changeMusicVolume(int delta){
    music_volume +=delta;
    if(music_volume > 100) music_volume = 100;
    if(music_volume < 0) music_volume = 0;
}


void Menu::changeSoundsVolume(int delta){
    sounds_volume += delta;
    if(sounds_volume > 100) sounds_volume = 100;
    if(sounds_volume < 0) sounds_volume = 0;
}


int Menu::getMusicVolume(){
    return music_volume;
}


int Menu::getSoundsVolume(){
    return sounds_volume;
}


void Menu::setDropPlaceState(bool state){
    show_drop_place = state;
}


bool Menu::getDropPlaceState(){
    return show_drop_place;
}

