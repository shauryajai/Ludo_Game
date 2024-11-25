#include "ludo.h"

std::mutex queueMutex;
std::condition_variable queueCV;  // Used for thread synchronization

void seedRandom() {
    unsigned int seed = time(NULL);
    
    // Combine various sources of entropy
    seed ^= (GetCurrentProcessId() << 16); // Include process ID
    seed ^= (GetCurrentThreadId() << 24);  // Include thread ID
    seed ^= (clock() % 256);                 // Include current clock time

    // Use high-resolution timer for additional entropy
    seed ^= (unsigned int)GetTickCount();    // Milliseconds since the system started

    // Add randomness from system entropy pool
    HCRYPTPROV hProvider = 0;
    if (CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        BYTE randomValue[4];
        if (CryptGenRandom(hProvider, sizeof(randomValue), randomValue)) {
            seed ^= *(unsigned int *)randomValue; // Incorporate randomness from CryptGenRandom
        }
        CryptReleaseContext(hProvider, 0);
    }

    // Add a hash of the current executable path
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, sizeof(path)); // Get the executable path
    for (size_t i = 0; i < strlen(path); i++) {
        seed ^= path[i]; // XOR with the ASCII values of the path
    }

    srand(seed);
}

int random(int min, int max)
{
  return rand() % (max - min + 1) + min;
}

void init_all_gotis(vector<Goti> &gotis)
{
  Goti goti;
  int i;

  gotis.clear();

  for(i=0; i<NUM_OF_GOTIS; i++)
  {
    goti.is_immortal = false;
    goti.position = 0;
    goti.status = SLEEPING;

    gotis.push_back(goti);
  }
}

sf::Vector2f get_player_pos(Position_on_board player_pos)
{
  sf::Vector2f pos_vector;

  switch(player_pos)
  {
    case LEFT_BOTTOM:
    {
      pos_vector.x = PLAYER_LEFT_BOTTOM_POS_X;
      pos_vector.y = PLAYER_LEFT_BOTTOM_POS_Y;
      break;
    }
    case LEFT_TOP:
    {
      pos_vector.x = PLAYER_LEFT_TOP_POS_X;
      pos_vector.y = PLAYER_LEFT_TOP_POS_Y;
      break;
    }
    case RIGHT_TOP:
    {
      pos_vector.x = PLAYER_RIGHT_TOP_POS_X;
      pos_vector.y = PLAYER_RIGHT_TOP_POS_Y;
      break;
    }
    case RIGHT_BOTTOM:
    {
      pos_vector.x = PLAYER_RIGHT_BOTTOM_POS_X;
      pos_vector.y = PLAYER_RIGHT_BOTTOM_POS_Y;
      break;
    }
  }

  return pos_vector;
}

sf::Vector2f get_dice_pos()
{
  sf::Vector2f pos_vector;

  pos_vector.x = DICE_POS_X;
  pos_vector.y = DICE_POS_Y;

  return pos_vector;
}

void init_all_players(vector<Player> &players)
{
  Player player;
  int i;

  players.clear();

  for(i=0; i<NUM_OF_TOTAL_PLAYERS; i++)
  {
    player.id = (Position_on_board)i;
    player.color = (Player_color)player.id;
    player.pos_vector = get_player_pos(player.id);
    player.is_active = false;
    player.num_of_liberations = 0;
    player.confirm_removal = false;

    init_all_gotis(player.gotis);

    players.push_back(player);
  }
}

void init_dice(Dice &dice)
{
  dice.curr_value = 0;
  dice.rolled = false;
  dice.num_of_faces = NUM_OF_DICE_FACES;
  dice.range_of_values = {DICE_RANGE_START,DICE_RANGE_END};

  dice.faces.insert({{0,IMAGE_DICE_FACE_0},
                     {1,IMAGE_DICE_FACE_1},
                     {2,IMAGE_DICE_FACE_2},
                     {3,IMAGE_DICE_FACE_3},
                     {4,IMAGE_DICE_FACE_4},
                     {5,IMAGE_DICE_FACE_5},
                     {6,IMAGE_DICE_FACE_6}});

  dice.pos_vector = get_dice_pos();
}

