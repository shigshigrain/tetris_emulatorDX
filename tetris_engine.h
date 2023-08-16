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
        if (id == 1) {
            VVVI I(4);
            VVI I1(4), I2(4), I3(4), I4(4);
            VI I10 = { 0, 0, 0, 0 }, I11 = { 1, 1, 1, 1 }, I12 = { 0, 0, 0, 0 }, I13 = { 0, 0, 0, 0 };
            VI I20 = { 0, 0, 1, 0 }, I21 = { 0, 0, 1, 0 }, I22 = { 0, 0, 1, 0 }, I23 = { 0, 0, 1, 0 };
            VI I30 = { 0, 0, 0, 0 }, I31 = { 0, 0, 0, 0 }, I32 = { 1, 1, 1, 1 }, I33 = { 0, 0, 0, 0 };
            VI I40 = { 0, 1, 0, 0 }, I41 = { 0, 1, 0, 0 }, I42 = { 0, 1, 0, 0 }, I43 = { 0, 1, 0, 0 };
            I1[0] = I10; I1[1] = I11; I1[2] = I12; I1[3] = I13;
            I2[0] = I20; I2[1] = I21; I2[2] = I22; I2[3] = I23;
            I3[0] = I30; I3[1] = I31; I3[2] = I32; I3[3] = I33;
            I4[0] = I40; I4[1] = I41; I4[2] = I42; I4[3] = I43;
            I[0] = I1; I[1] = I2; I[2] = I3; I[3] = I4;
            mino = I;
        }
        else if (id == 2) {
            VVVI J(4);
            VVI J0(3), J1(3), J2(3), J3(3);
            VI J00 = { 2, 0, 0 }, J01 = { 2, 2, 2 }, J02 = { 0, 0, 0 };
            VI J10 = { 0, 2, 2 }, J11 = { 0, 2, 0 }, J12 = { 0, 2, 0 };
            VI J20 = { 0, 0, 0 }, J21 = { 2, 2, 2 }, J22 = { 0, 0, 2 };
            VI J30 = { 0, 2, 0 }, J31 = { 0, 2, 0 }, J32 = { 2, 2, 0 };
            J0[0] = J00; J0[1] = J01; J0[2] = J02;
            J1[0] = J10; J1[1] = J11; J1[2] = J12;
            J2[0] = J20; J2[1] = J21; J2[2] = J22;
            J3[0] = J30; J3[1] = J31; J3[2] = J32;
            J[0] = J0; J[1] = J1; J[2] = J2; J[3] = J3;
            mino = J;
        }
        else if (id == 3) {
            VVVI L(4);
            VVI L0(3), L1(3), L2(3), L3(3);
            VI L00 = { 0, 0, 3 }, L01 = { 3, 3, 3 }, L02 = { 0, 0, 0 };
            VI L10 = { 0, 3, 0 }, L11 = { 0, 3, 0 }, L12 = { 0, 3, 3 };
            VI L20 = { 0, 0, 0 }, L21 = { 3, 3, 3 }, L22 = { 3, 0, 0 };
            VI L30 = { 3, 3, 0 }, L31 = { 0, 3, 0 }, L32 = { 0, 3, 0 };
            L0[0] = L00; L0[1] = L01; L0[2] = L02;
            L1[0] = L10; L1[1] = L11; L1[2] = L12;
            L2[0] = L20; L2[1] = L21; L2[2] = L22;
            L3[0] = L30; L3[1] = L31; L3[2] = L32;
            L[0] = L0; L[1] = L1; L[2] = L2; L[3] = L3;
            mino = L;
        }
        else if (id == 4) {
            VVVI O(4);
            VVI O0(3), O1(3), O2(3), O3(3);
            VI O00 = { 0, 4, 4, 0 }, O01 = { 0, 4, 4, 0 }, O02 = { 0, 0, 0, 0 };
            VI O10 = { 0, 4, 4, 0 }, O11 = { 0, 4, 4, 0 }, O12 = { 0, 0, 0, 0 };
            VI O20 = { 0, 4, 4, 0 }, O21 = { 0, 4, 4, 0 }, O22 = { 0, 0, 0, 0 };
            VI O30 = { 0, 4, 4, 0 }, O31 = { 0, 4, 4, 0 }, O32 = { 0, 0, 0, 0 };
            O0[0] = O00; O0[1] = O01; O0[2] = O02;
            O1[0] = O10; O1[1] = O11; O1[2] = O12;
            O2[0] = O20; O2[1] = O21; O2[2] = O22;
            O3[0] = O30; O3[1] = O31; O3[2] = O32;
            O[0] = O0; O[1] = O1; O[2] = O2; O[3] = O3;
            mino = O;
        }
        else if (id == 5) {
            VVVI S(4);
            VVI S0(3), S1(3), S2(3), S3(3);
            VI S00 = { 0, 5, 5 }, S01 = { 5, 5, 0 }, S02 = { 0, 0, 0 };
            VI S10 = { 0, 5, 0 }, S11 = { 0, 5, 5 }, S12 = { 0, 0, 5 };
            VI S20 = { 0, 0, 0 }, S21 = { 0, 5, 5 }, S22 = { 5, 5, 0 };
            VI S30 = { 5, 0, 0 }, S31 = { 5, 5, 0 }, S32 = { 0, 5, 0 };
            S0[0] = S00; S0[1] = S01; S0[2] = S02;
            S1[0] = S10; S1[1] = S11; S1[2] = S12;
            S2[0] = S20; S2[1] = S21; S2[2] = S22;
            S3[0] = S30; S3[1] = S31; S3[2] = S32;
            S[0] = S0; S[1] = S1; S[2] = S2; S[3] = S3;
            mino = S;
        }
        else if (id == 6) {
            VVVI T(4);
            VVI T0(3), T1(3), T2(3), T3(3);
            VI T00 = { 0, 6, 0 }, T01 = { 6, 6, 6 }, T02 = { 0, 0, 0 };
            VI T10 = { 0, 6, 0 }, T11 = { 0, 6, 6 }, T12 = { 0, 6, 0 };
            VI T20 = { 0, 0, 0 }, T21 = { 6, 6, 6 }, T22 = { 0, 6, 0 };
            VI T30 = { 0, 6, 0 }, T31 = { 6, 6, 0 }, T32 = { 0, 6, 0 };
            T0[0] = T00; T0[1] = T01; T0[2] = T02;
            T1[0] = T10; T1[1] = T11; T1[2] = T12;
            T2[0] = T20; T2[1] = T21; T2[2] = T22;
            T3[0] = T30; T3[1] = T31; T3[2] = T32;
            T[0] = T0; T[1] = T1; T[2] = T2; T[3] = T3;
            mino = T;
        }
        else if (id == 7) {
            VVVI Z(4);
            VVI Z0(3), Z1(3), Z2(3), Z3(3);
            VI Z00 = { 7, 7, 0 }, Z01 = { 0, 7, 7 }, Z02 = { 0, 0, 0 };
            VI Z10 = { 0, 0, 7 }, Z11 = { 0, 7, 7 }, Z12 = { 0, 7, 0 };
            VI Z20 = { 0, 0, 0 }, Z21 = { 7, 7, 0 }, Z22 = { 0, 7, 7 };
            VI Z30 = { 0, 7, 0 }, Z31 = { 7, 7, 0 }, Z32 = { 7, 0, 0 };
            Z0[0] = Z00; Z0[1] = Z01; Z0[2] = Z02;
            Z1[0] = Z10; Z1[1] = Z11; Z1[2] = Z12;
            Z2[0] = Z20; Z2[1] = Z21; Z2[2] = Z22;
            Z3[0] = Z30; Z3[1] = Z31; Z3[2] = Z32;
            Z[0] = Z0; Z[1] = Z1; Z[2] = Z2; Z[3] = Z3;
            mino = Z;
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

