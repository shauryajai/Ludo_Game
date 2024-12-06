#ifndef LUDO_DEFINES
#define LUDO_DEFINES

#define sleep(x) this_thread::sleep_for(chrono::seconds(x))
#define sleep_ms(x) this_thread::sleep_for(chrono::milliseconds(x))
#define sleep_us(x) this_thread::sleep_for(chrono::microseconds(x))

#define COMMAND_QUEUE_MAX_SIZE 2

#define BOARD_LEN 750
#define BOARD_WID 750

#define NUM_OF_HOME_POSITIONS 4
#define NUM_OF_LAP_POSITIONS 52
#define NUM_OF_FINAL_POSITIONS 5
#define NUM_OF_GOTIS 4
#define NUM_OF_TOTAL_PLAYERS 4

#define NUM_OF_DICE_FACES 6
#define DICE_RANGE_START 1
#define DICE_RANGE_END 6

#define IMAGE_HOME_SCREEN           "..\\Images\\home_screen.png"
#define IMAGE_LUDO_BOARD            "..\\Images\\ludo_board.png"
#define IMAGE_HOME_BUTTON           "..\\Images\\home_button.png"
#define IMAGE_REMOVE_PLAYER_BUTTON  "..\\Images\\remove_player_button.png"
#define IMAGE_EXIT_GAME             "..\\Images\\exit_game.png"
#define IMAGE_ADD_PLAYER            "..\\Images\\add_player.png"
#define IMAGE_REMOVE_PLAYER_SCREEN  "..\\Images\\remove_player.png"
#define IMAGE_FINISH_SCREEN         "..\\Images\\finish_screen.png"
#define IMAGE_DICE_FACE_0           "..\\Images\\dice_face_0.png"
#define IMAGE_DICE_FACE_1           "..\\Images\\dice_face_1.png"
#define IMAGE_DICE_FACE_2           "..\\Images\\dice_face_2.png"
#define IMAGE_DICE_FACE_3           "..\\Images\\dice_face_3.png"
#define IMAGE_DICE_FACE_4           "..\\Images\\dice_face_4.png"
#define IMAGE_DICE_FACE_5           "..\\Images\\dice_face_5.png"
#define IMAGE_DICE_FACE_6           "..\\Images\\dice_face_6.png"

#define PLAY_BUTTON_RADIUS 75
#define PLAY_BUTTON_POS_X 294
#define PLAY_BUTTON_POS_Y 252

#define HOME_BUTTON_POS_X 40
#define HOME_BUTTON_POS_Y 15
#define HOME_BUTTON_LEN 56
#define HOME_BUTTON_WID 46

#define EXIT_SCREEN_POS_X 105
#define EXIT_SCREEN_POS_Y 200
//#define EXIT_SCREEN_LEN 540
//#define EXIT_SCREEN_WID 405

#define FINISH_TEXT_POS_X EXIT_SCREEN_POS_X + 67
#define FINISH_TEXT_POS_Y EXIT_SCREEN_POS_Y + 111

#define EXIT_SCREEN_YES_BUTTON_POS_X 182
#define EXIT_SCREEN_YES_BUTTON_POS_Y 395
#define EXIT_SCREEN_YES_BUTTON_LEN 168
#define EXIT_SCREEN_YES_BUTTON_WID 83

#define EXIT_SCREEN_NO_BUTTON_POS_X 383
#define EXIT_SCREEN_NO_BUTTON_POS_Y 395
#define EXIT_SCREEN_NO_BUTTON_LEN 168
#define EXIT_SCREEN_NO_BUTTON_WID 83

#define PLAYER_LEFT_BOTTOM_POS_X 94
#define PLAYER_LEFT_BOTTOM_POS_Y 520
#define PLAYER_LEFT_TOP_POS_X 94
#define PLAYER_LEFT_TOP_POS_Y 123
#define PLAYER_RIGHT_TOP_POS_X 493
#define PLAYER_RIGHT_TOP_POS_Y 123
#define PLAYER_RIGHT_BOTTOM_POS_X 493
#define PLAYER_RIGHT_BOTTOM_POS_Y 520

