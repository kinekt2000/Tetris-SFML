#include <fstream>
#include <sstream>
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


Game_Over::Game_Over(int width, int height):
    width(width), height(height)
{   
    game_over_font.loadFromFile("tetris.ttf");
    additional_font.loadFromFile("whitestone.otf");

    max_frames = int(width/outline_thick/1.5);
}


void Game_Over::draw(sf::RenderTarget &target, sf::RenderStates) const{
    sf::FloatRect text_rect;

    sf::Text game_over_msg("GAME\nOVER", game_over_font, 80);
    text_rect = game_over_msg.getLocalBounds();
    game_over_msg.setPosition((width - text_rect.width)/2, height/4);
    game_over_msg.setFillColor(sf::Color::Red);

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
        target.draw(game_over_msg);
        target.draw(to_menu_msg);
        target.draw(score);
        target.draw(enter_name_label);
        target.draw(name_text);
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


int Game_Over::getFrame(){
    return frame;
}


bool Game_Over::finished(){
    if (frame == max_frames)
        return 1;
    return 0;
}


void Game_Over::addLetter(char l){
    if(name.size() < 6)
        name.push_back(l);
}


void Game_Over::delLetter(){
    if (name.size() > 0)
        name.pop_back();
}


std::string Game_Over::getName(){
    return name;
}


int Game_Over::getScore(){
    return final_score;
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

    showing_block = new Block(block.cell_size, block.outline_thick, 0, 0, block.type, block.fill_color);
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


Highscore::Highscore(int width, int height, const char* path):
    width(width), height(height)
{
    this->path = std::string(path);
    font.loadFromFile("whitestone.otf");
}

bool cmp(const std::pair<std::string, int> &a,
         const std::pair<std::string, int> &b){
    if(a.second == b.second) return a.first < b.first;
    return a.second > b.second;
}


void Highscore::draw(sf::RenderTarget &target, sf::RenderStates) const{
    target.clear();
    sf::Text board_label("Highscore", font, 45);
    board_label.setPosition((width-board_label.getLocalBounds().width)/2, 13);
    target.draw(board_label);

    sf::Text exit_msg("Press enter to exit menu", font, 20);
    exit_msg.setPosition((width-exit_msg.getLocalBounds().width)/2, height-65);
    target.draw(exit_msg);

    sf::Text name("", font, 30);
    sf::Text score("", font, 30);

    for(unsigned int i = 0; i < table.size() && i < 10; i++){
        auto item = table[i];
        name.setString(item.first);
        name.setPosition(55, 80+40*i);

        score.setString(std::to_string(item.second));
        score.setPosition(width-35-score.getLocalBounds().width, 80+40*i);

        target.draw(name);
        target.draw(score);
    }

    sf::Text number("", font, 30);
    sf::RectangleShape line(sf::Vector2f(width-25-35+2, 2));
    for(unsigned int i = 0; i < 10; i++){
        number.setString(std::to_string(i+1)+". ");
        number.setPosition(25, 80+40*i);

        line.setPosition(25, 113+40*i);
        target.draw(line);
        target.draw(number);
    }
}


void Highscore::addToTable(const std::string &name, int score){
    table.emplace_back(name, score);
    if(table.size()>10)
        table.pop_back();
    std::sort(table.begin(), table.end(), cmp);
}


void Highscore::save(){
    std::stringstream stream;
    std::string line;
    for(unsigned i = 0; i < token.size(); i++){
        stream << char(token[i] - (char(i)+1));
    }
    stream << "\n";
    for(auto item: table){
        line += item.first;
        line += std::string(10, '.');
        line += std::to_string(item.second);

        for(unsigned int i = 0; i < line.size(); i++){
            line[i] = char(line[i]-(char(i)+1));
        }

        stream << line << '\n';
        line = "";
    }

    std::ofstream outFile(path);
    if(outFile.is_open()){
        outFile << stream.str();
    }
}


void Highscore::open(){
    std::ifstream stream;
    stream.open(path);
    if (!stream.is_open()){
        opened = 0;
        return;
    }

    std::string raw;
    std::string name;
    std::string tmp_scr;
    int score;

    std::getline(stream, raw);
    for(unsigned int i = 0; i < raw.length(); i++){
        raw[i] += char(i)+1;
    }

    if(raw != token){
        stream.close();
        std::ofstream out(path, std::ofstream::out | std::ofstream::trunc);
        for(unsigned int i = 0; i < token.size(); i++){
            out << char(token[i] - (char(i)+1));
        }
        out << "\n";
        out.close();
        return;
    }

    bool success = 1;
    int i;
    for(i = 0; i < 10 && stream.good(); i++){
        std::getline(stream, raw);

        if(raw.empty()) continue;

        for(unsigned int i = 0; i < raw.size(); i++){
            raw[i] += char(i) + 1;
        }

        unsigned int j;
        for(j = 0; raw[j]!='.'; j++)
            name.push_back(raw[j]);
        for(auto c: name)
            if((c < 'A' || c > 'Z') && (c < 'a' || c > 'z')){
                success = 0;
                break;
            }
        if (success == 0) break;

        for(j; raw[j] == '.'; j++);
        tmp_scr.assign(raw.begin()+int(j), raw.end());
        for(auto c: tmp_scr)
            if(c < '0' || c > '9'){
                success = 0;
                break;
            }
        if (success == 0) break;

        score = std::stoi(tmp_scr);
        table.emplace_back(name, score);
        name.clear();
    }
    stream.close();
    if(success == 0){
        std::ofstream out(path, std::ofstream::out | std::ofstream::trunc);
        for(unsigned int i = 0; i < token.size(); i++){
            out << char(token[i] - (char(i)+1));
        }
        out << "\n";
        out.close();
        table.clear();
        return;
    }
    std::sort(table.begin(), table.end(), cmp);
}




