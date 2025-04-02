#include<iostream>
#include<fstream>
#include <string>
#include <ctime>
#include"clui.h"
using namespace std;

enum Level
{
    EASY, NORMAL, HARD
};

struct Game
{
    Level game_level;
    int const_sudoku[9][9];
    int play_sudoku[9][9];
    int complete_sudoku[9][9];
    bool is_saved = false;
    bool win = false;
    int remaining_time;
    int logical_wrongs = 0;
    int right = 0;
    int wrong = 0;
    int score = 0;
    int saved_game_id = 1000;
    int game_id;
    string game_name = "";
};

struct Player
{
    string username;
    Game game[20];
    int total_games = 0;
    int wins = 0;
    int loses = 0;
    int play_time = 0;
    int score = 0;
    int wrongs = 0;
    int rights = 0;
    int saved_games = 0;
};

Player players[10];
int count_players = 0;

void menu(Player &user);
void choose_level(Player &user);
void sudoku(Player &user, int);
void print_sudoku(int [9][9]);
void get_username();
void leader_board(Player &user);
void Save_informations(Player &user, int);
void show_saved_games(Player &user);
int** const_sudoku(Player);
int** completed_sudoku(Player);
bool is_valid_move(int sudoku[9][9], int row, int col, int num);
bool is_complete(int sudoku[9][9], int complete_sudoku[9][9]);
void calculate_score(Player &player);
void pause_game(time_t &start_time , int &remaining_time);
void end_game(Player &user, int game_id);
void sort_players(Player (&players)[10]);

void get_username()
{
    Player user;
    clear_screen();
    cursor_to_pos(get_window_rows()/2, (get_window_cols()-21)/2);
    change_color_rgb(252, 252, 3);
    cout<<"Enter your username: "<<endl;
    reset_color();
    cursor_to_pos(get_window_rows()/2, (get_window_cols()+21)/2);
    cin>>user.username;
    players[count_players-1].username = user.username;
    cursor_to_pos(get_window_rows()/2+1, (get_window_cols()-21)/2);
    ifstream infile("usernames.txt");
    bool exists = false;
    string existingUsername;
    if(infile.is_open())
    {
        while(infile >> existingUsername)
        {
            if(existingUsername == user.username)
            {
                exists = true;
                break;
            }
        }
        infile.close();
    }
    if(exists)
    {
        change_color_rgb(252, 40, 3);
        cout << "Error: Username already exists!" << endl;
        reset_color();
        cursor_to_pos(get_window_rows()/2+1, (get_window_cols()+40)/2);
        delay(3000);
        get_username();
    }
    else
    {
        ofstream outfile("usernames.txt", ios::app);
            outfile << user.username << endl;
            outfile.close();
    }
    count_players++;
    menu(user);
}

void menu(Player &user)
{
    int input_menu;
    clear_screen();
    change_color_rgb(3, 227, 252);
    cursor_to_pos(get_window_rows()/2-6, (get_window_cols()-19)/2);
    cout<<"In his sublime name";
    cursor_to_pos(get_window_rows()/2-5, get_window_cols()/2-10);
    cout<<"Let's play sudoku";
    cursor_to_pos(get_window_rows()/2-4, get_window_cols()/2-10);
    cout<<"<Game options>";
    cursor_to_pos(get_window_rows()/2-3, get_window_cols()/2-10);
    cout<<"You logged in as "<<user.username;
    reset_color();
    cursor_to_pos(get_window_rows()/2-1, get_window_cols()/2-10);
    cout<<"1.Start new game";
    cursor_to_pos(get_window_rows()/2, get_window_cols()/2-10);
    cout<<"2.Play a saved game";
    cursor_to_pos(get_window_rows()/2+1, get_window_cols()/2-10);
    cout<<"3.Leader board";
    cursor_to_pos(get_window_rows()/2+2, get_window_cols()/2-10);
    cout<<"4.Exit";
    cursor_to_pos(get_window_rows()/2+4, get_window_cols()/2-10);
    cout<<"Please enter a number from 1 to 4: ";
    cursor_to_pos(get_window_rows()/2+4, (get_window_cols()+50)/2);
    cin>>input_menu;
    if(input_menu == 1 || input_menu == 2 || input_menu == 3 || input_menu == 4)
    {
        switch(input_menu)
        {
            case 1: user.total_games++; choose_level(user); break;
            case 2: show_saved_games(user); break;;
            case 3: leader_board(user); break;
            case 4: get_username(); break;
        }
    }
    else
    {
        change_color_rgb(252, 40, 3);
        cursor_to_pos(get_window_rows()/2+6, get_window_cols()/2-10);
        cout<<"Please enter a correct number";
        delay(3000);
        menu(user);
    }
}