void init_board(Board *board)
{
  board->num_of_home_positions = NUM_OF_HOME_POSITIONS;
  board->num_of_lap_positions = NUM_OF_LAP_POSITIONS;
  board->num_of_final_positions = NUM_OF_FINAL_POSITIONS;
  
  init_all_players(board->players);

  board->active_players = 0;
  board->current_player = -1;
  board->finish_game = false;

  init_dice(board->dice);

  board->screen = HOME;
  board->aspectratio = 0;
}

int next_active_player(Board *board, int pos)
{
  int current_pos = pos;

  do
  {
    current_pos = (current_pos + 1) % NUM_OF_TOTAL_PLAYERS;

    if(board->players[current_pos].is_active)
      return current_pos;

  } while (current_pos != pos);

  return -1;  
}

void activate_player(Board *board, Position_on_board pos)
{
  board->active_players ++;

  if(board->current_player < 0)
    board->current_player = pos;

  board->players[pos].is_active = true;
}

void remove_player(Board *board, Position_on_board pos)
{
  board->players[pos].is_active = false;
  board->players[pos].confirm_removal = false;
  board->players[pos].num_of_liberations = 0;

  for(Goti &goti : board->players[pos].gotis)
  {
    goti.is_immortal = false;
    goti.position = 0;
    goti.status = SLEEPING;
  }

  board->active_players --;

  if(board->active_players == 0)
    board->current_player = -1;
  else
  {
    board->current_player = next_active_player(board, pos);

    // Lone player wins
    if(board->active_players == 1)
    {
      Player player = board->players[board->current_player];

      board->finish_game = true;
      cout<<"Game over: player won - "<<player.color<<" ("<<player.id<<")"<<endl;
    }
  }
}

void set_viewsize(Board *board, sf::View &view)
{
  if (board->aspectratio > 1.0f)
    view.setSize(BOARD_LEN * board->aspectratio, BOARD_WID);
  else
    view.setSize(BOARD_LEN, BOARD_WID / board->aspectratio);
}

void drawBoard(sf::RenderWindow &window) 
{
  // Board size = 750*750
  using namespace sf;
  Texture texture;
  Sprite sprite;

  if (!texture.loadFromFile(IMAGE_LUDO_BOARD)) 
    return;
  
  sprite = Sprite(texture);
  window.draw(sprite);
}

void draw_home_button(sf::RenderWindow &window) 
{
  using namespace sf;
  Texture texture;
  Sprite sprite;

  if (!texture.loadFromFile(IMAGE_HOME_BUTTON))
    return;
  
  sprite = Sprite(texture);
  sprite.setPosition(HOME_BUTTON_POS_X,HOME_BUTTON_POS_Y);
  window.draw(sprite);
}

void draw_dice(sf::RenderWindow &window, Dice &dice) 
{
  using namespace sf;
  Texture texture;
  Sprite sprite;
  string file = dice.faces[dice.curr_value];

  if (!texture.loadFromFile(file)) 
    return;
  
  sprite = Sprite(texture);
  sprite.setPosition(dice.pos_vector);
  window.draw(sprite);
}

void draw_remove_player_screen(sf::RenderWindow &window, Player &player)
{
  using namespace sf;
  Texture texture;
  Sprite sprite;

  if (!texture.loadFromFile(IMAGE_REMOVE_PLAYER_SCREEN))
    return;
  
  sprite = Sprite(texture);
  sprite.setPosition(player.pos_vector);
  window.draw(sprite); 
}

