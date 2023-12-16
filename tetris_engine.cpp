
#include "tetris_engine.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
#include <deque>
#include <random>
#include <set>
#include <chrono>
#include <thread>


using namespace std;
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

const int fh = 45;
VVI field(fh), p_field(fh);
VI sev = { 1, 2, 3, 4, 5, 6, 7 };
queue<int> q_next;
int hold_f = 0;
int hold = 0;
int current = 0;
int counter = 0;
int event = 1;
int hold_kind = 0;
int hd_cnt = 0;
int l_erasef = 0;
int delay_f = 0;
int ts_kind = 0;
int p_srs = -1;
int btb = -1;
int combo = 0;
int garbage_cmd = 0;

VS ts_state(2);
deque<string> mino_his;

Tetri now(0, 3, 21, 0);

template <typename T>
int binaryS(const vector<T> &v, const T key) {
    int ng = -1; int ok = (int)v.size(); int mid = -1;
    while (abs(ok - ng) > 1) {
        mid = (ok + ng) / 2;
        if ([&]() {return v.at(mid) >= key; }())ok = mid;
        else ng = mid;
    }
    return ok;
}

bool vfind_sorted(const vector<int> &v, const int key) {
    int itr = binaryS<int>(v, key);
    if (itr < 0 || itr >= v.size())return false;
    if (v.at(itr) == key)return true;
    else return false;
}

void add_next_que(queue<int>&que, VI & sev) {
    //VI sev = { 1, 2, 3, 4, 5, 6, 7 };
    random_device seed_gen;
    mt19937 engine(seed_gen());
    shuffle(sev.begin(), sev.end(), engine);
    shig_rep(i, 7)que.push(sev[i]);

}

int get_rnd(int l, int r) {
    random_device randomD;
    vector<uint32_t> randomSeedVec(10);
    generate(randomSeedVec.begin(), randomSeedVec.end(), ref(randomD));
    seed_seq randomSeed(randomSeedVec.begin(), randomSeedVec.end());
    mt19937 engine(randomSeed);
    uniform_int_distribution<int> randomIntDist(l, r);
    return randomIntDist(engine);
}

bool get_TF(double p) {
    random_device seed_gen;
    mt19937 engine(seed_gen());
    //double p = (double)(num / den);
    bernoulli_distribution uid(p);
    return uid(engine);
}

int shig_floorI(int i, int target) {
    target = abs(target);
    int flr = 0;
    if (i >= 0) {
        flr = i - i % target;
    }
    else {
        flr = i + (abs(i) % target);
        if (abs(i) % target != 0)flr -= target;
    }
    return flr;
}

int shig_ceilI(int i, int target) {
    target = abs(target);
    int ceil = 0;
    if (i >= 0) {
        ceil = i - i % target;
        if (i % target != 0)ceil += target;
    }
    else {
        ceil = i + (abs(i) % target);
    }
    return ceil;
}

bool add_garbage(int line) {
    VVI pre_field(fh);
    VI garbage(10, 8);
    int g = get_rnd(0, 9);
    shig_rep(i, line) {
        if(!get_TF(0.7)) g = get_rnd(0, 9);
        if (g < 0 || g > 9)g = 0;
        pre_field[i] = garbage;
        pre_field[i][g] = 0;
    }
    shig_rep(i, fh - line) {
        if (i >= field.size())return false;
        pre_field[(size_t)i + line] = field[i];
    }

    field = pre_field;

    return true;
}

bool make_garbage(int X, int Y, bool sft) {
    VVI pre_field(fh);
    VI garbage(10, 8);
    if (sft) {
        shig_rep(i, Y) {
            pre_field[i] = field[i];
        }
        shig_rep(i, field.size() - Y - 1) {
            pre_field[(size_t)i + Y + 1] = field[(size_t)i + Y];
        }
    }
    field[Y] = garbage;
    field[Y][X] = 0;

    return true;
}

