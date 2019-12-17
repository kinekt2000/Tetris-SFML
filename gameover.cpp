#include "gameover.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

GameOver::GameOver(int width, int height):
    width(width), height(height)
{
    gameover_font.loadFromFile("tetris.ttf");
    additional_font.loadFromFile("whitestone.otf");

    max_frames = int(width/outline_thick/1.5);
}


void GameOver::draw(sf::RenderTarget &target, sf::RenderStates) const{
    sf::FloatRect text_rect;

    sf::Text gameover_msg("GAME\nOVER", gameover_font, 80);
    text_rect = gameover_msg.getLocalBounds();
    gameover_msg.setPosition((width - text_rect.width)/2, height/4);
    gameover_msg.setFillColor(sf::Color::Red);

    sf::Text to_menu_msg("Press Enter to exit the menu", additional_font, 20);
    text_rect = to_menu_msg.getLocalBounds();
    to_menu_msg.setPosition((width-text_rect.width)/2, height-40);
    to_menu_msg.setFillColor(sf::Color::White);

    sf::Text score("Your score: " + std::to_string(final_score), additional_font, 30);
    text_rect = score.getLocalBounds();
    score.setPosition((width-text_rect.width)/2, height-170);
    score.setFillColor(sf::Color::White);

    sf::Text enter_name_label("Enter your name:", additional_font, 20);
    text_rect = enter_name_label.getLocalBounds();
    enter_name_label.setPosition((width-text_rect.width)/2, height - 130);
    enter_name_label.setFillColor(sf::Color::White);

    sf::RectangleShape shape;
    if (frame < max_frames){
        int fill_sqr_width = width - 2*frame*outline_thick;
        int fill_sqr_height = height - 2*frame*outline_thick;

        if(fill_sqr_width < 0){
            fill_sqr_width = 0;
            fill_sqr_height = 0;
        }

        if(fill_sqr_width > 0 && fill_sqr_height > 0){
            shape.setSize(sf::Vector2f(fill_sqr_width, fill_sqr_height));
            shape.setPosition((width - fill_sqr_width)/2, (height - fill_sqr_height)/2);
            shape.setFillColor(sf::Color(0, 0, 0, 0));
            shape.setOutlineColor(sf::Color(0x20, 0x20, 0x20, 0xEE));
            shape.setOutlineThickness(frame*outline_thick);
        } else {
            shape.setSize(sf::Vector2f(width, height));
            shape.setFillColor(sf::Color(0x20, 0x20, 0x20, 0xEE));
        }
        target.draw(shape);
    } else {
        std::string tmp_name = name;
        for(unsigned int i = 0; i < 6 - name.size(); i++)
            tmp_name.push_back('_');

        sf::Text name_text(tmp_name, additional_font, 30);
        name_text.setFillColor(sf::Color::White);
        text_rect = name_text.getLocalBounds();
        name_text.setPosition((width - text_rect.width)/2, height - 110);

        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(0x20, 0x20, 0x20, 0xFF));
        shape.setPosition(0,0);
        target.draw(shape);
        target.draw(gameover_msg);
        target.draw(to_menu_msg);
        target.draw(score);
        target.draw(enter_name_label);
        target.draw(name_text);
    }
}


void GameOver::nextFrame(){
    frame++;
    if(frame > max_frames)
        frame = max_frames;
}


void GameOver::dropFrames(){
    frame = 0;
}


void GameOver::finalScore(int final_score){
    this->final_score = final_score;
}


void GameOver::setFrame(int frame){
    if (frame < 0)
        this->frame = 0;
    else if (frame > max_frames)
        this->frame = max_frames;
    else
        this->frame = frame;
}


int GameOver::getFrame(){
    return frame;
}


bool GameOver::finished(){
    if (frame == max_frames)
        return 1;
    return 0;
}


void GameOver::addLetter(char l){
    if(name.size() < 6)
        name.push_back(l);
}


void GameOver::delLetter(){
    if (name.size() > 0)
        name.pop_back();
}


std::string GameOver::getName(){
    return name;
}


int GameOver::getScore(){
    return final_score;
}
