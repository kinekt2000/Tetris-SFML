#include "highscore.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <fstream>
#include <sstream>


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
