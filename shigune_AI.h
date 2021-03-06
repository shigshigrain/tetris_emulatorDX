#pragma once
#include "tetris_engine.h"


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
#define all(v) (v).begin(), (v).end()
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



const vector<int> scr_rate = { 100, 95, 95, 95, 90, 90, 90, 90 ,80, 80, 80, 70, 70, 70 ,60, 60, 60, 50, 50, 50 };

const VI NS_a = { 6, 1, 5, 7, 2, 3, 4 };

const VI ev_empty = { 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 };

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

};

const vector<vector<vector<pair<int, int>>>> touch_list = {

	// I
	{
		{
			{0, -1},
			{0, 4},
			{2, 4},
			{2, -1},
			{0, 0},
			{0, 1},
			{0, 2},
			{0, 3},
			{1, 4},
			{2, 3},
			{2, 2},
			{2 ,1},
			{2, 0},
			{1, -1}
		},
		{
			{-1, 1},
			{-1, 3},
			{4, 3},
			{4, 1},
			{-1, 2},
			{0, 3},
			{1, 3},
			{2, 3},
			{3, 3},
			{4, 2},
			{3, 1},
			{2 ,1},
			{1, 1},
			{0, 1}
		},
		{
			{1, -1},
			{1, 4},
			{3, 4},
			{3, -1},
			{1, 0},
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 4},
			{3, 3},
			{3, 2},
			{3 ,1},
			{3, 0},
			{2, -1}
		},
		{
			{-1, 0},
			{-1, 2},
			{4, 2},
			{4, 0},
			{-1, 1},
			{0, 2},
			{1, 2},
			{2, 2},
			{3, 2},
			{4, 1},
			{3, 0},
			{2 ,0},
			{1, 0},
			{0, 0}
		}
	},
	// J
	{
		{
			{-1, -1},
			{-1, 1},
			{0, 3},
			{2, 3},
			{2, -1},

			{-1, 0},
			{0, 1},
			{0, 2},
			{1, 3},
			{2, 2},
			{2, 1},
			{2, 0},
			{1 ,-1},
			{0, -1}
		},
		{
			{-1, 0},
			{-1, 3},
			{1, 3},
			{3, 2},
			{3, 0},

			{-1, 1},
			{-1, 2},
			{0, 3},
			{1, 2},
			{2, 2},
			{3, 1},
			{2, 0},
			{1 ,0},
			{0, 0}
		},
		{
			{0, -1},
			{0, 3},
			{3, 3},
			{3, 1},
			{2, -1},
			{0, 0},
			{0, 1},
			{0, 2},
			{1, 3},
			{2, 3},
			{3, 2},
			{2, 1},
			{2, 0},
			{1, -1}
		},
		{
			{-1, 0},
			{-1, 2},
			{3, 2},
			{3, -1},
			{1, -1},
			{-1, 1},
			{0, 2},
			{1, 2},
			{2, 2},
			{3, 1},
			{3, 0},
			{2, -1},
			{1 ,0},
			{0, 0}
		}
	},
	// L
	{
		{
			{-1, 1},
			{-1, 3},
			{3, 3},
			{3, -1},
			{0, -1},
			{-1, 2},
			{0, 3},
			{1, 3},
			{2, 2},
			{2, 1},
			{2, 0},
			{1, -1},
			{0 ,0},
			{0, 1}
		},
		{
			{-1, 0},
			{-1, 2},
			{1, 3},
			{3, 3},
			{3, 0},
			{-1, 1},
			{0, 2},
			{1, 2},
			{2, 3},
			{3, 2},
			{3, 1},
			{2, 0},
			{1 ,0},
			{0, 0}
		},
		{
			{0, -1},
			{0, 3},
			{2, 3},
			{3, 1},
			{3, -1},
			{0, 0},
			{0, 1},
			{0, 2},
			{1, 3},
			{2, 2},
			{2, 1},
			{3, 0},
			{2, -1},
			{1, -1}
		},
		{
			{-1, -1},
			{-1, 2},
			{3, 2},
			{3, 0},
			{1, -1},
			{-1, 0},
			{-1, 1},
			{0, 2},
			{1, 2},
			{2, 2},
			{3, 1},
			{2, 0},
			{1 ,0},
			{0, -1}
		}
	
	},
	// O
	{
		{
			{-1, 0},
			{-1, 3},
			{2, 3},
			{2, 0},
			{-1, 1},
			{-1, 2},
			{0, 3},
			{1, 3},
			{2, 2},
			{2, 1},
			{1, 0},
			{0 ,0}
		},
		{
			{-1, 0},
			{-1, 3},
			{2, 3},
			{2, 0},
			{-1, 1},
			{-1, 2},
			{0, 3},
			{1, 3},
			{2, 2},
			{2, 1},
			{1, 0},
			{0 ,0}
		},
		{
			{-1, 0},
			{-1, 3},
			{2, 3},
			{2, 0},
			{-1, 1},
			{-1, 2},
			{0, 3},
			{1, 3},
			{2, 2},
			{2, 1},
			{1, 0},
			{0 ,0}
		},
		{
			{-1, 0},
			{-1, 3},
			{2, 3},
			{2, 0},
			{-1, 1},
			{-1, 2},
			{0, 3},
			{1, 3},
			{2, 2},
			{2, 1},
			{1, 0},
			{0 ,0}
		}
	},
	// S
	{
		{
			{-1, 0},
			{-1, 3},
			{1, 3},
			{2, 2},
			{2, -1},
			{0, -1},
			{-1, 1},
			{-1, 2},
			{0, 3},
			{1, 2},
			{2, 1},
			{2, 0},
			{1, -1},
			{0 ,0}
		},
		{
			{-1, 0},
			{-1, 2},
			{0, 3},
			{3, 3},
			{3, 1},
			{2, 0},
			{-1, 1},
			{0, 2},
			{1, 3},
			{2, 3},
			{3, 2},
			{2, 1},
			{1, 0},
			{0 ,0}
		},
		{
			{0, 0},
			{0, 3},
			{2, 3},
			{3, 2},
			{3, -1},
			{1, -1},
			{0, 1},
			{0, 2},
			{1, 3},
			{2, 2},
			{3, 1},
			{3, 0},
			{2, -1},
			{1 ,0}
		},
		{
			{-1, -1},
			{-1, 1},
			{0, 2},
			{3, 2},
			{3, 0},
			{2, -1},
			{-1, 0},
			{0, 1},
			{1, 2},
			{2, 2},
			{3, 1},
			{2, 0},
			{1, -1},
			{0 ,-1}
		}
	},
	// T
	{
		{
			{-1, 0},
			{-1, 2},
			{0, 3},
			{2, 3},
			{2, -1},
			{0, -1},
			{-1, 1},
			{0, 2},
			{1, 3},
			{2, 2},
			{2, 1},
			{2, 0},
			{1, -1},
			{0 ,0}
		},
		{
			{-1, 0},
			{-1, 2},
			{0, 3},
			{2, 3},
			{3, 2},
			{3, 0},
			{-1, 1},
			{0, 2},
			{1, 3},
			{2, 2},
			{3, 1},
			{2, 0},
			{1, 0},
			{0 ,0}
		},
		{
			{0, -1},
			{0, 3},
			{2, 3},
			{3, 2},
			{3, 0},
			{2, -1},
			
			{0, 0},
			{0, 1},
			{0, 2},
			{1, 3},
			{2, 2},
			{3, 1},
			{2, 0},
			{1 ,-1}
		},
		{
			{-1, 0},
			{-1, 2},
			{3, 2},
			{3, 0},
			{2, -1},
			{0, -1},
			
			{-1, 1},
			{0, 2},
			{1, 2},
			{2, 2},
			{3, 1},
			{2, 0},
			{1 ,-1},
			{0, 0}
		}
	},
	// Z
	{
		{
			{-1, -1},
			{-1, 2},
			{0, 3},
			{2, 3},
			{2, 0},
			{1, -1},
			{-1, 0},
			{-1, 1},
			{0, 2},
			{1, 3},
			{2, 2},
			{2, 1},
			{1, 0},
			{0 ,-1}
		},
		{
			{-1, 1},
			{-1, 3},
			{2, 3},
			{3, 2},
			{3, 0},
			{0, 0},
			{-1, 2},
			{0, 3},
			{1, 3},
			{2, 2},
			{3, 1},
			{2, 0},
			{1, 0},
			{0 ,1}
		},
		{
			{0, -1},
			{0, 2},
			{1, 3},
			{3, 3},
			{3, 0},
			{2, -1},
			{0, 0},
			{0, 1},
			{1, 2},
			{2, 3},
			{3, 2},
			{3, 1},
			{2, 0},
			{1 ,-1}
		},
		{
			{-1, 0},
			{-1, 2},
			{2, 2},
			{3, 1},
			{3, -1},
			{0, -1},
			{-1, 1},
			{0, 2},
			{1, 2},
			{2, 1},
			{3, 0},
			{2, -1},
			{1, -1},
			{0 ,0}
		}
	},

};

