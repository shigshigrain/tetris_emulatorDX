#pragma once
#include "tetris_engine.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
#include <random>
#include <set>
#include <chrono>
#include <thread>

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

// shigune : main part of AI and do decide act;
const VI cx = { -1, 0, 1 };
const VI cy = { -1, 0, 1 };
const vector<pairI2> cc = {
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1}
};

const VVVI ch = {

	{
		{1, 1, 1, 1},
		{0, 0, 1, 0},
		{1, 1, 1, 1},
		{0, 1, 0, 0}
	},

	{
		{1, 1, 1},
		{0, 1, 1},
		{1, 1, 1},
		{1, 1, 0}
	},

	{
		{1, 1, 1},
		{0, 1, 1},
		{1, 1, 1},
		{1, 1, 0}
	},

	{
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0}
	},

	{
		{1, 1, 1},
		{0, 1, 1},
		{1, 1, 1},
		{1, 1, 0}
	},

	{
		{1, 1, 1},
		{0, 1, 1},
		{1, 1, 1},
		{1, 1, 0}
	},

	{
		{1, 1, 1},
		{0, 1, 1},
		{1, 1, 1},
		{1, 1, 0}
	},

};

const VVI base_cmd = {

	{6, 6, 6, 6, 6, 6, 3},
	{6, 6, 6, 6, 6, 3},
	{6, 6, 6, 6, 3},
	{6, 6, 6, 3},
	{6, 6, 3},
	{6, 3},
	{3},
	{7, 3},
	{7, 7, 3},
	{7, 7, 7, 3},
	{7, 7, 7, 7, 3},
	{7, 7, 7, 7, 7, 3},
	{7, 7, 7, 7, 7, 7, 3},
	{5, 6, 6, 6, 6, 6, 6, 3},
	{5, 6, 6, 6, 6, 6, 3},
	{5, 6, 6, 6, 6, 3},
	{5, 6, 6, 6, 3},
	{5, 6, 6, 3},
	{5, 6, 3},
	{5, 3},
	{5, 7, 3},
	{5, 7, 7, 3},
	{5, 7, 7, 7, 3},
	{5, 7, 7, 7, 7, 3},
	{5, 7, 7, 7, 7, 7, 3},
	{5, 7, 7, 7, 7, 7, 7, 3},
	{4, 6, 6, 6, 6, 6, 6, 3},
	{4, 6, 6, 6, 6, 6, 3},
	{4, 6, 6, 6, 6, 3},
	{4, 6, 6, 6, 3},
	{4, 6, 6, 3},
	{4, 6, 3},
	{4, 3},
	{4, 7, 3},
	{4, 7, 7, 3},
	{4, 7, 7, 7, 3},
	{4, 7, 7, 7, 7, 3},
	{4, 7, 7, 7, 7, 7, 3},
	{4, 7, 7, 7, 7, 7, 7, 3},
	{5, 6, 5, 6, 6, 6, 6, 6, 3},
	{5, 6, 5, 6, 6, 6, 6, 3},
	{5, 6, 5, 6, 6, 6, 3},
	{5, 6, 5, 6, 6, 3},
	{5, 6, 5, 6, 3},
	{5, 6, 5, 3},
	{5, 5, 3},
	{5, 7, 5, 3},
	{5, 7, 5, 7, 3},
	{5, 7, 5, 7, 7, 3},
	{5, 7, 5, 7, 7, 7, 3},
	{5, 7, 5, 7, 7, 7, 7, 3},
	{5, 7, 5, 7, 7, 7, 7, 7, 3},
	{1, 6, 6, 6, 6, 6, 6, 3},
	{1, 6, 6, 6, 6, 6, 3},
	{1, 6, 6, 6, 6, 3},
	{1, 6, 6, 6, 3},
	{1, 6, 6, 3},
	{1, 6, 3},
	{1, 3},
	{1, 7, 3},
	{1, 7, 7, 3},
	{1, 7, 7, 7, 3},
	{1, 7, 7, 7, 7, 3},
	{1, 7, 7, 7, 7, 7, 3},
	{1, 7, 7, 7, 7, 7, 7, 3},
	{1, 5, 6, 6, 6, 6, 6, 6, 3},
	{1, 5, 6, 6, 6, 6, 6, 3},
	{1, 5, 6, 6, 6, 6, 3},
	{1, 5, 6, 6, 6, 3},
	{1, 5, 6, 6, 3},
	{1, 5, 6, 3},
	{1, 5, 3},
	{1, 5, 7, 3},
	{1, 5, 7, 7, 3},
	{1, 5, 7, 7, 7, 3},
	{1, 5, 7, 7, 7, 7, 3},
	{1, 5, 7, 7, 7, 7, 7, 3},
	{1, 5, 7, 7, 7, 7, 7, 7, 3},
	{1, 4, 6, 6, 6, 6, 6, 6, 3},
	{1, 4, 6, 6, 6, 6, 6, 3},
	{1, 4, 6, 6, 6, 6, 3},
	{1, 4, 6, 6, 6, 3},
	{1, 4, 6, 6, 3},
	{1, 4, 6, 3},
	{1, 4, 3},
	{1, 4, 7, 3},
	{1, 4, 7, 7, 3},
	{1, 4, 7, 7, 7, 3},
	{1, 4, 7, 7, 7, 7, 3},
	{1, 4, 7, 7, 7, 7, 7, 3},
	{1, 4, 7, 7, 7, 7, 7, 7, 3},
	{1, 5, 6, 5, 6, 6, 6, 6, 6, 3},
	{1, 5, 6, 5, 6, 6, 6, 6, 3},
	{1, 5, 6, 5, 6, 6, 6, 3},
	{1, 5, 6, 5, 6, 6, 3},
	{1, 5, 6, 5, 6, 3},
	{1, 5, 6, 5, 3},
	{1, 5, 5, 3},
	{1, 5, 7, 5, 3},
	{1, 5, 7, 5, 7, 3},
	{1, 5, 7, 5, 7, 7, 3},
	{1, 5, 7, 5, 7, 7, 7, 3},
	{1, 5, 7, 5, 7, 7, 7, 7, 3},
	{1, 5, 7, 5, 7, 7, 7, 7, 7, 3},

};

