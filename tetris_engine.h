#pragma once

#include "shig_utility.h"


//using namespace std;
//using namespace shig;
typedef long long LL;
typedef pair<int, int> pairI2;
typedef pair<LL, LL> pairLL2;
typedef vector<int> VI;
typedef vector<LL> VLL;
typedef vector<VI> VVI;
typedef vector<VVI> VVVI;
typedef vector<string> VS;
#define shig_for(i, a, b) for(int i = (a); i < (b); ++i)
#define shig_rep(i, n) shig_for(i, 0, n)
#define shig_forB(bit,a,b) for(int bit = (a); bit < (1<<(b)); ++bit)
#define shig_repB(bit,n) shig_forB(bit,0,n)
#define CINI(a) int (a); cin >> (a)
#define CINLL(a) LL (a); cin >> (a)
#define CINI2(a, b) int a, b; cin >> (a) >> (b)
#define CINLL2(a, b) LL a, b; cin >> (a) >> (b)
#define tmp int temp = 0
#define str string s

const vector<vector<vector<int>>> minoI = {

            {
                { 0, 0, 0, 0 },{ 1, 1, 1, 1 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 }
            },
            {
                { 0, 0, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 1, 0 },{ 0, 0, 1, 0 }
            },
            {
                { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 1, 1, 1, 1 },{ 0, 0, 0, 0 }
            },
            {
                { 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 },{ 0, 1, 0, 0 }
            }

};

const vector<vector<vector<int>>> minoJ = {

            {
                { 2, 0, 0 },{ 2, 2, 2 },{ 0, 0, 0 }
            },
            {
                { 0, 2, 2 },{ 0, 2, 0 },{ 0, 2, 0 }
            },
            {
                { 0, 0, 0 },{ 2, 2, 2 },{ 0, 0, 2 }
            },
            {
                { 0, 2, 0 },{ 0, 2, 0 },{ 2, 2, 0 }
            }

};

const vector<vector<vector<int>>> minoL = {

            {
                { 0, 0, 3 },{ 3, 3, 3 },{ 0, 0, 0 }
            },
            {
                { 0, 3, 0 },{ 0, 3, 0 },{ 0, 3, 3 }
            },
            {
                { 0, 0, 0 },{ 3, 3, 3 },{ 3, 0, 0 }
            },
            {
                { 3, 3, 0 },{ 0, 3, 0 },{ 0, 3, 0 }
            }

};

const vector<vector<vector<int>>> minoO = {

            {
                { 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
            },
            {
                { 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
            },
            {
                { 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
            },
            {
                { 0, 4, 4, 0 },{ 0, 4, 4, 0 },{ 0, 0, 0, 0 }
            }

};

const vector<vector<vector<int>>> minoS = {

            {
                { 0, 5, 5 },{ 5, 5, 0 },{ 0, 0, 0 }
            },
            {
                { 0, 5, 0 },{ 0, 5, 5 },{ 0, 0, 5 }
            },
            {
                { 0, 0, 0 },{ 0, 5, 5 },{ 5, 5, 0 }
            },
            {
                { 5, 0, 0 },{ 5, 5, 0 },{ 0, 5, 0 }
            }

};

const vector<vector<vector<int>>> minoT = {

            {
                { 0, 6, 0 },{ 6, 6, 6 },{ 0, 0, 0 }
            },
            {
                { 0, 6, 0 },{ 0, 6, 6 },{ 0, 6, 0 }
            },
            {
                { 0, 0, 0 },{ 6, 6, 6 },{ 0, 6, 0 }
            },
            {
                { 0, 6, 0 },{ 6, 6, 0 },{ 0, 6, 0 }
            }

};

const vector<vector<vector<int>>> minoZ = {

            {
                { 7, 7, 0 },{ 0, 7, 7 },{ 0, 0, 0 }
            },
            {
                { 0, 0, 7 },{ 0, 7, 7 },{ 0, 7, 0 }
            },
            {
                { 0, 0, 0 },{ 7, 7, 0 },{ 0, 7, 7 }
            },
            {
                { 0, 7, 0 },{ 7, 7, 0 },{ 7, 0, 0 }
            }

};


struct Tetri {
    int rot = 0;
    int X = 0, Y = 0;
    int id = 0;
    int px_size = 30;
    
    Tetri() {
        minset(6);
    };
    Tetri(int rot, int X, int Y, int id) : rot(rot), X(X), Y(Y), id(id) {};

    VVVI mino;

    void set(int a, int b, int c, int d) {
        rot = a; X = b; Y = c; id = d;
        return;
    }

    void addX(int add) {
        X += add;
        return;
    }

    void addY(int add) {
        Y += add;
        return;
    }

    void set_rot(int to) {
        rot = to;
        return;
    }

    void set_id(int d) {
        id = d;
        return;
    }

    string get_mino_str() {
        string gms;
        gms = to_string(rot) + " " + to_string(X) + " " + to_string(Y) + " " + to_string(id);
        return gms;
    }

    void minset(int i) {
        if (i < 1 || i > 7)id = 6;
        else id = i;
        rot = 0; X = 3; Y = 21;

        switch (id)
        {

        case 1:
            mino = minoI;
            break;
        case 2:
            mino = minoJ;
            break;
        case 3:
            mino = minoL;
            break;
        case 4:
            mino = minoO;
            break;
        case 5:
            mino = minoS;
            break;
        case 6:
            mino = minoT;
            break;
        case 7:
            mino = minoZ;
            break;

        default:
            mino = minoT;
            break;
        }

    }

    bool operator < (const Tetri& ath) const {
        if (id < ath.id)return true;
        else if (id > ath.id)return false;
        else {
            if (X < ath.X)return true;
            else if (X > ath.X)return false;
            else {
                if (Y < ath.Y)return true;
                else if (Y > ath.Y)return false;
                else {
                    if (rot < ath.rot)return true;
                    else return false;
                    //return rot < ath.rot;
                }
            }
        }
    }

    bool operator == (const Tetri& ath) const {
        if(id == 4 && ath.id == 4)return (X == ath.X && Y == ath.Y);
        return (id == ath.id && X == ath.X && Y == ath.Y && rot == ath.rot);
    }

};


template <typename T>
int binaryS(const vector<T>& v, const T key);

bool vfind_sorted(const vector<int>& v, const int key);

void add_next_que(queue<int>& que, VI& sev);
int get_rnd(int l, int r);
bool get_TF(double p);
int shig_floorI(int i, int target);
int shig_ceilI(int i, int target);
bool add_garbage(int line);
bool make_garbage(int X, int Y, bool sft);
void tspin_check(int toX, int toY, Tetri& ts);
bool move_check(int toX, int toY);
bool move_check(int toX, int toY, Tetri& check);
void SRS_rot(int lr);
void print_mino(int p);
void print_ghost(int p);
set<int> erase_check();
int line_erase();
bool pc_check();
void act_hold(int& hold, int& current, int& event);
void act_soft();
void act_hard();
void ghost();
void act_left();
void act_right();
void act_rotL();
void act_rotR();
void run();
void set_field();
void copy_pfield();
void reset_pfield();
int game(int action);
int get_field_state(int i, int j, int m);
pair<int, queue<int>> get_mino_state();
Tetri get_current();
int get_delayF();
void edit_garbage_cmd(int i);
VI get_game_state();
VS get_ts_state();
pair<int, int> get_combo();
deque<string> get_mino_his();

