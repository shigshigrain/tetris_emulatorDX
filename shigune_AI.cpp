#include "shigune_AI.h"

int i32_zero = 0;
int i32_one = 1;
int i32_minus_one = -1;
const int branch_num = 40;//branch_num >= 30
const vector<int> expl_width = { 40, 40, 40, 30, 30, 30, 30, 10, 10, 10, 10, 10, 10, 10, 10 };
//const vector<int> expl_width = { 20, 20, 20, 20, 20, 20, 20, 20, 15, 15, 15, 10, 10, 10, 10 };
const int cyc_num = 7;//up to 14
const int fh = 45;
//const int thd_num = 5;

static std::mutex mtx;


//using namespace shig;

namespace shig {

    CmdPattern::CmdPattern() {
        score = 0;
        scr = cmd_score();
        index = -1;
        srs = 0, kind = 0;
        ttrp_f = -1;
        pre_gc = 0;
        isSFT = false;
        cmd_list = {};
        pat = Tetri();
    }

    CmdPattern::CmdPattern(const Tetri& p, const VI& list, const int& d) : pat(p), cmd_list(list), index(d) {
        score = 0;
        scr = cmd_score();
        srs = 0, kind = 0;
        ttrp_f = -1;
        pre_gc = 0;
        isSFT = false;
    };

    void CmdPattern::update() {
        score += scr.get_sum();
        return;
    }

    void CmdPattern::update(LL u) {
        score += u;
        return;
    }

    void CmdPattern::set_ttrpF(int sf) {
        ttrp_f = sf;
    }

    void CmdPattern::set_isSFT(const bool& sf) {
        isSFT = sf;
    }

    void CmdPattern::set(LL& s, const Tetri& p, VI& list, int& d, int& r, int& k) {
        score = s;
        pat = p;
        cmd_list = list;
        index = d;
        srs = r;
        kind = k;
        //d_line = l;
    }

    void CmdPattern::set_ts(int r, int k) {
        srs = r;
        kind = k;
        return;
    }

    bool operator < (const CmdPattern& a, const CmdPattern& b) {
        return (a.pat < b.pat);
    }

    bool operator == (const CmdPattern& a, const CmdPattern& b) {
        return (a.pat == b.pat);
    }

    GameContainer::GameContainer() {

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
        cv = vector<CmdPattern>(0);
        cv.reserve(600);
        ec = vector<CmdPattern>(0);
        ec.reserve(10);
        int z = 0;
        string zs = "nothing";
        now_ttrp.set(z, z, z, z, zs, z);
        VI zv(0, 0);
        now_ttrp.set_id_list(zv);
        vector<pairI2> zp(0, make_pair(0, 0));
        now_ttrp.set_terms(zp);
    }

    bool GameContainer::set_gc_int(const VI& si) {
        if ((int)si.size() != 12) return false;
        hold_AI = si[0];
        current_AI = si[1];
        pc_cnt = si[2];
        SRS_kind = si[3];
        TS_kind = si[4];
        combo = si[5];
        btb = si[6];
        height_sum = si[7];
        height_mxm = si[8];
        ttrp_size = si[9];
        ttrp_ofsY = si[10];
        total_s = si[11];
        return true;

    }

    bool GameContainer::set_gc_VI(const vector<VI>& sv) {
        if ((int)sv.size() != 1) return false;
        height = sv[0];
        return true;
    }

    bool GameContainer::set_gc_bool(const vector<bool>& sb) {
        if ((int)sb.size() != 1) return false;
        ttrp_able = sb[0];
        return true;
    }

    bool GameContainer::set_gc_next(const VI& nx, const queue<int>& qnx) {
        next_AI = nx;
        q_next_AI = qnx;
        return true;
    }

    bool GameContainer::set_gc_field(const vector<VVI>& sf) {
        if ((int)sf.size() != 3) return false;
        field_AI = sf[0];
        p_field_AI = sf[1];
        strategy_map = sf[2];
        return true;
    }

    bool GameContainer::set_gc_ttrp(const TetriPlate& st) {
        now_ttrp = st;
        return true;
    }

    bool GameContainer::add_gc_cmdp(const CmdPattern& cg) {

        ec.push_back(cg);

        return true;
    }

    CmdPattern GameContainer::get_gc_cmdp(int itr) {
        if (itr < 0 || itr >= ec.size()) {
            return CmdPattern();
        }
        else {
            return ec.at(itr);
        }
    }

    GameContainer::~GameContainer() {

    }

    //

    Tetri t(0, 3, 21, 0);
    TetriPlate now_ttrp;
    Tetri null_tetri(0, 0, 0, 0);
    CmdPattern null_cmd(null_tetri, { 3 }, -1);



    AIshigune::AIshigune(int ii) {
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
        cv = vector<CmdPattern>(0);
        cv.reserve(100);
        cp_itr = cp.begin();
        ttrp_name_list = VS(0);
        ttrp_id_list = VI(0, 0);
        ttrp_list = vector<TetriPlate>(0);
        ttrp_bgnF = VI(0, 0);
        int z = 0;
        string zs = "nothing";
        now_ttrp.set(z, z, z, z, zs, z);
        VI zv(0, 0);
        now_ttrp.set_id_list(zv);
        vector<pairI2> zp(0, make_pair(0, 0));
        now_ttrp.set_terms(zp);

        gc_slot = vector<GameContainer>(branch_num);
        now_gc = GameContainer();
        null_tetri.minset(6);
        null_cmd.pat.minset(6);
        std::random_device Sg;
        Rengine = std::mt19937_64(Sg());

    }