void tspin_check(int toX, int toY, Tetri& ts) {
    if (ts.id != 6) {
        ts_kind = 0;
        return;
    }
    int ts_cnt = 0;
    //int size = ts.px_size;
    int rot = ts.rot;

    VI testX = { 0, 0, 2, 2 };
    VI testY = { 0, 2, 0, 2 };

    pairI2 check = {0, 0};

    shig_rep(i, 4) {
        int sX = ts.X + testX[i] + toX;
        if (sX < 0 || sX >= 10) {
            ts_cnt++;
            continue;
        }
        int sY = ts.Y - testY[i] + toY - 1;
        if (sY < 0 || sY >= (field.size() - 1)) {
            ts_cnt++;
            continue;
        }
        if (field[sY][sX] != 0)ts_cnt++;
        else check = { testX[i], testY[i] };
    }

    if (ts_cnt == 3 && p_srs != 4) {
        if (rot == 0) {
            if (check.first == 0 && check.second == 0)ts_kind = 2;
            else if (check.first == 2 && check.second == 0)ts_kind = 2;
            else ts_kind = 1;
        }
        else if (rot == 1) {
            if (check.first == 2 && check.second == 0)ts_kind = 2;
            else if (check.first == 2 && check.second == 2)ts_kind = 2;
            else ts_kind = 1;
        }
        else if (rot == 2) {
            if (check.first == 2 && check.second == 2)ts_kind = 2;
            else if (check.first == 0 && check.second == 2)ts_kind = 2;
            else ts_kind = 1;
        }
        else if (rot == 3) {
            if (check.first == 0 && check.second == 2)ts_kind = 2;
            else if (check.first == 0 && check.second == 0)ts_kind = 2;
            else ts_kind = 1;
        }
    }
    else if (ts_cnt == 3 || ts_cnt == 4)ts_kind = 1;//normal
    else ts_kind = 0;

    return;
}

bool move_check(int toX, int toY) {
    int cnt = 0, cntt = 4;
    int rot = now.rot;
    //int size = now.px_size;
    int H = now.mino[rot].size();
    int W = now.mino[rot][0].size();
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (now.mino[rot][i][j] != 0) {
                int sX = now.X + j + toX;
                if (sX < 0 || sX >= 10)continue;
                int sY = now.Y - i + toY;
                if (sY <= 0 || sY >= (field.size() - 1))continue;
                if (field[sY - 1LL][sX] == 0)cnt++;
            }
        }
    }
    if (cnt == cntt) return true;
    else return false;
}

bool move_check(int toX, int toY, Tetri& check) {
    int cnt = 0, cntt = 4;
    int rot = check.rot;
    //int size = check.px_size;
    int H = check.mino[rot].size();
    int W = check.mino[rot][0].size();
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (check.mino[rot][i][j] != 0) {
                int sX = check.X + j + toX;
                if (sX < 0 || sX >= 10)continue;
                int sY = check.Y - i + toY;
                if (sY <= 0 || sY >= (field.size() - 1))continue;
                if (field[sY - 1LL][sX] == 0)cnt++;
            }
        }
    }
    if (cnt == cntt) return true;
    else return false;
}

