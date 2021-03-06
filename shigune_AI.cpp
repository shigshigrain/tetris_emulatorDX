#include "shigune_AI.h"

int i32_zero = 0;
int i32_one = 1;
int i32_minus_one = -1;
const int branch_num = 30;//branch_num >= 20
const vector<int> expl_width = { 30, 30, 25, 25, 20, 20, 15, 15, 10, 10, 10, 10, 10, 10, 10 };
const int cyc_num = 6;//up to 14
const int fh = 45;
const int thd_num = 5;

std::mutex mtx;


//using namespace shig;

namespace shig {

    //

    cmd_pattern::cmd_pattern() {
        score = 1000000000;
        scr = cmd_score();
        index = -1;
        srs = 0, kind = 0;
        ttrp_f = -1;
        pre_gc = 0;
        isSFT = false;
        cmd_list = {};
        pat = tetri();
    }

    cmd_pattern::cmd_pattern(const tetri& p, const VI& list, const int& d) : pat(p), cmd_list(list), index(d) {};

    void cmd_pattern::update() {
        score += scr.get_sum();
        return;
    }

    void cmd_pattern::update(LL u) {
        score += u;
        return;
    }

    void cmd_pattern::set_ttrpF(int& sf) {
        ttrp_f = sf;
    }

    void cmd_pattern::set_isSFT(const bool& sf) {
        isSFT = sf;
    }

    void cmd_pattern::set(LL& s, const tetri& p, VI& list, int& d, int& r, int& k) {
        score = s;
        pat = p;
        cmd_list = list;
        index = d;
        srs = r;
        kind = k;
        //d_line = l;
    }

    void cmd_pattern::set_ts(int r, int k) {
        srs = r;
        kind = k;
        return;
    }

    bool operator < (const cmd_pattern& a, const cmd_pattern& b) {
        return (a.pat < b.pat);
    }

    bool operator == (const cmd_pattern& a, const cmd_pattern& b) {
        return (a.pat == b.pat);
    }


    game_container::game_container() {

        slot_id = 0;
        pre_score = 0;
        hold_AI = 0;
        current_AI = 0;
        next_AI = VI(0);
        q_next_AI = queue<int>();
        pc_cnt = 0;
        SRS_kind = 0;
        TS_kind = 0;
        combo = 0;
        btb = -1;
        height_sum = 0;
        height_mxm = 0;
        ttrp_size = 0;
        ttrp_ofsY = 0;
        total_s = 0;
        ttrp_able = false;
        field_AI = VVI(45, (VI(10, 0)));
        p_field_AI = VVI(45, (VI(10, 0)));
        height = VI(10, 0);
        strategy_map = VVI(45, (VI(10, 0)));
        strategy_list = vector<strategy>(0);
        cv = vector<cmd_pattern>(0);
        cv.reserve(600);
        ec = vector<cmd_pattern>(0);
        ec.reserve(10);
        int z = 0;
        string zs = "nothing";
        now_ttrp.set(z, z, z, z, zs, z);
        VI zv(0, 0);
        now_ttrp.set_id_list(zv);
        vector<pairI2> zp(0, make_pair(0, 0));
        now_ttrp.set_terms(zp);
    }

    bool game_container::set_gc_int(const VI& si) {
        if (si.size() != 12) return false;
        hold_AI     = si[0];
        current_AI  = si[1];
        pc_cnt      = si[2];
        SRS_kind    = si[3];
        TS_kind     = si[4];
        combo       = si[5];
        btb         = si[6];
        height_sum  = si[7];
        height_mxm  = si[8];
        ttrp_size   = si[9];
        ttrp_ofsY   = si[10];
        total_s     = si[11];
        return true;

    }

    bool game_container::set_gc_VI(const vector<VI>& sv) {
        if (sv.size() != 1) return false;
        height = sv[0];
        return true;
    }

    bool game_container::set_gc_bool(const vector<bool>& sb) {
        if (sb.size() != 1) return false;
        ttrp_able = sb[0];
        return true;
    }

    bool game_container::set_gc_next(const VI& nx, const queue<int>& qnx) {
        next_AI = nx;
        q_next_AI = qnx;
        return true;
    }

    bool game_container::set_gc_field(const vector<VVI>& sf) {
        if (sf.size() != 3) return false;
        field_AI = sf[0];
        p_field_AI = sf[1];
        strategy_map = sf[2];
        return true;
    }

    bool game_container::set_gc_ttrp(const tetriplate& st) {
        now_ttrp = st;
        return true;
    }

    bool game_container::add_gc_cmdp(const cmd_pattern& cg) {

        ec.push_back(cg);

        return true;
    }

    cmd_pattern game_container::get_gc_cmdp(int itr) {
        if (itr < 0 || itr >= ec.size()) {
            return cmd_pattern();
        }
        else {
            return ec.at(itr);
        }
    }

    game_container::~game_container() {

    }

    //

	tetri t(0, 3, 21, 0);
    tetriplate now_ttrp;
    tetri null_tetri(0, 0, 0, 0);
    cmd_pattern null_cmd(null_tetri, { 3 }, -1);



	shigune_AI::shigune_AI(int ii) {
        identifier = ii;
		hold_AI = 0;
		current_AI = 0;
        next_AI = VI(0);
		q_next_AI = queue<int>();
		mind = -1;
		pc_cnt = 0;
        SRS_kind = 0;
        TS_kind = 0;
        combo = 0;
        btb = -1;
        height_sum = 0;
        height_mxm = 0;
        ttrp_size = 0;
        ttrp_ofsY = 0;
        exp_cyc_lim = cyc_num;
        ttrp_able = false;
		todo = VI(0);
		field_AI = VVI(45, (VI(10, 0)));
		p_field_AI = VVI(45, (VI(10, 0)));
		H_target = VI(10, -1);
		W_target = VI(10, -1);
        height = VI(10, 0);
		strategy_map = VVI(45, (VI(10, 0)));
		strategy_list = vector<strategy>(0);
		cmd_list = VI(0);
        cmd_list.reserve(40);
        cv = vector<cmd_pattern>(0);
        cv.reserve(100);
        cp_itr = cp.begin();
        ttrp_name_list = VS(10, "");
        ttrp_id_list = VI(0, 0);
        ttrp_list = vector<tetriplate>(0);
        ttrp_bgnF = VI(0, 0);
        int z = 0;
        string zs = "nothing";
        now_ttrp.set(z, z, z, z, zs, z);
        VI zv(0, 0);
        now_ttrp.set_id_list(zv);
        vector<pairI2> zp(0, make_pair(0, 0));
        now_ttrp.set_terms(zp);

        gc_slot = vector<game_container>(branch_num);
        now_gc = game_container();
        null_tetri.minset(6);
        null_cmd.pat.minset(6);


	}


	VI shigune_AI::shigune_main() {

		get_state();
		get_field();
        strategy_mark();

        set_gc(now_gc);

        cmd_pattern select = explore_choices(now_gc);
        
        /*
        if (cp.size() == 0)return VI({3});
        sort(cv.begin(), cv.end(), [&](cmd_pattern const& l, cmd_pattern const& r) {return l.score > r.score; });

        cmd_pattern select = cv.front();
        */
        
        set<int> el = erase_check_AI(select.pat);
        int els = el.size(); 
        

        VI empty = { 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 };
        VVI proxy(0); proxy.reserve(45);
        shig_rep(i, 45) {
            decltype(el)::iterator it = el.find(i);
            if (it == el.end()) proxy.push_back(strategy_map[i]);
        }
        while (proxy.size() < 45) proxy.push_back(empty);
        strategy_map = proxy;

        VI mnL(8, 8);
        mnL[current_AI] = 0;
        mnL[hold_AI] = -1;
        shig_rep(i, min({ 5, (int)next_AI.size() })) {
            mnL[next_AI[i]] = i + 1;
        }

        bool all_TF = next_crr_check();

        shigune_AI::ttrp_check(select, els, mnL);

		return select.cmd_list;
	}

	VI shigune_AI::get_cmd() {
		// hold : 1, soft : 2, hard : 3, L_rot : 4, R_rot : 5, l_move : 6, r_move : 7;
        VI cmd = {};

		return cmd;
	}

	void shigune_AI::get_field() {
		shig_rep(i, 45) {
			shig_rep(j, 10) {
				field_AI[i][j] = get_field_state(i, j, 1);
                p_field_AI[i][j] = get_field_state(i, j, 1);
			}
		}
        height_sum = 0;
        height_mxm = 0;
        shig_rep(j, 10) {
            int h = 44;
            while (h >= 0) {
                if (field_AI[h][j] == 0) {
                    height[j] = h;
                    h--;
                }
                else {
                    break;
                }
            }
            h++;
            height_sum += h;
            height_mxm = max(height_mxm, h);
        }

		return;
	}

	void shigune_AI::get_state() {
		t = get_current();
        current_AI = t.id;
		pair<int, queue<int>> temp = get_mino_state();
		hold_AI = temp.first;
		q_next_AI = temp.second;
        next_AI.clear();
        while (!temp.second.empty()) {
            int q = temp.second.front();
            next_AI.push_back(q);
            temp.second.pop();
        }
        auto[combo, btb] = get_combo();
		return;
	}

	bool shigune_AI::strategy_mark() {

		if (pc_check())bgn_strategy();

		return true;
	}

	bool shigune_AI::pc_check() {
		shig_rep(i, 10) {
			if (field_AI[0][i] != 0)return false;
		}
		return true;
	}

	void shigune_AI::bgn_strategy() {
		// I J L O S T Z
        strategy_list.clear();
        //bool all_TF = false;
        VI mnL(8, 8);

        mnL[current_AI] = 0;
        mnL[hold_AI] = -1;
        shig_rep(i, min({ 5, (int)next_AI.size() }) ) {
            mnL[next_AI[i]] = i + 1;
        }

        bool all_TF = next_crr_check();

        shig_rep(i, 45) shig_rep(j, 10) strategy_map[i][j] = 0;
        bool decide = false;
        
        if (all_TF) {
            ttrp_able = true;
        }
        else {
            ttrp_able = false;
        }

        if (ttrp_able) {

            shig_rep(i, ttrp_size) {

                if (ttrp_list[i].bgn_f != 1)continue;
                int tm_cnt = 0;
                shig_rep(j, ttrp_list[i].terms_num) {
                    int tL = ttrp_list[i].terms[j].first;
                    int tR = ttrp_list[i].terms[j].second;
                    if (mnL[tL] < mnL[tR])tm_cnt++;
                }

                if (tm_cnt >= ttrp_list[i].terms_num) {
                    now_ttrp = ttrp_list[i];
                    break;
                }


            }


        }

        
        /*
        bool HK_TF = true;
        if (HK_TF && !decide) {
            if (mnL[2] < 4) {
                shig_rep(i, HK_ZZ.size()) {
                    strategy_map[i] = HK_ZZ[i];
                }
                decide = true;
            }
            else if (mnL[3] < 4) {
                shig_rep(i, HK_SS.size()) {
                    strategy_map[i] = HK_SS[i];
                }
                decide = true;
            }
        }

        */
        
        /*
        if (current_AI == 1 || hold_AI == 1 || next_AI[0] == 1|| next_AI[2] == 1) {
			if (current_AI == 2 || hold_AI == 2 || next_AI[0] == 2 || next_AI[2] == 2) {
                shig_rep(i, TSD_J.size()) {
                    strategy_map[i] = TSD_J[i];
                }
			}else if (current_AI == 3 || hold_AI == 3 || next_AI[0] == 3 || next_AI[2] == 3) {
                shig_rep(i, TSD_L.size()) {
                    strategy_map[i] = TSD_L[i];
                }
			}
			
		}
        */
		
		return;
	}

