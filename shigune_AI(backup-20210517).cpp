#include "shigune_AI.h"


//using namespace shig;

namespace shig {

    tetri t(0, 3, 21, 0);

    shigune_AI::shigune_AI(int ii) {
        identifier = ii;
        hold_AI = 0;
        current_AI = 0;
        next_AI = VI(0);
        q_next_AI = queue<int>();
        mind = -1;
        pc_cnt = 0;

        todo = VI(0);
        field_AI = VVI(45, (VI(10, 0)));
        p_field_AI = VVI(45, (VI(10, 0)));
        H_target = VI(10, -1);
        W_target = VI(10, -1);
        strategy_map = VVI(45, (VI(10, 0)));
        strategy_list = vector<pair<int, pairI2>>(0, make_pair(0, make_pair(0, 0)));
        cmd_list = VI(0);
        cp_itr = cp.begin();
    }

    VI shigune_AI::shigune_main() {
        VI cmd = {};
        shigune_AI::get_state();
        shigune_AI::get_field();

        shigune_AI::strategy_mark();

        return cmd;
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
        strategy_list.erase(strategy_list.begin(), strategy_list.end());
        if (current_AI == 1 || hold_AI == 1 || next_AI[0] == 1 || next_AI[2] == 1) {
            if (current_AI == 2 || hold_AI == 2 || next_AI[0] == 2 || next_AI[2] == 2) {
                strategy_list.push_back(make_pair(1, make_pair(7, 2)));
                shig_for(i, 3, 7)strategy_map[0][i] = 1;
                shig_for(i, 0, 2)strategy_map[i][9] = 2;
                strategy_map[0][8] = 2;
            }
            else if (current_AI == 3 || hold_AI == 3 || next_AI[0] == 3 || next_AI[2] == 3) {
                strategy_list.push_back(make_pair(1, make_pair(1, 2)));
            }

        }
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
                shig_rep(i, search_tree[w].size() - 1) {
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
                while (move_check(0, sft * (hd_cnt + 1), test)) hd_cnt++;
                move_mino(test, 3);
                cmd_pattern c(search_tree[w], parent_tree[w], test);
                decltype(cp)::iterator it = cp.find(c);
                if (it != cp.end()) {
                    w++;
                    continue;
                }
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
                        cmd_pattern c(add_tree, w, test);
                        decltype(cp)::iterator it = cp.find(c);
                        if (it != cp.end()) continue;
                        cp.insert(c);
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

    bool shigune_AI::SRS_check(int lr, tetri& now) {
        tetri test = now;
        int size = test.px_size;
        int to_X = 0, to_Y = 0, rot = test.rot, p_srs = -1;
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
            now.set_rot(rot);
            now.addX(to_X);
            now.addY(to_Y);
        }
        return can;

    }

    set<int> shigune_AI::erase_check_AI(VVI& c_field, tetri& s_now) {
        set<int> erase_itr;
        p_field_AI = field_AI;
        shigune_AI::apply_mino(p_field_AI, s_now);
        int rot = s_now.rot;
        int size = s_now.px_size;
        int H = s_now.mino[rot].size();
        int pW = field_AI[0].size();

        shig_rep(i, H) {
            int sY = s_now.Y - i;
            int itr = sY - 1;
            int cnt = 0;
            if (itr < 0 || itr >= 45)continue;
            shig_rep(j, pW) {
                if (field_AI[itr][j] != 0) cnt++;
                else break;
            }
            if (cnt == pW) erase_itr.insert(itr);
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

    shigune_AI::~shigune_AI() {

    }

}