struct cmd_score {
	
	LL sum = 0;

	LL height  = 0;
	LL touch   = 0;
	LL contact = 0;
	LL erase   = 0;
	LL cmb     = 0;
	LL btbc    = 0;
	LL closed  = 0;
	LL PC      = 0;

	cmd_score() {
		height  = 0;
		touch   = 0;
		contact = 0;
		erase   = 0;
		cmb     = 0;
		btbc    = 0;
		closed  = 0;
		PC      = 0;
	}


	bool init() {
		height  = 0;
		touch   = 0;
		contact = 0;
		erase   = 0;
		cmb     = 0;
		btbc    = 0;
		closed  = 0;
		PC      = 0;
		return true;
	}

	bool init_s() {
		sum = 0;
		height = 0;
		touch = 0;
		contact = 0;
		erase = 0;
		cmb = 0;
		btbc = 0;
		closed = 0;
		PC = 0;
		return true;
	}

	bool t_calc_sum() {

		__m256i vec1 = _mm256_set_epi64x(height, touch, contact, erase);
		__m256i vec2 = _mm256_set_epi64x(cmb, btbc, closed, PC);

		__m256i sumA = _mm256_add_epi64(vec1, vec2);

		this->sum = sumA.m256i_i64[0] + sumA.m256i_i64[1] + sumA.m256i_i64[2] + sumA.m256i_i64[3];

		return true;
	}