void choose_level(Player &user)
{
    clear_screen();
    int level;
    change_color_rgb(3, 227, 252);
    cursor_to_pos(get_window_rows()/2-4, (get_window_cols()-24)/2);
    cout<<"Select your game level: ";
    reset_color();
    cursor_to_pos(get_window_rows()/2-2, (get_window_cols()-24)/2);
    cout<<"1.Easy";
    cursor_to_pos(get_window_rows()/2, (get_window_cols()-24)/2);
    cout<<"2.Normal";
    cursor_to_pos(get_window_rows()/2+2, (get_window_cols()-24)/2);
    cout<<"3.Hard";
    cursor_to_pos(get_window_rows()/2+4, (get_window_cols()-24)/2);
    cout<<"Please enter a number from 1 to 3: ";
    cursor_to_pos(get_window_rows()/2+4, (get_window_cols()+46)/2);
    cin>>level;
    if(level == 1 || level == 2 || level == 3)
    {
        switch(level)
        {
            case 1: user.game[user.total_games-1].game_level = EASY; sudoku(user, user.total_games-1); break;
            case 2: user.game[user.total_games-1].game_level = NORMAL; sudoku(user, user.total_games-1); break;
            case 3: user.game[user.total_games-1].game_level = HARD; sudoku(user, user.total_games-1); break;
        }
    }
    else
    {
        change_color_rgb(252, 40, 3);
        cursor_to_pos(get_window_rows()/2+6, (get_window_cols()-24)/2);
        cout<<"Please enter a correct number";
        delay(3000);
        choose_level(user);
    }
}

int** const_sudoku(Player user)
{
    int** table = new int*[9];
    for(int i=0; i<9; i++)
    {
        table[i] = new int[9];
    }
    if(user.game[user.total_games-1].game_level == EASY)
    {
        ifstream infile("Easy.txt");
        if(infile.is_open())
        {
            for(int i=0; i<9; i++)
            {
                for(int j=0; j<9; j++)
                {
                    infile>>table[i][j];
                }
            }
            infile.close();
        }
    }
    else if(user.game[user.total_games-1].game_level == NORMAL)
    {
        ifstream infile("Normal.txt");
        if(infile.is_open())
        {
            for(int i=0; i<9; i++)
            {
                for(int j=0; j<9; j++)
                {
                    infile>>table[i][j];
                }
            }
            infile.close();
        }
    }
    else if(user.game[user.total_games-1].game_level == HARD)
    {
        ifstream infile("Hard.txt");
        if(infile.is_open())
        {
            for(int i=0; i<9; i++)
            {
                for(int j=0; j<9; j++)
                {
                    infile>>table[i][j];
                }
            }
            infile.close();
        }
    }
    return table;
}

int** completed_sudoku(Player user)
{
    int** table = new int*[9];
    for(int i=0; i<9; i++)
    {
        table[i] = new int[9];
    }
    if(user.game[user.total_games-1].game_level == EASY)
    {
        ifstream infile("Easy_answer.txt");
        if(infile.is_open())
        {
            for(int i=0; i<9; i++)
            {
                for(int j=0; j<9; j++)
                {
                    infile>>table[i][j];
                }
            }
            infile.close();
        }
    }
    else if(user.game[user.total_games-1].game_level == NORMAL)
    {
        ifstream infile("Normal_answer.txt");
        if(infile.is_open())
        {
            for(int i=0; i<9; i++)
            {
                for(int j=0; j<9; j++)
                {
                    infile>>table[i][j];
                }
            }
            infile.close();
        }
    }
    else if(user.game[user.total_games-1].game_level == HARD)
    {
        ifstream infile("Hard_answer.txt");
        if(infile.is_open())
        {
            for(int i=0; i<9; i++)
            {
                for(int j=0; j<9; j++)
                {
                    infile>>table[i][j];
                }
            }
            infile.close();
        }
    }
    return table;
}



