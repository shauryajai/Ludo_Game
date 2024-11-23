#ifndef LUDO_DEFINES
#define LUDO_DEFINES

#define sleep(x) this_thread::sleep_for(chrono::seconds(x))
#define sleep_ms(x) this_thread::sleep_for(chrono::milliseconds(x))
#define sleep_us(x) this_thread::sleep_for(chrono::microseconds(x))

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

#define PLAYER_POSITION_ON_BOARD {LEFT_BOTTOM, LEFT_TOP, RIGHT_TOP, RIGHT_BOTTOM}
#define PLAYER_COLORS {BLUE, RED, GREEN, YELLOW}
#define GOTI_STATUS {SLEEPING, ACTIVE, LASTLEG, LIBERATED}
#define SCREEN {HOME, PLAY, EXIT, FINISH}
#define COMMAND {IDLE, \
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
                ROLL_DICE}

#endif