void draw_add_remove_player_button(sf::RenderWindow &window, vector<Player> &players) 
{
  using namespace sf;
  Texture texture;
  Sprite sprite;
  sf::Vector2f pos;

  for(Player player : players)
  {
    if(player.is_active)
    {
      if (!texture.loadFromFile(IMAGE_REMOVE_PLAYER_BUTTON))
        return;
      
      sprite = Sprite(texture);

      pos.x = player.pos_vector.x + REMOVE_PLAYER_ICON_RELATIVE_X;
      pos.y = player.pos_vector.y + REMOVE_PLAYER_ICON_RELATIVE_Y;
      
      sprite.setPosition(pos);
      window.draw(sprite);

      if(player.confirm_removal)
        draw_remove_player_screen(window, player);
    }
    else
    {
      if (!texture.loadFromFile(IMAGE_ADD_PLAYER))
        return;
      
      sprite = Sprite(texture);
      sprite.setPosition(player.pos_vector);
      window.draw(sprite);
    }
  }
}

void drawHomeScreen(sf::RenderWindow &window) 
{
  using namespace sf;
  Texture texture;
  Sprite sprite;

  if (!texture.loadFromFile(IMAGE_HOME_SCREEN))
    return;
  
  sprite = Sprite(texture);
  window.draw(sprite);
}

void display_play_screen(sf::RenderWindow &window, Board *board)
{
  drawBoard(window);
  draw_home_button(window);
  // draw_player_gotis(window, board->players); // draw all gotis of all active players
  draw_dice(window, board->dice);
  draw_add_remove_player_button(window, board->players);
}

void draw_exit_screen(sf::RenderWindow &window) 
{
  using namespace sf;
  Texture texture;
  Sprite sprite;

  if (!texture.loadFromFile(IMAGE_EXIT_GAME))
    return;
  
  sprite = Sprite(texture);
  sprite.setPosition(EXIT_SCREEN_POS_X,EXIT_SCREEN_POS_Y);
  window.draw(sprite);
}

void display_finish_screen(sf::RenderWindow &window, Board *board) 
{
  using namespace sf;

  Texture texture;
  Sprite sprite;
  Text text;
  std::stringstream sstream;

  if (!texture.loadFromFile(IMAGE_FINISH_SCREEN))
    return;

  // text.setFont(font);
  sstream << "Game over!" << std::endl;
  text.setString(sstream.str());
  text.setCharacterSize(50);
  text.setFillColor(sf::Color::White);
  // text.setStyle(sf::Text::Bold | sf::Text::Underlined);
  text.setPosition(EXIT_SCREEN_POS_X + 69,EXIT_SCREEN_POS_Y + 111);

  sprite = Sprite(texture);
  sprite.setPosition(EXIT_SCREEN_POS_X,EXIT_SCREEN_POS_Y);
  //window.draw(sprite);
  window.draw(text);
}

void display(sf::RenderWindow &window, Board *board)
{
  window.clear();

  switch(board->screen)
  {
    case HOME:
    {
      drawHomeScreen(window);
      break;
    }
    case PLAY:
    {
      display_play_screen(window, board);
      break;
    }
    case EXIT:
    {
      display_play_screen(window, board);
      draw_exit_screen(window);
      break;
    }
    case FINISH:
    {
      display_play_screen(window, board);
      display_finish_screen(window, board);
      break;
    }
  }

  window.display();
}

bool is_play_button_clicked(sf::Vector2f mousePos)
{
  using namespace sf;
  
  CircleShape play_button(PLAY_BUTTON_RADIUS);
  Vector2f circleCenter;
  float dx, dy, distance;

  play_button.setPosition(PLAY_BUTTON_POS_X, PLAY_BUTTON_POS_Y);

  circleCenter = play_button.getPosition() + 
                      Vector2f(play_button.getRadius(), play_button.getRadius());
 
  dx = mousePos.x - circleCenter.x;
  dy = mousePos.y - circleCenter.y;
  distance = std::sqrt(dx * dx + dy * dy);

  // Check if the mouse click is inside the circle
  if (distance <= play_button.getRadius())
    return true;

  return false;
}