void print_sudoku(int sudoku[9][9])
{
    int count = 0;
    cursor_to_pos(get_window_rows()/2-10, (get_window_cols()-25)/2);
    for(int i=0; i<13; i++)
    {
        cursor_to_pos(get_window_rows()/2-10+i, (get_window_cols()-25)/2);
        if(i==0 || i==4 || i==8 || i==12)
        {
            count++;
            for(int j=0; j<25; j++)
            {
                cout<<"*";
            }
        }
        else
        {
            cout<<"* ";
            for(int j=0; j<9; j++)
            {
                if(j==2 || j==5 || j==8)
                {
                    if(sudoku[i-count][j] == 0)
                        cout<<"  * ";
                    else
                        cout<<sudoku[i-count][j]<<" * ";
                }
                else
                {
                    if(sudoku[i-count][j] == 0)
                        cout<<"  ";
                    else
                        cout<<sudoku[i-count][j]<<" ";
                }
            }
        }
    }
}

void sudoku(Player &user, int game_id)
{
    clear_screen();
    if(user.game[game_id].is_saved == false)
    {
        int** temp_const_sudoku = const_sudoku(user);
        int** temp_complete_sudoku = completed_sudoku(user);
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                user.game[game_id].const_sudoku[i][j] = temp_const_sudoku[i][j];
                user.game[game_id].complete_sudoku[i][j] = temp_complete_sudoku[i][j];
                user.game[game_id].play_sudoku[i][j] = temp_const_sudoku[i][j];
            }
        }
        for(int i = 0; i < 9; i++)
        {
            delete[] temp_const_sudoku[i];
            delete[] temp_complete_sudoku[i];
        }
        delete[] temp_const_sudoku;
        delete[] temp_complete_sudoku;
        if(user.game[game_id].game_level == EASY)
            user.game[game_id].remaining_time = 300;
        else if(user.game[game_id].game_level == NORMAL)
            user.game[game_id].remaining_time = 180;
        else if(user.game[game_id].game_level == HARD)
            user.game[game_id].remaining_time = 120;
    }

    clear_screen();
    change_color_rgb(15, 252, 3);
    print_sudoku(user.game[game_id].play_sudoku);
    cursor_to_pos(get_window_rows()/2-9, (get_window_cols()-25)/2+1);
    int x = 0, y = 0, ROWS = get_window_rows()/2-10, COLS = (get_window_cols()-25)/2;
    time_t start_time = time(nullptr);
    time_t current_time;
    int elapsed_time = 0;

    while (user.game[game_id].remaining_time > 0)
    {
        current_time = time(nullptr);
        elapsed_time = (int) difftime (current_time, start_time);
        start_time = current_time;
        user.game[game_id].remaining_time -= elapsed_time;
        user.play_time += elapsed_time;

        cursor_to_pos(1 + y + (y / 3) + ROWS , 2 + (x * 2) + (2 * (x / 3)) + COLS);
        change_color_rgb(230, 55, 100);
        printf("%d", user.game[game_id].play_sudoku[y][x]);
        reset_color();
        cursor_backward(1);
        flush();
        if(is_keyboard_hit())
        {
            int key = getch();
            if (key == 224)
            {
                key = getch();
                switch (key)
                {
                    case 72:
                        if (y > 0)
                            y--;
                        break;
                    case 80:
                        if (y < 8)
                            y++;
                        break;
                    case 75:
                        if (x > 0)
                            x--;
                        break;
                    case 77:
                        if (x < 8)
                            x++;
                        break;
                }
            }
            else if(key >= '1' && key <= '9')
            {
                int num = key - '0';
                if (user.game[game_id].const_sudoku[y][x] == 0) {
                    if (is_valid_move(user.game[game_id].play_sudoku, y, x, num)) {
                        user.game[game_id].play_sudoku[y][x] = num;
                        if (user.game[game_id].play_sudoku[y][x] == user.game[game_id].complete_sudoku[y][x]) {
                            user.game[game_id].right++;
                        } else {
                            user.game[game_id].wrong++;
                            cursor_to_pos(get_window_rows()/2+8, (get_window_cols()-25)/2);
                            change_color_rgb(252, 40, 3);
                            cout<<"Wrong answer!"<<endl;
                            delay(2000);
                            cursor_to_pos(get_window_rows()/2+8, (get_window_cols()-25)/2);
                            cout<<"              "<<endl;
                        }
                    }
                    else
                    {
                        cursor_to_pos(get_window_rows()/2+8, (get_window_cols()-25)/2);
                        change_color_rgb(252, 40, 3);
                        user.game[game_id].logical_wrongs++;
                        cout<<"Invalid move! Try again."<<endl;
                        delay(2000);
                        cursor_to_pos(get_window_rows()/2+8, (get_window_cols()-25)/2);
                        cout<<"                         "<<endl;
                    }
                }
                else
                {
                    cursor_to_pos(get_window_rows()/2+8, (get_window_cols()-25)/2);
                    change_color_rgb(252, 40, 3);
                    cout<<"You can't change the constant sudoku"<<endl;
                    delay(2000);
                    cursor_to_pos(get_window_rows()/2+8, (get_window_cols()-25)/2);
                    cout<<"                                     "<<endl;
                }
            }
            else if (key == 27)
            {
                pause_game(start_time, user.game[game_id].remaining_time);
            }
            else if(key == 's')
            {
                Save_informations(user, game_id);
            }
            else if(key == 'q')
            {
                clear_screen();
                user.loses++;
                change_color_rgb(252, 40, 3);
                cursor_to_pos(get_window_rows()/2, get_window_cols()/2-12);
                cout<<"You are losed the game!";
                delay(2000);
                menu(user);
            }
            change_color_rgb(15, 252, 3);
            print_sudoku(user.game[game_id].play_sudoku);

        }

        if (is_complete(user.game[game_id].play_sudoku, user.game[game_id].complete_sudoku)) {
            clear_screen();
            cursor_to_pos(get_window_rows() * 4 / 10, get_window_cols() * 4 / 10);
            change_color_rgb(0, 255, 0);
            cout << "Wow Congratulations! You won the game!";
            user.wins++;
            user.game[game_id].win = true;
            reset_color();
            delay(3000);
            end_game(user, game_id);
        }
        string temp_time = (user.game[game_id].remaining_time % 60 > 9) ? to_string(user.game[game_id].remaining_time % 60) : "0" + to_string(user.game[game_id].remaining_time % 60);
        cursor_to_pos(get_window_rows()/2+11, (get_window_cols()-25)/2);
        cout << "Time Remaining: "<<"0"<<user.game[game_id].remaining_time/60<<":"<<temp_time<<endl;
        user.game[game_id].score = user.game[game_id].right - user.game[game_id].wrong;
        cursor_to_pos(get_window_rows()/2+12, (get_window_cols()-25)/2);
        cout<<"Score: ";
        printf("%3d", user.game[game_id].score);
    }

    if (user.game[game_id].remaining_time <= 0) {
        clear_screen();
        cursor_to_pos(get_window_rows()/2, get_window_cols()/2-15);
        cout << "Time is up! You lost the game.";
        user.loses++;
        user.game[game_id].win = false;
        delay(2000);
        end_game(user, game_id);
    }

    if(user.game[game_id].wrong >= 5)
    {
        clear_screen();
        cursor_to_pos(get_window_rows()/2, get_window_cols()/2-12);
        change_color_rgb(252, 40, 3);
        cout<<"You are losed the game!";
        user.game[game_id].win = false;
        delay(2000);
        end_game(user, game_id);
    }
    user.game[game_id].score = user.game[game_id].right - user.game[game_id].wrong;
    calculate_score(user);
    getch();
}