#define ADD_PLAYER_ICON_LEN 160
#define ADD_PLAYER_ICON_WID 160

#define REMOVE_PLAYER_ICON_RELATIVE_X 176
#define REMOVE_PLAYER_ICON_RELATIVE_Y -47
#define REMOVE_PLAYER_ICON_LEN 28
#define REMOVE_PLAYER_ICON_WID 28

#define REMOVE_PLAYER_BUTTON_RELATIVE_X 1
#define REMOVE_PLAYER_BUTTON_RELATIVE_Y 101
#define CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_X 85
#define CANCEL_REMOVE_PLAYER_BUTTON_RELATIVE_Y 101
#define REMOVE_PLAYER_BUTTON_LEN 75
#define REMOVE_PLAYER_BUTTON_WID 27

#define FINISH_SCREEN_RESTART_GAME_BUTTON_POS_X EXIT_SCREEN_POS_X + 42
#define FINISH_SCREEN_NEW_GAME_BUTTON_POS_X EXIT_SCREEN_POS_X + 201
#define FINISH_SCREEN_Quit_BUTTON_POS_X EXIT_SCREEN_POS_X + 362
#define FINISH_SCREEN_ALL_BUTTON_POS_Y EXIT_SCREEN_POS_Y + 202
#define FINISH_SCREEN_ALL_BUTTON_LEN 138
#define FINISH_SCREEN_ALL_BUTTON_WID 70

#define DICE_POS_X 345
#define DICE_POS_Y 371
#define DICE_LEN 60
#define DICE_WID 60

#define POSITION_ON_BOARD {LEFT_BOTTOM, LEFT_TOP, RIGHT_TOP, RIGHT_BOTTOM}
#define PLAYER_COLORS {BLUE, RED, GREEN, YELLOW}
#define GOTI_STATUS {SLEEPING, ACTIVE, LASTLEG, LIBERATED}
#define SCREEN {HOME, PLAY, EXIT, FINISH}
#define COMMAND { \
                  IDLE, \
                  START_GAME, \
                  EXIT_GAME, \
                  RESTART_GAME, \
                  NEW_GAME, \
                  GOTO_HOME, \
                  BACK_TO_GAME, \
                  FINISH_GAME, \
                  ADD_PLAYER_LEFT_BOTTOM, \
                  ADD_PLAYER_LEFT_TOP, \
                  ADD_PLAYER_RIGHT_TOP, \
                  ADD_PLAYER_RIGHT_BOTTOM, \
                  CONFIRM_PLAYER_REMOVAL_LEFT_BOTTOM, \
                  CONFIRM_PLAYER_REMOVAL_LEFT_TOP, \
                  CONFIRM_PLAYER_REMOVAL_RIGHT_TOP, \
                  CONFIRM_PLAYER_REMOVAL_RIGHT_BOTTOM, \
                  REMOVE_PLAYER_LEFT_BOTTOM, \
                  REMOVE_PLAYER_LEFT_TOP, \
                  REMOVE_PLAYER_RIGHT_TOP, \
                  REMOVE_PLAYER_RIGHT_BOTTOM, \
                  CLOSE_WINDOW, \
                  RESIZE_WINDOW, \
                  ROLL_DICE, \
                  MOVE_GOTI, \
                }