bool is_home_button_clicked(sf::Vector2f mousePos)
{
  if( mousePos.x >= HOME_BUTTON_POS_X && 
      mousePos.x <= HOME_BUTTON_POS_X + HOME_BUTTON_LEN &&
      mousePos.y >= HOME_BUTTON_POS_Y && 
      mousePos.y <= HOME_BUTTON_POS_Y + HOME_BUTTON_WID )
    return true;

  return false;
}

bool is_yes_button_clicked(sf::Vector2f mousePos)
{
  if( mousePos.x >= EXIT_SCREEN_YES_BUTTON_POS_X && 
      mousePos.x <= EXIT_SCREEN_YES_BUTTON_POS_X + EXIT_SCREEN_YES_BUTTON_LEN &&
      mousePos.y >= EXIT_SCREEN_YES_BUTTON_POS_Y && 
      mousePos.y <= EXIT_SCREEN_YES_BUTTON_POS_Y + EXIT_SCREEN_YES_BUTTON_WID )
    return true;

  return false;
}

bool is_no_button_clicked(sf::Vector2f mousePos)
{
  if( mousePos.x >= EXIT_SCREEN_NO_BUTTON_POS_X && 
      mousePos.x <= EXIT_SCREEN_NO_BUTTON_POS_X + EXIT_SCREEN_NO_BUTTON_LEN &&
      mousePos.y >= EXIT_SCREEN_NO_BUTTON_POS_Y && 
      mousePos.y <= EXIT_SCREEN_NO_BUTTON_POS_Y + EXIT_SCREEN_NO_BUTTON_WID )
    return true;

  return false;
}