void pause_game(time_t &start_time, int &remaining_time)
{
    clear_screen();
    cursor_to_pos(get_window_rows()/2, get_window_cols()/2-24);
    change_color_rgb(252, 40, 3);
    cout<<"Game is paused. To continue press Esc key again: ";
    int key = getch();
    while(key != 27)
    {
        key = getch();
    }
    clear_screen();
    start_time = time(nullptr);
}

bool is_valid_move(int sudoku[9][9], int row, int col, int num) {
    for (int x = 0; x < 9; x++) {
        if (sudoku[row][x] == num || sudoku[x][col] == num) {
            return false;
        }
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    return true;
}

bool is_complete(int sudoku[9][9], int complete_sudoku[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != complete_sudoku[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void calculate_score(Player &user)
{
    for(int i=0; i<user.total_games; i++)
    {
        user.score = user.game[i].score;
    }
}


void Save_informations(Player &user, int game_id)
{
    user.wrongs += user.game[game_id].wrong;
    user.rights += user.game[game_id].right;
    players[count_players-1] = user;
    user.game[game_id].score = user.game[game_id].right - user.game[game_id].wrong;
    calculate_score(user);
    change_color_rgb(3, 227, 252);
    cursor_to_pos(get_window_rows()/2-4, 10);
    cout<<"remaining time: "<<user.game[game_id].remaining_time;
    cursor_to_pos(get_window_rows()/2-3, 10);
    cout<<"right: "<<user.game[game_id].right;
    cursor_to_pos(get_window_rows()/2-2, 10);
    cout<<"wrong: "<<user.game[game_id].wrong;
    cursor_to_pos(get_window_rows()/2-1, 10);
    cout<<"score: "<<user.game[game_id].score;
    cursor_to_pos(get_window_rows()/2, 10);
    cout<<"Enter your game name: ";
    reset_color();
    string game_name;
    cin>>game_name;
    bool result = true;
    while(result)
    {
        result = false;
        for(int i=0; i<=user.saved_games; i++)
        {
            if(game_name == user.game[i].game_name)
            {
                if(i==user.game[game_id].saved_game_id)
                {
                break;
                }
                else
                {
                    cursor_to_pos(get_window_rows()/2, 32);
                    cout<<"                              ";
                    cursor_to_pos(get_window_rows()/2+1, 10);
                    change_color_rgb(3, 227, 252);
                    cout<<"This name already exist!";
                    cursor_to_pos(get_window_rows()/2, 32);
                    cin>>game_name;
                    result = true;
                    break;
                }
            }
        }
    }
    user.game[game_id].game_name = game_name;
    if(user.game[game_id].is_saved == false)
    {
        user.game[game_id].is_saved = true;
        user.saved_games++;
        user.game[game_id].saved_game_id = user.saved_games-1;
    }
    cursor_to_pos(get_window_rows()/2+1, 10);
    change_color_rgb(3, 227, 252);
    cout<<"Game saved successfully!";
    delay(2000);
    menu(user);
}

void show_saved_games(Player &user)
{
    clear_screen();
    if(user.saved_games == 0)
    {
        change_color_rgb(252, 40, 3);
        cursor_to_pos(get_window_rows()/2, get_window_cols()/2-15);
        cout<<"You don't have any saved games!";
        reset_color();
        delay(3000);
        menu(user);
    }
    change_color_rgb(3, 227, 252);
    cursor_to_pos(3, 10);
    cout<<"Saved games:";
    reset_color();
    for(int i=0; i<user.saved_games; i++)
    {
        cursor_to_pos(5+i, 10);
        cout<<i+1<<". "<<user.game[i].game_name;
    }
    cursor_to_pos(5+user.saved_games, 10);
    cout<<user.saved_games+1<<". Back to menu";
    cursor_to_pos(6+user.saved_games, 10);
    cout<<"Please enter a number from 1 to "<<user.saved_games+1<<": ";
    int input;
    cin>>input;
    clear_screen();
    if(input == user.saved_games+1)
    {
        menu(user);
    }
    else if(input >= 1 && input <= user.saved_games)
    {
        int game_id;
        for(int i=0; i<user.saved_games; i++)
        {
            if(input-1 == user.game[i].saved_game_id)
            {
                game_id = i;
                sudoku(user, game_id);
                break;
            }
        }
    }
    else
    {
        change_color_rgb(252, 40, 3);
        cursor_to_pos(6+user.saved_games+2, 10);
        cout<<"Please enter a correct number";
        delay(3000);
        show_saved_games(user);
    }
}

void end_game(Player &user, int game_id)
{
    clear_screen();
    cursor_to_pos(get_window_rows()/2, get_window_cols()/2-12);
    reset_color();
    cout<<"Game results: ";
    if(user.game[game_id].win)
    {
        user.wins++;
        change_color_rgb(0, 255, 0);
        cursor_to_pos(get_window_rows()/2+2, get_window_cols()/2-12);
        cout<<"You won the game!";
        reset_color();
    }
    else
    {
        user.loses++;
        change_color_rgb(252, 40, 3);
        cursor_to_pos(get_window_rows()/2+2, get_window_cols()/2-12);
        cout<<"You lost the game!";
        reset_color();
    }
    cursor_to_pos(get_window_rows()/2+4, get_window_cols()/2-12);
    cout<<"Your score: "<<user.game[game_id].score;
    cursor_to_pos(get_window_rows()/2+6, get_window_cols()/2-12);
    cout<<"Correct answers: "<<user.game[game_id].right;
    cursor_to_pos(get_window_rows()/2+8, get_window_cols()/2-12);
    cout<<"Wrong answers: "<<user.game[game_id].wrong;
    cursor_to_pos(get_window_rows()/2+10, get_window_cols()/2-12);
    cout<<"remaining time: "<<user.game[game_id].remaining_time;
    cursor_to_pos(get_window_rows()/2+12, get_window_cols()/2-12);
    cout<<"Press m key to continue...";
    Save_informations(user, game_id);
    int key = getch();
    if(key == 'm')
    {
        menu(user);
    }
    else
    {
        end_game(user, game_id);
    }
}

void sort_players(Player (&players)[10])
{
    for(int i=0; i<count_players; i++)
    {
        for(int j=i+1; j<count_players; j++)
        {
            if(players[i].score < players[j].score)
            {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
            else if(players[i].score == players[j].score)
            {
                if(players[i].play_time > players[j].play_time)
                {
                    Player temp = players[i];
                    players[i] = players[j];
                    players[j] = temp;
                }
            }
        }
    }
}

void leader_board(Player &user)
{
    sort_players(players);
    clear_screen();
    change_color_rgb(3, 227, 252);
    cursor_to_pos(0, 10);
    cout<<"Leader board";
    reset_color();
    if(count_players == 0)
    {
        change_color_rgb(252, 40, 3);
        cursor_to_pos(2, 10);
        cout<<"There is no player!";
        reset_color();
        delay(3000);
        get_username();
    }
    else
    {
        for(int i=0; i<count_players; i++)
        {
            cursor_to_pos(2+((i)*7), 10);
            cout<<i+1<<". "<<players[i].username;
            cursor_to_pos(3+((i)*7), 10);
            cout<<"Score: "<<players[i].score;
            cursor_to_pos(4+((i)*7), 10);
            cout<<"Correct answer: "<<players[i].rights;
            cursor_to_pos(5+((i)*7), 10);
            cout<<"Wrong answer: "<<players[i].wrongs;
            cursor_to_pos(6+((i)*7), 10);
            cout<<"Wins: "<<players[i].wins;
            cursor_to_pos(7+((i)*7), 10);
            cout<<"Loses: "<<players[i].loses;
            cursor_to_pos(8+((i)*7), 10);
            cout<<"________________________________________________________";
        }
    }
    cursor_to_pos(3+(count_players*7), 10);
    cout<<"Press m key to continue...";
    int key = getch();
    if(key == 'm')
    {
        menu(user);
    }
    else
    {
        leader_board(user);
    }
}

int main()
{
    get_username();
    return 0;
}