#include <QDebug>
#include "menu.h"

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

        sf::Text quit_option("Quit game", font, 30);
        quit_option.setPosition(50, height/2+150);

        if(option == 3) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        quit_option.setFillColor(option_color);

        target.draw(new_game_option);
        target.draw(resume_option);
        target.draw(settings_option);
        target.draw(quit_option);
        break;
    }
    case State::Settings:{
        sf::Text volume_option("Volume:", font, 30);
        volume_option.setPosition(50, height/2);

        sf::RectangleShape filled_bar(sf::Vector2f(Volume*2, 30));
        filled_bar.setPosition(50 + volume_option.getLocalBounds().width + 20, height/2);
        sf::RectangleShape empty_bar(sf::Vector2f(200 - Volume*2, 30));
        empty_bar.setPosition(filled_bar.getPosition().x+Volume*2, height/2);

        if(option == 0) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        volume_option.setFillColor(option_color);
        filled_bar.setFillColor(option_color);
        option_color.a = 0x80;
        empty_bar.setFillColor(option_color);

        sf::Text accept_option("OK", font, 30);
        accept_option.setPosition(50, height/2+50);

        if(option == 1) option_color = sf::Color(0xFF, 0x80, 0x00, 0xFF);
        else option_color = sf::Color(0xE0, 0xE0, 0xE0, 0xFF);
        accept_option.setFillColor(option_color);

        target.draw(filled_bar);
        target.draw(empty_bar);
        target.draw(volume_option);
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
        if(option > 3) option = 0;
        break;
    case State::Settings:
        option++;
        if(option > 1) option = 0;
        break;
    }
}


void Menu::prevOption(){
    switch(state){
    case State::Main:
        option--;
        if(game_not_started && option == 1) option = 0;
        if(option < 0) option = 3;
        break;
    case State::Settings:
        option--;
        if(option < 0) option = 1;
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


void Menu::changeVolume(int delta){
    Volume +=delta;
    if(Volume > 100) Volume = 100;
    if(Volume < 0) Volume = 0;
}


int Menu::getVolume(){
    return Volume;
}


Game_Over::Game_Over(int width, int height):
    width(width), height(height)
{
    game_over_font.loadFromFile("tetris.ttf");
    to_menu_font.loadFromFile("whitestone.otf");

    max_frames = int(width/outline_thick/1.5);
}


void Game_Over::draw(sf::RenderTarget &target, sf::RenderStates) const{
    sf::FloatRect text_rect;

    sf::Text game_over_msg("GAME\nOVER", game_over_font, 80);
    text_rect = game_over_msg.getLocalBounds();
    game_over_msg.setPosition((width - text_rect.width)/2, height/4);
    game_over_msg.setFillColor(sf::Color::Red);

    sf::Text to_menu_msg("Press Enter to exit the menu", to_menu_font, 20);
    text_rect = to_menu_msg.getLocalBounds();
    to_menu_msg.setPosition((width-text_rect.width)/2, height-40);
    to_menu_msg.setFillColor(sf::Color::White);

    sf::Text score("Your score: " + std::to_string(final_score), to_menu_font, 30);
    text_rect = score.getLocalBounds();
    score.setPosition((width-text_rect.width)/2, height-75);
    score.setFillColor(sf::Color::White);

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
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(0x20, 0x20, 0x20, 0xFF));
        shape.setPosition(0,0);
        target.draw(shape);
        target.draw(game_over_msg);
        target.draw(to_menu_msg);
        target.draw(score);
    }
}


void Game_Over::nextFrame(){
    frame++;
    if(frame > max_frames)
        frame = max_frames;
}


void Game_Over::dropFrames(){
    frame = 0;
}


void Game_Over::finalScore(int final_score){
    this->final_score = final_score;
}


void Game_Over::setFrame(int frame){
    if (frame < 0)
        this->frame = 0;
    else if (frame > max_frames)
        this->frame = max_frames;
    else
        this->frame = frame;
}


bool Game_Over::finished(){
    if (frame == max_frames)
        return 1;
    return 0;
}


Statistic::Statistic(int width, int height, int x_offset):
    x_offset(x_offset), width(width), height(height)
{
    font.loadFromFile("whitestone.otf");
}


void Statistic::draw(sf::RenderTarget &target, sf::RenderStates) const{
    ColorTable color_table;

    sf::RectangleShape area(sf::Vector2f(width, height));
    area.setFillColor(sf::Color(45, 45, 45, 0xFF));
    area.setPosition(x_offset, 0);
    target.draw(area);

    sf::RectangleShape block(sf::Vector2f(140, 140));
    float left_field =  x_offset + (width - block.getLocalBounds().width)/2;
    block.setFillColor(sf::Color(90, 90, 90, 255));
    block.setPosition(left_field, 110);

    sf::Text score_label("Score:", font, 20);
    score_label.setPosition(left_field, 20);

    sf::Text score_line(std::to_string(score), font, 25);
    score_line.setPosition(left_field, 40);

    sf::Text next_block("Next block:", font, 20);
    next_block.setPosition(left_field, 85);

    target.draw(score_label);
    target.draw(score_line);
    target.draw(next_block);
    target.draw(block);
    if(showing_block) target.draw(*showing_block);

    const int tetromino_table[8][4] = {{0, 1, 2, 3},  // I
                                       {1, 2, 4, 5},  // S
                                       {0, 1, 5, 6},  // Z
                                       {0, 1, 2, 4},  // L
                                       {0, 4, 5, 6},  // J
                                       {1, 4, 5, 6},  // T
                                       {0, 1, 4, 5},  // O
                                       {8, 8, 8, 8}}; // STOPPER

    sf::Text block_count("", font, 20);
    sf::RectangleShape demo_cell(sf::Vector2f(16, 16));
    demo_cell.setOutlineThickness(2);
    demo_cell.setOutlineColor(sf::Color::Black);
    int cell_y_offset = 260;

    for(int i = 0; i < 8; i++){
        demo_cell.setFillColor(color_table.getColor(i+1));
        int j = 0;
        for(int y = 0; y < 2; y++)
            for(int x = 0; x < 4; x++)
                if(x + y*4 == tetromino_table[i][j]){
                    demo_cell.setPosition(left_field + 2 + x*18, cell_y_offset + y*18);
                    target.draw(demo_cell);
                    j += 1;
                }

        block_count.setString(std::to_string(blocks_count[i]));
        block_count.setPosition(left_field + 85, i ? cell_y_offset+8 : cell_y_offset - 2);
        target.draw(block_count);

        cell_y_offset+= 50;
        if(i == 0) cell_y_offset -= 20;
    }
}


void Statistic::upScore(int score){
    this->score += score;
}


void Statistic::countBlock(Block::Type type){
    blocks_count[static_cast<int>(type)-1]++;
}


void Statistic::showBlock(const Block &block){
    if(showing_block) delete showing_block;

    showing_block = new Block(block.cell_size, block.outline_thick, 0, 0, block.type, block.color);
    showing_block->x = 11;
    showing_block->y = 4;
}


int Statistic::getScore(){
    return score;
}


void Statistic::reset(){
    score = 0;
    for(int i = 0; i < 7; i++)
        blocks_count[i] = 0;
}