bool is_add_remove_player_icon_clicked(Board *board, sf::Vector2f mousePos, Command &command)
{
  if( mousePos.x >= PLAYER_LEFT_BOTTOM_POS_X && 
      mousePos.x <= PLAYER_LEFT_BOTTOM_POS_X + ADD_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_LEFT_BOTTOM_POS_Y && 
      mousePos.y <= PLAYER_LEFT_BOTTOM_POS_Y + ADD_PLAYER_ICON_WID &&
      !board->players[LEFT_BOTTOM].is_active )
  {
    command = ADD_PLAYER_LEFT_BOTTOM;
    return true;
  }

  if( mousePos.x >= PLAYER_LEFT_TOP_POS_X && 
      mousePos.x <= PLAYER_LEFT_TOP_POS_X + ADD_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_LEFT_TOP_POS_Y && 
      mousePos.y <= PLAYER_LEFT_TOP_POS_Y + ADD_PLAYER_ICON_WID &&
      !board->players[LEFT_TOP].is_active )
  {
    command = ADD_PLAYER_LEFT_TOP;
    return true;
  }

  if( mousePos.x >= PLAYER_RIGHT_TOP_POS_X && 
      mousePos.x <= PLAYER_RIGHT_TOP_POS_X + ADD_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_RIGHT_TOP_POS_Y && 
      mousePos.y <= PLAYER_RIGHT_TOP_POS_Y + ADD_PLAYER_ICON_WID &&
      !board->players[RIGHT_TOP].is_active )
  {
    command = ADD_PLAYER_RIGHT_TOP;
    return true;
  }

  if( mousePos.x >= PLAYER_RIGHT_BOTTOM_POS_X && 
      mousePos.x <= PLAYER_RIGHT_BOTTOM_POS_X + ADD_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_RIGHT_BOTTOM_POS_Y && 
      mousePos.y <= PLAYER_RIGHT_BOTTOM_POS_Y + ADD_PLAYER_ICON_WID &&
      !board->players[RIGHT_BOTTOM].is_active )
  {
    command = ADD_PLAYER_RIGHT_BOTTOM;
    return true;
  }

  if( mousePos.x >= PLAYER_LEFT_BOTTOM_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X && 
      mousePos.x <= PLAYER_LEFT_BOTTOM_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X + REMOVE_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_LEFT_BOTTOM_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y && 
      mousePos.y <= PLAYER_LEFT_BOTTOM_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y + REMOVE_PLAYER_ICON_WID &&
      board->players[LEFT_BOTTOM].is_active &&
      !board->players[LEFT_BOTTOM].confirm_removal )
  {
    command = CONFIRM_PLAYER_REMOVAL_LEFT_BOTTOM;
    return true;
  }

  if( mousePos.x >= PLAYER_LEFT_TOP_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X && 
      mousePos.x <= PLAYER_LEFT_TOP_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X + REMOVE_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_LEFT_TOP_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y && 
      mousePos.y <= PLAYER_LEFT_TOP_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y + REMOVE_PLAYER_ICON_WID &&
      board->players[LEFT_TOP].is_active &&
      !board->players[LEFT_TOP].confirm_removal )
  {
    command = CONFIRM_PLAYER_REMOVAL_LEFT_TOP;
    return true;
  }

  if( mousePos.x >= PLAYER_RIGHT_TOP_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X && 
      mousePos.x <= PLAYER_RIGHT_TOP_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X + REMOVE_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_RIGHT_TOP_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y && 
      mousePos.y <= PLAYER_RIGHT_TOP_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y + REMOVE_PLAYER_ICON_WID &&
      board->players[RIGHT_TOP].is_active &&
      !board->players[RIGHT_TOP].confirm_removal )
  {
    command = CONFIRM_PLAYER_REMOVAL_RIGHT_TOP;
    return true;
  }

  if( mousePos.x >= PLAYER_RIGHT_BOTTOM_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X && 
      mousePos.x <= PLAYER_RIGHT_BOTTOM_POS_X + REMOVE_PLAYER_ICON_RELATIVE_X + REMOVE_PLAYER_ICON_LEN &&
      mousePos.y >= PLAYER_RIGHT_BOTTOM_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y && 
      mousePos.y <= PLAYER_RIGHT_BOTTOM_POS_Y + REMOVE_PLAYER_ICON_RELATIVE_Y + REMOVE_PLAYER_ICON_WID &&
      board->players[RIGHT_BOTTOM].is_active &&
      !board->players[RIGHT_BOTTOM].confirm_removal )
  {
    command = CONFIRM_PLAYER_REMOVAL_RIGHT_BOTTOM;
    return true;
  }

  return false;
}

bool is_remove_cancel_player_button_clicked(Board *board, sf::Vector2f mousePos, Command &command)
{
  if( mousePos.x >= PLAYER_LEFT_BOTTOM_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_LEFT_BOTTOM_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_LEFT_BOTTOM_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_LEFT_BOTTOM_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[LEFT_BOTTOM].confirm_removal )
  {
    command = REMOVE_PLAYER_LEFT_BOTTOM;
    return true;
  }
  if( mousePos.x >= PLAYER_LEFT_TOP_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_LEFT_TOP_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_LEFT_TOP_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_LEFT_TOP_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[LEFT_TOP].confirm_removal )
  {
    command = REMOVE_PLAYER_LEFT_TOP;
    return true;
  }
  if( mousePos.x >= PLAYER_RIGHT_TOP_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_RIGHT_TOP_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_RIGHT_TOP_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_RIGHT_TOP_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[RIGHT_TOP].confirm_removal )
  {
    command = REMOVE_PLAYER_RIGHT_TOP;
    return true;
  }
  if( mousePos.x >= PLAYER_RIGHT_BOTTOM_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_RIGHT_BOTTOM_POS_X + REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_RIGHT_BOTTOM_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_RIGHT_BOTTOM_POS_Y + REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[RIGHT_BOTTOM].confirm_removal )
  {
    command = REMOVE_PLAYER_RIGHT_BOTTOM;
    return true;
  }
  if( mousePos.x >= PLAYER_LEFT_BOTTOM_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_LEFT_BOTTOM_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_LEFT_BOTTOM_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_LEFT_BOTTOM_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[LEFT_BOTTOM].confirm_removal )
  {
    board->players[LEFT_BOTTOM].confirm_removal = false;
    return false;
  }
  if( mousePos.x >= PLAYER_LEFT_TOP_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_LEFT_TOP_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_LEFT_TOP_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_LEFT_TOP_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[LEFT_TOP].confirm_removal )
  {
    board->players[LEFT_TOP].confirm_removal = false;
    return false;
  }
  if( mousePos.x >= PLAYER_RIGHT_TOP_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_RIGHT_TOP_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_RIGHT_TOP_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_RIGHT_TOP_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[RIGHT_TOP].confirm_removal )
  {
    board->players[RIGHT_TOP].confirm_removal = false;
    return false;
  }
  if( mousePos.x >= PLAYER_RIGHT_BOTTOM_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X && 
      mousePos.x <= PLAYER_RIGHT_BOTTOM_POS_X + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X + REMOVE_PLAYER_BUTTON_LEN &&
      mousePos.y >= PLAYER_RIGHT_BOTTOM_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y && 
      mousePos.y <= PLAYER_RIGHT_BOTTOM_POS_Y + CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y + REMOVE_PLAYER_BUTTON_WID &&
      board->players[RIGHT_BOTTOM].confirm_removal )
  {
    board->players[RIGHT_BOTTOM].confirm_removal = false;
    return false;
  }

  return false;
}

