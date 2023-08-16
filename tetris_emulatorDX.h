#pragma once

#include "resource.h"
#include "framework.h"

#include <dwmapi.h>

#include "shigune_AI.h"
#include "DxLib.h"

using namespace std;

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

void act_ref(int i, VI & check);
void show_game_state();
void show_ts_state();
void show_mino_his();
void show_field(int flag);
void show_field_AI(int flag, shig::AIshigune& ai);
void run_game();
void run_AI(shig::AIshigune& ai);
void run_cmd(queue<int>&s_list, shig::AIshigune &aii);
bool get_key_buf();
int get_RFR(int fps);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
static BOOL monitorenumproc(HMONITOR hMon, HDC hMonDC, LPRECT lpMonRect, LPARAM monRects);
static LRESULT CALLBACK WndProcHook(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