const VVI TSD_L = {
	{3, 3, 0, 1, 1, 1, 1, 5, 4, 4},
	{3, 0, 0, 0, 7, 7, 5, 5, 4, 4},
	{3, -1, -1, 7, 7, 2, 5, 0, 0, 0},
	{0, -1, -1, 2, 2, 2, 0, 0, 0, 0},
};

const VVI TSD_J = {
	{4, 4, 7, 1, 1, 1, 1, 0, 2, 2},
	{4, 4, 7, 7, 5, 5, 0, 0, 0, 2},
	{0, 0, 0, 7, 3, 5, 5, -1, -1, 2},
	{0, 0, 0, 0, 3, 3, 3, -1, -1, 0},
};

const VVI HK_ZZ = {
	{4, 4, 3, 3, 0, 7, 7, 6, 2, 2},
	{4, 4, 3, 0, 7, 7, 6, 6, 1, 2},
	{5, 5, 3, 0, 0, 7, 7, 6, 1, 2},
	{2, 5, 5, 0, 7, 7, 4, 4, 1, 1},
	{2, 5, 5, 0, 0, 0, 4, 4, 1, 1},
	{2, 2, 5, 5, 0, -1, 3, 3, 3, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
};

const VVI HK_SS = {
	{3, 3, 6, 5, 5, 0, 2, 2, 4, 4},
	{3, 1, 6, 6, 5, 5, 0, 2, 4, 4},
	{3, 1, 6, 5, 5, 0, 0, 2, 7, 7},
	{1, 1, 4, 4, 5, 5, 0, 7, 7, 3},
	{1, 1, 4, 4, 0, 0, 0, 7, 7, 3},
	{1, 2, 2, 2, -1, 0, 7, 7, 3, 3},
	{1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct cmd_pattern {
	LL score = 0;
	int index = -1;
	int srs = 0, kind = 0;
	int ttrp_f = -1;
	VI cmd_list = {};
	tetri pat;

	cmd_pattern() {};
	cmd_pattern(const tetri& p, const VI& list, const int& d) : pat(p), cmd_list(list), index(d) {};
	
	//set<int> d_line;

	void update(int u) {
		score += u;
		return;
	}

	void set_ttrpF(int& sf) {
		ttrp_f = sf;
	}

	void set(int& s, const tetri& p, VI& list, int& d, int& r, int& k) {
		score = s;
		pat = p;
		cmd_list = list;
		index = d;
		srs = r;
		kind = k;
		//d_line = l;
	}

	void set_ts(int r, int k){
		srs = r;
		kind = k;
		return;
	}

	bool operator < (const cmd_pattern& ath) const {
		return (pat < ath.pat);
	}

	bool operator == (const cmd_pattern& ath) const {
		return (pat == ath.pat);
	}

};

struct strategy {
	int id = 0;
	/*
	 
	-10:
	-9:
	-8:
	-7:
	-6:
	-5:
	-4:
	-3:
	-2:
	-1:?J??TSD
	0:?p?t?F
	1:?e?g???X????
	2:TSS
	3:TSS-mini
	4:TSD
	5:TSD-mini
	6:TST
	7:STSD
	8:DT
	9:?C???y???A???N???X
	10:
	11:
	12:
	13:
	14:
	15:
	*/
	int priority = 0;


};

struct tetriplate {
	bool avalable = true;
	int mino_num = 7;
	int list_size = 0;
	int tp_id = 0;
	int terms_num = 0;
	int bgn_f = 0;
	vector<pairI2> terms = {};
	VI list_id = {};
	vector<bool> mino_check = vector<bool>(7, false);
	string temp_name = "template";
	vector<tetri> list_mino = {};
	vector<int> list_mino_s = {};

	//tetriplate(const int& a, const int& b, const int& c, const int& d, const string& e) : mino_num(a), list_size(b), tp_id(c), terms_num(d), temp_name(e) {};

	bool set(int& a, int& b, int& c, int& d, string& e, int& f) {
		mino_num = a;
		mino_check = vector<bool>(mino_num, false);
		list_size = b;
		tp_id = c;
		terms_num = d;
		temp_name = e;
		bgn_f = f;

		return true;
	}

	bool set_id_list(VI& l) {
		list_id = l;
		return true;
	}

	bool set_list(vector<tetri>& rd, VI& rs) {
		list_mino = rd;
		list_mino_s = rs;
		return true;
	}


	bool set_terms(vector<pairI2>& tr){
		terms = tr;
		return true;
	}


	/*
	-1:Tspin deny
	0:Ignorance
	1:Tspin-single
	2:Tspin-double
	3:Tspin-triple
	4:Tspin-single mini
	5:Tspin-double mini
	6:Tspin-triple mini
	7:
	8:
	*/

};

namespace shig {

	class shigune_AI
	{
	private:
		int identifier;
		int mind;
		int hold_AI;
		int current_AI;
		int pc_cnt;
		int SRS_kind;
		int TS_kind;
		int combo;
		int height_sum;
		int height_mxm;
		int ttrp_size;
		int ttrp_ofsY;
		bool ttrp_able;
		//tetriplate now_selected;
		VI next_AI;
		queue<int> q_next_AI;
		VI todo;
		VVI field_AI;
		VVI p_field_AI;
		VI H_target;
		VI W_target;
		VI height;
		VVI strategy_map;
		vector<strategy> strategy_list;
		VI cmd_list;
		set<cmd_pattern> cp;
		vector<cmd_pattern> cv;
		decltype(cp)::iterator cp_itr;
		VS ttrp_name_list;
		VI ttrp_id_list;
		vector<tetriplate> ttrp_list;
		VI ttrp_bgnF;

	public:
		shigune_AI(int ii);
		VI shigune_main();
		VI get_cmd();
		void get_field();
		void get_state();
		bool strategy_mark();
		bool pc_check();
		void bgn_strategy();
		void search_way();
		void get_score(cmd_pattern& cd);
		bool move_check(int to_x, int to_y, tetri& s_check);
		bool SRS_check(int lr, tetri& s_now);
		int TS_check(int toX, int toY, tetri& ts);
		set<int> erase_check_AI(tetri& s_now);
		void apply_mino(VVI& c_field, tetri& s_now);
		bool move_mino(tetri& m_now, int s_action);
		void print_gh(tetri& s_now);
		void move_itr(int lr);
		VVI get_AI_field(int p, int m);
		VI get_AI_cmd();
		pair<int, string> get_sttrp_name();
		bool load_ttrp();
		bool ttrp_crr(tetri& now_p, int& size_l);
		bool ttrp_check(cmd_pattern& slc, int& sle, VI& mnL);
		bool next_crr_check();

		~shigune_AI();
	};

	// hold : 1, soft : 2, hard : 3, L_rot : 4, R_rot : 5, l_move : 6, r_move : 7;


}

bool GetTempNameList(VS& name_list);
bool ReadTempData(string& name, tetriplate& ttrp);