#define GOTI_MAP  {                                                                               \
                    { SLEEPING, {                                                                 \
                                  {0 ,{135,635}}, {1 ,{135,562}}, {2 ,{214,562}}, {3 ,{214,635}}, \
                                  {4 ,{135,237}}, {5 ,{135,165}}, {6 ,{214,165}}, {7 ,{214,237}}, \
                                  {8 ,{533,237}}, {9 ,{533,165}}, {10,{612,165}}, {11,{612,237}}, \
                                  {12,{533,635}}, {13,{533,562}}, {14,{612,562}}, {15,{612,635}}, \
                                }                                                                 \
                    },                                                                            \
                    { ACTIVE,   {                                                                 \
                                  {0 ,{330,667}}, {1 ,{330,623}}, {2 ,{330,579}}, {3 ,{330,535}}, \
                                  {4 ,{330,491}}, {5 ,{286,446}}, {6 ,{242,446}}, {7 ,{198,446}}, \
                                  {8 ,{154,446}}, {9 ,{110,446}}, {10,{65 ,446}}, {11,{65 ,402}}, \
                                  {12,{65 ,358}}, {13,{110,358}}, {14,{154,358}}, {15,{198,358}}, \
                                  {16,{242,358}}, {17,{286,358}}, {18,{330,312}}, {19,{330,268}}, \
                                  {20,{330,224}}, {21,{330,180}}, {22,{330,136}}, {23,{330,92 }}, \
                                  {24,{374,92 }}, {25,{418,92 }}, {26,{418,136}}, {27,{418,180}}, \
                                  {28,{418,224}}, {29,{418,268}}, {30,{418,312}}, {31,{463,358}}, \
                                  {32,{507,358}}, {33,{551,358}}, {34,{595,358}}, {35,{639,358}}, \
                                  {36,{683,358}}, {37,{683,402}}, {38,{683,446}}, {39,{639,446}}, \
                                  {40,{595,446}}, {41,{551,446}}, {42,{507,446}}, {43,{463,446}}, \
                                  {44,{418,491}}, {45,{418,535}}, {46,{418,579}}, {47,{418,623}}, \
                                  {48,{418,667}}, {49,{418,711}}, {50,{374,711}}, {51,{330,711}}, \
                                }                                                                 \
                    },                                                                            \
                    { LASTLEG, {                                                                  \
                                  {0 ,{374,667}}, {1 ,{374,623}}, {2 ,{374,579}}, {3 ,{374,535}}, \
                                  {4 ,{374,491}}, {5 ,{110,402}}, {6 ,{154,402}}, {7 ,{198,402}}, \
                                  {8 ,{242,402}}, {9 ,{286,402}}, {10,{374,136}}, {11,{374,180}}, \
                                  {12,{374,224}}, {13,{374,268}}, {14,{374,312}}, {15,{639,402}}, \
                                  {16,{595,402}}, {17,{551,402}}, {18,{507,402}}, {19,{463,402}}, \
                                }                                                                 \
                    },                                                                            \
                    { LIBERATED, {                                                                \
                                  {0 ,{363,454}}, {1 ,{385,454}}, {2 ,{341,454}}, {3 ,{407,454}}, \
                                  {4 ,{321,391}}, {5 ,{321,413}}, {6 ,{321,369}}, {7 ,{321,435}}, \
                                  {8 ,{385,348}}, {9 ,{363,348}}, {10,{407,348}}, {11,{341,348}}, \
                                  {12,{428,413}}, {13,{428,391}}, {14,{428,435}}, {15,{428,369}}, \
                                }                                                                 \
                    },                                                                            \
                  }                                                                               \

#define GOTI_BLK_MAX_RADIUS 22
#define GOTI_RADIUS GOTI_BLK_MAX_RADIUS/2
#define GOTI_OUTLINE_COLOR Color::Black
#define GOTI_OUTLINE_THICKNESS 2
#define GOTI_SHADOW_OFFSET 5,5
#define GOTI_SHADOW_COLOR Color(0, 0, 0, 100) // Semi-transparent black color

       
#define Command_q ludo_queue<pair<Command,uint32_t>>
#define Current_player board->players[board->current_player]
#define Goti_map map<Goti_status,map<int,sf::Vector2f>>

#endif