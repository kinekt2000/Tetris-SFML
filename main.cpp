// Drawing Cell +
// Drawing figure +
// Figure placement +
// Figure movement +
// Walls collisions +
// Draw pit +
// New block creating +
// Filling into the pit +
// Pit blocks collision +
// Randomly generating blocks +
// Autofalling and timer +
// Rotation +
// Lines removing +
// Overflowing check +
// Game pause +
// Game over +
// Menu +
// Statistics +
// Music +
// Volume regulator +

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <ctime>

#include "game_elements.h"
#include "menu.h"


const int cell_size = 30;
const int cell_outline_thick = 3;
const int width = 10;
const int height = 20;
const int statistic_size = 170;

enum Game_State{
    play, pause, not_started, game_over
};

int main()
{
    // Create game window
    sf::RenderWindow window(sf::VideoMode(width*cell_size+cell_outline_thick+statistic_size,
                                          height*cell_size+cell_outline_thick), "title");
    sf::Event event;
    window.setFramerateLimit(60);

    // Create random generator for tetrominos
    std::mt19937 generator;
    generator.seed(unsigned(std::time(nullptr)));
    std::uniform_int_distribution<int> dist_block(1, 7);
    std::uniform_int_distribution<int> dist_rotation(0, 3);

    Menu menu(statistic_size + width*cell_size+cell_outline_thick, height*cell_size+cell_outline_thick);
    Statistic stats(statistic_size, height*cell_size+cell_outline_thick, width*cell_size+cell_outline_thick);
    Game_Over game_over_screen(width*cell_size+cell_outline_thick, height*cell_size+cell_outline_thick);
    Game_State state = not_started;

    // Initialize color_table (dictionary of 9 colors)
    ColorTable color_table;

    // Create block, which will be next with random parameters
    Block next_block(cell_size, cell_outline_thick, 3, -4,
                     static_cast<Block::Type>(dist_block(generator)),
                     color_table.getColor(dist_block(generator)));
    for(int i = 0; i < dist_rotation(generator); i++){
        next_block.rotateRight();
    }
    stats.showBlock(next_block);

    // Create block, which will be shown on start of game with random parameters
    Block current_block(cell_size, cell_outline_thick, 3, -4,
                        static_cast<Block::Type>(dist_block(generator)),
                        color_table.getColor(dist_block(generator)));
    for(int i = 0; i < dist_rotation(generator); i++){
        current_block.rotateRight();
    }

    // Create a pit. A.k.a cells field which will collect tetrominos
    Pit pit(cell_size, cell_outline_thick, width, height);


    sf::Music game_music;
    sf::Music menu_music;
    game_music.openFromFile("game.ogg");
    menu_music.openFromFile("menu.ogg");
    game_music.setLoop(1);
    menu_music.setLoop(1);
    game_music.setVolume(100.f);
    menu_music.setVolume(100.f);

    menu_music.play();
    // Initialize clock for automaticaly actions
    sf::Clock clock;
    while(window.isOpen()){

        // each 0.5 second move tetromino down
        static float down_timer = clock.getElapsedTime().asSeconds();
        if(state == play && clock.getElapsedTime().asSeconds() - down_timer >= 0.5f){
            down_timer = clock.getElapsedTime().asSeconds();
            current_block.move(Direction::Down);

            // if block reached the bottom, creates new block
            if(current_block.checkCollision(pit)){
                current_block.move(Direction::Up);
                pit.addBlock(current_block);
                stats.countBlock(current_block.getType());
                current_block = next_block;
                next_block.reset(cell_size, cell_outline_thick, 3, -4,
                                 static_cast<Block::Type>(dist_block(generator)),
                                 color_table.getColor(dist_block(generator)));

                stats.upScore(10);
                stats.showBlock(next_block);

                for(int i = 0; i < dist_rotation(generator); i++){
                    next_block.rotateRight();
                }
            }
        }

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed) window.close();

            // keyboard handler

            if(event.type == sf::Event::KeyPressed){
                if(state == play){
                    if(event.key.code == sf::Keyboard::Left){
                        current_block.move(Direction::Left);
                        if(current_block.checkCollision(pit)){
                            current_block.move(Direction::Right);
                        }
                    } else if(event.key.code == sf::Keyboard::Right){
                        current_block.move(Direction::Right);
                        if(current_block.checkCollision(pit)){
                            current_block.move(Direction::Left);
                        }
                    } else if(event.key.code == sf::Keyboard::Down){
                        current_block.move(Direction::Down);
                        if(current_block.checkCollision(pit)){
                            current_block.move(Direction::Up);
                            pit.addBlock(current_block);
                            stats.countBlock(current_block.getType());
                            current_block = next_block;
                            next_block.reset(cell_size, cell_outline_thick, 3, -4,
                                             static_cast<Block::Type>(dist_block(generator)),
                                             color_table.getColor(dist_block(generator)));

                            stats.upScore(10);
                            stats.showBlock(next_block);

                            for(int i = 0; i < dist_rotation(generator); i++){
                                next_block.rotateRight();
                            }
                        }
                    }

                    if(event.key.code == sf::Keyboard::Up){
                        current_block.rotateLeft();
                        if(current_block.checkCollision(pit)){
                            current_block.rotateRight();
                        }
                    }

                    if(event.key.code == sf::Keyboard::Escape){
                        game_music.pause();
                        menu_music.play();
                        state = pause;
                    }
                }else if (state != game_over){
                    switch(menu.getState()){
                    case Menu::State::Main:{
                        if(event.key.code == sf::Keyboard::Escape){
                            if(state == pause){
                                menu_music.pause();
                                game_music.play();
                                state = play;
                            }
                        } else if (event.key.code == sf::Keyboard::Up){
                            menu.prevOption();
                        } else if (event.key.code == sf::Keyboard::Down){
                            menu.nextOption();
                        } else if (event.key.code == sf::Keyboard::Enter){
                            switch(menu.getOption()){
                            case 0:
                                menu_music.pause();
                                game_music.play();
                                pit.clear();
                                stats.reset();

                                current_block.reset(cell_size, cell_outline_thick, 3, -4,
                                                    static_cast<Block::Type>(dist_block(generator)),
                                                    color_table.getColor(dist_block(generator)));
                                for(int i = 0; i < dist_rotation(generator); i++){
                                    current_block.rotateRight();
                                }

                                next_block.reset(cell_size, cell_outline_thick, 3, -4,
                                                 static_cast<Block::Type>(dist_block(generator)),
                                                 color_table.getColor(dist_block(generator)));
                                for(int i = 0; i < dist_rotation(generator); i++){
                                    next_block.rotateRight();
                                }
                                state = play;
                                down_timer = clock.getElapsedTime().asSeconds();
                                break;

                            case 1:
                                menu_music.pause();
                                game_music.play();
                                down_timer = clock.getElapsedTime().asSeconds();
                                state = play;
                                break;

                            case 2:
                                menu.changeState(Menu::State::Settings);
                                break;

                            case 3:
                                window.close();
                                break;

                            default:
                                break;
                            }
                        }
                        break;
                    }
                    case Menu::State::Settings:{
                        if(event.key.code == sf::Keyboard::Escape){
                            menu.changeState(Menu::State::Main);
                        } else if (event.key.code == sf::Keyboard::Up){
                            menu.prevOption();
                        } else if (event.key.code == sf::Keyboard::Down){
                            menu.nextOption();
                        }

                        switch(menu.getOption()){
                        case 0:
                            if(event.key.code == sf::Keyboard::Left){
                                menu.changeVolume(-5);
                                menu_music.setVolume(menu.getVolume());
                                game_music.setVolume(menu.getVolume());
                            }
                            if(event.key.code == sf::Keyboard::Right){
                                menu_music.setVolume(menu.getVolume());
                                game_music.setVolume(menu.getVolume());
                                menu.changeVolume(+5);
                            }
                            break;
                        case 1:
                            if(event.key.code == sf::Keyboard::Enter){
                                menu.changeState(Menu::State::Main);
                            }
                            break;
                        }

                    }
                    }
                }else{
                    if(event.key.code == sf::Keyboard::Escape){
                        game_over_screen.setFrame(100);
                    }
                    if(game_over_screen.finished() && event.key.code == sf::Keyboard::Enter){
                        game_music.pause();
                        menu_music.play();
                        game_over_screen.dropFrames();
                        pit.clear();
                        stats.reset();
                        state = not_started;
                    }
                }
            }
        }

        static float full_lines_timer = clock.getElapsedTime().asSeconds();
        if(clock.getElapsedTime().asSeconds() - full_lines_timer >= 0.1f){
            full_lines_timer = clock.getElapsedTime().asSeconds();
            pit.checkLines();
        }


        window.clear(sf::Color(255, 255, 255, 255));

        window.draw(pit);
        if(pit.checkOverflow())state = game_over;
        window.draw(current_block);
        window.draw(stats);

        switch(state){
        case play:
                break;

        case pause:
        case not_started:
            window.draw(menu);
            break;

        case game_over:{
            game_over_screen.finalScore(stats.getScore());
            static float game_over_timer = clock.getElapsedTime().asSeconds();
            window.draw(game_over_screen);
            menu.finishGame();
            if(clock.getElapsedTime().asSeconds() - game_over_timer >= 0.07f){
                game_over_screen.nextFrame();
                game_over_timer = clock.getElapsedTime().asSeconds();
            }
        }
        }
        window.display();
    }
}