bool is_restart_game_button_clicked(sf::Vector2f mousePos)
{
  if( mousePos.x >= FINISH_SCREEN_RESTART_GAME_BUTTON_POS_X && 
      mousePos.x <= FINISH_SCREEN_RESTART_GAME_BUTTON_POS_X + FINISH_SCREEN_ALL_BUTTON_LEN &&
      mousePos.y >= FINISH_SCREEN_ALL_BUTTON_POS_Y && 
      mousePos.y <= FINISH_SCREEN_ALL_BUTTON_POS_Y + FINISH_SCREEN_ALL_BUTTON_WID )
    return true;

  return false;
}

bool is_new_game_button_clicked(sf::Vector2f mousePos)
{
  if( mousePos.x >= FINISH_SCREEN_NEW_GAME_BUTTON_POS_X && 
      mousePos.x <= FINISH_SCREEN_NEW_GAME_BUTTON_POS_X + FINISH_SCREEN_ALL_BUTTON_LEN &&
      mousePos.y >= FINISH_SCREEN_ALL_BUTTON_POS_Y && 
      mousePos.y <= FINISH_SCREEN_ALL_BUTTON_POS_Y + FINISH_SCREEN_ALL_BUTTON_WID )
    return true;

  return false;
}

bool is_quit_button_clicked(sf::Vector2f mousePos)
{
  if( mousePos.x >= FINISH_SCREEN_Quit_BUTTON_POS_X && 
      mousePos.x <= FINISH_SCREEN_Quit_BUTTON_POS_X + FINISH_SCREEN_ALL_BUTTON_LEN &&
      mousePos.y >= FINISH_SCREEN_ALL_BUTTON_POS_Y && 
      mousePos.y <= FINISH_SCREEN_ALL_BUTTON_POS_Y + FINISH_SCREEN_ALL_BUTTON_WID )
    return true;

  return false;
}

bool is_dice_clicked(Board *board, sf::Vector2f mousePos)
{
  if(board->active_players < 2)
  { 
    cout<<"Minimum 2 players are required to play the game"<<endl;
    return false;
  }
  
  for(Player &player : board->players)
  {
    if(player.confirm_removal)
    {
      cout<<"Game paused while a player wants to quit"<<endl;
      return false;
    }
  }

  if( mousePos.x >= DICE_POS_X && 
      mousePos.x <= DICE_POS_X + DICE_LEN &&
      mousePos.y >= DICE_POS_Y && 
      mousePos.y <= DICE_POS_Y + DICE_WID )
    return true;

  return false;
}

