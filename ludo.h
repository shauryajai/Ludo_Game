#ifndef LUDO
#define LUDO

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // For getpid() and getuid()
#include <sys/types.h> // For getuid()
#include <chrono>  // For std::chrono
#include <thread>
#include <Windows.h>   // For GetTickCount()
#include <wincrypt.h> // For CryptoAPI functions
#include <time.h>
#include <vector>
#include "SFML/Graphics.hpp"
#include <ctime>
#include <cstdlib> // For system()
#include "ludo_queue.h"
#include <mutex>
#include <condition_variable>
#include <cmath>
#include "ludo_defines.h"
#include <sstream> 

using namespace std;

enum Player_color PLAYER_COLORS;
enum Position_on_board PLAYER_POSITION_ON_BOARD;
enum Goti_status GOTI_STATUS;
enum Screen SCREEN;
enum Command COMMAND;

std::ostream& operator<<(std::ostream& out, const Player_color value) {
    switch (value) {
        case BLUE: return out << "blue";
        case RED: return out << "red";
        case GREEN: return out << "green";
        case YELLOW: return out << "yellow";
    }

    return out << "invalid color";
}

std::ostream& operator<<(std::ostream& out, const Position_on_board value) {
    switch (value) {
        case LEFT_BOTTOM: return out << "LEFT_BOTTOM";
        case LEFT_TOP: return out << "LEFT_TOP";
        case RIGHT_TOP: return out << "RIGHT_TOP";
        case RIGHT_BOTTOM: return out << "RIGHT_BOTTOM";
    }

    return out << "invalid position";
}

std::ostream& operator<<(std::ostream& out, const Goti_status value) {
    switch (value) {
        case SLEEPING: return out << "sleeping";
        case ACTIVE: return out << "active";
        case LASTLEG: return out << "lastleg";
        case LIBERATED: return out << "liberated";
    }

    return out << "invalid goti_status";
}

std::ostream& operator<<(std::ostream& out, const Screen value) {
    switch (value) {
        case HOME: return out << "home";
        case PLAY: return out << "play";
        case EXIT: return out << "exit";
        case FINISH: return out << "finish";
    }

    return out << "invalid screen";
}

std::ostream& operator<<(std::ostream& out, const Command value) {
    switch (value) {
        case IDLE: return out << "idle";
        case START_GAME: return out << "start game";
        case EXIT_GAME: return out << "exit game";
        case RESTART_GAME: return out << "restart game";
        case NEW_GAME: return out << "new game";
        case GOTO_HOME: return out << "goto home";
        case BACK_TO_GAME: return out << "back to game";
        case FINISH_GAME: return out << "finish game";
        case ADD_PLAYER_LEFT_BOTTOM: return out << "add player "<< BLUE;
        case ADD_PLAYER_LEFT_TOP: return out << "add player " << RED;
        case ADD_PLAYER_RIGHT_TOP: return out << "add player " << GREEN;
        case ADD_PLAYER_RIGHT_BOTTOM: return out << "add player " << YELLOW;
        case CONFIRM_PLAYER_REMOVAL_LEFT_BOTTOM: return out << "confirm removal of player "<< BLUE;
        case CONFIRM_PLAYER_REMOVAL_LEFT_TOP: return out << "confirm removal of player " << RED;
        case CONFIRM_PLAYER_REMOVAL_RIGHT_TOP: return out << "confirm removal of player " << GREEN;
        case CONFIRM_PLAYER_REMOVAL_RIGHT_BOTTOM: return out << "confirm removal of player " << YELLOW;
        case REMOVE_PLAYER_LEFT_BOTTOM: return out << "remove player "<< BLUE;
        case REMOVE_PLAYER_LEFT_TOP: return out << "remove player " << RED;
        case REMOVE_PLAYER_RIGHT_TOP: return out << "remove player " << GREEN;
        case REMOVE_PLAYER_RIGHT_BOTTOM: return out << "remove player " << YELLOW;
        case CLOSE_WINDOW: return out << "close window";
        case RESIZE_WINDOW: return out << "resize window";
        case ROLL_DICE: return out << "roll dice";
    }

    return out << "invalid Command";
}

struct Goti {
  Goti_status status;
  int position;
  bool is_immortal;
  bool is_movable;
};

struct Player {
  Position_on_board id; // can be used as player_id
  Player_color color;
  sf::Vector2f pos_vector;
  bool is_active;
  int num_of_liberations;
  vector<Goti> gotis;
  bool confirm_removal;
  bool taking_turn;
};

struct Dice {
  int curr_value;
  bool rolled;
  pair<int, int> range_of_values;
  int num_of_faces;
  map<int,string> faces;
  sf::Vector2f pos_vector;
};

struct Board {
  int num_of_home_positions;
  int num_of_lap_positions;
  int num_of_final_positions;
  int active_players;
  int current_player;
  bool finish_game;
  vector<Player> players;
  Dice dice;
  Screen screen;
  float aspectratio;
};

#endif