void SRS_rot(int lr) {//l=-1 r=1
    Tetri test = now;
    //int size = test.px_size;
    int to_X = 0, to_Y = 0, rot = test.rot;
    bool can = true;
    if (test.id == 1) {//I-mino SRS
        if (test.rot == 0) {
            if (lr == -1) {
                test.set_rot(3);
                rot = 3;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(2, 0, test)) {
                    p_srs = 2;
                    to_X = 2;
                }
                else if (move_check(-1, 2, test)) {
                    p_srs = 3;
                    to_X = -1;
                    to_Y = 2;
                }
                else if (move_check(2, -1, test)) {
                    p_srs = 4;
                    to_X = 2;
                    to_Y = -1;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(1);
                rot = 1;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-2, 0, test)) {
                    p_srs = 1;
                    to_X = -2;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 2;
                    to_X = 1;
                }
                else if (move_check(-2, -1, test)) {
                    p_srs = 3;
                    to_X = -2;
                    to_Y = -1;
                }
                else if (move_check(1, 2, test)) {
                    p_srs = 4;
                    to_X = 1;
                    to_Y = 2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
        else if (test.rot == 1) {
            if (lr == -1) {
                test.set_rot(0);
                rot = 0;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(2, 0, test)) {
                    p_srs = 1;
                    to_X = 2;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 2;
                    to_X = -1;
                }
                else if (move_check(2, 1, test)) {
                    p_srs = 3;
                    to_X = 2;
                    to_Y = 1;
                }
                else if (move_check(-1, -2, test)) {
                    p_srs = 4;
                    to_X = -1;
                    to_Y = -2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(2);
                rot = 2;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(2, 0, test)) {
                    p_srs = 2;
                    to_X = 2;
                }
                else if (move_check(-1, 2, test)) {
                    to_X = -1;
                    to_Y = 2;
                }
                else if (move_check(2, -1, test)) {
                    to_X = 2;
                    to_Y = -1;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
        else if (test.rot == 2) {
            if (lr == -1) {
                test.set_rot(1);
                rot = 1;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    to_X = 1;
                    p_srs = 1;
                }
                else if (move_check(-2, 0, test)) {
                    to_X = -2;
                    p_srs = 2;
                }
                else if (move_check(1, -2, test)) {
                    to_X = 1;
                    to_Y = -2;
                    p_srs = 3;
                }
                else if (move_check(-2, 1, test)) {
                    to_X = -2;
                    to_Y = 1;
                    p_srs = 4;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(3);
                rot = 3;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(2, 0, test)) {
                    to_X = 2;
                    p_srs = 1;
                }
                else if (move_check(-1, 0, test)) {
                    to_X = -1;
                    p_srs = 2;
                }
                else if (move_check(2, 1, test)) {
                    to_X = 2;
                    to_Y = 1;
                    p_srs = 3;
                }
                else if (move_check(-1, -2, test)) {
                    to_X = -1;
                    to_Y = -2;
                    p_srs = 4;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
        else if (test.rot == 3) {
            if (lr == -1) {
                test.set_rot(2);
                rot = 2;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-2, 0, test)) {
                    to_X = -2;
                    p_srs = 1;
                }
                else if (move_check(1, 0, test)) {
                    to_X = 1;
                    p_srs = 2;
                }
                else if (move_check(-2, -1, test)) {
                    to_X = -2;
                    to_Y = -1;
                    p_srs = 3;
                }
                else if (move_check(1, 2, test)) {
                    to_X = 1;
                    to_Y = 2;
                    p_srs = 4;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(0);
                rot = 0;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    to_X = 1;
                    p_srs = 1;
                }
                else if (move_check(-2, 0, test)) {
                    to_X = -2;
                    p_srs = 2;
                }
                else if (move_check(1, -2, test)) {
                    to_X = 1;
                    to_Y = -2;
                    p_srs = 3;
                }
                else if (move_check(-2, 1, test)) {
                    to_X = -2;
                    to_Y = 1;
                    p_srs = 4;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
    }
    else {
        if (test.rot == 0) {
            if (lr == -1) {
                test.set_rot(3);
                rot = 3;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, 1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(1, -2, test)) {
                    p_srs = 4;
                    to_X = 1;
                    to_Y = -2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(1);
                rot = 1;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, 1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(-1, -2, test)) {
                    p_srs = 4;
                    to_X = -1;
                    to_Y = -2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
        else if (test.rot == 1) {
            if (lr == -1) {
                test.set_rot(0);
                rot = 0;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, -1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(1, 2, test)) {
                    p_srs = 4;
                    to_X = 1;
                    to_Y = 2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(2);
                rot = 2;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, -1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(1, 2, test)) {
                    p_srs = 4;
                    to_X = 1;
                    to_Y = 2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
        else if (test.rot == 2) {
            if (lr == -1) {
                test.set_rot(1);
                rot = 1;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, 1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(-1, -2, test)) {
                    p_srs = 4;
                    to_X = -1;
                    to_Y = -2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(3);
                rot = 3;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(1, 0, test)) {
                    p_srs = 1;
                    to_X = 1;
                }
                else if (move_check(1, 1, test)) {
                    p_srs = 2;
                    to_X = 1;
                    to_Y = 1;
                }
                else if (move_check(0, -2, test)) {
                    p_srs = 3;
                    to_Y = -2;
                }
                else if (move_check(1, -2, test)) {
                    p_srs = 4;
                    to_X = 1;
                    to_Y = -2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
        else if (test.rot == 3) {
            if (lr == -1) {
                test.set_rot(2);
                rot = 2;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, -1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(-1, 2, test)) {
                    p_srs = 4;
                    to_X = -1;
                    to_Y = 2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
            else if (lr == 1) {
                test.set_rot(0);
                rot = 0;
                if (move_check(0, 0, test)) {
                    p_srs = 0;
                }
                else if (move_check(-1, 0, test)) {
                    p_srs = 1;
                    to_X = -1;
                }
                else if (move_check(-1, -1, test)) {
                    p_srs = 2;
                    to_X = -1;
                    to_Y = -1;
                }
                else if (move_check(0, 2, test)) {
                    p_srs = 3;
                    to_Y = 2;
                }
                else if (move_check(-1, 2, test)) {
                    p_srs = 4;
                    to_X = -1;
                    to_Y = 2;
                }
                else {
                    p_srs = -1;
                    can = false;
                }
            }
        }
    }

    if (can) {
        tspin_check(to_X, to_Y, test);
        now.set_rot(rot);
        now.addX(to_X);
        now.addY(to_Y);
    }

    return;
}

void print_mino(int p) {
    int rot = now.rot;
    //int size = now.px_size;
    int H = now.mino[rot].size();
    int W = now.mino[rot][0].size();
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (now.mino[rot][i][j] != 0) {
                int sX = now.X + j;
                int sY = now.Y - i;
                if (sX < 0 || sX >= 10)continue;
                if (sY - 1 < 0 || sY - 1 > 45)continue;
                if (p == 1) {
                    field[sY - 1LL][sX] = now.id;
                }
                else {
                    p_field[sY - 1LL][sX] = now.id;
                }
            }
        }
    }
}

void print_ghost(int p) {
    Tetri ghost_mino = now;
    int sft = -1;
    while (move_check(0, sft, ghost_mino))ghost_mino.addY(sft);
    int rot = ghost_mino.rot;
    //int size = ghost_mino.px_size;
    int id = ghost_mino.id;
    int H = ghost_mino.mino[rot].size();
    int W = ghost_mino.mino[rot][0].size();
    shig_rep(i, H) {
        shig_rep(j, W) {
            if (ghost_mino.mino[rot][i][j] != 0) {
                int sX = ghost_mino.X + j;
                int sY = ghost_mino.Y - i;
                if (sX < 0 || sX >= 10)continue;
                if (sY - 1 < 0 || sY - 1 > 45)continue;
                if (p == 1) {
                    field[sY - 1LL][sX] = -1 * id;
                }
                else {
                    p_field[sY - 1LL][sX] = -1 * id;
                }
            }
        }
    }
}

set<int> erase_check() {
    set<int> erase_itr;
    print_mino(2);
    int rot = now.rot;
    //int size = now.px_size;
    int H = now.mino[rot].size();
    int pW = field[0].size();

    shig_rep(i, H) {
        int sY = now.Y - i;
        int itr = sY - 1;
        int cnt = 0;
        if (itr < 0 || itr >= fh)continue;
        shig_rep(j, pW) {
            if (field[itr][j] != 0) cnt++;
            else break;
        }
        if (cnt == pW) erase_itr.insert(itr);

    }
    int size_e = (int)erase_itr.size();
    if (size_e == 1) {
        delay_f = 40;
    }
    else if (size_e == 2 || size_e == 3) {
        delay_f = 45;
    }
    else {
        delay_f = 50;
    }
    return erase_itr;
}

int line_erase() {
    set<int> itr = erase_check();
    VI empty = { 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 };
    VVI proxy(0); proxy.reserve(45);
    VVI pre(0); pre.reserve(45);

    shig_rep(i, fh) {
        decltype(itr)::iterator it = itr.find(i);
        if (it == itr.end()) pre.push_back(field[i]);
        else pre.push_back(empty);
    }

    p_field = pre;
    if (itr.size() != 0) l_erasef = 1;
    

    shig_rep(i, fh) {
        decltype(itr)::iterator it = itr.find(i);
        if (it == itr.end()) proxy.push_back(field[i]);
    }
    while (proxy.size() < fh) proxy.push_back(empty);

    field = proxy;

    int s = (int)itr.size();

    if (s > 0 && ts_kind > 0) {
        combo++;
        btb++;
        if (s == 1)ts_state[0] = "T-spin Single";
        else if (s == 2)ts_state[0] = "T-spin Double";
        else if (s == 3)ts_state[0] = "T-spin Triple";

        if (ts_kind == 2)ts_state[1] = "mini";
        else ts_state[1] = "    ";
    }
    else if (s == 4) {
        combo++;
        btb++;
        ts_state[0] = "4-LINE-eraser";
        ts_state[1] = "    ";
    }
    else if(s > 0){
        combo++;
        btb = -1;
        ts_state[0] = "    ";
        ts_state[1] = "    ";
    }
    else {
        combo = 0;
    }
    

    return s;
}

bool pc_check() {
    int cnt = 0;
    shig_rep(i, 10) {
        cnt += field[0][i];
    }
    if (cnt > 0)return false;
    else {
        delay_f = 1;
        return true;
    }
}

void act_hold(int& hold, int& current, int& event) {
    if (hold == 0) {
        delay_f = 2 + 5;
        hold = current;
        event = -1;
    }
    else {
        swap(hold, current);
        event = -2;
        delay_f = 2;
    }

    return;
}

void act_soft() {
    int sft = -1;
    if (move_check(0, sft)) {
        now.addY(sft);
        ts_kind = 0;
    }
    delay_f = 2;
    return;
}

void act_hard() {
    int sft = -1;
    hd_cnt = 0;
    while (move_check(0, sft)) {
        now.addY(sft);
        hd_cnt++;
    }
    if (move_check(0, 0))print_mino(1);

    mino_his.push_front(now.get_mino_str());
    if (mino_his.size() > 7)mino_his.pop_back();
    line_erase();
    pc_check();
    if (garbage_cmd == 1) add_garbage(get_rnd(1, 6));
    garbage_cmd = 0;

    return;
}

void ghost() {
    int sft = -1;
    while (move_check(0, sft))now.addY(sft);
    if (move_check(0, 0))print_mino(1);
    line_erase();
    return;
}

void act_left() {
    int sft = -1;
    if (move_check(sft, 0)) {
        now.addX(sft);
        ts_kind = 0;
    }
    delay_f = 2;
    return;
}

void act_right() {
    int sft = 1;
    if (move_check(sft, 0)) {
        now.addX(sft);
        ts_kind = 0;
    }
    delay_f = 2;
    return;
}

void act_rotL() {
    SRS_rot(-1);
    delay_f = 2;
    return;
}

void act_rotR() {
    SRS_rot(1);
    delay_f = 2;
    return;
}

void run() {
    return;
}

void set_field() {
    shig_rep(i, fh)field[i] = { 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };
    shig_rep(i, fh)p_field[i] = { 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };
    while (!q_next.empty())q_next.pop();
    add_next_que(q_next, sev);
    current = q_next.front();
    q_next.pop();
    now.minset(current);
    hold_f = 0;
    hold = 0;
    counter = 0;
    event = 1;
    hold_kind = 0;
    hd_cnt = 0;
    delay_f = 0;
    ts_kind = 0;
    p_srs = -1;
    btb = -1;
    combo = 0;
    garbage_cmd = 0;
    ts_state[0] = "             ";
    ts_state[1] = "    ";
    mino_his.clear();
    return;
}

void copy_pfield() {
    p_field = field;
    print_ghost(2);
    print_mino(2);
    return;
}

void reset_pfield() {
    p_field = field;
    return;
}

int game(int action) {

    if (event == 1) {

        //if (action != 4 && action != 5 && action != 3)ts_kind = 0;

        if (action == 1) {//hold action
            if (hold_f == 0) {
                act_hold(hold, current, event);
                hold_f = 1;
            }

        }
        else if (action == 2) {//soft drop
            act_soft();
        }
        else if (action == 3) {//hard drop
            act_hard();
            hold_f = 0;
            event = -1;
        }
        else if (action == 4) {//left rot Z
            act_rotL();
        }
        else if (action == 5) {//right rot up
            act_rotR();
        }
        else if (action == 6) {//left move
            act_left();
        }
        else if (action == 7) {//right move
            act_right();
        }
    }

    if (event == -1 || event == -2) {
        if (q_next.size() <= 6)add_next_que(q_next, sev);
        if (event == -1) {
            current = q_next.front();
            q_next.pop();
        }
        event = 1;
        now.minset(current);
        if (!(move_check(0, 0))) {
            return 1;
        }
    }
    if (l_erasef == 1) {
        l_erasef = 0;
        return 2;
    }
    return 0;
}

int get_field_state(int i, int j, int m) {
    
    if(m == 0) return p_field[i][j];
    else if(m == 1) return field[i][j];
    return 0;
}

pair<int, queue<int>> get_mino_state() {
    return make_pair(hold, q_next);
}

Tetri get_current() {
    return now;
}

int get_delayF() {
    return delay_f;
}

void edit_garbage_cmd(int i) {
    garbage_cmd = i;
    return;
}

VI get_game_state() {
    VI state(13);
    state[0] = hold_f;
    state[1] = hold;
    state[2] = current;
    state[3] = counter;
    state[4] = event;
    state[5] = hold_kind;
    state[6] = hd_cnt;
    state[7] = l_erasef;
    state[8] = delay_f;
    state[9] = ts_kind;
    state[10] = p_srs;
    state[11] = btb;
    state[12] = combo;
    return state;
}

VS get_ts_state() {
    return ts_state;
}

pair<int, int> get_combo() {
    return { combo, btb };
}

deque<string> get_mino_his() {
    return mino_his;
}