    void shigune_AI::search_way() {
        p_field_AI = field_AI;
        constexpr int mxm = 400;
        VVI search_tree(mxm, VI(0));
        VI parent_tree(mxm, 0);
        int to = 0;
        int parent = 0;
        cp.clear();
        cv.clear();

        shig_rep(i, base_cmd.size()) {
            search_tree[i] = base_cmd[i];
            parent_tree[i] = i;
            to++;
        }

        int w = 0;
        while (!search_tree[w].empty() && (w < mxm - 1)) {
            bool can = true;
            int w_size = search_tree[w].size();
            tetri test = t;
            if (parent_tree[w] == w) {
                test = get_current();
                shig_rep(i, search_tree[w].size()-1) {
                    if (!move_mino(test, search_tree[w][i])) {
                        can = false;
                        break;
                    }
                }
                if (!can) {
                    w++;
                    continue;
                }
                int sft = -1;
                int hd_cnt = 0;
                while (move_check(0, sft*(hd_cnt + 1), test)) hd_cnt++;
                move_mino(test, 3);
                cmd_pattern c(test, search_tree[w], parent_tree[w]);
                decltype(cp)::iterator it = cp.find(c);
                if (it != cp.end()) {
                    w++;
                    continue;
                }
                get_score(c);
                cv.push_back(c);
                cp.insert(c);
                VI w_sft = search_tree[w];
                w_sft.pop_back();
                shig_rep(i, hd_cnt)w_sft.push_back(2);
                search_tree[to] = w_sft; parent_tree[to] = parent_tree[w]; to++;
                w++;
            }
            else {
                if (search_tree[w].back() != 3) {
                    const VI test_case = { 6, 7, 4, 5 };
                    shig_rep(h, test_case.size()) {
                        test = get_current();
                        can = true;
                        VI add_tree = search_tree[w];
                        add_tree.push_back(test_case[h]);
                        shig_rep(i, add_tree.size()) {
                            if (!move_mino(test, add_tree[i])) {
                                can = false;
                                break;
                            }
                        }
                        if (!can) continue;
                        int sft = -1; int hd_cnt = 0;
                        while (move_check(0, sft * (hd_cnt + 1), test)) hd_cnt++;
                        move_mino(test, 3);
                        add_tree.push_back(3);
                        cmd_pattern c(test, add_tree, w);
                        decltype(cp)::iterator it = cp.find(c);
                        if (it != cp.end()) continue;
                        cp.insert(c);
                        get_score(c);
                        cv.push_back(c);
                        add_tree.pop_back();
                        shig_rep(i, hd_cnt)add_tree.push_back(2);
                        if (to < mxm - 1) {
                            search_tree[to] = add_tree;
                            parent_tree[to] = parent_tree[w];
                            to++;
                        }
                    }
                    w++;
                }
                else {
                    w++;
                }
            }
        }

        cp_itr = cp.begin();

        return;
    }