void left_mouse_button_pressed(Board *board, sf::Vector2f mousePos, Command_q *command_q)
{
  Command cmd;

  if(board->screen == HOME)
  {
    if(is_play_button_clicked(mousePos))
      command_q->push({START_GAME,nullptr});
  }
  else if(board->screen == PLAY)
  {
    if(is_home_button_clicked(mousePos))
      command_q->push({EXIT_GAME,nullptr});
    else if(is_add_remove_player_icon_clicked(board, mousePos, cmd))
      command_q->push({cmd,nullptr});
    else if(is_remove_cancel_player_button_clicked(board, mousePos, cmd))
      command_q->push({cmd,nullptr});
    else if(is_dice_clicked(board, mousePos))
      command_q->push({ROLL_DICE,nullptr});
    else
      command_q->push({IDLE,nullptr});
  }
  else if(board->screen == EXIT)
  {
    if(is_yes_button_clicked(mousePos))
      command_q->push({GOTO_HOME,nullptr});
    else if(is_no_button_clicked(mousePos))
      command_q->push({BACK_TO_GAME,nullptr});
  }
  else if(board->screen == FINISH)
  {
    if(is_restart_game_button_clicked(mousePos))
      command_q->push({RESTART_GAME,nullptr});
    else if(is_new_game_button_clicked(mousePos))
      command_q->push({NEW_GAME,nullptr});
    else if(is_quit_button_clicked(mousePos))
      command_q->push({GOTO_HOME,nullptr});
  }
}

void get_command(Board *board, sf::RenderWindow &window, Command_q *command_q)
{
  using namespace sf;
  Event event;

  while (window.pollEvent(event)) {
    switch(event.type)
    {
      case Event::Closed:
      {
        command_q->push({CLOSE_WINDOW,nullptr});
        break;
      }
      case Event::Resized:
      {
        board->aspectratio = float(event.size.width) / float(event.size.height);
        command_q->push({RESIZE_WINDOW,nullptr});
        break;
      }
      case Event::KeyPressed:
      {
        if(event.key.code == Keyboard::Escape)
          command_q->push({CLOSE_WINDOW,nullptr});
        if(event.key.code == Keyboard::Space)
        {
          if(board->screen == PLAY && is_dice_clicked(board, {DICE_POS_X,DICE_POS_Y}))
            command_q->push({ROLL_DICE,nullptr});
        }
        break;
      }
      case Event::MouseButtonPressed:
      {
        if(event.mouseButton.button == Mouse::Left)
        {
          Vector2i mousePos = Mouse::getPosition(window);
          Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

          left_mouse_button_pressed(board, mouseWorldPos, command_q);
        }
        break;
      }
      default:
      {
        if(board->finish_game)
          command_q->push({FINISH_GAME,nullptr});
      }
    }
  }
}

void command_thread(Board *board, Command_q *command_q)
{
  while(true)
  {
    // TODO: update the board command queue if needed based on the command line user input
    // Developers can interrupt the game at any point from command line.

    sleep_ms(1);
  }
}

void roll_dice(Board *board, Command_q *command_q)
{
  static int count = 1;
  static int random_animation = 0;
  board->dice.curr_value = rand() % 6 + 1;

  if(random_animation == 0)
  {
      random_animation = rand() % 20 + 5;
  }

  printf("Dice: Value<%d> random_animation<%d> count<%d>\n",
          board->dice.curr_value, 
          random_animation, 
          count);

  if(count >= random_animation) // Roll complete
  {
    board->dice.rolled = true;
    count = 1;
    random_animation = 0;
  }
  else
  {
    command_q->push({ROLL_DICE,nullptr});
    count ++;
  }
}

