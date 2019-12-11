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
// Music & Sounds ++
// Volume regulator +
// Show phantom +
// Space function +
// Time loss +
// User name input +
// Highscore table +

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
    Highscore highscore(statistic_size + width*cell_size+cell_outline_thick, height*cell_size+cell_outline_thick, "highscore.bin");
    highscore.open();
    Game_Over game_over_screen(width*cell_size+cell_outline_thick, height*cell_size+cell_outline_thick);
    Game_State state = not_started;
    bool show_highscore = 0;

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

    Block projection = current_block;
    projection.setFillColor(sf::Color(180, 180, 180, 255));
    projection.setLineColor(sf::Color(45, 45, 45, 255));

    // Create a pit. A.k.a cells field which will collect tetrominos
    Pit pit(cell_size, cell_outline_thick, width, height);


    sf::Music game_music;
    sf::Music menu_music;
    game_music.openFromFile("game.ogg");
    menu_music.openFromFile("menu.ogg");
    game_music.setLoop(1);
    menu_music.setLoop(1);
    game_music.setVolume(20.f);
    menu_music.setVolume(20.f);

    sf::SoundBuffer fall_buffer;
    fall_buffer.loadFromFile("fall.wav");
    sf::Sound fall(fall_buffer);
    fall.setVolume(100.f);

    sf::SoundBuffer gameover_buffer;
    gameover_buffer.loadFromFile("gameover.wav");
    sf::Sound gameover(gameover_buffer);
    gameover.setVolume(100.f);

    menu_music.play();

    bool fast_falling = 0;
    float falling_time = 0.5f;
    int difficult_factor = 0;
    // Initialize clock for automaticaly actions
    sf::Clock clock;
    while(window.isOpen()){
        if(difficult_factor > 2){
            difficult_factor = 0;
            falling_time -= 0.005f;
            if(falling_time < 0.15f) falling_time = 0.15f;
        }

        // each 0.5 second move tetromino down
        static float down_timer = clock.getElapsedTime().asSeconds();
        if(state == play && clock.getElapsedTime().asSeconds() - down_timer >= falling_time){
            down_timer = clock.getElapsedTime().asSeconds();
            current_block.move(Direction::Down);

            // if block reached the bottom, creates new block
            if(current_block.checkCollision(pit)){
                difficult_factor++;
                current_block.move(Direction::Up);
                pit.addBlock(current_block);
                stats.countBlock(current_block.getType());
                current_block = next_block;
                next_block.reset(cell_size, cell_outline_thick, 3, -4,
                                 static_cast<Block::Type>(dist_block(generator)),
                                 color_table.getColor(dist_block(generator)));

                fall.play();
                stats.upScore(10);
                stats.showBlock(next_block);

                for(int i = 0; i < dist_rotation(generator); i++){
                    next_block.rotateRight();
                }
            }
        }

        static float fast_falling_timer = clock.getElapsedTime().asSeconds();
        if(state == play && fast_falling && clock.getElapsedTime().asSeconds() - fast_falling_timer > 0.0005f){
            fast_falling_timer = clock.getElapsedTime().asSeconds();
            current_block.move(Direction::Down);

            // if block reached the bottom, creates new block
            if(current_block.checkCollision(pit)){
                difficult_factor++;
                fast_falling = 0;
                current_block.move(Direction::Up);
                pit.addBlock(current_block);
                stats.countBlock(current_block.getType());
                current_block = next_block;
                next_block.reset(cell_size, cell_outline_thick, 3, -4,
                                 static_cast<Block::Type>(dist_block(generator)),
                                 color_table.getColor(dist_block(generator)));

                fall.play();
                stats.upScore(5);
                stats.showBlock(next_block);

                for(int i = 0; i < dist_rotation(generator); i++){
                    next_block.rotateRight();
                }
            }
        }

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed) window.close();

            // keyboard handler

            if(event.type == sf::Event::TextEntered){
                if(game_over_screen.finished()){
                    unsigned int letter_code = event.text.unicode;
                    if ((letter_code > 64 && letter_code < 91) ||
                        (letter_code > 96 && letter_code < 123)){
                        game_over_screen.addLetter(static_cast<char>(letter_code));
                    }
                    if (letter_code == 8){
                        game_over_screen.delLetter();
                    }
                }
            }

            if(event.type == sf::Event::KeyPressed){
                if(show_highscore){
                    if(event.key.code == sf::Keyboard::Enter)
                        show_highscore = 0;
                    continue;
                }

                if(state == play){
                    if(event.key.code == sf::Keyboard::Left && !fast_falling){
                        current_block.move(Direction::Left);
                        if(current_block.checkCollision(pit)){
                            current_block.move(Direction::Right);
                        }
                    } else if(event.key.code == sf::Keyboard::Right && !fast_falling){
                        current_block.move(Direction::Right);
                        if(current_block.checkCollision(pit)){
                            current_block.move(Direction::Left);
                        }
                    } else if(event.key.code == sf::Keyboard::Down && !fast_falling){
                        current_block.move(Direction::Down);
                        if(current_block.checkCollision(pit)){
                            difficult_factor++;
                            current_block.move(Direction::Up);
                            pit.addBlock(current_block);
                            stats.countBlock(current_block.getType());
                            current_block = next_block;
                            next_block.reset(cell_size, cell_outline_thick, 3, -4,
                                             static_cast<Block::Type>(dist_block(generator)),
                                             color_table.getColor(dist_block(generator)));

                            fall.play();
                            stats.upScore(5);
                            stats.showBlock(next_block);

                            for(int i = 0; i < dist_rotation(generator); i++){
                                next_block.rotateRight();
                            }
                        }
                    } else if(event.key.code == sf::Keyboard::Space){
                        fast_falling = 1;
                    }

                    if(event.key.code == sf::Keyboard::Up && !fast_falling){
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
                                show_highscore = 1;
                                break;

                            case 4:
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
                                menu.changeMusicVolume(-5);
                                menu_music.setVolume(menu.getMusicVolume()/5.f);
                                game_music.setVolume(menu.getMusicVolume()/5.f);
                            }
                            if(event.key.code == sf::Keyboard::Right){
                                menu.changeMusicVolume(+5);
                                menu_music.setVolume(menu.getMusicVolume()/5.f);
                                game_music.setVolume(menu.getMusicVolume()/5.f);
                            }
                            break;

                        case 1:
                            if(event.key.code == sf::Keyboard::Left){
                                menu.changeSoundsVolume(-5);
                                fall.setVolume(menu.getSoundsVolume());
                                gameover.setVolume(menu.getSoundsVolume());
                                pit.changeLineSound(menu.getSoundsVolume());
                            }
                            if(event.key.code == sf::Keyboard::Right){
                                menu.changeSoundsVolume(5);
                                fall.setVolume(menu.getSoundsVolume());
                                gameover.setVolume(menu.getSoundsVolume());
                                pit.changeLineSound(menu.getSoundsVolume());
                            }
                            break;

                        case 2:
                            if(event.key.code == sf::Keyboard::Enter){
                                menu.setDropPlaceState(!menu.getDropPlaceState());
                            }
                            break;

                        case 3:
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
                        menu.prevOption();
                        game_music.pause();
                        menu_music.play();
                        game_over_screen.dropFrames();
                        pit.clear();
                        stats.reset();
                        state = not_started;

                        show_highscore = 1;
                        highscore.addToTable(game_over_screen.getName(), game_over_screen.getScore());
                        highscore.save();
                    }
                }
            }
        }

        if(menu.getDropPlaceState()){
            projection = current_block;
            projection.setFillColor(sf::Color(180, 180, 180, 255));
            projection.setLineColor(sf::Color(45, 45, 45, 255));

            int proj_offset = pit.projectionY(current_block);
            projection.setY(proj_offset);
        }

        static float full_lines_timer = clock.getElapsedTime().asSeconds();
        if(clock.getElapsedTime().asSeconds() - full_lines_timer >= 0.1f){
            full_lines_timer = clock.getElapsedTime().asSeconds();
            stats.upScore(pit.checkLines()*10);
        }


        window.clear(sf::Color(255, 255, 255, 255));

        window.draw(pit);
        if(pit.checkOverflow()){
            state = game_over;
            game_music.stop();
            if(game_over_screen.getFrame() == 0) gameover.play();
        }

        if(menu.getDropPlaceState()) window.draw(projection);
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
        if(show_highscore)
            window.draw(highscore);
        window.display();
    }
}