    vector<cmd_pattern> shigune_AI::search_way(game_container gc, int loop) {
        
        constexpr int mxm = 200;
        
        vector<cmd_pattern> pcv(0);
        
        if (gc.current_AI == 0)return pcv;
        gc.p_field_AI = gc.field_AI;
        VI rsv(0); rsv.reserve(30);
        VVI search_tree(mxm, rsv);
        VI parent_tree(mxm, 0);
        int to = 0, parent = 0;
        gc.cp.clear(); gc.cv.clear();

        shig_rep(i, base_cmd.size()) {
            search_tree[i] = base_cmd[i];
            parent_tree[i] = i;
            to++;
        }

        int w = 0;
        tetri test;
        while (!search_tree[w].empty() && (w < mxm - 1)) {
            bool can = true;
            int w_size = search_tree[w].size();

            
            if (parent_tree[w] == w) {
                test.minset(gc.current_AI);
                shig_rep(i, search_tree[w].size() - 1) {
                    if (!move_mino(test, search_tree[w][i], gc)) {
                        can = false;
                        break;
                    }
                }
                if (!can) {
                    w++;
                    continue;
                }
                int sft = -1, hd_cnt = 0;
                while (move_check(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
                move_mino(test, 3, gc);
                cmd_pattern c(test, search_tree[w], parent_tree[w]);
                decltype(gc.cp)::iterator it = gc.cp.find(c);
                if (it != gc.cp.end()) {
                    w++;
                    continue;
                }
                get_score(c, gc, loop);
                c.pre_gc = gc.slot_id;
                pcv.push_back(c);
                gc.cp.insert(c);
                VI w_sft = search_tree[w];
                w_sft.pop_back();
                shig_rep(i, hd_cnt)w_sft.push_back(2);
                search_tree[to] = w_sft; parent_tree[to] = parent_tree[w]; to++;
                w++;
            }
            else {
                if (search_tree[w].back() != 3) {
                    const VI test_case = { 6, 7, 4, 5 };
                    shig_rep(h, test_case.size()) {
                        test.minset(gc.current_AI);
                        can = true;
                        VI add_tree = search_tree[w];
                        add_tree.push_back(test_case[h]);
                        shig_rep(i, add_tree.size()) {
                            if (!move_mino(test, add_tree[i], gc)) {
                                can = false;
                                break;
                            }
                        }
                        if (!can) continue;

                        int sft = -1; int hd_cnt = 0;
                        while (move_check(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
                        move_mino(test, 3, gc);
                        add_tree.push_back(3);
                        cmd_pattern c(test, add_tree, w); c.set_isSFT(true);
                        decltype(gc.cp)::iterator it = gc.cp.find(c);
                        if (it != gc.cp.end()) continue;
                        get_score(c, gc, loop);
                        gc.cp.insert(c);
                        add_tree.pop_back();
                        c.pre_gc = gc.slot_id;
                        pcv.push_back(c);
                        shig_rep(i, hd_cnt)add_tree.push_back(2);
                        if (to < mxm - 1) {
                            search_tree[to] = add_tree;
                            parent_tree[to] = parent_tree[w];
                            to++;
                        }
                    }
                    w++;
                }
                else {
                    w++;
                }
            }
        }


        return pcv;


    }

    void shigune_AI::do_sw(vector<cmd_pattern> &ctl, game_container gc, int loop) {

        //vector<pair<cmd_pattern, int>> pcv(0);
        //pcv.reserve(100);
        //pcv.emplace_back(search_way(gc, loop));

        constexpr int mxm = 200;
        vector<cmd_pattern> pcv(0);

        if (gc.current_AI == 0)return;
        gc.p_field_AI = gc.field_AI;
        VI rsv(0); rsv.reserve(30);
        VVI search_tree(mxm, rsv);
        VI parent_tree(mxm, 0);
        int to = 0, parent = 0;
        gc.cp.clear(); gc.cv.clear();

        shig_rep(i, base_cmd.size()) {
            search_tree[i] = base_cmd[i];
            parent_tree[i] = i;
            to++;
        }

        int w = 0;
        tetri test;
        while (!search_tree[w].empty() && (w < mxm - 1)) {
            bool can = true;
            int w_size = search_tree[w].size();


            if (parent_tree[w] == w) {
                test.minset(gc.current_AI);
                shig_rep(i, search_tree[w].size() - 1) {
                    if (!move_mino(test, search_tree[w][i], gc)) {
                        can = false;
                        break;
                    }
                }
                if (!can) {
                    w++;
                    continue;
                }
                int sft = -1, hd_cnt = 0;
                while (move_check(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
                move_mino(test, 3, gc);
                cmd_pattern c(test, search_tree[w], parent_tree[w]);
                decltype(gc.cp)::iterator it = gc.cp.find(c);
                if (it != gc.cp.end()) {
                    w++;
                    continue;
                }
                get_score(c, gc, loop);
                c.pre_gc = gc.slot_id;
                pcv.push_back(c);
                gc.cp.insert(c);
                VI w_sft = search_tree[w];
                w_sft.pop_back();
                shig_rep(i, hd_cnt)w_sft.push_back(2);
                search_tree[to] = w_sft; parent_tree[to] = parent_tree[w]; to++;
                w++;
            }
            else {
                if (search_tree[w].back() != 3) {
                    const VI test_case = { 6, 7, 4, 5 };
                    shig_rep(h, test_case.size()) {
                        test.minset(gc.current_AI);
                        can = true;
                        VI add_tree = search_tree[w];
                        add_tree.push_back(test_case[h]);
                        shig_rep(i, add_tree.size()) {
                            if (!move_mino(test, add_tree[i], gc)) {
                                can = false;
                                break;
                            }
                        }
                        if (!can) continue;

                        int sft = -1; int hd_cnt = 0;
                        while (move_check(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
                        move_mino(test, 3, gc);
                        add_tree.push_back(3);
                        cmd_pattern c(test, add_tree, w); c.set_isSFT(true);
                        decltype(gc.cp)::iterator it = gc.cp.find(c);
                        if (it != gc.cp.end()) continue;
                        get_score(c, gc, loop);
                        gc.cp.insert(c);
                        add_tree.pop_back();
                        c.pre_gc = gc.slot_id;
                        pcv.push_back(c);
                        shig_rep(i, hd_cnt)add_tree.push_back(2);
                        if (to < mxm - 1) {
                            search_tree[to] = add_tree;
                            parent_tree[to] = parent_tree[w];
                            to++;
                        }
                    }
                    w++;
                }
                else {
                    w++;
                }
            }
        }


        std::lock_guard<std::mutex> lock(mtx);
        for (cmd_pattern p : pcv) {
            ctl.push_back(p);
        }
        
        return;
    }

    cmd_pattern shigune_AI::explore_choices(game_container& gc_org) {

        gc_slot = vector<game_container>(branch_num);
        vector<vector<cmd_pattern>> branch(branch_num, vector<cmd_pattern>(0));
        vector<cmd_pattern> catalog(0);
        constexpr int cls = 100 * branch_num;
        catalog.reserve(cls);

        gc_org.cp.clear(); gc_org.cv.clear();
        //catalog.clear();
        
        //catalog.emplace_back(search_way(gc_org, -1));
        do_sw(ref(catalog), gc_org, -1);

        sort(all(catalog), [&](const cmd_pattern &l, const cmd_pattern &r) { return r.scr < l.scr; });

        if (catalog.size() == 0) {
            return null_cmd;
        }
        else if (catalog.size() < branch_num && catalog.size() > 0) {
            for (int i = 0; i < catalog.size(); i++) {
                //auto [pct, ci] = catalog.at(i);

                branch.at(i).push_back(catalog.at(i));
                gc_slot.at(i) = update_gc(catalog.at(i), gc_org);
                gc_slot.at(i).slot_id = i;
            }
            for (int i = catalog.size(); i < branch_num; i++) {
                gc_slot.at(i) = game_container();
                gc_slot.at(i).slot_id = i;
            }
        }
        else {
            shig_rep(i, branch_num) {
                //auto [pct, ci] = catalog.at(i);
                branch.at(i).push_back(catalog.at(i));
                gc_slot.at(i) = update_gc(catalog.at(i), gc_org);
                gc_slot.at(i).slot_id = i;
            }
        }

        shig_rep(n, exp_cyc_lim - 1) {
            catalog.clear(); catalog.reserve(cls);


            int loop = n;

            int epwtn = expl_width.at(n);
            std::vector<std::thread> threads;

            /*
            for (int i = 0; i < epwtn; i += thd_num) {
                int rep_num = min((epwtn - i), thd_num);
                for (int j = 0; j < rep_num; j++) {

                    //std::thread expl(do_sw, catalog, gc_slot.at(i + j), n);
                    game_container gc = gc_slot.at(i + j);

                    //threads.emplace_back(std::thread(do_sw, gc, loop));

                    threads.emplace_back(std::thread([this, &catalog, gc, loop]() {this->do_sw(ref(catalog), gc, loop); }));

                }

                for (auto& thr : threads) {
                    thr.join();
                }
                threads.clear();
            }
            */

            for (int i = 0; i < epwtn; i++) {
                game_container gc = gc_slot.at(i);
                threads.emplace_back(std::thread([this, &catalog, gc, loop]() {this->do_sw(ref(catalog), gc, loop); }));
            }

            for (auto& thr : threads) {
                thr.join();
            }


            sort(all(catalog), [&](const cmd_pattern &l, const cmd_pattern &r) { return r.scr < l.scr; });
            vector<game_container> proxy_slot(expl_width.at(n));
            vector<vector<cmd_pattern>> proxy_branch(expl_width.at(n), vector<cmd_pattern>(0));
            shig_rep(i, expl_width.at(n)) {
                if (catalog.size() <= i || catalog.size() == 0) {
                    branch.at(i).push_back(null_cmd);
                    proxy_slot.at(i) = update_gc(null_cmd, gc_slot.at(0));
                    proxy_slot.at(i).slot_id = i;
                }
                else {
                    //auto [pct, ci] = catalog.at(i);
                    int ci = catalog.at(i).pre_gc;
                    proxy_branch.at(i) = branch.at(ci);
                    proxy_branch.at(i).push_back(catalog.at(i));
                    proxy_slot.at(i) = update_gc(catalog.at(i), gc_slot.at(ci));
                    proxy_slot.at(i).slot_id = i;
                }
                
            }
            gc_slot = proxy_slot;
            branch = proxy_branch;

        }

        if (catalog.size() == 0)return null_cmd;
        //auto [r, ri] = catalog.at(0);
        int ri = catalog.at(0).pre_gc;
        return branch.at(ri).front();


    }

    void shigune_AI::get_score(cmd_pattern& cd) {
        
        cd.set_ts(SRS_kind, TS_kind);
        set<int> L = erase_check_AI(cd.pat);
        LL fusion = 0;
        LL contact = 0; 
        LL touch = 0;
        LL high = cd.pat.Y;
        LL cmb = 0;

        int rot = cd.pat.rot;
        int idnt = cd.pat.id - 1;
        int size = cd.pat.px_size;
        int H = cd.pat.mino[rot].size();
        int W = cd.pat.mino[rot][0].size();

        //fusion score
        /*
        shig_rep(i, H) {
            shig_rep(j, W) {
                int sX = cd.pat.X + j;
                if (sX < 0 || sX >= 10)continue;
                int sY = cd.pat.Y - i - 1;
                if (sY < 0 || sY >= (field_AI.size() - 1))continue;
                if (strategy_map[sY][sX] == cd.pat.mino[rot][i][j] && strategy_map[sY][sX] != 0) {
                    fusion += 100;
                }
                else {
                    if (strategy_map[sY][sX] == -1 && cd.pat.mino[rot][i][j] != 0) {
                        fusion -= 100;
                    }
                    else if (strategy_map[sY][sX] > 0 && cd.pat.mino[rot][i][j] != 0) {
                        fusion -= 50;
                    }
                    else if (strategy_map[sY][sX] > 0 && cd.pat.mino[rot][i][j] == 0) {
                        //fusion += 500;
                    }
                }
            }
        }
        
        */

        //contact
        shig_rep(j, W) {
            if (ch[idnt][rot][j] == 1) {
                int xp = cd.pat.X + j;
                int h = cd.pat.Y - height[xp] - 1;
                if (h >= 0 && h < H) {
                    if (cd.pat.mino[rot][h][j] != 0)contact += 4;
                    else contact -= 10;
                }
                else contact -= 10;

            }
        }
        
        
        //high
        if (high > 8) {
            high = (-1 * high * high);
        }
        else {
            high = (high * -9);
        }
        //high *= -1000;
        

        //closure & touch
        shig_rep(i, H) {
            shig_rep(j, W) {
                if (cd.pat.mino[rot][i][j] == 0)continue;
                int sX = cd.pat.X + j;
                if (sX < 0 || sX >= 10)continue;
                int sY = cd.pat.Y - i - 1;
                if (sY < 0 || sY >= (field_AI.size() - 1))continue;
                apply_mino(p_field_AI, cd.pat);
                set<pairI2> p_check;
                shig_rep(k, cy.size())shig_rep(l, cx.size()) {
                    if (k == 0 && l == 0)continue;
                    int ssX = cd.pat.X + j + cx[k];
                    if (ssX < 0 || ssX >= 10) {
                        touch += 10;
                        continue;
                    }
                    int ssY = cd.pat.Y - i - 1 + cy[l];
                    if (ssY < 0 || ssY >= (field_AI.size() - 1)) {
                        touch += 10;
                        continue;
                    }
                    if (field_AI[ssY][ssX] != 0)touch += 30;
                    else touch -= 30;

                    if (p_field_AI[ssY][ssX] != 0)continue;

                    int closure = 0;
                    shig_rep(m, cc.size()) {
                        int sssX = cd.pat.X + j + cx[k] +cc[m].first;
                        if (sssX < 0 || sssX >= 10) {
                            closure++;
                            continue;
                        }
                        int sssY = cd.pat.Y - i - 1 + cy[l] + cc[m].second;
                        if (sssY < 0 || sssY >= (field_AI.size() - 1)) {
                            closure++;
                            continue;
                        }
                        if (p_field_AI[sssY][sssX] != 0)closure++;
                    }

                    if (closure == 4)touch -= 100;


                }

            }
        }

        //cmb
        LL cmb_rate = 100;
        if (combo * L.size() > 0) {
            cd.update(cmb_rate * 10LL);
        }
        else if(combo == 0 && L.size() == 0){
            cd.update(cmb_rate);
        }
        else {
            cd.update(cmb_rate * -1LL);
        }

        auto chk_PC = [&] {
            bool cnt_pc = true;
            shig_rep(i, 10) {
                if (p_field_AI[0][i] != 0) {
                    cnt_pc = false;
                    break;
                }
            }
            return cnt_pc;
        }();

        if (chk_PC)cd.update(114514810);

        
        int p_A = 128;
        LL ve = 0;
        if (L.size() == 0) {
            
            if (height_sum > p_A || height_mxm > 13) {
                ve -= 500;
            }
            else {
                ve += 20;
            }
        }
        else if (L.size() == 1) {
            if (TS_kind == 1) {
                
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 500;
                }
                else {
                    ve += 80;
                }
            }
            else if (TS_kind == 2) {
                
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 40;
                }
                else {
                    ve += 160;
                }
            }
            else {
                
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 60;
                }
                else {
                    ve += 30;
                }
            }
        }
        else if (L.size() == 2) {
            if (TS_kind == 1) {
                
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 150;
                }
                else {
                    ve += 200;
                }
            }
            else if (TS_kind == 2) {
                ve += 1;
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 5;
                }
            }
            else {
                
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 100;
                }
                else {
                    ve += 50;
                }
            }
        }
        else if (L.size() == 3) {
            if (TS_kind == 1) {
                //ve += 400;
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 300;
                }
                else {
                    ve += 400;
                }
            }
            else if (TS_kind == 2) {
                ve += 0;
                if (height_sum > p_A || height_mxm > 13) {
                    ve -= 0;
                }
            }
            else {
                
                if (height_sum > p_A || height_mxm > 13) {
                    ve += 120;
                }
                else {
                    ve += 10;
                }
            }
        }
        else if (L.size() == 4) {
            ve += 2000;
            if (height_sum > p_A || height_mxm > 13) {
                ve += 3000;
            }
        }

        
        LL ttrp_rate = 100000;

        bool chk_f = false;
        tetri ofs_cdp = cd.pat;
        ofs_cdp.addY(ttrp_ofsY);


        if (ttrp_able) {
            shig_rep(i, min({ now_ttrp.mino_num , (int)now_ttrp.list_mino.size()})) {
                if (now_ttrp.list_mino[i] == ofs_cdp) {
                    if (cd.pat.id == 6) {
                        if ((now_ttrp.list_mino_s[i]) == L.size() && TS_kind == 1) {
                            chk_f = true;
                        }
                        else if (now_ttrp.list_mino_s[i] == 0 && TS_kind == 0){
                            chk_f = true;
                        }
                        else {
                            chk_f = false;
                            cd.update(ttrp_rate * -1LL);
                        }
                    }
                    else {
                        chk_f = true;
                    }

                    break;

                }
                else {
                    chk_f = false;
                }
            }

        }

        if (chk_f == true) {
            cd.update(ttrp_rate);
            cd.set_ttrpF(cd.pat.id);
            return;
        }
        else {
            LL pnl = -100;
            cd.update(pnl);
        }

        fusion *= 100;
        cd.update(fusion);

        LL ve_rate = 100;
        if (height_sum > p_A || height_mxm > 11) {
            ve_rate *= 10;
        }
        ve *= ve_rate;
        cd.update(ve);

        touch *= 50;
        cd.update(touch);

        cd.update(high);

        contact *= 50;
        cd.update(contact);

        return;
    }

    void shigune_AI::get_score(cmd_pattern& cd, game_container& gcs, int loopc) {

        cd.scr.init_s();//without sum

        //gcs.p_field_AI = gcs.field_AI;
        //shigune_AI::apply_mino(gcs.p_field_AI, cd.pat); ??????????????????
        set<int> L = erase_check_AI(cd.pat, gcs);

        //shigune_AI::height_calc(gcs);

        cd.set_ts(gcs.SRS_kind, gcs.TS_kind);

        VVI q_field_AI(0); q_field_AI.reserve(45);
        if (L.size() > 0) {
            shig_rep(i, fh) {
                decltype(L)::iterator it = L.find(i);
                if (it == L.end()) q_field_AI.push_back(gcs.p_field_AI[i]);
            }
            while (q_field_AI.size() < fh) q_field_AI.push_back(ev_empty);
        }
        else {
            q_field_AI = VVI(gcs.p_field_AI);
        }  
        
        LL fusion = 0;
        LL contact = 0;
        LL touch = 0;
        LL high = 0;
        LL cmb = 0;
        LL btbc = 0;

        int rot = cd.pat.rot;
        int idnt = cd.pat.id - 1;
        int size = cd.pat.px_size;
        int H = cd.pat.mino[rot].size();
        int W = cd.pat.mino[rot][0].size();
        int Ls = L.size();

        constexpr int hm = 10;
        constexpr int hs = hm * 8;

        bool isPC = true;
        if (Ls > 0) {
            for (int i = 0; i < 10; i++) {
                if (q_field_AI.at(0).at(i) != 0) {
                    isPC = false;
                    break;
                }
            }
        }
        else {
            isPC = false;
        }

        if (isPC)cd.scr.sum = shig::secure_add(1145141919810000LL, cd.scr.sum);

        LL ttrp_rate = 1000000000000000;

        bool chk_f = false;
        tetri ofs_cdp = cd.pat;
        ofs_cdp.addY(gcs.ttrp_ofsY);

        if (gcs.ttrp_able) {
            for (int i = 0; i < (int)gcs.now_ttrp.list_mino.size(); i++) {
                if (gcs.now_ttrp.list_mino.at(i) == ofs_cdp) {
                    if (cd.pat.id == 6) {
                        if (gcs.now_ttrp.list_mino_s[i] == 0 && gcs.TS_kind == 0) {
                            chk_f = true;
                            cd.set_ttrpF(i);
                        }
                        else if ((gcs.now_ttrp.list_mino_s[i]) == L.size() && gcs.TS_kind == 1) {
                            chk_f = true;
                            cd.set_ttrpF(i);
                        }
                        else {
                            chk_f = false;
                            cd.set_ttrpF(i32_minus_one);
                            //cd.update(ttrp_rate * -1LL);
                        }
                    }
                    else {
                        chk_f = true;
                        cd.set_ttrpF(i);
                    }

                    break;

                }
                else {
                    chk_f = false;
                    cd.set_ttrpF(i32_minus_one);
                }
            }

        }

        if (chk_f == true) {
            //cd.update(ttrp_rate);
            cd.scr.sum = shig::secure_add(ttrp_rate, cd.scr.sum);
            cd.scr.sum = shig::secure_add(gcs.pre_score, cd.scr.sum);
            cd.score = shig::secure_add(cd.score, cd.scr.sum);
            //cd.set_ttrpF(cd.pat.id);
            return;
        }
        else {
            LL pnl = -10000000;
            cd.scr.sum = shig::secure_add(cd.scr.sum, pnl);
        }

        //fusion score
        /*
        shig_rep(i, H) {
            shig_rep(j, W) {
                int sX = cd.pat.X + j;
                if (sX < 0 || sX >= 10)continue;
                int sY = cd.pat.Y - i - 1;
                if (sY < 0 || sY >= (field_AI.size() - 1))continue;
                if (strategy_map[sY][sX] == cd.pat.mino[rot][i][j] && strategy_map[sY][sX] != 0) {
                    fusion += 100;
                }
                else {
                    if (strategy_map[sY][sX] == -1 && cd.pat.mino[rot][i][j] != 0) {
                        fusion -= 100;
                    }
                    else if (strategy_map[sY][sX] > 0 && cd.pat.mino[rot][i][j] != 0) {
                        fusion -= 50;
                    }
                    else if (strategy_map[sY][sX] > 0 && cd.pat.mino[rot][i][j] == 0) {
                        //fusion += 500;
                    }
                }
            }
        }

        */



        //contact.V1
        int cnt_m = 0, cnt_n = 0;
        vector<bool> chk_con(W, false);
        shig_rep(j, W) {
            if (ch.at(idnt).at(rot).at(j) == 1) {
                cnt_m++;
                int xp = cd.pat.X + j;
                int h  = cd.pat.Y - gcs.height.at(xp) - 1;
                if (h >= 0 && h < H) {
                    if (cd.pat.mino.at(rot).at(h).at(j) != 0) {
                        cnt_n++;
                    }
                    else {
                        if (h >= 1) {
                            if (cd.pat.mino.at(rot).at(h - 1).at(j) != 0) {
                                chk_con.at(j) = true;
                                contact -= 200;
                            }
                            else {
                                contact -= 5000;
                            }
                        }
                        
                    }
                }
                else if (h >= H) {
                    contact -= 10000;
                }

            }
        }

        // contact V2
        const LL pnl_A = 2000, pnl_B = -1000, pnl_C = -2000;

        if (cd.pat.id == 1) {
            if (cd.pat.rot == 0 || cd.pat.rot == 2) {
                if (cnt_n == 4)contact += pnl_A;
                else if (cnt_n == 3 && (chk_con.at(0) == true || chk_con.at(3) == true))contact += pnl_B;
                else contact += pnl_C;
            }
            else {
                if (cnt_n == 1)contact += pnl_A;
                else contact += pnl_C;
            }
        }
        else if (cd.pat.id == 2) {
            if (cd.pat.rot == 0 || cd.pat.rot == 2) {
                if (cnt_n == 3)contact += pnl_A;
                else contact += pnl_C;
            }
            else {
                if (cnt_n == 2)contact += pnl_A;
                else contact += pnl_C;
            }
        }
        else if (cd.pat.id == 3) {
            if (cd.pat.rot == 0 || cd.pat.rot == 2) {
                if (cnt_n == 3)contact += pnl_A;
                else if (cnt_n == 2 && chk_con.at(0) == true)contact += pnl_B;
                else contact += pnl_C;
            }
            else {
                if (cnt_n == 2)contact += pnl_A;
                else contact += pnl_C;
            }
        }
        else if (cd.pat.id == 4) {
            if (cnt_n == 2)contact += pnl_A;
            else contact += pnl_C;
        }
        else if (cd.pat.id == 5) {
            if (cd.pat.rot == 0 || cd.pat.rot == 2) {
                if (cnt_n == 3)contact += pnl_A;
                else if (cnt_n == 2 && chk_con.at(2) == true)contact += pnl_B;
                else contact += pnl_C;
            }
            else {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1 && chk_con.at(1) == true)contact += pnl_B;
                else contact += pnl_C;
            }
        }
        else if (cd.pat.id == 6) {
            if (cd.pat.rot == 0 || cd.pat.rot == 2) {
                if (cnt_n == 3)contact += pnl_A;
                else if (cnt_n == 2 && (chk_con.at(0) == true || chk_con.at(2) == true))contact += pnl_B;
                else contact += pnl_C;
            }
            else if (cd.pat.rot == 1) {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1 && chk_con.at(2) == true)contact += pnl_B;
                else contact += pnl_C;
            }
            else if (cd.pat.rot == 3) {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1 && chk_con.at(0) == true)contact += pnl_B;
                else contact += pnl_C;
            }

        }
        else if (cd.pat.id == 7) {
            if (cd.pat.rot == 0 || cd.pat.rot == 2) {
                if (cnt_n == 3)contact += pnl_A;
                else if (cnt_n == 2 && chk_con.at(0) == true)contact += pnl_B;
                else contact += pnl_C;
            }
            else {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1 && chk_con.at(1) == true)contact += pnl_B;
                else contact += pnl_C;
            }
        }


        //high
        
        LL cdY = cd.pat.Y;

        if (cdY >= 14) {
            high = (-10 * cdY * cdY * cdY);
        }
        else if(cdY >= 7) {
            high = (-10 * cdY * cdY);

            if (gcs.height_sum > hs && gcs.height_mxm > hm) {
                high += 100 * (6 - cdY);
            }

        }
        else {
            high = (-10LL * cdY);

            if (gcs.height_sum > hs && gcs.height_mxm > hm) {
               high += 500 * (7 - cdY);
            }

        }

        //closure & touch
        int tls = touch_list.at(idnt).at(rot).size();
        LL thc = 0, thcls = 0;
        for (int i = 0; i < tls; i++) {
            auto [ty, tx] = touch_list.at(idnt).at(rot).at(i);
            tx = cd.pat.X + tx;
            if (tx < 0 || tx >= 10) {
                thc++;
                continue;
            }

            ty = cd.pat.Y - ty - 1;
            if (ty < 0 || ty >= fh - 1) {
                thc++;
                continue;
            }

            if (gcs.p_field_AI[ty][tx] != 0) {
                thc++;

                if (tx == 0 || tx == 9) {
                    //touch = shig::secure_add(touch, 100);
                }

                continue;
            }
            else {
                if (tx == 0 || tx == 9) {
                    //touch = shig::secure_add(touch, -100);
                }

            }


            int closure = 0;
            for (int k = 0; k < cc.size(); k++) {
                int yk = cc.at(k).first + ty;
                int xk = cc.at(k).second + tx;
;
                if (xk < 0 || xk >= 10) {
                    closure++;
                    continue;
                }
                if (yk < 0 || yk >= fh - 1) {
                    closure++;
                    continue;
                }

                if (gcs.p_field_AI.at(yk).at(xk) != 0) {
                    closure++;
                    continue;
                }
                else {
                    continue;
                }

            }

            if (closure >= 4) {
                touch = shig::secure_add(touch, -8000);
                thcls++;
            }
            else {
                touch = shig::secure_add(touch, -1000 * closure);
            }

        }

        touch = shig::secure_add(touch, thc * 1000);

        if (thcls == 0) {
            touch = shig::secure_add(touch, 10000);
        }

        /*
        
        shig_rep(i, H) {
            shig_rep(j, W) {
                if (cd.pat.mino[rot][i][j] == 0)continue;
                int sX = cd.pat.X + j;
                if (sX < 0 || sX >= 10)continue;
                int sY = cd.pat.Y - i - 1;
                if (sY < 0 || sY >= fh - 1)continue;

                for (int k = 0; k < cc.size(); k++) {
                    int yk = cc.at(k).first  + sY;
                    int xk = cc.at(k).second + sX;


                }

                shig_rep(k, cy.size())shig_rep(l, cx.size()) {
                    if (k == 0 && l == 0)continue;
                    int ssX = cd.pat.X + j + cx[k];
                    if (ssX < 0 || ssX >= 10) {
                        touch += 20;
                        if (cd.pat.id == 1)touch += 500;
                        continue;
                    }
                    int ssY = cd.pat.Y - i - 1 + cy[l];
                    if (ssY < 0 || ssY >= (gcs.p_field_AI.size() - 1)) {
                        touch += 20;
                        if (cd.pat.id == 1)touch += 500;
                        continue;
                    }
                    if (gcs.p_field_AI[ssY][ssX] != 0) {
                        touch += 20;
                    }
                    else {
                        touch += -15;
                        if (cd.pat.id == 1)touch += 10;
                    }

                    if (gcs.p_field_AI[ssY][ssX] != 0)continue;

                    int closure = 0;
                    shig_rep(m, cc.size()) {
                        int sssX = cd.pat.X + j + cx[k] + cc[m].first;
                        if (sssX < 0 || sssX >= 10) {
                           closure++;
                           continue;
                        }
                        int sssY = cd.pat.Y - i - 1 + cy[l] + cc[m].second;
                        if (sssY < 0 || sssY >= (gcs.field_AI.size() - 1)) {
                            closure++;
                            continue;
                        }
                        if (gcs.p_field_AI[sssY][sssX] != 0)closure++;
                    }

                if (closure >= 4)touch -= 2000;

                }

            }
        }

        */
        

        //cmb
        LL cmb_rate = 100;
        if (gcs.combo * L.size() > 0) {
            LL c = (cmb_rate * 10LL * (gcs.combo + 1));
            cd.scr.cmb = shig::secure_add(cd.scr.cmb, c);

        }
        else if (gcs.combo > 0 && L.size() == 0) {
            LL c = (cmb_rate * -10LL * (gcs.combo + 1));
            cd.scr.cmb = shig::secure_add(cd.scr.cmb, c);
        }
        else if (gcs.combo == 0 && L.size() == 0) {
            LL c = cmb_rate;
            cd.scr.cmb = shig::secure_add(cd.scr.cmb, c);
        }
        else {
            //cd.update(gcs.combo * 0LL);
        }

        int p_A = 80;

        

        LL ve = 0;
        if (L.size() == 0) {

            if (gcs.height_sum > 96 && gcs.height_mxm > 12) {
                ve += 50;

            }
            else if (gcs.height_sum > hs && gcs.height_mxm > hm) {
                ve += 150;
                if (cd.pat.id == 6)ve -= 200;
                else if (cd.pat.id == 1)ve -= 400;

            }
            else {
                ve += 400;

                if (cd.pat.id == 6)ve -= 500;
                else if (cd.pat.id == 1)ve -= 500;

            }

        }
        else if (L.size() == 1) {
            if (gcs.TS_kind == 1) {

                if (gcs.height_sum > hs && gcs.height_mxm > hm) {
                    ve += 50000;
                }
                else {
                    ve += 80000;
                }

                if (gcs.btb > -1)btbc += 100;

            }
            else if (gcs.TS_kind == 2) {

                if (gcs.height_sum > hs && gcs.height_mxm > hm) {
                    ve += 50;
                }
                else {
                    ve += -50;
                }

                if (gcs.btb > -1)btbc += 50;
            }
            else {

                if (gcs.height_sum > hs && gcs.height_mxm > hm) {
                    ve += 400;
                    if (cd.pat.id == 1) {
                        ve += 500;
                    }
                }
                else {
                    ve += -400;
                    if (cd.pat.id == 6) {
                        ve += -1000;
                    }
                    else if (cd.pat.id == 1) {
                        ve += -500;
                    }
                }

                if (gcs.btb > -1)btbc += -1000;

            }
        }
        else if (L.size() == 2) {
            if (gcs.TS_kind == 1) {

                if (gcs.height_sum > hs || gcs.height_mxm > hm) {
                    ve += 300000;
                }
                else {
                    ve += 4000000;
                }

                if (gcs.btb > -1)btbc += 100000;

            }
            else if (gcs.TS_kind == 2) {
                ve += 1;
                if (gcs.height_sum > p_A || gcs.height_mxm > 8) {
                    ve += 5;
                }

                if (gcs.btb > -1)btbc += 100;

            }
            else {

                if (gcs.height_sum > hs || gcs.height_mxm > hm) {
                    ve += 5000;

                    if (cd.pat.id == 6) {
                        ve += 50;
                    }
                    else if (cd.pat.id == 1) {
                        ve += 2000;
                    }

                }
                else {
                    ve += -400;

                    if (cd.pat.id == 6) {
                        ve += -3000;
                    }
                    else if (cd.pat.id == 1) {
                        ve += -600;
                    }

                }

                if (cd.pat.id == 6) {
                    ve += -100;
                }

                if (gcs.btb > -1)btbc += -2000;

            }
        }
        else if (L.size() == 3) {
            if (gcs.TS_kind == 1) {
                if (gcs.height_sum > hs || gcs.height_mxm > hm) {
                    ve += 300000;
                }
                else {
                    ve += 500000;
                }

                if (gcs.btb > -1)btbc += 1000;

            }
            else if (gcs.TS_kind == 2) {
                ve += 0;
                if (gcs.height_sum > hs || gcs.height_mxm > hm) {
                    ve -= 0;
                }
            }
            else {

                if (gcs.height_sum > hs || gcs.height_mxm > hm) {
                    ve += 700000;

                    if (cd.pat.id == 1) {
                        ve += 500000;
                    }
                }
                else {
                    ve += -200;
                    if (cd.pat.id == 1) {
                        ve += -500;
                    }
                }

                if (gcs.btb > -1)btbc += -1000;

            }
        }
        else if (L.size() == 4) {
                ve += 4000000;
            if (gcs.height_sum > hs || gcs.height_mxm > hm) {
                ve += 2000000;
            }

            if (gcs.btb > -1)btbc += 1000;

        }

        //fusion *= 100; cd.update(fusion);

        LL ve_rate = 500;
        if (gcs.height_sum > hs || gcs.height_mxm > hm) {
            ve_rate *= 100;
        }
        ve *= ve_rate;
        //cd.update(ve);
        cd.scr.erase = ve;


        touch *= 500;
        //cd.update(touch);
        cd.scr.touch = touch;

        contact *= 1000;
        //cd.update(contact);
        cd.scr.contact = contact;

        high *= 3500;
        //cd.update(high);
        cd.scr.height = high;

        //cd.update(shigune_AI::gs_BFS(cd, gcs));
        //cd.scr.closed = shigune_AI::gs_BFS(cd, q_field_AI);
        if (gcs.height_sum < hs || gcs.height_mxm < hm) {
            cd.scr.closed = shigune_AI::gs_BFS(cd, q_field_AI) * 2500;
        }
        else {
            cd.scr.closed = shigune_AI::gs_BFS(cd, q_field_AI) * 1500;
        }

        if (gcs.height_sum < hs && gcs.height_mxm < hm) {
            if (cd.isSFT) {
                //cd.update(-20);
                cd.scr.sum += 0;
            }
        }
        else
        {
            if (cd.isSFT) {
                //cd.update(0);
                cd.scr.sum += 0;
            }
        }

        if (gcs.height_sum < 96 || gcs.height_mxm < 12) {
            btbc *= 10;
        }
        //cd.update(btbc);
        cd.scr.btbc = btbc;

        //cd.update(gcs.pre_score);
        cd.scr.sum += gcs.pre_score;

        //cd.update();
        cd.scr.calc_sum();
        if(loopc == -1)cd.score = shig::secure_add(cd.score, cd.scr.sum);
        else {
            //cd.score = shig::secure_add(cd.score, (cd.scr.sum * scr_rate.at(loopc) / 100));
            cd.score = shig::secure_add(cd.score, cd.scr.sum);
        }
        //cd.score /= 100;

        return;
    }

    LL shigune_AI::gs_BFS(cmd_pattern& cb, VVI& qf) {

        LL open_blc = 0;
        
        LL clos_blc = 0;
        LL mino_blc = 0;
        LL grbg_blc = 0;

        const int h_limit = 40;

        VVI b_field_AI(h_limit+1, (VI(10, -1)));

        queue<pair<int, int>> xy;
        xy.push({ h_limit, 0 });

        while (!xy.empty()) {
            pair<int, int> now = xy.front(); xy.pop();
            if (b_field_AI.at(now.first).at(now.second) != -1)continue;
            else b_field_AI.at(now.first).at(now.second) = 0;

            for (int i = 0; i < cc.size(); i++) {
                int y = now.first  + cc.at(i).first;
                int x = now.second + cc.at(i).second;

                if (y < 0 || y >= h_limit)continue;
                else if (x < 0 || x >= 10)continue;
                else if (qf.at(y).at(x) != 0)continue;
                else if (b_field_AI.at(y).at(x) != -1)continue;
                else xy.push({ y, x });

            }

        }

        for (int i = 0; i < h_limit; i++) {
            for (int j = 0; j < 10; j++) {

                if (qf.at(i).at(j) > 0)mino_blc++;
                else if (qf.at(i).at(j) == 8)grbg_blc++;
                else if (b_field_AI.at(i).at(j) == -1)clos_blc++;
                else if (b_field_AI.at(i).at(j) == 0)open_blc++;
            }
        }

        LL b_closure = 10000;
        if (clos_blc > 0)b_closure = 0;
        b_closure -= 1000 * clos_blc;
        b_closure -= 100 * grbg_blc;
        b_closure += 0 * open_blc;

        b_closure *= 1;

        return b_closure;

    }

    bool shigune_AI::height_calc(game_container& gch) {
        gch.height_sum = 0;
        gch.height_mxm = 0;
        shig_rep(j, 10) {
            int h = 44;
            while (h >= 0) {
                if (gch.field_AI[h][j] == 0) {
                    gch.height[j] = h;
                    h--;
                }
                else {
                    break;
                }
            }
            h++;
            gch.height_sum += h;
            gch.height_mxm = max(gch.height_mxm, h);
        }

        return true;
    }

	bool shigune_AI::move_check(int to_x, int to_y, tetri& s_check) {
		int cnt = 0, cntt = 4;
		int rot = s_check.rot;
		int size = s_check.px_size;
		int H = s_check.mino[rot].size();
		int W = s_check.mino[rot][0].size();
		shig_rep(i, H) {
			shig_rep(j, W) {
				if (s_check.mino[rot][i][j] != 0) {
					int sX = s_check.X + j + to_x;
					if (sX < 0 || sX >= 10)break;
					int sY = s_check.Y - i + to_y;
					if (sY <= 0 || sY >= (field_AI.size() - 1))break;
					if (field_AI[sY - 1LL][sX] == 0)cnt++;
				}
			}
		}
        return cnt == cntt;
	}

    bool shigune_AI::move_check(int to_x, int to_y, tetri& s_check, game_container& ggc) {
        int cnt = 0, cntt = 4;
        int rot = s_check.rot;
        int size = s_check.px_size;
        int H = s_check.mino[rot].size();
        int W = s_check.mino[rot][0].size();
        shig_rep(i, H) {
            shig_rep(j, W) {
                if (s_check.mino[rot][i][j] != 0) {
                    int sX = s_check.X + j + to_x;
                    if (sX < 0 || sX >= 10)break;
                    int sY = s_check.Y - i + to_y;
                    if (sY <= 0 || sY >= (ggc.field_AI.size() - 1))break;
                    if (ggc.field_AI[sY - 1LL][sX] == 0)cnt++;
                }
            }
        }
        return cnt == cntt;
    }

	bool shigune_AI::SRS_check(int lr, tetri& now) {
        tetri test = now;
        int size = test.px_size;
        int to_X = 0, to_Y = 0, rot = test.rot;
        SRS_kind = -1;
        bool can = true;
        if (test.id == 1) {//I-mino SRS
            if (test.rot == 0) {
                if (lr == -1) {
                    test.set_rot(3);
                    rot = 3;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test)) {
                        SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(2, 0, test)) {
                        SRS_kind = 2;
                        to_X = 2;
                    }
                    else if (move_check(-1, 2, test)) {
                        SRS_kind = 3;
                        to_X = -1;
                        to_Y = 2;
                    }
                    else if (move_check(2, -1, test)) {
                        SRS_kind = 4;
                        to_X = 2;
                        to_Y = -1;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-2, 0, test)) {
                        SRS_kind = 1;
                        to_X = -2;
                    }
                    else if (move_check(1, 0, test)) {
                        SRS_kind = 2;
                        to_X = 1;
                    }
                    else if (move_check(-2, -1, test)) {
                        SRS_kind = 3;
                        to_X = -2;
                        to_Y = -1;
                    }
                    else if (move_check(1, 2, test)) {
                        SRS_kind = 4;
                        to_X = 1;
                        to_Y = 2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 1) {
                if (lr == -1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(2, 0, test)) {
                        SRS_kind = 1;
                        to_X = 2;
                    }
                    else if (move_check(-1, 0, test)) {
                        SRS_kind = 2;
                        to_X = -1;
                    }
                    else if (move_check(2, 1, test)) {
                        SRS_kind = 3;
                        to_X = 2;
                        to_Y = 1;
                    }
                    else if (move_check(-1, -2, test)) {
                        SRS_kind = 4;
                        to_X = -1;
                        to_Y = -2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test)) {
                        SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(2, 0, test)) {
                        SRS_kind = 2;
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
                        SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 2) {
                if (lr == -1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test)) {
                        to_X = 1;
                        SRS_kind = 1;
                    }
                    else if (move_check(-2, 0, test)) {
                        to_X = -2;
                        SRS_kind = 2;
                    }
                    else if (move_check(1, -2, test)) {
                        to_X = 1;
                        to_Y = -2;
                        SRS_kind = 3;
                    }
                    else if (move_check(-2, 1, test)) {
                        to_X = -2;
                        to_Y = 1;
                        SRS_kind = 4;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(3);
                    rot = 3;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(2, 0, test)) {
                        to_X = 2;
                        SRS_kind = 1;
                    }
                    else if (move_check(-1, 0, test)) {
                        to_X = -1;
                        SRS_kind = 2;
                    }
                    else if (move_check(2, 1, test)) {
                        to_X = 2;
                        to_Y = 1;
                        SRS_kind = 3;
                    }
                    else if (move_check(-1, -2, test)) {
                        to_X = -1;
                        to_Y = -2;
                        SRS_kind = 4;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 3) {
                if (lr == -1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-2, 0, test)) {
                        to_X = -2;
                        SRS_kind = 1;
                    }
                    else if (move_check(1, 0, test)) {
                        to_X = 1;
                        SRS_kind = 2;
                    }
                    else if (move_check(-2, -1, test)) {
                        to_X = -2;
                        to_Y = -1;
                        SRS_kind = 3;
                    }
                    else if (move_check(1, 2, test)) {
                        to_X = 1;
                        to_Y = 2;
                        SRS_kind = 4;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test)) {
                        to_X = 1;
                        SRS_kind = 1;
                    }
                    else if (move_check(-2, 0, test)) {
                        to_X = -2;
                        SRS_kind = 2;
                    }
                    else if (move_check(1, -2, test)) {
                        to_X = 1;
                        to_Y = -2;
                        SRS_kind = 3;
                    }
                    else if (move_check(-2, 1, test)) {
                        to_X = -2;
                        to_Y = 1;
                        SRS_kind = 4;
                    }
                    else {
                        SRS_kind = -1;
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
                        SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test)) {
                        SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, 1, test)) {
                        SRS_kind = 2;
                        to_X = 1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test)) {
                        SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(1, -2, test)) {
                        SRS_kind = 4;
                        to_X = 1;
                        to_Y = -2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test)) {
                        SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, 1, test)) {
                        SRS_kind = 2;
                        to_X = -1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test)) {
                        SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(-1, -2, test)) {
                        SRS_kind = 4;
                        to_X = -1;
                        to_Y = -2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 1) {
                if (lr == -1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test)) {
                        SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, -1, test)) {
                        SRS_kind = 2;
                        to_X = 1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test)) {
                        SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(1, 2, test)) {
                        SRS_kind = 4;
                        to_X = 1;
                        to_Y = 2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test)) {
                        SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, -1, test)) {
                        SRS_kind = 2;
                        to_X = 1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test)) {
                        SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(1, 2, test)) {
                        SRS_kind = 4;
                        to_X = 1;
                        to_Y = 2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 2) {
                if (lr == -1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test)) {
                        SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, 1, test)) {
                        SRS_kind = 2;
                        to_X = -1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test)) {
                        SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(-1, -2, test)) {
                        SRS_kind = 4;
                        to_X = -1;
                        to_Y = -2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(3);
                    rot = 3;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test)) {
                        SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, 1, test)) {
                        SRS_kind = 2;
                        to_X = 1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test)) {
                        SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(1, -2, test)) {
                        SRS_kind = 4;
                        to_X = 1;
                        to_Y = -2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 3) {
                if (lr == -1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test)) {
                        SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, -1, test)) {
                        SRS_kind = 2;
                        to_X = -1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test)) {
                        SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(-1, 2, test)) {
                        SRS_kind = 4;
                        to_X = -1;
                        to_Y = 2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test)) {
                        SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test)) {
                        SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, -1, test)) {
                        SRS_kind = 2;
                        to_X = -1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test)) {
                        SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(-1, 2, test)) {
                        SRS_kind = 4;
                        to_X = -1;
                        to_Y = 2;
                    }
                    else {
                        SRS_kind = -1;
                        can = false;
                    }
                }
            }
        }

        if (can) {
            TS_check(to_X, to_Y, test);
            now.set_rot(rot);
            now.addX(to_X);
            now.addY(to_Y);
        }
        
        return can;

	}

    bool shigune_AI::SRS_check(int lr, tetri& now, game_container& ggc) {
        tetri test = now;
        int size = test.px_size;
        int to_X = 0, to_Y = 0, rot = test.rot;
        ggc.SRS_kind = -1;
        bool can = true;
        if (test.id == 1) {//I-mino SRS
            if (test.rot == 0) {
                if (lr == -1) {
                    test.set_rot(3);
                    rot = 3;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(2, 0, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = 2;
                    }
                    else if (move_check(-1, 2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_X = -1;
                        to_Y = 2;
                    }
                    else if (move_check(2, -1, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = 2;
                        to_Y = -1;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-2, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = -2;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = 1;
                    }
                    else if (move_check(-2, -1, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_X = -2;
                        to_Y = -1;
                    }
                    else if (move_check(1, 2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = 1;
                        to_Y = 2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 1) {
                if (lr == -1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(2, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = 2;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = -1;
                    }
                    else if (move_check(2, 1, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_X = 2;
                        to_Y = 1;
                    }
                    else if (move_check(-1, -2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = -1;
                        to_Y = -2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(2, 0, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = 2;
                    }
                    else if (move_check(-1, 2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_X = -1;
                        to_Y = 2;
                    }
                    else if (move_check(2, -1, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = 2;
                        to_Y = -1;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 2) {
                if (lr == -1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        to_X = 1;
                        ggc.SRS_kind = 1;
                    }
                    else if (move_check(-2, 0, test, ggc)) {
                        to_X = -2;
                        ggc.SRS_kind = 2;
                    }
                    else if (move_check(1, -2, test, ggc)) {
                        to_X = 1;
                        to_Y = -2;
                        ggc.SRS_kind = 3;
                    }
                    else if (move_check(-2, 1, test, ggc)) {
                        to_X = -2;
                        to_Y = 1;
                        ggc.SRS_kind = 4;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(3);
                    rot = 3;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(2, 0, test, ggc)) {
                        to_X = 2;
                        ggc.SRS_kind = 1;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        to_X = -1;
                        ggc.SRS_kind = 2;
                    }
                    else if (move_check(2, 1, test, ggc)) {
                        to_X = 2;
                        to_Y = 1;
                        ggc.SRS_kind = 3;
                    }
                    else if (move_check(-1, -2, test, ggc)) {
                        to_X = -1;
                        to_Y = -2;
                        ggc.SRS_kind = 4;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 3) {
                if (lr == -1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-2, 0, test, ggc)) {
                        to_X = -2;
                        ggc.SRS_kind = 1;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        to_X = 1;
                        ggc.SRS_kind = 2;
                    }
                    else if (move_check(-2, -1, test, ggc)) {
                        to_X = -2;
                        to_Y = -1;
                        ggc.SRS_kind = 3;
                    }
                    else if (move_check(1, 2, test, ggc)) {
                        to_X = 1;
                        to_Y = 2;
                        ggc.SRS_kind = 4;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        to_X = 1;
                        ggc.SRS_kind = 1;
                    }
                    else if (move_check(-2, 0, test, ggc)) {
                        to_X = -2;
                        ggc.SRS_kind = 2;
                    }
                    else if (move_check(1, -2, test, ggc)) {
                        to_X = 1;
                        to_Y = -2;
                        ggc.SRS_kind = 3;
                    }
                    else if (move_check(-2, 1, test, ggc)) {
                        to_X = -2;
                        to_Y = 1;
                        ggc.SRS_kind = 4;
                    }
                    else {
                        ggc.SRS_kind = -1;
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
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, 1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = 1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(1, -2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = 1;
                        to_Y = -2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, 1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = -1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(-1, -2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = -1;
                        to_Y = -2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 1) {
                if (lr == -1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, -1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = 1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(1, 2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = 1;
                        to_Y = 2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, -1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = 1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(1, 2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = 1;
                        to_Y = 2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 2) {
                if (lr == -1) {
                    test.set_rot(1);
                    rot = 1;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, 1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = -1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(-1, -2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = -1;
                        to_Y = -2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(3);
                    rot = 3;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = 1;
                    }
                    else if (move_check(1, 1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = 1;
                        to_Y = 1;
                    }
                    else if (move_check(0, -2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = -2;
                    }
                    else if (move_check(1, -2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = 1;
                        to_Y = -2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
            }
            else if (test.rot == 3) {
                if (lr == -1) {
                    test.set_rot(2);
                    rot = 2;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, -1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = -1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(-1, 2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = -1;
                        to_Y = 2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
                else if (lr == 1) {
                    test.set_rot(0);
                    rot = 0;
                    if (move_check(0, 0, test, ggc)) {
                        ggc.SRS_kind = 0;
                    }
                    else if (move_check(-1, 0, test, ggc)) {
                        ggc.SRS_kind = 1;
                        to_X = -1;
                    }
                    else if (move_check(-1, -1, test, ggc)) {
                        ggc.SRS_kind = 2;
                        to_X = -1;
                        to_Y = -1;
                    }
                    else if (move_check(0, 2, test, ggc)) {
                        ggc.SRS_kind = 3;
                        to_Y = 2;
                    }
                    else if (move_check(-1, 2, test, ggc)) {
                        ggc.SRS_kind = 4;
                        to_X = -1;
                        to_Y = 2;
                    }
                    else {
                        ggc.SRS_kind = -1;
                        can = false;
                    }
                }
            }
        }

        if (can) {
            ggc.TS_kind = TS_check(to_X, to_Y, test, ggc);
            now.set_rot(rot);
            now.addX(to_X);
            now.addY(to_Y);
        }
        return can;

    }

    int shigune_AI::TS_check(int toX, int toY, tetri& ts) {
        if (ts.id != 6) {
            TS_kind = 0;
            return 0;
        }
        int ts_cnt = 0;
        int size = ts.px_size;
        int rot = ts.rot;

        VI testX = { 0, 0, 2, 2 };
        VI testY = { 0, 2, 0, 2 };

        pairI2 check = { 0, 0 };

        shig_rep(i, 4) {
            int sX = ts.X + testX[i] + toX;
            if (sX < 0 || sX >= 10) {
                ts_cnt++;
                continue;
            }
            int sY = ts.Y - testY[i] + toY - 1;
            if (sY < 0 || sY >= (field_AI.size() - 1)) {
                ts_cnt++;
                continue;
            }
            if (field_AI[sY][sX] != 0)ts_cnt++;
            else check = { testX[i], testY[i] };
        }

        if (ts_cnt == 3 && SRS_kind != 4) {
            if (rot == 0) {
                if (check.first == 0 && check.second == 0)TS_kind = 2;
                else if (check.first == 2 && check.second == 0)TS_kind = 2;
                else TS_kind = 1;
            }
            else if (rot == 1) {
                if (check.first == 2 && check.second == 0)TS_kind = 2;
                else if (check.first == 2 && check.second == 2)TS_kind = 2;
                else TS_kind = 1;
            }
            else if (rot == 2) {
                if (check.first == 2 && check.second == 2)TS_kind = 2;
                else if (check.first == 0 && check.second == 2)TS_kind = 2;
                else TS_kind = 1;
            }
            else if (rot == 3) {
                if (check.first == 0 && check.second == 2)TS_kind = 2;
                else if (check.first == 0 && check.second == 0)TS_kind = 2;
                else TS_kind = 1;
            }
        }
        else if (ts_cnt == 3 || ts_cnt == 4)TS_kind = 1;//normal
        else TS_kind = 0;

        return TS_kind;
    }

    int shigune_AI::TS_check(int toX, int toY, tetri& ts, game_container& ggc) {
        if (ts.id != 6) {
            ggc.TS_kind = 0;
            return 0;
        }

        int ts_cnt = 0, size = ts.px_size, rot = ts.rot;
        VI testX = { 0, 0, 2, 2 }, testY = { 0, 2, 0, 2 };
        pairI2 check = { 0, 0 };

        shig_rep(i, 4) {
            int sX = ts.X + testX[i] + toX;
            if (sX < 0 || sX >= 10) {
                ts_cnt++;
                continue;
            }
            int sY = ts.Y - testY[i] + toY - 1;
            if (sY < 0 || sY >= (ggc.field_AI.size() - 1)) {
                ts_cnt++;
                continue;
            }
            if (ggc.field_AI[sY][sX] != 0)ts_cnt++;
            else check = { testX[i], testY[i] };
        }

        if (ts_cnt == 3 && SRS_kind != 4) {
            if (rot == 0) {
                if (check.first == 0 && check.second == 0)ggc.TS_kind = 2;
                else if (check.first == 2 && check.second == 0)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
            else if (rot == 1) {
                if (check.first == 2 && check.second == 0)ggc.TS_kind = 2;
                else if (check.first == 2 && check.second == 2)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
            else if (rot == 2) {
                if (check.first == 2 && check.second == 2)ggc.TS_kind = 2;
                else if (check.first == 0 && check.second == 2)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
            else if (rot == 3) {
                if (check.first == 0 && check.second == 2)ggc.TS_kind = 2;
                else if (check.first == 0 && check.second == 0)ggc.TS_kind = 2;
                else ggc.TS_kind = 1;
            }
        }
        else if (ts_cnt == 3 || ts_cnt == 4)ggc.TS_kind = 1;//normal
        else ggc.TS_kind = 0;

        return ggc.TS_kind;
    }

    set<int> shigune_AI::erase_check_AI(tetri& s_now) {
        set<int> erase_itr;
        p_field_AI = field_AI;
        shigune_AI::apply_mino(p_field_AI, s_now);
        int rot = s_now.rot;
        int size = s_now.px_size;
        int H = s_now.mino[rot].size();
        int pW = p_field_AI[0].size();

        shig_rep(i, H) {
            int sY = s_now.Y - i - 1;
            int cnt = 0;
            if (sY < 0 || sY >= 44)continue;
            shig_rep(j, pW) {
                if (p_field_AI[sY][j] != 0) cnt++;
                else break;
            }
            if (cnt == pW) erase_itr.insert(sY);
        }
        return erase_itr;
    }

    set<int> shigune_AI::erase_check_AI(tetri& s_now, game_container& gce) {
        set<int> erase_itr;
        gce.p_field_AI = gce.field_AI;
        shigune_AI::apply_mino(gce.p_field_AI, s_now);
        int rot = s_now.rot;
        int size = s_now.px_size;
        int H = s_now.mino[rot].size();
        int pW = 10;

        shig_rep(i, H) {
            int sY = s_now.Y - i - 1;
            int cnt = 0;
            if (sY < 0 || sY >= fh - 1)continue;
            shig_rep(j, pW) {
                if (gce.p_field_AI[sY][j] != 0) cnt++;
                else break;
            }
            if (cnt == pW) erase_itr.insert(sY);
        }
        return erase_itr;
    }

    void shigune_AI::apply_mino(VVI& c_field, tetri& s_now) {
        int rot = s_now.rot;
        int size = s_now.px_size;
        int H = s_now.mino[rot].size();
        int W = s_now.mino[rot][0].size();
        shig_rep(i, H) {
            shig_rep(j, W) {
                if (s_now.mino[rot][i][j] != 0) {
                    int sX = s_now.X + j;
                    int sY = s_now.Y - i;
                    if (sX < 0 || sX >= 10)continue;
                    if (sY - 1 < 0 || sY - 1 > 45)continue;
                    c_field[sY - 1LL][sX] = s_now.id;
                }
            }
        }
    }

    bool shigune_AI::move_mino(tetri& m_now, int s_action) {
        int size = m_now.px_size;
        if (s_action == 1) {
            if (hold_AI == 0) {
                m_now.minset(q_next_AI.front());
            }
            else {
                m_now.minset(hold_AI);
            }
            
            return true;
        }
        else if (s_action == 2) {
            int sft = -1;
            if (move_check(0, sft, m_now)) {
                m_now.addY(sft);
                return true;
            }
            else return false;
        }
        else if (s_action == 3) {
            int sft = -1;
            while (move_check(0, sft, m_now)) {
                m_now.addY(sft);
            }
            return true;
        }
        else if (s_action == 4) {
            return SRS_check(-1, m_now);
        }
        else if (s_action == 5) {
            return SRS_check(1, m_now);
        }
        else if (s_action == 6) {
            int sft = -1;
            if (move_check(sft, 0, m_now)) {
                m_now.addX(sft);
                return true;
            }
            else return false;
        }
        else if (s_action == 7) {
            int sft = 1;
            if (move_check(sft, 0, m_now)) {
                m_now.addX(sft);
                return true;
            }
            else return false;
        }
        return true;
    }

    bool shigune_AI::move_mino(tetri& m_now, int s_action, game_container& ggc) {
        int size = m_now.px_size;
        if (s_action == 1) {
            if (ggc.hold_AI == 0) {
                m_now.minset(ggc.q_next_AI.front());
            }
            else {
                m_now.minset(ggc.hold_AI);
            }
            return true;
        }
        else if (s_action == 2) {
            int sft = -1;
            if (move_check(0, sft, m_now, ggc)) {
                m_now.addY(sft);
                return true;
            }
            else return false;
        }
        else if (s_action == 3) {
            int sft = -1;
            while (move_check(0, sft, m_now, ggc)) {
                m_now.addY(sft);
            }
            return true;
        }
        else if (s_action == 4) {
            return SRS_check(-1, m_now, ggc);
        }
        else if (s_action == 5) {
            return SRS_check(1, m_now, ggc);
        }
        else if (s_action == 6) {
            int sft = -1;
            if (move_check(sft, 0, m_now, ggc)) {
                m_now.addX(sft);
                return true;
            }
            else return false;
        }
        else if (s_action == 7) {
            int sft = 1;
            if (move_check(sft, 0, m_now, ggc)) {
                m_now.addX(sft);
                return true;
            }
            else return false;
        }
        return true;
    }

    void shigune_AI::print_gh(tetri& s_now) {
        int rot = s_now.rot;
        int size = s_now.px_size;
        int id = s_now.id;
        int H = s_now.mino[rot].size();
        int W = s_now.mino[rot][0].size();
        shig_rep(i, H) {
            shig_rep(j, W) {
                if (s_now.mino[rot][i][j] != 0) {
                    int sX = s_now.X + j;
                    int sY = s_now.Y - i;
                    if (sX < 0 || sX >= 10)continue;
                    if (sY - 1 < 0 || sY - 1 > 45)continue;
                    p_field_AI[sY - 1LL][sX] = -1 * id;
                }
            }
        }

        return;
    }

    void shigune_AI::move_itr(int lr) {

        if (lr == -1) {
            if (cp_itr != cp.begin()) --cp_itr;
        }
        else if (lr == 1) {
            if (++cp_itr != cp.end()) ++cp_itr;
        }

        return;
    }

    VVI shigune_AI::get_AI_field(int p, int m) {
        if (p == 1) {
            p_field_AI = field_AI;
        }
        if (m == 1) {
            if (cp_itr == cp.end())--cp_itr;
            cmd_pattern cm = *cp_itr;
            print_gh(cm.pat);
        }
        return p_field_AI;
    }

    VI shigune_AI::get_AI_cmd() {
        cmd_pattern cl = *cp_itr;
        return cl.cmd_list;
    }

    bool shigune_AI::load_ttrp() {
        GetTempNameList(ttrp_name_list);
        ttrp_size = ttrp_name_list.size();
        //template_list.reserve(ttrp_size+1);
        vector<tetriplate> vt(ttrp_size);
        shig_rep(i, ttrp_size) {
            ReadTempData(ttrp_name_list[i], vt[i]);
            ttrp_id_list.push_back(vt[i].tp_id);
        }

        ttrp_list = vt;

        return true;
    }

    bool shigune_AI::ttrp_crr(tetri& now_p, int& size_l) {
        bool chk_f = false;
        if (ttrp_able) {
            shig_rep(i, min({ now_ttrp.mino_num , (int)now_ttrp.list_mino.size() })) {
                if (now_ttrp.list_mino[i] == now_p) {
                    if (now_p.id == 6) {
                        if ((now_ttrp.list_mino_s[i]) == size_l && TS_kind == 1) {
                            chk_f = true;
                        }
                        else if (now_ttrp.list_mino_s[i] == 0 && TS_kind == 0){
                            chk_f = true;
                        }
                        else {
                            chk_f = false;
                        }
                    }
                    else {
                        chk_f = true;
                    }

                    break;

                }
                else {
                    chk_f = false;
                }
            }

        }

        return chk_f;
    }

    bool shigune_AI::ttrp_check(cmd_pattern& slc, int& sle, VI& mnL) {

        if (!ttrp_able)return true;

        if (slc.ttrp_f >= 0) {
            now_ttrp.mino_check.at(slc.ttrp_f) = true;
            int mchk_cnt = 0;
            shig_rep(i, now_ttrp.mino_num) {
                if (now_ttrp.mino_check.at(i) == true) {
                    mchk_cnt++;
                }
            }
            if (mchk_cnt >= now_ttrp.mino_num) {

                if (now_ttrp.list_size == 0) {
                    ttrp_able = false;
                }
                else {
                    shig_rep(i, now_ttrp.list_size) {
                        bool lpf = false;
                        shig_rep(j, ttrp_list.size()) {

                            if (ttrp_list[j].tp_id != now_ttrp.list_id[i])continue;

                            int tm_cnt = 0;
                            shig_rep(k, ttrp_list[j].terms_num) {
                                int tL = ttrp_list[j].terms[k].first;
                                int tR = ttrp_list[j].terms[k].second;
                                if (mnL[tL] < mnL[tR])tm_cnt++;
                            }

                            if (tm_cnt >= ttrp_list[j].terms_num) {
                                now_ttrp = ttrp_list[j];
                                ttrp_able = true;
                                lpf = true;
                                break;
                            }
                            else {
                                lpf = false;
                                break;
                            }
                        }
                        if (lpf)break;
                    }
                }

            }
        }
        else {
            ttrp_able = false;
        }

        if (ttrp_able) {
            ttrp_ofsY += sle;
        }
        else {
            ttrp_ofsY = 0;
        }

        return true;
    }

    bool shigune_AI::ttrp_check(cmd_pattern& slc, int& sle, VI& mnL, game_container& gct) {

        if (!gct.ttrp_able)return true;

        if (slc.ttrp_f > 0) {
            gct.now_ttrp.mino_check[slc.ttrp_f - 1LL] = true;
            int mchk_cnt = 0;
            shig_rep(i, gct.now_ttrp.mino_num) {
                if (gct.now_ttrp.mino_check[i] == true) {
                    mchk_cnt++;
                }
            }
            if (mchk_cnt >= gct.now_ttrp.mino_num) {
                gct.ttrp_able = false;
            }
        }
        else {
            gct.ttrp_able = false;
        }

        if (gct.ttrp_able) {
            gct.ttrp_ofsY += sle;
        }
        else {
            gct.ttrp_ofsY = 0;
        }

        return true;
    }

    bool shigune_AI::next_crr_check() {

        set<int> all_chk;
        int lim_s = min(5, (int)next_AI.size());
        shig_rep(i, lim_s)all_chk.insert(next_AI[i]);
        all_chk.insert(current_AI);
        all_chk.insert(hold_AI);
        if (all_chk.size() == 7)return true;
        else if (hold_AI == 0 && all_chk.size() == 6)return true;
        else return false;

        return false;

    }

    pair<int, string> shigune_AI::get_sttrp_name() {
        str = "noting selected";
        if (ttrp_able)return make_pair(now_ttrp.tp_id, now_ttrp.temp_name);
        else return make_pair(i32_minus_one, s);
    }

    bool shigune_AI::set_gc(game_container &gc) {

        VI gi(12, 0);
        gi[0] = hold_AI;
        gi[1] = current_AI;
        gi[2] = pc_cnt;
        gi[3] = SRS_kind;
        gi[4] = TS_kind;
        gi[5] = btb;
        gi[6] = combo;
        gi[7] = height_sum;
        gi[8] = height_mxm;
        gi[9] = ttrp_size;
        gi[10] = ttrp_ofsY;
        gi[11] = 0;
        //if (!gc.set_gc_int(gi))return false;
        gc.set_gc_int(gi);

        vector<bool> gb(1);
        gb[0] = ttrp_able;
        //if (!gc.set_gc_bool(gb))return false;
        gc.set_gc_bool(gb);

        VVI gv(1, VI(0, 0));
        gv[0] = this->height;
        //if (!gc.set_gc_VI(gv))return false;
        
        gc.set_gc_VI(gv);

        //if(!gc.set_gc_next(next_AI, q_next_AI))return false;
        gc.set_gc_next(next_AI, q_next_AI);

        vector<VVI> gf(3, VVI(0, VI(0, 0)));
        gf[0] = field_AI;
        gf[1] = p_field_AI;
        gf[2] = strategy_map;
        //if(!gc.set_gc_field(gf))return false;
        gc.set_gc_field(gf);


        //if(!gc.set_gc_ttrp(now_ttrp))return false;
        gc.set_gc_ttrp(now_ttrp);

        //gc.p_field_AI = p_field_AI;
        //strategy_map = VVI(45, (VI(10, 0)));
        //strategy_list = vector<strategy>(0);
        //cv = vector<cmd_pattern>(0);
        //cv.reserve(600);
        //ec = vector<cmd_pattern>(0);
        //ec.reserve(10);
        

        return true;
    }

    game_container shigune_AI::update_gc(cmd_pattern& ct, game_container gcp) {
        
        
        
        int rot = ct.pat.rot, size = ct.pat.px_size, H = ct.pat.mino[rot].size(), pW = 10;

        //gcp.p_field_AI = gcp.field_AI;
        //shigune_AI::apply_mino(gcp.p_field_AI, ct.pat);

        set<int> itr = shigune_AI::erase_check_AI(ct.pat, gcp);
        int itr_s = itr.size();

        //const VI empty = { 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 };
        VVI proxy(0); proxy.reserve(45);

        shig_rep(i, fh) {
            decltype(itr)::iterator it = itr.find(i);
            if (it == itr.end()) proxy.push_back(gcp.p_field_AI[i]);
        }
        while (proxy.size() < fh) proxy.push_back(ev_empty);

        gcp.field_AI = proxy;

        if (ct.cmd_list.at(0) == 1) gcp.hold_AI = gcp.current_AI;
        if (gcp.q_next_AI.size() == 0) {
            //gcp.current_AI = 0;
            //*
            shig_rep(i, NS_a.size()) {
                gcp.q_next_AI.push(NS_a.at(i));
            }
            //*/
        }
        
        if (gcp.q_next_AI.size() != 0) {
            gcp.current_AI = gcp.q_next_AI.front();
            gcp.q_next_AI.pop();  gcp.next_AI.clear();
            while (!gcp.q_next_AI.empty()) {
                int q = gcp.q_next_AI.front();
                gcp.next_AI.push_back(q);
                gcp.q_next_AI.pop();
            }
            shig_rep(i, gcp.next_AI.size()) {
                gcp.q_next_AI.push(gcp.next_AI.at(i));
            }
        }
        
        
        shigune_AI::height_calc(gcp);

        VI mnL(8, 8);
        mnL[gcp.current_AI] = 0;
        mnL[gcp.hold_AI] = -1;
        shig_rep(i, min({ 5, (int)gcp.next_AI.size() })) {
            mnL[gcp.next_AI[i]] = i + 1;
        }
        shigune_AI::ttrp_check(ct, itr_s, mnL, gcp);

        if (gcp.combo == 0) {
            if (itr_s == 0) {

            }
            else {
                gcp.combo++;
            }
        }else if (gcp.combo > 0) {
            if (itr_s == 0) {
                gcp.combo = 0;
            }
            else {
                gcp.combo++;
            }
        }

        gcp.pre_score = shig::secure_add(gcp.pre_score, ct.score);
        //gcp.pre_score += ct.score;

        return gcp;
    }

    int shigune_AI::ttrp_check_mino(tetri& fd, game_container& gcf) {

        bool chk_f = false;

        for (int i = 0; i < gcf.now_ttrp.mino_num; i++) {
            if (gcf.now_ttrp.list_mino.at(i) == fd)return i;
        }

        return -1;

    }

	shigune_AI::~shigune_AI() {

	}

}


bool GetTempNameList(VS& name_list) {
    int n = 0;
    FILE* fp;
    auto em = fopen_s(&fp, "template\\tetriplate_list.txt", "r");
    //hndl_tmplist = FileRead_open("template\\tetriplate_list.txt");
    //if (hndl_tmplist == 0)return false;

    auto at = fscanf_s(fp, "%d", &n);
    if (n < 0)n = 0;

    VS list(n);
    shig_rep(i, n) {
        char tmpC[64];
        if (feof(fp))break;
        auto ao = fscanf_s(fp, "%s", tmpC, 64);
        if (tmpC[0] == '\0' || tmpC[0] == '/') {
            i--;
            continue;
        }
        list[i] = tmpC;
        list[i] = "template\\data\\" + list[i];
    }

    name_list = list;

   fclose(fp);
    return true;

}

bool ReadTempData(string& name, tetriplate& ttrp) {

    const char* fname = name.c_str();
    FILE* fp;
    auto em = fopen_s(&fp ,fname, "r");
    if (feof(fp)) {
        int z = 0;
        string zs = "";
        ttrp.set(z, z, z, z, zs, z);
        vector<tetri> zt = { tetri(0, 0, 0, 0) };
        VI zts(0,0);
        ttrp.set_list(zt, zts);
        VI zv(0, 0);
        ttrp.set_id_list(zv);
        vector<pairI2> zp(0, make_pair(0, 0));
        ttrp.set_terms(zp);
        return false;
    }
    
    int l, ls, id, tn, bf; //mino num : connect list num : ttrp id
    char c[64];
    string s = "";
    auto at = fscanf_s(fp, "%d %d %d %d %d %s ", &l, &ls, &id, &tn, &bf, c, 64);
    s = c;
    ttrp.set(l, ls, id, tn, s, bf);

    //
    vector<tetri> tpl(l);
    VI ts(l);
    shig_rep(i, l) {
        int r, x, y, d, s;
        auto ao = fscanf_s(fp, "%d %d %d %d %d", &r, &x, &y, &d, &s);
        tetri tp(r, x, y, d);
        tpl[i] = tp;
        ts[i] = s;
    }
    ttrp.set_list(tpl, ts);

    //
    VI vil(ls);
    shig_rep(i, ls) {
        int temp = 0;
        if (feof(fp)) {
            vil[i] = 0;
        }
        else {
            auto lo = fscanf_s(fp, "%d", &temp);
            vil[i] = temp;
        }
    }
    ttrp.set_id_list(vil);

    //
    vector<pairI2> vmp(tn);
    shig_rep(i, tn) {
        int ll = 0, rr = 0;
        if (feof(fp)) {
            vmp[i] = make_pair(0, 0);
        }
        else {
            auto at = fscanf_s(fp, "%d %d", &ll, &rr);
            vmp[i] = make_pair(ll, rr);
        }
    }
    ttrp.set_terms(vmp);

    fclose(fp);

    return true;
}
