#include <bangtal>
#include<iostream>

using namespace std;
using namespace bangtal;


ObjectPtr board[8][8];

enum class State {

    BLANK,
    POSSIBLE,
    BLACK,
    WHITE
};


State board_state[8][8];

enum class Turn {

    BLACK,
    WHITE
};

Turn turn = Turn::BLACK;




bool checkPossible(int x, int y, int dx, int dy) {

    State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
    State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;


    bool possible = false;

    for (x += dx, y += dy; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy) {

        if (board_state[y][x] == other) {

            possible = true;
        }
        else if (board_state[y][x] == self) {

            return possible;
        }
        else {

            return false;
        }


    }

    return false;
}

void setState(int x, int y, State state) {

    switch (state) {
    case State::BLANK:board[y][x]->setImage("Images/blank.png"); break;
    case State::POSSIBLE:board[y][x]->setImage(turn == Turn::BLACK ? "Images/black possible.png" : "Images/white possible.png"); break;
    case State::BLACK:board[y][x]->setImage("Images/black.png"); break;
    case State::WHITE:board[y][x]->setImage("Images/white.png"); break;


    }

    board_state[y][x] = state;
}


void reverse(int x, int y, int dx, int dy) {
    State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
    State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;


    bool possible = false;

    for (x += dx, y += dy; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy) {

        if (board_state[y][x] == other) {

            possible = true;
        }
        else if (board_state[y][x] == self) {
            if (possible) {
                for (x -= dx, y -= dy; x >= 0 && x < 8 && y >= 0 && y < 8; x -= dx, y -= dy) {
                    if (board_state[y][x] == other) {
                        setState(x, y, self);
                    }
                    else {

                        return;

                    }

                }
            };
        }
        else {

            return;
        }


    }

}

void reverse(int x, int y) {

    int delta[8][2] = {
        {0,1},
        {1,1},
        {1,0},
        {1,-1},
        {0,-1},
        {-1,-1},
        {-1,0},
        {-1,1}

    };

    bool possible = false;

    for (auto d : delta) {
        reverse(x, y, d[0], d[1]);
    }

}


bool checkPossible(int x, int y) {

    if (board_state[y][x] == State::BLACK) return false;
    if (board_state[y][x] == State::WHITE) return false;

    setState(x, y, State::BLANK);
    int delta[8][2] = {
        {0,1},
        {1,1},
        {1,0},
        {1,-1},
        {0,-1},
        {-1,-1},
        {-1,0},
        {-1,1}

    };



    bool possible = false;

    for (auto d : delta) {
        if (checkPossible(x, y, d[0], d[1])) possible = true;
    }


    return possible;
}



bool setPossible() {


    bool possible = false;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {

            if (checkPossible(x, y)) {
                setState(x, y, State::POSSIBLE);

                possible = true;
            }


        }
    }
    return possible;
}


int b_score = 0;
int w_score = 0;

void wh_score() {

    int b = 0;
    int w = 0;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board_state[x][y] == State::BLACK) {
                b++;

            }
            else if (board_state[x][y] == State::WHITE) {
                w++;
            }
        }
    }

    b_score = b;
    w_score = w;



}

int main()
{

    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);


    auto scene = Scene::create("Odello", "Images/background.png");

    auto index_b = Object::create("Images/L0.png", scene, 830, 225);
    auto index_w = Object::create("Images/L0.png", scene, 1150, 225);//320
    auto after_indx_b = Object::create("Images/L0.png", scene, 750, 225);
    auto after_indx_w = Object::create("Images/L0.png", scene, 1070, 225);


    for (int y = 0; y < 8; y++) {

        for (int x = 0; x < 8; x++) {

            board[y][x] = Object::create("Images/blank.png", scene, 40 + x * 80, 40 + y * 80);
            board[y][x]->setOnMouseCallback([&, x, y](ObjectPtr, int, int, MouseAction)->bool {


                if (board_state[y][x] == State::POSSIBLE) {

                    if (turn == Turn::BLACK) {
                        setState(x, y, State::BLACK);
                        reverse(x, y);
                        wh_score();

                        string ab = "Images/L" + std::to_string(b_score / 10) + ".png";
                        string b = "Images/L" + std::to_string(b_score % 10) + ".png";
                        string aw = "Images/L" + std::to_string(w_score / 10) + ".png";
                        string w = "Images/L" + std::to_string(w_score % 10) + ".png";

                        after_indx_b->setImage(ab);
                        index_b->setImage(b);
                        after_indx_w->setImage(aw);
                        index_w->setImage(w);


                        turn = Turn::WHITE;
                    }
                    else {
                        setState(x, y, State::WHITE);
                        reverse(x, y);

                        wh_score();

                        string ab = "Images/L" + std::to_string(b_score / 10) + ".png";
                        string b = "Images/L" + std::to_string(b_score % 10) + ".png";
                        string aw = "Images/L" + std::to_string(w_score / 10) + ".png";
                        string w = "Images/L" + std::to_string(w_score % 10) + ".png";

                        after_indx_b->setImage(ab);
                        index_b->setImage(b);
                        after_indx_w->setImage(aw);
                        index_w->setImage(w);


                        turn = Turn::BLACK;
                    }
                    if (!setPossible()) {
                        turn = turn == Turn::BLACK ? Turn::WHITE : Turn::BLACK;
                        if (!setPossible()) {//검흰 둘다 안됌

                            showMessage("END GAME");

                        }

                    };


                }

                return true;


            });

            board_state[y][x] = State::BLANK;

        }

    }


    setState(3, 3, State::BLACK);
    setState(4, 4, State::BLACK);
    setState(3, 4, State::WHITE);
    setState(4, 3, State::WHITE);


    setPossible();


    startGame(scene);
}