    VI AIshigune::shigune_main() {

        get_state();
        get_field();
        strategy_mark();

        set_gc(now_gc);

        CmdPattern select = explore_choices(now_gc);

        set<int> el = erase_check_AI(select.pat, now_gc);
        int els = (int)el.size();

        VVI proxy(0); proxy.reserve(45);
        shig_rep(i, 45) {
            decltype(el)::iterator it = el.find(i);
            if (it == el.end()) proxy.push_back(strategy_map[i]);
        }
        while (proxy.size() < 45) proxy.push_back(ev_empty);
        strategy_map = proxy;

        /*VI mnL(8, 8);
        mnL[current_AI] = 0;
        mnL[hold_AI] = -1;
        shig_rep(i, min({ 5, (int)next_AI.size() })) {
            mnL[next_AI[i]] = i + 1;
        }*/

        //bool all_TF = 
        next_crr_check();

        AIshigune::ttrp_check(select, els);

        return select.cmd_list;
    }

    VI AIshigune::get_cmd() {
        // hold : 1, soft : 2, hard : 3, L_rot : 4, R_rot : 5, l_move : 6, r_move : 7;
        VI cmd = {};

        return cmd;
    }

    void AIshigune::get_field() {
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

    void AIshigune::get_state() {
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
        //auto [combo, btb] = 
        get_combo();
        return;
    }

    VI AIshigune::make_order_list()
    {
        VI mnl(8, 0);
        for (size_t i = 0; i < mnl.size(); i++)
        {
            mnl.at(i) = (int)i + 8;
        }

        mnl.at(current_AI) = 0;
        mnl.at(hold_AI) = -1;
        for (size_t i = 0; i < std::min((size_t)5, next_AI.size()); i++) {
            mnl.at(next_AI.at(i)) = (int)i + 1;
        }

        return mnl;
    }

    VI AIshigune::make_order_list(GameContainer& gc)
    {
        VI mnl(8, 0);
        for (size_t i = 0; i < mnl.size(); i++)
        {
            mnl.at(i) = (int)i + 8;
        }

        mnl.at(gc.current_AI) = 0;
        mnl.at(gc.hold_AI) = -1;
        for (size_t i = 0; i < std::min((size_t)5, gc.next_AI.size()); i++) {
            mnl.at(gc.next_AI.at(i)) = (int)i + 1;
        }

        return mnl;
    }

    bool AIshigune::strategy_mark() {

        if (pc_check())bgn_strategy();

        return true;
    }

    bool AIshigune::pc_check() {
        shig_rep(i, 10) {
            if (field_AI[0][i] != 0)return false;
        }
        return true;
    }

    void AIshigune::bgn_strategy() {
        // I J L O S T Z
        strategy_list.clear();
        //bool all_TF = false;


        VI mnL = make_order_list();

        bool all_TF = next_crr_check();

        shig_rep(i, 45) shig_rep(j, 10) strategy_map[i][j] = 0;

        if (all_TF) {
            ttrp_able = true;
        }
        else {
            ttrp_able = false;
        }

        if (ttrp_able) {

            shig_rep(i, ttrp_size) {

                if (ttrp_list[i].bgn_f != 0)continue;
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

        return;
    }

    vector<CmdPattern> AIshigune::search_way(GameContainer gc, int loop) {

        constexpr int mxm = 200;

        vector<CmdPattern> pcv(0);

        if (gc.current_AI == 0)return pcv;
        gc.p_field_AI = gc.field_AI;
        VI rsv(0); rsv.reserve(30);
        VVI search_tree(mxm, rsv);
        VI parent_tree(mxm, 0);
        int to = 0;
        gc.cp.clear(); gc.cv.clear();

        shig_rep(i, base_cmd.size()) {
            search_tree[i] = base_cmd[i];
            parent_tree[i] = i;
            to++;
        }

        int w = 0;
        Tetri test;
        while (!search_tree[w].empty() && (w < mxm - 1)) {
            bool can = true;
            
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
                CmdPattern c(test, search_tree[w], parent_tree[w]);
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
                        CmdPattern c(test, add_tree, w); c.set_isSFT(true);
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

    void AIshigune::do_sw(vector<CmdPattern>& ctl, GameContainer gc, int loop) {

        //constexpr size_t mxm = 600;
        vector<CmdPattern> pcv(0);

        if (gc.current_AI == 0)return;
        gc.p_field_AI = gc.field_AI;
 
        Tetri test;
        CmdPattern c;
        gc.cp.clear(); gc.cv.clear();

        std::set<Tetri> SearchList;

        std::queue<std::vector<int>> SearchTree;
        std::vector<int> add_tree;
        std::vector<int> soft_tree;


        // í èÌíTçı 
        int w = 0;
        for (auto&& bc : BaseCmdS) {
            SearchTree.push(bc);
        }
        while (!SearchTree.empty()) {
            bool can = true;

            test.minset(gc.current_AI);
            soft_tree = SearchTree.front();

            for (size_t i = 0; i < SearchTree.front().size(); i++) {
                if (!move_mino(test, SearchTree.front().at(i), gc)) {
                    can = false;
                    break;
                }
            }
            if (!can) {
                w++;
                SearchTree.pop();
                continue;
            }

            int sft = -1, hd_cnt = 0;
            while (move_check(0, sft * (hd_cnt + 1), test, gc)) {
                soft_tree.push_back(2);
                hd_cnt++;
            }
            move_mino(test, 3, gc);
            //SearchList.insert(test);
            c = CmdPattern(test, SearchTree.front(), w);
            c.cmd_list.push_back(3);
            decltype(gc.cp)::iterator it = gc.cp.find(c);
            if (it != gc.cp.end()) {
                w++;
                SearchTree.pop();
                continue;
            }
            get_score(c, gc, loop);
            c.pre_gc = gc.slot_id;
            pcv.push_back(c);
            gc.cp.insert(c);

            //SearchTree.push(add_tree);

            // É\ÉtÉhÉçâÒì]ì¸óÕ
            for (auto&& tm_soft : TestMove_soft) {
                test.minset(gc.current_AI);
                can = true;
                add_tree = soft_tree;
                add_tree.push_back(tm_soft);
                for (size_t i = 0; i < add_tree.size(); i++) {
                    if (!move_mino(test, add_tree.at(i), gc)) {
                        can = false;
                        break;
                    }
                }
                if (!can) continue;
                SearchTree.push(add_tree);
            }

            w++;
            SearchTree.pop();

            //if (parent_tree.at(w) == w) {
            //    test.minset(gc.current_AI);
            //    shig_rep(i, search_tree.at(w).size() - 1) {
            //        if (!move_mino(test, search_tree.at(w).at(i), gc)) {
            //            can = false;
            //            break;
            //        }
            //    }
            //    if (!can) {
            //        w++;
            //        continue;
            //    }
            //    int sft = -1, hd_cnt = 0;
            //    while (move_check(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
            //    move_mino(test, 3, gc);
            //    CmdPattern c(test, search_tree.at(w), parent_tree.at(w));
            //    decltype(gc.cp)::iterator it = gc.cp.find(c);
            //    if (it != gc.cp.end()) {
            //        w++;
            //        continue;
            //    }
            //    get_score(c, gc, loop);
            //    c.pre_gc = gc.slot_id;
            //    pcv.push_back(c);
            //    gc.cp.insert(c);
            //    VI w_sft = search_tree.at(w);
            //    w_sft.pop_back();
            //    shig_rep(i, hd_cnt)w_sft.push_back(2);
            //    search_tree[to] = w_sft; parent_tree[to] = parent_tree.at(w); to++;
            //    w++;
            //}
            //else {
            //    if (search_tree.at(w).back() != 3) {
            //        const VI test_case = { 6, 7, 4, 5 };
            //        shig_rep(h, test_case.size()) {
            //            test.minset(gc.current_AI);
            //            can = true;
            //            VI add_tree = search_tree.at(w);
            //            add_tree.push_back(test_case[h]);
            //            shig_rep(i, add_tree.size()) {
            //                if (!move_mino(test, add_tree.at(i), gc)) {
            //                    can = false;
            //                    break;
            //                }
            //            }
            //            if (!can) continue;
            //            int sft = -1; int hd_cnt = 0;
            //            while (move_check(0, sft * (hd_cnt + 1), test, gc)) hd_cnt++;
            //            move_mino(test, 3, gc);
            //            add_tree.push_back(3);
            //            CmdPattern c(test, add_tree, w); c.set_isSFT(true);
            //            decltype(gc.cp)::iterator it = gc.cp.find(c);
            //            if (it != gc.cp.end()) continue;
            //            get_score(c, gc, loop);
            //            gc.cp.insert(c);
            //            add_tree.pop_back();
            //            c.pre_gc = gc.slot_id;
            //            pcv.push_back(c);
            //            shig_rep(i, hd_cnt)add_tree.push_back(2);
            //            if (to < mxm - 1) {
            //                search_tree[to] = add_tree;
            //                parent_tree[to] = parent_tree.at(w);
            //                to++;
            //            }
            //        }
            //        w++;
            //    }
            //    else {
            //        w++;
            //    }
            //}

        }

        // â°ì¸ÇÍíTçı 
        w = 0;
        if (gc.ttrp_able) {
            for (auto&& bc : BaseCmdD) {
                SearchTree.push(bc);
            }
        }
        while (!SearchTree.empty()) {
            bool can = true;

            test.minset(gc.current_AI);
            soft_tree = SearchTree.front();

            for (size_t i = 0; i < SearchTree.front().size(); i++) {
                if (!move_mino(test, SearchTree.front().at(i), gc)) {
                    can = false;
                    break;
                }
            }

            if (!can) {
                w++;
                SearchTree.pop();
                continue;
            }

            decltype(SearchList)::iterator isl = SearchList.find(test);
            if (isl != SearchList.end()) {
                w++;
                SearchTree.pop();
                continue;
            }
            SearchList.insert(test);
            
            // ìríÜâ°ì¸ÇÍìÆçÏ
            for (auto&& tm_side : TestMove_side) {
                test.minset(gc.current_AI);
                can = true;
                add_tree = SearchTree.front();
                add_tree.push_back(tm_side);
                for (size_t i = 0; i < add_tree.size(); i++) {
                    if (!move_mino(test, add_tree.at(i), gc)) {
                        can = false;
                        break;
                    }
                }
                if (!can) continue;
                SearchTree.push(add_tree);
            }

            move_mino(test, 3, gc);
            c = CmdPattern(test, SearchTree.front(), w);
            c.cmd_list.push_back(3);
            decltype(gc.cp)::iterator it = gc.cp.find(c);
            if (it != gc.cp.end()) {
                w++;
                SearchTree.pop();
                continue;
            }
            get_score(c, gc, loop);
            c.pre_gc = gc.slot_id;
            pcv.push_back(c);
            gc.cp.insert(c);

            w++;
            SearchTree.pop();

        }

        std::lock_guard<std::mutex> lock(mtx);
        for (CmdPattern p : pcv) {
            ctl.push_back(p);
        }

        return;
    }

    CmdPattern AIshigune::explore_choices(GameContainer gc_org) {

        gc_slot = vector<GameContainer>(branch_num);
        vector<vector<CmdPattern>> branch(branch_num, vector<CmdPattern>(0));
        vector<CmdPattern> catalog(0);
        constexpr int cls = 60 * branch_num;
        catalog.reserve(cls);

        gc_org.cp.clear(); gc_org.cv.clear();
        //catalog.clear();

        static const size_t frt = 25;

        //catalog.emplace_back(search_way(gc_org, -1));
        // 1èÑñ⁄íTçı
        do_sw(std::ref(catalog), gc_org, -1);

        std::sort(all(catalog), [&](const CmdPattern& l, const CmdPattern& r) { return r.scr < l.scr; });

        if ((int)catalog.size() == 0) {
            return null_cmd;
        }
        else if ((int)catalog.size() < branch_num && (int)catalog.size() > 0) {
            for (int i = 0; i < catalog.size(); i++) {
                //auto [pct, ci] = catalog.at(i);

                branch.at(i).push_back(catalog.at(i));
                gc_slot.at(i) = update_gc(catalog.at(i), gc_org);
                gc_slot.at(i).slot_id = i;
            }
            for (int i = (int)catalog.size(); i < branch_num; i++) {
                gc_slot.at(i) = GameContainer();
                gc_slot.at(i).slot_id = i;
            }
        }
        else {
            
            
            for (size_t i = 0; i < frt; i++) { //
                //auto [pct, ci] = catalog.at(i);
                branch.at(i).push_back(catalog.at(i));
                gc_slot.at(i) = update_gc(catalog.at(i), gc_org);
                gc_slot.at(i).slot_id = i;
            }

            std::vector<int> RndChoise(catalog.size() - frt, 0);
            std::iota(RndChoise.begin(), RndChoise.end(), 0);
            std::shuffle(RndChoise.begin(), RndChoise.end(), Rengine);

            for (size_t i = 0; i < std::min(branch_num - frt, RndChoise.size()); i++) { //
                branch.at(i + frt).push_back(catalog.at(RndChoise.at(i)));
                gc_slot.at(i + frt) = update_gc(catalog.at(RndChoise.at(i)), gc_org);
                gc_slot.at(i + frt).slot_id = i + frt;
            }

            /*int mid = (int)(catalog.size() / 2);

            for (int i = 0; i < branch_num - frt; i++) {
                branch.at((size_t)(frt + i)).push_back(catalog.at((size_t)(i + mid)));
                gc_slot.at((size_t)(frt + i)) = update_gc(catalog.at((size_t)(i + mid)), gc_org);
                gc_slot.at((size_t)(frt + i)).slot_id = frt + i;
            }*/


        }

        // 2èÑñ⁄à»ç~ÇÃêÊì«Ç›ÉãÅ[Év
        shig_rep(n, exp_cyc_lim - 1) {
            catalog.clear(); catalog.reserve(cls);

            int loop = n;

            int epwtn = expl_width.at(n);
            std::vector<std::thread> threads;

            for (int i = 0; i < epwtn; i++) {
                GameContainer gc = gc_slot.at(i);
                threads.emplace_back(std::thread([this, &catalog, gc, loop]() {this->do_sw(ref(catalog), gc, loop); }));
            }

            for (auto& thr : threads) {
                thr.join();
            }

            sort(all(catalog), [&](const CmdPattern& l, const CmdPattern& r) { return r.scr < l.scr; });
            vector<GameContainer> proxy_slot(branch_num);
            vector<vector<CmdPattern>> proxy_branch(branch_num, vector<CmdPattern>(0));
            shig_rep(i, epwtn) {
                if ((int)catalog.size() <= i || (int)catalog.size() == 0) {
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

            std::vector<int> RndChoise(catalog.size() - frt, 0);
            std::iota(RndChoise.begin(), RndChoise.end(), 0);
            std::shuffle(RndChoise.begin(), RndChoise.end(), Rengine);

            for (size_t i = 0; i < std::min(branch_num - frt, RndChoise.size()); i++) { //
                branch.at(i + frt).push_back(catalog.at(RndChoise.at(i)));
                gc_slot.at(i + frt) = update_gc(catalog.at(RndChoise.at(i)), gc_org);
                gc_slot.at(i + frt).slot_id = i + frt;
            }


            //int mid = (int)catalog.size() / 2;
            //for (int i = 0; i < branch_num - epwtn; i++) {
            //    if ((int)catalog.size() <= i + mid || (int)catalog.size() == 0) {
            //        branch.at((size_t)(epwtn + i)).push_back(null_cmd);
            //        proxy_slot.at((size_t)(epwtn + i)) = update_gc(null_cmd, gc_slot.at(0));
            //        proxy_slot.at((size_t)(epwtn + i)).slot_id = epwtn + i;
            //    }
            //    else {
            //        //auto [pct, ci] = catalog.at(i);
            //        int ci = catalog.at((size_t)(i + mid)).pre_gc;
            //        proxy_branch.at((size_t)(epwtn + i)) = branch.at(ci);
            //        proxy_branch.at((size_t)(epwtn + i)).push_back(catalog.at((size_t)(i + mid)));
            //        proxy_slot.at((size_t)(epwtn + i)) = update_gc(catalog.at((size_t)(i + mid)), gc_slot.at(ci));
            //        proxy_slot.at((size_t)(epwtn + i)).slot_id = epwtn + i;
            //    }
            //}


            gc_slot = proxy_slot;
            branch = proxy_branch;

        }

        if ((int)catalog.size() == 0)return null_cmd;
        //auto [r, ri] = catalog.at(0);
        int ri = catalog.at(0).pre_gc;
        return branch.at(ri).front();


    }

    // score evaluating
    void AIshigune::get_score(CmdPattern& cd, GameContainer& gcs, int loopc) {

        cd.scr.init_s();//without sum

        //gcs.p_field_AI = gcs.field_AI;
        //AIshigune::apply_mino(gcs.p_field_AI, cd.pat);
        set<int> L = erase_check_AI(cd.pat, gcs);

        //AIshigune::height_calc(gcs);

        cd.set_ts(gcs.SRS_kind, gcs.TS_kind);

        VVI q_field_AI(0); q_field_AI.reserve(45);
        if ((int)L.size() > 0) {
            shig_rep(i, fh) {
                decltype(L)::iterator it = L.find(i);
                if (it == L.end()) q_field_AI.push_back(gcs.p_field_AI.at(i));
            }
            while ((int)q_field_AI.size() < fh) q_field_AI.push_back(ev_empty);
        }
        else {
            q_field_AI = VVI(gcs.p_field_AI);
        }

        //LL fusion = 0;
        LL contact = 0;
        LL touch = 0;
        LL high = 0;
        LL btbc = 0;

        //LL cmb = 0;

        int rot = cd.pat.rot;
        int idnt = cd.pat.id - 1;
        int H = (int)cd.pat.mino[rot].size();
        int W = (int)cd.pat.mino[rot][0].size();
        int Ls = (int)L.size();

        //constexpr int hm = 12;
        //constexpr int hs = hm * 8;

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
        Tetri ofs_cdp = cd.pat;
        ofs_cdp.addY(gcs.ttrp_ofsY);

        if (gcs.ttrp_able == true) {

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
                            cd.set_ttrpF(-1);
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
                    cd.set_ttrpF(-1);
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
            LL pnl = -100000000;
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
        int cnt_n = 0;
        //int cnt_m = 0;
        vector<bool> chk_con(W, false);
        shig_rep(j, W) {
            if (ch.at(idnt).at(rot).at(j) == 1) {
                //cnt_m++;
                int xp = cd.pat.X + j;
                if (xp < 0 || xp >= 10)continue;
                int h = cd.pat.Y - gcs.height.at(xp) - 1;
                if (h >= 0 && h < H) {
                    if (cd.pat.mino.at(rot).at(h).at(j) != 0) {
                        cnt_n++;
                    }
                    else {
                        if (h >= 1) {
                            if (cd.pat.mino.at(rot).at((size_t)h - 1).at(j) != 0) {
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
        const LL pnl_A = 3000, pnl_B = 1000, pnl_C = -1000;

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
                else if (cnt_n == 2) {
                    if (chk_con.at(0) == true || chk_con.at(2) == true) {
                        contact += pnl_B;
                    }
                    else contact += pnl_C;
                }
                else contact += pnl_C;
            }
            else if (cd.pat.rot == 1) {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1) {
                    if (chk_con.at(1) == false && chk_con.at(2) == true) {
                        contact += pnl_B;
                    }
                    else contact += pnl_C;
                }
                else contact += pnl_C;
            }
            else if (cd.pat.rot == 3) {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1) {
                    if (chk_con.at(1) == false && chk_con.at(0) == true) {
                        contact += pnl_B;
                    }
                    else contact += pnl_C;
                }
                else contact += pnl_C;
            }
        }
        else if (cd.pat.id == 3) {
            if (cd.pat.rot == 0 || cd.pat.rot == 2) {
                if (cnt_n == 3)contact += pnl_A;
                else if (cnt_n == 2) {
                    if (chk_con.at(0) == true || chk_con.at(2) == true) {
                        contact += pnl_B;
                    }
                    else contact += pnl_C;
                }
                else contact += pnl_C;
            }
            else if (cd.pat.rot == 1) {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1) {
                    if (chk_con.at(1) == false && chk_con.at(2) == true) {
                        contact += pnl_B;
                    }
                    else contact += pnl_C;
                }
                else contact += pnl_C;
            }
            else if (cd.pat.rot == 3) {
                if (cnt_n == 2)contact += pnl_A;
                else if (cnt_n == 1) {
                    if (chk_con.at(1) == false && chk_con.at(0) == true) {
                        contact += pnl_B;
                    }
                    else contact += pnl_C;
                }
                else contact += pnl_C;
            }
        }
        else if (cd.pat.id == 4) {
            if (cnt_n == 2)contact += pnl_A;
            else if (cnt_n == 1)contact += pnl_B;
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
        high = 5000;
        if (cdY > 15) {
            high = (-20 * cdY * cdY + 2865);
            high -= (cdY - gcs.height_mxm + 1) * 4000;
        }
        else if (cdY > 10) {
            high = (-10 * cdY * cdY + 615);
            high -= (cdY - gcs.height_mxm + 1) * 1000;

        }
        else if (cdY > 5)
        {
            high = (-5LL * cdY * cdY + 115);
            high -= (cdY - gcs.height_mxm + 1) * 250;

        }
        else {
            high = (-2LL * cdY - 0);
            high -= (cdY - gcs.height_mxm + 1) * 0;
        }


        //closure & touch
        int tls = (int)touch_list.at(idnt).at(rot).size();
        LL thc = 0, thcls = 0;
        for (int i = 0; i < tls; i++) {

            int ty = 0, tx = 0;
            pairI2 tt = touch_list.at(idnt).at(rot).at(i);
            ty = tt.first; tx = tt.second;

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
                    touch = shig::secure_add(touch, 10);
                }

                continue;
            }
            else {
                if (tx == 0 || tx == 9) {
                    touch = shig::secure_add(touch, -10);
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
                touch = shig::secure_add(touch, -8000LL);
                thcls++;
            }
            else {
                touch = shig::secure_add(touch, -1000LL * closure);
            }

        }

        touch = shig::secure_add(touch, thc * 1000);

        if (thcls == 0) {
            touch = shig::secure_add(touch, 10000LL);
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
        LL cmb_rate = 1000000;
        if (gcs.combo * L.size() > 0) {
            LL c = (cmb_rate * 10LL * (gcs.combo + 1LL));
            cd.scr.cmb = shig::secure_add(cd.scr.cmb, c);

        }
        else if (gcs.combo > 0 && L.size() == 0) {
            LL c = (cmb_rate * -20LL * (gcs.combo + 1LL));
            cd.scr.cmb = shig::secure_add(cd.scr.cmb, c);
        }
        else if (gcs.combo == 0 && L.size() == 0) {
            LL c = cmb_rate;
            cd.scr.cmb = shig::secure_add(cd.scr.cmb, c);
        }
        else {
            //cd.update(gcs.combo * 0LL);
        }



        LL ve = 0;
        if (L.size() == 0) {

            if (gcs.height_sum > 144 || gcs.height_mxm > 16) {
                ve += 50;

            }
            else if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                ve += 500;
                /*
                if (cd.pat.id == 6)ve += 200;
                else if (cd.pat.id == 1)ve += 300;
                */

            }
            else if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                ve += 2000;
                if (cd.pat.id == 6)ve += -800;
                else if (cd.pat.id == 1)ve += -400;
            }
            else {
                ve += 4000;

                if (cd.pat.id == 6)ve += -1000;
                else if (cd.pat.id == 1)ve += -500;

            }

        }
        else if (L.size() == 1) {
            if (gcs.TS_kind == 1) {

                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve += 20000;
                }
                else if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                    ve += 100000;
                }
                else {
                    ve += 50000;
                }

                if (gcs.btb > -1)btbc += 1000;

            }
            else if (gcs.TS_kind == 2) {

                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve += 500;
                }
                else {
                    ve += 1000;
                }

                if (gcs.btb > -1)btbc += 50;
            }
            else {

                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve +=1000;
                    if (cd.pat.id == 1) {
                        ve += 1000;
                    }
                }
                else if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                    ve += -10000;
                    if (cd.pat.id == 6) {
                        ve += -1000;
                    }
                    else if (cd.pat.id == 1) {
                        ve += -1000;
                    }
                }
                else {
                    ve += -500000;
                    if (cd.pat.id == 6) {
                        ve += -200000;
                    }
                    else if (cd.pat.id == 1) {
                        ve += -100000;
                    }
                }

                if (gcs.btb > -1)btbc += -3000;

            }
        }
        else if (L.size() == 2) {
            if (gcs.TS_kind == 1) {

                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve += 300000;
                }
                else if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                    ve += 400000000;
                }
                else {
                    ve += 2000000000;
                }

                if (gcs.btb > -1)btbc += 10000000;

            }
            else if (gcs.TS_kind == 2) {
                ve += 200;
                if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                    ve -= 500;
                }

                if (gcs.btb > -1)btbc += 100;

            }
            else {

                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve += 2000;

                    if (cd.pat.id == 6) {
                        ve += 500;
                    }
                    else if (cd.pat.id == 1) {
                        ve += 1000;
                    }

                }
                else if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                    ve += -12000;
                    if (cd.pat.id == 6) {
                        ve += -8000;
                    }
                    else if (cd.pat.id == 1) {
                        ve += -4000;
                    }
                }
                else {
                    ve += -60000;
                    if (cd.pat.id == 6) {
                        ve += -30000;
                    }
                    else if (cd.pat.id == 1) {
                        ve += -10000;
                    }

                }

                if (gcs.btb > -1)btbc += -20000;

            }
        }
        else if (L.size() == 3) {
            if (gcs.TS_kind == 1) {
                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve += 200000;
                }
                else {
                    ve += 700000;
                }

                if (gcs.btb > -1)btbc += 100000;

            }
            else if (gcs.TS_kind == 2) {
                ve += 0;
                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve -= 0;
                }
            }
            else {

                if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                    ve += 10000;
                    if (cd.pat.id == 1) {
                        ve += 4000;
                    }
                }
                else if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                    ve += -10000;
                    /*if (cd.pat.id == 6) {
                        ve += -1000;
                    }
                    else if (cd.pat.id == 1) {
                        ve += -1000;
                    }*/
                }
                else {
                    ve += -20000;
                    if (cd.pat.id == 1) {
                        ve += -5000;
                    }
                }

                if (gcs.btb > -1)btbc += -10000;

            }
        }
        else if (L.size() == 4) {
            if (gcs.height_sum >= 96 && gcs.height_mxm >= 12) {
                ve += 200000;
            }
            else if (gcs.height_sum >= 64 && gcs.height_mxm >= 8) {
                ve += 400000;
            }
            else {
                ve += 700000;
            }

            if (gcs.btb > -1)btbc += 80000;

        }


        LL ve_rate = 500;
        /*if (gcs.height_sum > hs || gcs.height_mxm > hm) {
            ve_rate *= 10;
        }*/

        ve *= ve_rate;
        //cd.update(ve);
        cd.scr.erase = ve;


        touch *= 1500;
        //cd.update(touch);
        cd.scr.touch = touch;

        contact *= 1200;
        //cd.update(contact);
        cd.scr.contact = contact;

        high *= 5400;
        //cd.update(high);
        cd.scr.height = high;

        //cd.update(AIshigune::gs_BFS(cd, gcs));
        //cd.scr.closed = AIshigune::gs_BFS(cd, q_field_AI);
        if (gcs.height_sum <= 72 || gcs.height_mxm <= 8) {
            cd.scr.closed = AIshigune::gs_BFS(cd, q_field_AI) * 1500;
        }
        else {
            cd.scr.closed = AIshigune::gs_BFS(cd, q_field_AI) * 1000;
        }

        if (gcs.height_sum < 90 && gcs.height_mxm < 10) {
            if (cd.isSFT) {
                //cd.update(-20);
                cd.scr.sum += -20;
            }
        }
        else
        {
            if (cd.isSFT) {
                //cd.update(0);
                cd.scr.sum += 0;
            }
        }

        if (gcs.height_sum < 90 || gcs.height_mxm < 10) {
            btbc *= 10;
        }
        //cd.update(btbc);
        cd.scr.btbc = btbc;

        //cd.update(gcs.pre_score);
        cd.scr.sum += gcs.pre_score;

        //cd.update();
        cd.scr.calc_sum();
        if (loopc == -1)cd.score = shig::secure_add(cd.score, cd.scr.sum);
        else {
            //cd.score = shig::secure_add(cd.score, (cd.scr.sum * scr_rate.at(loopc) / 100));
            cd.score = shig::secure_add(cd.score, cd.scr.sum);
        }
        //cd.score /= 100;

        return;
    }

    LL AIshigune::gs_BFS(CmdPattern& cb, VVI& qf) {

        LL open_blc = 0;
        LL clos_blc = 0;
        LL mino_blc = 0;
        LL grbg_blc = 0;

        const int h_limit = 40;

        VVI b_field_AI(h_limit + 1, (VI(10, -1)));

        queue<pair<int, int>> xy;
        xy.push({ h_limit, 0 });

        while (!xy.empty()) {
            pair<int, int> now = xy.front(); xy.pop();
            if (b_field_AI.at(now.first).at(now.second) != -1)continue;
            else b_field_AI.at(now.first).at(now.second) = 0;

            for (int i = 0; i < cc.size(); i++) {
                int y = now.first + cc.at(i).first;
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
        b_closure -= 500 * grbg_blc;
        b_closure += 0 * open_blc;

        b_closure *= 1;

        return b_closure;

    }

    LL AIshigune::gs_well(CmdPattern& cp, VVI& qf)
    {

        //LL well = 0;





        return LL();
    }

    bool AIshigune::height_calc(GameContainer& gch) {
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

    bool AIshigune::move_check(int to_x, int to_y, Tetri& s_check, GameContainer& ggc) {
        int cnt = 0, cntt = 4;
        int rot = s_check.rot;
        //int size = s_check.px_size;
        int H = (int)s_check.mino[rot].size();
        int W = (int)s_check.mino[rot][0].size();
        shig_rep(i, H) {
            shig_rep(j, W) {
                if (s_check.mino[rot][i][j] != 0) {
                    int sX = s_check.X + j + to_x;
                    if (sX < 0 || sX >= 10)break;
                    int sY = s_check.Y - i + to_y;
                    if (sY <= 0 || sY >= ((int)ggc.field_AI.size() - 1))break;
                    if (ggc.field_AI[sY - 1LL][sX] == 0)cnt++;
                }
            }
        }
        return cnt == cntt;
    }

    bool AIshigune::SRS_check(int lr, Tetri& now, GameContainer& ggc) {
        Tetri test = now;
        //int size = test.px_size;
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

    int AIshigune::TS_check(int toX, int toY, Tetri& ts, GameContainer& ggc) {
        if (ts.id != 6) {
            ggc.TS_kind = 0;
            return 0;
        }

        int ts_cnt = 0, rot = ts.rot;
        VI testX = { 0, 0, 2, 2 }, testY = { 0, 2, 0, 2 };
        pairI2 check = { 0, 0 };

        shig_rep(i, 4) {
            int sX = ts.X + testX[i] + toX;
            if (sX < 0 || sX >= 10) {
                ts_cnt++;
                continue;
            }
            int sY = ts.Y - testY[i] + toY - 1;
            if (sY < 0 || sY >= ((int)ggc.field_AI.size() - 1)) {
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

    set<int> AIshigune::erase_check_AI(Tetri& s_now, GameContainer& gce) {
        set<int> erase_itr;
        gce.p_field_AI = gce.field_AI;
        AIshigune::apply_mino(gce.p_field_AI, s_now);
        int rot = s_now.rot;
        //int size = s_now.px_size;
        int H = (int)s_now.mino[rot].size();
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

    void AIshigune::apply_mino(VVI& c_field, Tetri& s_now) {
        int rot = s_now.rot;
        //int size = s_now.px_size;
        int H = (int)s_now.mino[rot].size();
        int W = (int)s_now.mino[rot][0].size();
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

    bool AIshigune::move_mino(Tetri& m_now, int s_action, GameContainer& ggc) {
        //int size = m_now.px_size;
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

    void AIshigune::print_gh(Tetri& s_now) {
        int rot = s_now.rot;
        //int size = s_now.px_size;
        int id = s_now.id;
        int H = (int)s_now.mino[rot].size();
        int W = (int)s_now.mino[rot][0].size();
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

    void AIshigune::move_itr(int lr) {

        if (lr == -1) {
            if (cp_itr != cp.begin()) --cp_itr;
        }
        else if (lr == 1) {
            if (++cp_itr != cp.end()) ++cp_itr;
        }

        return;
    }

    VVI AIshigune::get_AI_field(int p, int m) {
        if (p == 1) {
            p_field_AI = field_AI;
        }
        if (m == 1) {
            if (cp_itr == cp.end())--cp_itr;
            CmdPattern cm = *cp_itr;
            print_gh(cm.pat);
        }
        return p_field_AI;
    }

    VI AIshigune::get_AI_cmd() {
        CmdPattern cl = *cp_itr;
        return cl.cmd_list;
    }

    bool AIshigune::load_ttrp() {
        GetTempNameList(ttrp_name_list);
        ttrp_size = (int)ttrp_name_list.size();
        //template_list.reserve(ttrp_size+1);
        vector<TetriPlate> vt(ttrp_size);
        shig_rep(i, ttrp_size) {
            ReadTempData(ttrp_name_list[i], vt[i]);
            ttrp_id_list.push_back(vt[i].tp_id);
        }

        ttrp_list = vt;

        return true;
    }

    bool AIshigune::ttrp_crr(Tetri& now_p, int& size_l) {
        bool chk_f = false;
        if (ttrp_able == true) {
            shig_rep(i, min({ now_ttrp.mino_num , (int)now_ttrp.list_mino.size() })) {
                if (now_ttrp.list_mino[i] == now_p) {
                    if (now_p.id == 6) {
                        if ((now_ttrp.list_mino_s[i]) == size_l && TS_kind == 1) {
                            chk_f = true;
                        }
                        else if (now_ttrp.list_mino_s[i] == 0 && TS_kind == 0) {
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

    bool AIshigune::ttrp_check(CmdPattern& slc, int& sle) {

        VI mnL = make_order_list();

        if (ttrp_able == false)return false;

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

    bool AIshigune::ttrp_check(CmdPattern& slc, int& sle, VI& mnL)
    {
        return false;
    }

    bool AIshigune::ttrp_check(CmdPattern& slc, int& sle, GameContainer& gct) {

        //VI mnL = make_order_list(gct);
         
        if (gct.ttrp_able == false)return true;

        if (slc.ttrp_f >= 0) {
            gct.now_ttrp.mino_check.at((size_t)slc.ttrp_f) = true;
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

    bool AIshigune::next_crr_check() {

        set<int> all_chk;
        int lim_s = min(5, (int)next_AI.size());
        shig_rep(i, lim_s)all_chk.insert(next_AI[i]);
        all_chk.insert(current_AI);
        all_chk.insert(hold_AI);
        if ((int)all_chk.size() == 7)return true;
        else if (hold_AI == 0 && (int)all_chk.size() == 6)return true;
        else return false;

        return false;

    }

    pair<int, string> AIshigune::get_sttrp_name() {
        str = "noting selected";
        if (ttrp_able)return make_pair(now_ttrp.tp_id, now_ttrp.temp_name);
        else return make_pair(i32_minus_one, s);
    }

    bool AIshigune::set_gc(GameContainer& gc) {

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
        //cv = vector<CmdPattern>(0);
        //cv.reserve(600);
        //ec = vector<CmdPattern>(0);
        //ec.reserve(10);


        return true;
    }

    GameContainer AIshigune::update_gc(CmdPattern& ct, GameContainer gcp) {

        set<int> itr = AIshigune::erase_check_AI(ct.pat, gcp);
        int itr_s = (int)itr.size();

        VVI proxy(0); proxy.reserve(45);

        shig_rep(i, fh) {
            decltype(itr)::iterator it = itr.find(i);
            if (it == itr.end()) proxy.push_back(gcp.p_field_AI[i]);
        }
        while ((int)proxy.size() < fh) proxy.push_back(ev_empty);

        gcp.field_AI = proxy;

        if (ct.cmd_list.at(0) == 1) gcp.hold_AI = gcp.current_AI;
        if ((int)gcp.q_next_AI.size() <= 1) {
            shig_rep(i, NS_a.size()) {
                gcp.q_next_AI.push(NS_a.at(i));
            }
        }
        if ((int)gcp.q_next_AI.size() != 0) {
            gcp.current_AI = gcp.q_next_AI.front();
            gcp.q_next_AI.pop();  gcp.next_AI.clear();
            while (!gcp.q_next_AI.empty()) {
                int q = gcp.q_next_AI.front();
                gcp.next_AI.push_back(q);
                gcp.q_next_AI.pop();
            }
            for (size_t i = 0; i < gcp.next_AI.size(); i++) {
                gcp.q_next_AI.push(gcp.next_AI.at(i));
            }
        }


        AIshigune::height_calc(gcp);

        AIshigune::ttrp_check(ct, itr_s, gcp);

        if (gcp.combo == 0) {
            if (itr_s == 0) {

            }
            else {
                gcp.combo++;
            }
        }
        else if (gcp.combo > 0) {
            if (itr_s == 0) {
                gcp.combo = 0;
            }
            else {
                gcp.combo++;
            }
        }

        if (ct.scr.btbc > 0) {
            gcp.btb += 1;
        }
        else if (ct.scr.btbc <= 0) {
            gcp.btb = -1;
        }

        gcp.pre_score = shig::secure_add(gcp.pre_score, ct.score);
        //gcp.pre_score += ct.score;

        return gcp;
    }

    int AIshigune::ttrp_check_mino(Tetri& fd, GameContainer& gcf) {

        for (int i = 0; i < gcf.now_ttrp.mino_num; i++) {
            if (gcf.now_ttrp.list_mino.at(i) == fd)return i;
        }

        return -1;

    }

    AIshigune::~AIshigune() {

    }

}


bool GetTempNameList(VS& name_list) {
    int n = 0;
    //FILE* fp = NULL;
    //fopen_s(&fp, "template\\tetriplate_list.txt", "r");
    ////hndl_tmplist = FileRead_open("template\\tetriplate_list.txt");
    ////if (hndl_tmplist == 0)return false;

    //if (fp != NULL) {
    //    fscanf_s(fp, "%d", &n);
    //    if (n < 0)n = 0;

    //    VS list(n);
    //    shig_rep(i, n) {
    //        char tmpC[64];
    //        fscanf_s(fp, "%s", tmpC, 64);
    //        if (tmpC[0] == '\0' || tmpC[0] == '/') {
    //            i--;
    //            continue;
    //        }
    //        list[i] = tmpC;
    //        list[i] = "template\\data\\" + list[i];
    //    }

    //    name_list = list;

    //    fclose(fp);
    //}

    std::ifstream ifs("template\\tetriplate_list.txt");
    if (ifs.fail()) {
        std::cerr << "failed to open \"template\\tetriplate_list.txt\" ";
        exit(-4);
    }

    std::string readS;
    std::getline(ifs, readS);
    stringstream ss(readS);
    ss >> n;

    while (std::getline(ifs, readS)) {
        stringstream sl(readS);
        std::string tempS;
        sl >> tempS;
        if (tempS[0] == '\0' || tempS[0] == '/') {

        }
        else {
            name_list.push_back("template\\data\\" + tempS);
        }
    }

    return true;

}

bool ReadTempData(string& name, TetriPlate& ttrp) {

    const char* fname = name.c_str();
    FILE* fp = NULL;
    fopen_s(&fp, fname, "r");
    if (fp == NULL) {
        int z = 0;
        string zs = "";
        ttrp.set(z, z, z, z, zs, z);
        vector<Tetri> zt = { Tetri(0, 0, 0, 0) };
        VI zts(0, 0);
        ttrp.set_list(zt, zts);
        VI zv(0, 0);
        ttrp.set_id_list(zv);
        vector<pairI2> zp(0, make_pair(0, 0));
        ttrp.set_terms(zp);
        return false;
    }
    else {
        int l, ls, id, tn, bf; //mino num : connect list num : ttrp id
        char rtdC[256];
        string rtdS = "";
        fscanf_s(fp, "%d %d %d %d %d %s ", &l, &ls, &id, &tn, &bf, rtdC, 256);
        for (int i = 0; i < 256; i++) {
            if (rtdC[i] == '\0') {
                rtdS = std::string(rtdC, i);
                break;
            }
        }
        ttrp.set(l, ls, id, tn, rtdS, bf);

        //
        vector<Tetri> tpl(l);
        VI ts(l);
        shig_rep(i, l) {
            int r, x, y, d, s;
            fscanf_s(fp, "%d %d %d %d %d", &r, &x, &y, &d, &s);
            Tetri tp(r, x, y, d);
            tpl[i] = tp;
            ts[i] = s;
        }
        ttrp.set_list(tpl, ts);

        //
        VI vil(ls);
        shig_rep(i, ls) {
            int temp = 0;
            fscanf_s(fp, "%d", &temp);
            vil[i] = temp;
        }
        ttrp.set_id_list(vil);

        //
        vector<pairI2> vmp(tn);
        shig_rep(i, tn) {
            int ll = 0, rr = 0;
            fscanf_s(fp, "%d %d", &ll, &rr);
            vmp[i] = make_pair(ll, rr);
        }
        ttrp.set_terms(vmp);
        fclose(fp);
    }

    return true;
}