	bool calc_sum() {

		//this->sum = 0;
		this->sum = shig::secure_add(sum, height);
		this->sum = shig::secure_add(sum, touch);
		this->sum = shig::secure_add(sum, contact);
		this->sum = shig::secure_add(sum, erase);
		this->sum = shig::secure_add(sum, cmb);
		this->sum = shig::secure_add(sum, btbc);
		this->sum = shig::secure_add(sum, closed);
		this->sum = shig::secure_add(sum, PC);

		return true;
	}

	LL get_sum() {
		calc_sum();
		return this->sum;
	}

	bool operator < (const cmd_score& ath) const {
		
		if (sum != ath.sum)return sum < ath.sum;
		else {
			if (PC != ath.PC)return PC < ath.PC;
			else {
				if (height != ath.height)return height < ath.height;
				else {
					if (touch != ath.touch)return touch < ath.touch;
					else {
						if (contact != ath.contact)return contact < ath.contact;
						else {
							if (erase != ath.erase)return erase < ath.erase;
							else {
								if (closed != ath.closed)return closed < ath.closed;
								else return btbc < ath.btbc;

							}

						}

					}
				}
			}
		}

	}



};


/*
struct cmd_pattern {
	LL score = 1000000000;
	cmd_score scr;
	int index = -1;
	int srs = 0, kind = 0;
	int ttrp_f = -1;
	bool isSFT = false;
	VI cmd_list = {};
	tetri pat;

	cmd_pattern() {};
	cmd_pattern(const tetri& p, const VI& list, const int& d) : pat(p), cmd_list(list), index(d) {};
	
	void update() {
		score += scr.get_sum();
		return;
	}

	void update(LL u) {
		score += u;
		return;
	}

	void set_ttrpF(int& sf) {
		ttrp_f = sf;
	}

	void set_isSFT(const bool& sf) {
		isSFT = sf;
	}

	void set(LL& s, const tetri& p, VI& list, int& d, int& r, int& k) {
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

*/

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
	vector<pairI2> terms = vector<pairI2>(0);
	vector<int> list_id = vector<int>(0);
	vector<bool> mino_check = vector<bool>(0);
	string temp_name = "template";
	vector<tetri> list_mino = vector<tetri>(0);
	vector<int> list_mino_s = vector<int>(0);

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

	class cmd_pattern {

	public:
		LL score;
		cmd_score scr;
		int index;
		int srs;
		int kind;
		int ttrp_f;
		int pre_gc;
		bool isSFT;
		VI cmd_list;
		tetri pat;

	public:
		cmd_pattern();
		cmd_pattern(const tetri& p, const VI& list, const int& d);

		void update();

		void update(LL u);

		void set_ttrpF(int& sf);

		void set_isSFT(const bool& sf);

		void set(LL& s, const tetri& p, VI& list, int& d, int& r, int& k);

		void set_ts(int r, int k);

	};

	class game_container
	{
	public:
		//private:
		int slot_id;
		LL pre_score;
		int hold_AI;
		int current_AI;
		int pc_cnt;
		int SRS_kind;
		int TS_kind;
		int combo;
		int btb;
		int height_sum;
		int height_mxm;
		int ttrp_size;
		int ttrp_ofsY;
		int total_s;
		bool ttrp_able;
		VI next_AI;
		queue<int> q_next_AI;
		VVI field_AI;
		VVI p_field_AI;
		VI height;
		VVI strategy_map;
		vector<strategy> strategy_list;
		VI cmd_list;
		set<cmd_pattern> cp;
		vector<cmd_pattern> cv;
		tetriplate now_ttrp;
		vector<cmd_pattern> ec;
	public:
		game_container();
		~game_container();
		bool set_gc_int(const VI& si);
		bool set_gc_bool(const vector<bool>& sb);
		bool set_gc_VI(const vector<VI>& sv);
		bool set_gc_next(const VI& nx, const queue<int>& qnx);
		bool set_gc_field(const vector<VVI>& sf);
		bool set_gc_ttrp(const tetriplate& st);
		bool add_gc_cmdp(const cmd_pattern& cg);
		cmd_pattern get_gc_cmdp(int itr);

	};

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
		int btb;
		int height_sum;
		int height_mxm;
		int ttrp_size;
		int ttrp_ofsY;
		int exp_cyc_lim;
		bool ttrp_able;
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
		vector<game_container> gc_slot;

		game_container now_gc;

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
		vector<cmd_pattern> search_way(game_container gc, int loop);
		void do_sw(vector<cmd_pattern> &ctl, game_container gc, int loop);
		cmd_pattern explore_choices(game_container &gc_org);
		void get_score(cmd_pattern& cd);
		void get_score(cmd_pattern& cd, game_container& gcs, int loopc);
		LL gs_BFS(cmd_pattern& cb, VVI& qf);
		bool height_calc(game_container& gch);
		bool move_check(int to_x, int to_y, tetri& s_check);
		bool move_check(int to_x, int to_y, tetri& s_check, game_container& ggc);
		bool SRS_check(int lr, tetri& s_now);
		bool SRS_check(int lr, tetri& s_now, game_container& ggc);
		int TS_check(int toX, int toY, tetri& ts);
		int TS_check(int toX, int toY, tetri& ts, game_container& ggc);
		set<int> erase_check_AI(tetri& s_now);
		set<int> erase_check_AI(tetri& s_now, game_container& gce);
		void apply_mino(VVI& c_field, tetri& s_now);
		bool move_mino(tetri& m_now, int s_action);
		bool move_mino(tetri& m_now, int s_action, game_container& ggc);
		void print_gh(tetri& s_now);
		void move_itr(int lr);
		VVI get_AI_field(int p, int m);
		VI get_AI_cmd();
		pair<int, string> get_sttrp_name();
		bool load_ttrp();
		bool ttrp_crr(tetri& now_p, int& size_l);
		bool ttrp_check(cmd_pattern& slc, int& sle, VI& mnL);
		bool ttrp_check(cmd_pattern& slc, int& sle, VI& mnL, game_container& gct);
		bool next_crr_check();
		bool set_gc(game_container &gc);
		game_container update_gc(cmd_pattern& ct, game_container gcp);

		int ttrp_check_mino(tetri& fd, game_container& gcf);

		~shigune_AI();
	};

	// hold : 1, soft : 2, hard : 3, L_rot : 4, R_rot : 5, l_move : 6, r_move : 7;

}

bool GetTempNameList(VS& name_list);
bool ReadTempData(string& name, tetriplate& ttrp);