void game_thread(Board *board, Command_q *command_q)
{
  Command command;
  void *data;
  sf::RenderWindow window(sf::VideoMode(BOARD_LEN, BOARD_WID), "Ludo Game");
  sf::View view(sf::FloatRect(0, 0, BOARD_LEN, BOARD_WID));

  // Enable vertical synchronization (V-Sync)
  window.setVerticalSyncEnabled(true);

  command_q->push({IDLE,nullptr});
  while (window.isOpen()) {
    get_command(board, window, command_q);

    if(!command_q->empty())
    {
        command = command_q->front().first;
        data = command_q->front().second;
        command_q->pop();
        cout<<command<<endl;
    }
    else command = IDLE;

    switch(command)
    {
      case IDLE:
      {
        break;
      }
      case START_GAME:
      {
        board->screen = PLAY;
        break;
      }
      case EXIT_GAME:
      {
        board->screen = EXIT;
        break;
      }
      case RESTART_GAME:
      {
        board->dice.curr_value = 0;
        board->finish_game = false;

        for(Player &player : board->players)
        {
          player.num_of_liberations = 0;
          init_all_gotis(player.gotis);
        }

        board->screen = PLAY;
        break;
      }
      case NEW_GAME:
      {
        init_board(board);
        board->screen = PLAY;
        break;
      }
      case GOTO_HOME:
      {
        init_board(board);
        break;
      }
      case BACK_TO_GAME:
      {
        board->screen = PLAY;
        break;
      }
      case FINISH_GAME:
      {
        board->finish_game = false;
        board->screen = FINISH;
        break;
      }
      case ADD_PLAYER_LEFT_BOTTOM:
      {
        activate_player(board, LEFT_BOTTOM);
        break;
      }
      case ADD_PLAYER_LEFT_TOP:
      {
        activate_player(board, LEFT_TOP);
        break;
      }
      case ADD_PLAYER_RIGHT_TOP:
      {
        activate_player(board, RIGHT_TOP);
        break;
      }
      case ADD_PLAYER_RIGHT_BOTTOM:
      {
        activate_player(board, RIGHT_BOTTOM);
        break;
      }
      case CONFIRM_PLAYER_REMOVAL_LEFT_BOTTOM:
      {
        board->players[LEFT_BOTTOM].confirm_removal = true;
        break;
      }
      case CONFIRM_PLAYER_REMOVAL_LEFT_TOP:
      {
        board->players[LEFT_TOP].confirm_removal = true;
        break;
      }
      case CONFIRM_PLAYER_REMOVAL_RIGHT_TOP:
      {
        board->players[RIGHT_TOP].confirm_removal = true;
        break;
      }
      case CONFIRM_PLAYER_REMOVAL_RIGHT_BOTTOM:
      {
        board->players[RIGHT_BOTTOM].confirm_removal = true;
        break;
      }
      case REMOVE_PLAYER_LEFT_BOTTOM:
      {
        remove_player(board, LEFT_BOTTOM);
        break;
      }
      case REMOVE_PLAYER_LEFT_TOP:
      {
        remove_player(board, LEFT_TOP);
        break;
      }
      case REMOVE_PLAYER_RIGHT_TOP:
      {
        remove_player(board, RIGHT_TOP);
        break;
      }
      case REMOVE_PLAYER_RIGHT_BOTTOM:
      {
        remove_player(board, RIGHT_BOTTOM);
        break;
      }
      case CLOSE_WINDOW:
      {
        window.close();
        break;
      }
      case RESIZE_WINDOW:
      {
        set_viewsize(board, view);
        window.setView(view);
        break;
      }
      case ROLL_DICE:
      {
          // Roll dice only if board->screen == PLAY;
          roll_dice(board, command_q);
          break;
      }
    }

    display(window, board);
    sleep_ms(1);
  }
}

int main() {
  Board *board = new Board;
  Command_q *command_q;
  command_q = new (Command_q)(COMMAND_QUEUE_MAX_SIZE);

  thread Command_thread(command_thread, board, command_q);
  thread Game_thread(game_thread, board, command_q);

  init_board(board);
  seedRandom();

  Command_thread.join();
  Game_thread.join();

  return 0;
}
