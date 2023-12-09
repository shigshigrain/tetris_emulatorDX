// tetris_emulatorDX.cpp : アプリケーションのエントリ ポイントを定義します。
// v.3.5.0_new_explore


#include "tetris_emulatorDX.h"


//using namespace DxLib;
//using namespace shig;

#define MAX_LOADSTRING 100

const int refrashRate15 = 1000 / 15;
const int refrashRate30 = 1000 / 30;
const int refrashRate60 = 1000 / 60;

int loopF = 0, g_check = 0;
int delay_cnt = 0;

int g_event = 0;

int push_cnt = 0;

int BG_hndl = 0;
VI Gr_mino(16, 0);
VI f_handle(1);

int sectime = GetNowCount();
VI act_flag(8, 0);

char KeyBuf[256];

const int null = NULL;

static constexpr int INIT_WINDOW_CLIENT_X_SIZE = 1536;
static constexpr int INIT_WINDOW_CLIENT_Y_SIZE = 800;

const int dxLibError = -1;
inline bool DXLibError(int result) {
    return result == dxLibError ? true : false;
}

// ameyasu's code
static BOOL monitorenumproc(HMONITOR hMon, HDC hMonDC, LPRECT lpMonRect, LPARAM monRects) {
    ((std::vector<RECT>*)monRects)->push_back(*lpMonRect);
    return TRUE;
}

// ameyasu's code
static LRESULT CALLBACK WndProcHook(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {

    static std::vector<RECT> s_monRects;

    // スナップするか否か、するなら四隅のどこか
    static enum class DoSnap {
        NO, TL, TR, BL, BR, FW
    } s_doSnap = DoSnap::NO;

    // スナップすることが決まったときのモニターのインデックス
    static int s_monIndex = 0;

    // ウィンドウサイズとクライアントサイズの差
    static RECT s_wndDiffRect = {};

    switch (msg) {
    case WM_CREATE:
    {
        // 全モニターのRECTを取得する
        EnumDisplayMonitors(NULL, NULL, monitorenumproc, (LPARAM)(&s_monRects));

        // ウィンドウサイズとクライアントサイズの差を計算
        DWORD style = (DWORD)GetWindowLong(handle, GWL_STYLE);
        s_wndDiffRect = { 0 , 0, INIT_WINDOW_CLIENT_X_SIZE, INIT_WINDOW_CLIENT_Y_SIZE };
        AdjustWindowRect(&s_wndDiffRect, style, FALSE);
        s_wndDiffRect.right -= INIT_WINDOW_CLIENT_X_SIZE;
        s_wndDiffRect.bottom -= INIT_WINDOW_CLIENT_Y_SIZE;
    }
    break;
    case WM_MOVING:
    {
        s_doSnap = DoSnap::NO;

        // 各モニターのRECTでループ
        for (int i = 0; i < s_monRects.size(); ++i)
        {
            RECT& r = s_monRects[i];

            POINT p = {};
            GetCursorPos(&p);

            // カーソルが存在するモニターでしか処理しない
            if (p.x < r.left || r.right < p.x || p.y < r.top || r.bottom < p.y)
                continue;

            // スナップ処理が発動するマウス座標の画面端判定範囲に余裕を持たせる
            static constexpr int padding = 50;
            // 画面中央の位置を計算
            int centerX = (r.left + r.right) / 2;

            // 左上
            if (p.x <= r.left + padding && p.y <= r.top + padding)
            {
                s_doSnap = DoSnap::TL;
                s_monIndex = i;
            }
            // 右上
            if (p.x >= r.right - padding && p.y <= r.top + padding)
            {
                s_doSnap = DoSnap::TR;
                s_monIndex = i;
            }
            // 左下
            if (p.x <= r.left + padding && p.y >= r.bottom - padding)
            {
                s_doSnap = DoSnap::BL;
                s_monIndex = i;
            }
            // 右下
            if (p.x >= r.right - padding && p.y >= r.bottom - padding)
            {
                s_doSnap = DoSnap::BR;
                s_monIndex = i;
            }
            // 中央上 x軸中央から左右200px,↓100pxのマージン
            if (p.x >= centerX - padding * 4 && p.x <= centerX + padding * 4 && p.y <= r.top + padding * 2)
            {
                s_doSnap = DoSnap::FW;
                s_monIndex = i;
            }


        }
    }
    break;

    case WM_EXITSIZEMOVE:
    {
        if (s_doSnap == DoSnap::NO)
            break;
        else if (s_doSnap == DoSnap::FW) {
            RECT& monRect = s_monRects[s_monIndex];

            // ウィンドウのRECT(見た目より当たり判定?が広い)
            RECT wndRect = {};
            GetWindowRect(handle, &wndRect);

            // ウィンドウの見たままの大きさのRECT
            RECT looksRect = {};
            DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &looksRect, sizeof(RECT));

            // A.横幅基準でウィンドウサイズを計算
            RECT aRect = {};
            {
                aRect.right = (monRect.right - monRect.left);                            // 見た目の幅が画面の 1/1 ※最大化なので
                aRect.right += (wndRect.right - wndRect.left) - (looksRect.right - looksRect.left); // 見た目の幅をウィンドウの幅に変換
                aRect.right -= (s_wndDiffRect.right - s_wndDiffRect.left);                          // ウィンドウの幅をクライアントの幅に変換
                aRect.bottom = aRect.right * INIT_WINDOW_CLIENT_Y_SIZE / INIT_WINDOW_CLIENT_X_SIZE; // クライアントのアスペクト比を保つように高さを計算
            }

            RECT dstRect = aRect;

            // クライアントサイズをこの関数に設定する
            DxLib::SetWindowSize(dstRect.right, dstRect.bottom);

            // ウィンドウサイズを変更したのでサイズを取得し直す
            GetWindowRect(handle, &wndRect);
            DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &looksRect, sizeof(RECT));

            SetWindowPosition(monRect.left + (wndRect.left - looksRect.left), monRect.top + (wndRect.top - looksRect.top));
            s_doSnap = DoSnap::NO;


        }
        else {
            // モニターのRECT
            RECT& monRect = s_monRects[s_monIndex];

            // ウィンドウのRECT(見た目より当たり判定?が広い)
            RECT wndRect = {};
            GetWindowRect(handle, &wndRect);

            // ウィンドウの見たままの大きさのRECT
            RECT looksRect = {};
            DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &looksRect, sizeof(RECT));

            // A.横幅基準でウィンドウサイズを計算
            RECT aRect = {};
            {
                aRect.right = (monRect.right - monRect.left) / 2;                                   // 見た目の幅が画面の 1/2
                aRect.right += (wndRect.right - wndRect.left) - (looksRect.right - looksRect.left); // 見た目の幅をウィンドウの幅に変換
                aRect.right -= (s_wndDiffRect.right - s_wndDiffRect.left);                          // ウィンドウの幅をクライアントの幅に変換
                aRect.bottom = aRect.right * INIT_WINDOW_CLIENT_Y_SIZE / INIT_WINDOW_CLIENT_X_SIZE; // クライアントのアスペクト比を保つように高さを計算
            }

            /*

            // B.高さ基準でウィンドウサイズを計算
            RECT bRect = {};
            {
                bRect.bottom = (monRect.bottom - monRect.top) / 2;                                   // 見た目の高さが画面の 1/2
                bRect.bottom += (wndRect.bottom - wndRect.top) - (looksRect.bottom - looksRect.top); // 見た目の高さをウィンドウの高さに変換
                bRect.bottom -= (s_wndDiffRect.bottom - s_wndDiffRect.top);                          // ウィンドウの高さをクライアントの高さに変換
                bRect.right = bRect.bottom * INIT_WINDOW_CLIENT_X_SIZE / INIT_WINDOW_CLIENT_Y_SIZE;  // クライアントのアスペクト比を保つように幅を計算
            }

            RECT dstRect = {};

            // AとBの内、小さい方を選択
            if (aRect.right <= bRect.right)
                dstRect = aRect;
            else
                dstRect = bRect;

            */

            // ウィンドウを縦に並べる使い方はしないので
            // Bはコメントアウトし、Aを選択することにした
            RECT dstRect = aRect;

            // クライアントサイズをこの関数に設定する
            DxLib::SetWindowSize(dstRect.right, dstRect.bottom);

            // ウィンドウサイズを変更したのでサイズを取得し直す
            GetWindowRect(handle, &wndRect);
            DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &looksRect, sizeof(RECT));

            switch (s_doSnap)
            {
            case DoSnap::TL:
                // 左上
                SetWindowPosition(monRect.left + (wndRect.left - looksRect.left), monRect.top + (wndRect.top - looksRect.top));
                break;
            case DoSnap::TR:
                // 右上
                SetWindowPosition(monRect.right - (looksRect.right - looksRect.left) + (wndRect.left - looksRect.left), monRect.top + (wndRect.top - looksRect.top));
                break;
            case DoSnap::BL:
                // 左下
                SetWindowPosition(monRect.left + (wndRect.left - looksRect.left), monRect.bottom - (looksRect.bottom - looksRect.top) + (wndRect.top - looksRect.top));
                break;
            case DoSnap::BR:
                // 右下
                SetWindowPosition(monRect.right - (looksRect.right - looksRect.left) + (wndRect.left - looksRect.left), monRect.bottom - (looksRect.bottom - looksRect.top) + (wndRect.top - looksRect.top));
                break;
            }

            s_doSnap = DoSnap::NO;

        }

        
    }
    break;
    }


    return 0;

}

void act_ref(int i, VI& check) {
    int size = (int)check.size();
    shig_rep(j, size) {
        if (j == 0)continue;
        check[j]++;
        if (check[j] > 10000)check[j] = 1;
    }
    //if (i < size)check[i] = 1;
}

void show_game_state() {

    VI state = get_game_state();
    int pad_num = GetJoypadNum();

    unsigned int Color = GetColor(0, 0, 0);
    DrawFormatString(20, 200, Color, "hold_f =  %d", state[0]);
    DrawFormatString(20, 212, Color, "hold =  %d", state[1]);
    DrawFormatString(20, 224, Color, "current =  %d", state[2]);
    DrawFormatString(20, 236, Color, "counter =  %d", state[3]);
    DrawFormatString(20, 248, Color, "event =  %d", state[4]);
    DrawFormatString(20, 260, Color, "hold_kind =  %d", state[5]);
    DrawFormatString(20, 272, Color, "hd_cnt =  %d", state[6]);
    DrawFormatString(20, 284, Color, "l_erasef =  %d", state[7]);
    DrawFormatString(20, 296, Color, "delay_f =  %d", state[8]);
    DrawFormatString(20, 308, Color, "ts_kind =  %d", state[9]);
    DrawFormatString(20, 320, Color, "p_srs =  %d", state[10]);
    DrawFormatString(20, 332, Color, "btb =  %d", state[11]);
    DrawFormatString(20, 344, Color, "combo =  %d", state[12]);
    DrawFormatString(20, 600, Color, "pad_num : %d", pad_num);

    return;
}

void show_ts_state() {
    unsigned int Color = GetColor(0, 0, 0);
    VS state = get_ts_state();

    DrawFormatString(20, 550, Color, "%s", state[0].c_str());
    DrawFormatString(20, 570, Color, "%s", state[1].c_str());

    return;
}

void show_mino_his() {

    deque<string> mh = get_mino_his();
    unsigned int Color = GetColor(0, 0, 0);
    DrawFormatString(20, 375, Color, "%s", "n : r,x,y,id");
    shig_rep(i, mh.size()) {
        DrawFormatString(20, 400 + 15 * i, Color, "%d : %s", i + 1, mh[i]);
    }

    return;
}

void show_field(int flag) {

    ClearDrawScreen();//裏画面消す
    SetDrawScreen(DX_SCREEN_BACK);

    DrawGraph(0, 0, BG_hndl, FALSE);

    if (flag == 0)copy_pfield();

    DxLib::DrawBox(200, 20, 501, 651, GetColor(255, 255, 255), TRUE);
    shig_rep(i, 11) {
        DrawLine(200 + i * 30, 20, 200 + i * 30, 651, GetColor(0, 0, 0));
    }
    shig_rep(i, 22) {
        DrawLine(200, 20 + i * 30, 501, 20 + i * 30, GetColor(0, 0, 0));
    }

    unsigned int color = GetColor(0, 0, 0);
    shig_rep(i, 21) {
        shig_rep(j, 10) {
            int px = get_field_state(20 - i, j, 0);
            if (px == 0)color = GetColor(255, 255, 255);
            else if (px == 1)color = GetColor(0, 191, 255);
            else if (px == 2)color = GetColor(66, 66, 255);
            else if (px == 3)color = GetColor(255, 165, 0);
            else if (px == 4)color = GetColor(255, 255, 0);
            else if (px == 5)color = GetColor(0, 208, 0);
            else if (px == 6)color = GetColor(156, 73, 231);
            else if (px == 7)color = GetColor(255, 80, 49);
            else if (px == 8)color = GetColor(119, 136, 153);
            else if (px == -1)color = GetColor(128, 223, 255);
            else if (px == -2)color = GetColor(104, 104, 255);
            else if (px == -3)color = GetColor(255, 214, 136);
            else if (px == -4)color = GetColor(255, 255, 155);
            else if (px == -5)color = GetColor(140, 208, 140);
            else if (px == -6)color = GetColor(208, 169, 243);
            else if (px == -7)color = GetColor(255, 139, 119);
            else if (px == -8)color = GetColor(0, 128, 128);
            DxLib::DrawBox(201 + (j * 30), 21 + (i * 30), 230 + (j * 30), 50 + (i * 30), color, TRUE);
        }
    }

    if (flag == 0)reset_pfield();

    pair<int, queue<int>> data = get_mino_state();
    int bhold = data.first;
    VI next_que(0);
    queue<int> c_next = data.second;
    while (!c_next.empty()) {
        int q = c_next.front();
        next_que.push_back(q);
        c_next.pop();
    }

    DrawGraph(50, 50, Gr_mino[bhold], FALSE);

    int n_size = min(5, (int)next_que.size());
    shig_rep(i, n_size) {
        int nq = next_que[i] + 8;
        DrawGraph(550, 50 + i * 100, Gr_mino[nq], FALSE);
    }

    show_game_state();
    show_ts_state();
    show_mino_his();


    DxLib::ScreenFlip();

    return;
}

void show_field_AI(int flag, shig::AIshigune &ai) {
    //VVI s_field = ai.get_AI_field(1, 1);

    ClearDrawScreen();//裏画面消す
    SetDrawScreen(DX_SCREEN_BACK);

    DrawGraph(0, 0, BG_hndl, FALSE);

    if (flag == 0)copy_pfield();

    DxLib::DrawBox(200, 20, 501, 651, GetColor(255, 255, 255), TRUE);
    shig_rep(i, 11) {
        DrawLine(200 + i * 30, 20, 200 + i * 30, 651, GetColor(0, 0, 0));
    }
    shig_rep(i, 22) {
        DrawLine(200, 20 + i * 30, 501, 20 + i * 30, GetColor(0, 0, 0));
    }

    unsigned int color = GetColor(0, 0, 0);
    shig_rep(i, 21) {
        shig_rep(j, 10) {
            int px = get_field_state(20 - i, j, 0);
            if (px == 0)color = GetColor(255, 255, 255);
            else if (px == 1)color = GetColor(0, 191, 255);
            else if (px == 2)color = GetColor(66, 66, 255);
            else if (px == 3)color = GetColor(255, 165, 0);
            else if (px == 4)color = GetColor(255, 255, 0);
            else if (px == 5)color = GetColor(0, 208, 0);
            else if (px == 6)color = GetColor(156, 73, 231);
            else if (px == 7)color = GetColor(255, 80, 49);
            else if (px == 8)color = GetColor(119, 136, 153);
            else if (px == -1)color = GetColor(128, 223, 255);
            else if (px == -2)color = GetColor(104, 104, 255);
            else if (px == -3)color = GetColor(255, 214, 136);
            else if (px == -4)color = GetColor(255, 255, 155);
            else if (px == -5)color = GetColor(140, 208, 140);
            else if (px == -6)color = GetColor(208, 169, 243);
            else if (px == -7)color = GetColor(255, 139, 119);
            else if (px == -8)color = GetColor(0, 128, 128);
            DxLib::DrawBox(201 + (j * 30), 21 + (i * 30), 230 + (j * 30), 50 + (i * 30), color, TRUE);
        }
    }
    
    if (flag == 0)reset_pfield();

    pair<int, queue<int>> data = get_mino_state();
    int bhold = data.first;
    VI next_que(0);
    queue<int> c_next = data.second;
    while (!c_next.empty()) {
        int q = c_next.front();
        next_que.push_back(q);
        c_next.pop();
    }

    DrawGraph(50, 50, Gr_mino[bhold], FALSE);

    int n_size = min(5, (int)next_que.size());
    shig_rep(i, n_size) {
        int nq = next_que[i] + 8;
        DrawGraph(525, 50 + i * 100, Gr_mino[nq], FALSE);
    }

    show_game_state();
    show_ts_state();
    show_mino_his();

    unsigned int Color = GetColor(0, 0, 0);
    
    /*
    VI list = ai.get_AI_cmd();
    shig_rep(i, list.size()) {
        DrawFormatString(650, 50 + i * 20, Color, "%d", list[i]);
    }
    */
    

    pair<int, string> sttrp = ai.get_sttrp_name();
    DrawFormatString(20, 620, Color, "%s", "id: name:");
    DrawFormatString(20, 635, Color, "%d  %s", sttrp.first, sttrp.second);


    DxLib::ScreenFlip();

    return;
}

void run_game() {

    if (GetNowCount() - sectime >= refrashRate60) {

        if (delay_cnt > 0) {
            delay_cnt--;
            return;
        }

        GetHitKeyStateAll(KeyBuf);

        DINPUT_JOYSTATE DIS;
        int err = GetJoypadDirectInputState(DX_INPUT_PAD1, &DIS);

        int pad_sts = GetJoypadInputState(DX_INPUT_PAD1);
        //int key_sts = GetJoypadInputState(DX_INPUT_KEY);

        if (DIS.POV[0] == 22500 || DIS.POV[0] == 27000 || DIS.POV[0] == 31500) {
            KeyBuf[KEY_INPUT_LEFT] = 1;
        }
        if (DIS.POV[0] == 4500 || DIS.POV[0] == 9000 || DIS.POV[0] == 13500) {
            KeyBuf[KEY_INPUT_RIGHT] = 1;
        }
        if (DIS.POV[0] == 13500 || DIS.POV[0] == 18000 || DIS.POV[0] == 22500) {
            KeyBuf[KEY_INPUT_DOWN] = 1;
        }
        if (DIS.Buttons[1] == 128) {
            KeyBuf[KEY_INPUT_SPACE] = 1;
        }
        if (DIS.Buttons[5] == 128 || DIS.Buttons[7] == 128) {
            KeyBuf[KEY_INPUT_UP] = 1;
        }
        if (DIS.Buttons[4] == 128 || DIS.Buttons[6] == 128) {
            KeyBuf[KEY_INPUT_Z] = 1;
        }
        if (DIS.Buttons[0] == 128) {
            KeyBuf[KEY_INPUT_C] = 1;
        }
        if (DIS.Buttons[8] == 128) {
            KeyBuf[KEY_INPUT_R] = 1;
        }
        if (DIS.Buttons[12] == 128) {
            KeyBuf[KEY_INPUT_ESCAPE] = 1;
        }

        //int Xbuf = 0, Ybuf = 0;
        //int err = GetJoypadAnalogInput(&Xbuf, &Ybuf, DX_INPUT_PAD1);
        
        if (DIS.Y >= 710) {
            KeyBuf[KEY_INPUT_DOWN] = 1;
        }
        if (DIS.X <= -710) {
            KeyBuf[KEY_INPUT_LEFT] = 1;
        }
        if (DIS.X >= 710) {
            KeyBuf[KEY_INPUT_RIGHT] = 1;
        }

        bool any_put = false;

        if (KeyBuf[KEY_INPUT_R]) {
            show_field(0);
            WaitTimer(500);
            set_field();
            g_check = 0;
        }

        if (KeyBuf[KEY_INPUT_G]) {
            edit_garbage_cmd(1);
            g_check = 0;
        }

        if (KeyBuf[KEY_INPUT_LEFT] && !KeyBuf[KEY_INPUT_RIGHT]) {
            if (act_flag[6] == 0) {
                //act_ref(6, act_flag);
                act_flag[6] = -6;
                g_check = game(6);
            }
            else if (act_flag[6] == -1) {
                act_flag[6] = 1;
            }
            else if (act_flag[6] > 0) {
                g_check = game(6);
            }
            //else act_flag[6] = 0;
            delay_cnt = 2;
            any_put = true;
        }

        if (KeyBuf[KEY_INPUT_RIGHT] && !KeyBuf[KEY_INPUT_LEFT]) {
            if (act_flag[7] == 0) {
                //act_ref(7, act_flag);
                act_flag[7] = -6;
                g_check = game(7);
            }
            else if(act_flag[7] == -1){
                act_flag[7] = 1;
            }
            else if(act_flag[7] > 0){
                g_check = game(7);
            }
            //else act_flag[7] = 0;
            delay_cnt = 2;
            any_put = true;
        }


        if (KeyBuf[KEY_INPUT_UP] && !KeyBuf[KEY_INPUT_Z]) {
            if (act_flag[5] >= 0) {
                //act_ref(5, act_flag);
                act_flag[5] = -2;
                g_check = game(5);
            }
            else act_flag[5]--;
            delay_cnt = 2;
            any_put = true;
        }

        if (KeyBuf[KEY_INPUT_Z] && !KeyBuf[KEY_INPUT_UP]) {
            if (act_flag[4] >= 0) {
                //act_ref(4, act_flag);
                act_flag[4] = -2;
                g_check = game(4);
            }
            else act_flag[4]--;
            delay_cnt = 2;
            any_put = true;
        }

        if (KeyBuf[KEY_INPUT_DOWN]) {
            if (act_flag[2] >= 0) {
                //act_ref(2, act_flag);
                act_flag[2] = 1;
                g_check = game(2);
            }
            else act_flag[2] = 0;
            delay_cnt = 2;
            any_put = true;
        }
        
        if (KeyBuf[KEY_INPUT_SPACE]) {
            if (act_flag[3] >= 0) {
                //act_ref(3, act_flag);
                act_flag[3] = -2;
                g_check = game(3);
                delay_cnt = get_delayF();
            }
            else {
                act_flag[3]--;
            }

            if (g_check == 2) {
                show_field(1);
                int d = 1000 / 60 * get_delayF();
                WaitTimer(d);
                delay_cnt = 0;
                g_check = 0;
            }

            any_put = true;

        }
        
        //else {
        //    act_ref(0, act_flag);
        //}

        if (KeyBuf[KEY_INPUT_C]) {
            if (act_flag[1] >= 0) {
                //act_ref(1, act_flag);
                act_flag[1] = -2;
                g_check = game(1);
            }
            else {
                act_flag[1]--;
            }
            delay_cnt = 2;
            any_put = true;
        }

        act_ref(0, act_flag);

        if (!KeyBuf[KEY_INPUT_RIGHT] && !KeyBuf[KEY_INPUT_LEFT]) {
            act_flag[6] = 0;
            act_flag[7] = 0;
        }

        if (KeyBuf[KEY_INPUT_RIGHT] && KeyBuf[KEY_INPUT_LEFT]) {
            act_flag[6] = 1;
            act_flag[7] = 1;
        }

        if (g_check == 1) {
            show_field(0);
            WaitTimer(1200);
            set_field();
            g_check = 0;
        }

        

        sectime = GetNowCount();

    }

    show_field(0);
    DxLib::WaitVSync(1);

    return;
}

void run_AI(shig::AIshigune& ai) {


    if (GetNowCount() - sectime >= get_RFR(60)) {

        if (push_cnt == 0) {
            push_cnt = 1;
        }
        else if (push_cnt == 1) {
            push_cnt = 0;
            sectime = GetNowCount();
            return;
        }

        /*
        
        if (CheckHitKey(KEY_INPUT_RIGHT)) {
            //.move_itr(1);
        }
        else if (CheckHitKey(KEY_INPUT_LEFT)) {
            //ai.move_itr(-1);
        }
        
        */

        GetHitKeyStateAll(KeyBuf);
        
        if (KeyBuf[KEY_INPUT_R]) {
            show_field(0);
            WaitTimer(500);
            set_field();
            g_check = 0;
        }

        if (KeyBuf[KEY_INPUT_G]) {
            edit_garbage_cmd(1);
            g_check = 0;
        }

        VI command = ai.shigune_main();
        queue<int> c_list;
        shig_rep(i, command.size())c_list.push(command[i]);
        run_cmd(c_list, ai);

        sectime = GetNowCount();
    }

    //DxLib::WaitVSync(1);

    return;
}

void run_cmd(queue<int>& s_list, shig::AIshigune& aii) {

    sectime = GetNowCount();

    while (s_list.size() > 0) {

        if (GetNowCount() - sectime >= get_RFR(60)) {

            if (delay_cnt > 0) {
                delay_cnt--;
                //continue;
            }

            int d_action = s_list.front();

            if (d_action == 6) {
                if (act_flag[6] >= 0) {
                    //act_ref(6, act_flag);
                    //g_check = game(6);
                }
                g_check = game(6);
                //else act_flag[6] = 0;
                delay_cnt = 2;
            }
            else if (d_action == 5) {
                if (act_flag[5] == 0) {
                    //act_ref(5, act_flag);
                    //g_check = game(5);
                }
                g_check = game(5);
                delay_cnt = 1;
            }
            else if (d_action == 7) {
                if (act_flag[7] >= 0) {
                    //act_ref(7, act_flag);
                    //g_check = game(7);
                }
                g_check = game(7);
                //else act_flag[7] = 0;
                delay_cnt = 2;
            }
            else if (d_action == 2) {
                if (act_flag[2] >= 0) {
                    //act_ref(2, act_flag);
                    //g_check = game(2);
                }
                g_check = game(2);
                //else act_flag[2] = 0;
                delay_cnt = 2;

            }
            else if (d_action == 1) {
                if (act_flag[1] == 0) {
                    //act_ref(1, act_flag);
                }
                g_check = game(1);
                delay_cnt = 2;
            }
            else if (d_action == 4) {
                if (act_flag[4] == 0) {
                    //act_ref(4, act_flag);
                    //g_check = game(4);
                }
                g_check = game(4);
                delay_cnt = 1;
            }
            else if (d_action == 3) {
                if (act_flag[3] == 0) {
                    //act_ref(3, act_flag);
                    
                }

                g_check = game(3);
                delay_cnt = get_delayF();

                if (g_check == 2) {
                    show_field_AI(1, aii);
                    int d = refrashRate60 / 3 * get_delayF();
                    WaitTimer(d);
                    delay_cnt = 0;
                    g_check = 0;
                }

            }
            else {
                //act_ref(0, act_flag);
            }

            if (g_check == 1) {
                int d_seg = 300;
                show_field_AI(0, aii);
                WaitTimer(d_seg);
                set_field();
                g_check = 0;
            }

            sectime = GetNowCount();
            s_list.pop();
            show_field_AI(0, aii);
        }
        
        
    }

    return;
}

bool get_key_buf() {
    DINPUT_JOYSTATE DIS;
    int err = GetJoypadDirectInputState(DX_INPUT_PAD1, &DIS);
    GetHitKeyStateAll(KeyBuf);

    int pad_sts = GetJoypadInputState(DX_INPUT_PAD1);
    //int key_sts = GetJoypadInputState(DX_INPUT_KEY);

    if (DIS.POV[0] == 22500 || DIS.POV[0] == 27000 || DIS.POV[0] == 31500) {
        KeyBuf[KEY_INPUT_LEFT] = 1;
    }
    if (DIS.POV[0] == 4500 || DIS.POV[0] == 9000 || DIS.POV[0] == 13500) {
        KeyBuf[KEY_INPUT_RIGHT] = 1;
    }
    if (DIS.POV[0] == 13500 || DIS.POV[0] == 18000 || DIS.POV[0] == 22500) {
        KeyBuf[KEY_INPUT_DOWN] = 1;
    }
    if (DIS.Buttons[1] == 128) {
        KeyBuf[KEY_INPUT_SPACE] = 1;
    }
    if (DIS.Buttons[5] == 128 || DIS.Buttons[7] == 128) {
        KeyBuf[KEY_INPUT_UP] = 1;
    }
    if (DIS.Buttons[4] == 128 || DIS.Buttons[6] == 128) {
        KeyBuf[KEY_INPUT_Z] = 1;
    }
    if (DIS.Buttons[0] == 128) {
        KeyBuf[KEY_INPUT_C] = 1;
    }
    if (DIS.Buttons[8] == 128) {
        KeyBuf[KEY_INPUT_R] = 1;
    }
    if (DIS.Buttons[12] == 128) {
        KeyBuf[KEY_INPUT_ESCAPE] = 1;
    }

    return true;
}

int get_RFR(int fps) {
    return 1000 / fps;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    /*/
    SetMainWindowText(_T("tetris_emulator_ver1.2"));
    ChangeWindowMode(TRUE);
    SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_MAXIMUM);
    SetFullScreenScalingMode(DX_FSSCALINGMODE_BILINEAR);
    SetGraphMode(1200, 800, 32);
    SetWindowSizeChangeEnableFlag(TRUE);
    SetAlwaysRunFlag(TRUE);
    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1) return -1;// エラーが起きたら直ちに終了
    //*/

    SetGraphMode(INIT_WINDOW_CLIENT_X_SIZE, INIT_WINDOW_CLIENT_Y_SIZE, 32);
    ChangeWindowMode(TRUE);
    SetWindowSizeChangeEnableFlag(TRUE);
    SetAlwaysRunFlag(TRUE);
    SetMainWindowText(_T("tetris_emulator ver 3.7.5"));
   
    // ameyasu's suggestion
    SetHookWinProc(WndProcHook);

    //SetUseASyncChangeWindowModeFunction(TRUE, nullptr, nullptr);
    // 最大化ボタンやALT+ENTERキーによる非同期なウインドウモードの変更の機能の設定を行う( Flag:非同期ウインドウモード変更を有効にするかどうかのフラグ( TRUE:有効にする  FALSE:無効にする( デフォルト ) )   CallBackFunction:非同期ウインドウモード変更が行われた場合に呼ばれるコールバック関数のアドレス   Data:CallBackFunction の引数に渡される void 型のポインタ )


    if (DXLibError(DxLib_Init())) return dxLibError;

    //SetUseMenuFlag(TRUE);



    SetDrawScreen(DX_SCREEN_BACK);
    DxLib::SetMouseDispFlag(TRUE);
    DxLib::SetBackgroundColor(255, 255, 255);


    Gr_mino[0] = LoadGraph(_T("tex\\mino\\flame_L.bmp"));
    Gr_mino[1] = LoadGraph(_T("tex\\mino\\I-mino_100.bmp"));
    Gr_mino[2] = LoadGraph(_T("tex\\mino\\J-mino_100.bmp"));
    Gr_mino[3] = LoadGraph(_T("tex\\mino\\L-mino_100.bmp"));
    Gr_mino[4] = LoadGraph(_T("tex\\mino\\O-mino_100.bmp"));
    Gr_mino[5] = LoadGraph(_T("tex\\mino\\S-mino_100.bmp"));
    Gr_mino[6] = LoadGraph(_T("tex\\mino\\T-mino_100.bmp"));
    Gr_mino[7] = LoadGraph(_T("tex\\mino\\Z-mino_100.bmp"));
    Gr_mino[8] = LoadGraph(_T("tex\\mino\\flame_S.bmp"));
    Gr_mino[9] = LoadGraph(_T("tex\\mino\\I-mino_90.bmp"));
    Gr_mino[10] = LoadGraph(_T("tex\\mino\\J-mino_90.bmp"));
    Gr_mino[11] = LoadGraph(_T("tex\\mino\\L-mino_90.bmp"));
    Gr_mino[12] = LoadGraph(_T("tex\\mino\\O-mino_90.bmp"));
    Gr_mino[13] = LoadGraph(_T("tex\\mino\\S-mino_90.bmp"));
    Gr_mino[14] = LoadGraph(_T("tex\\mino\\T-mino_90.bmp"));
    Gr_mino[15] = LoadGraph(_T("tex\\mino\\Z-mino_90.bmp"));
    BG_hndl = LoadGraph(_T("tex\\background\\tetris_emulator_background02.bmp"));

    f_handle[0] = CreateFontToHandle(NULL, 10, 2, DX_FONTTYPE_NORMAL);

    set_field();

    shig::AIshigune ai(1);
    ai.load_ttrp();

    int m_push = 0;

    while (ProcessMessage() == 0) {
        // メッセージループに代わる処理をする
        //if (ProcessMessage() == -1)break;

        get_key_buf();

        if (KeyBuf[KEY_INPUT_ESCAPE]) {
            break;
            g_event = -1;
        }

        if (CheckHitKey(KEY_INPUT_M)) {
            if (m_push == 1) {
                continue;
            }
            else m_push = 1;

            const int mod = 2;
            g_event++;
            g_event %= mod;
            if (g_event == 1) {
                /*
                ai.get_field();
                ai.get_state();
                ai.search_way();
                */
            }
        }
        else {
            m_push = 0;
        }

        if (g_event == 0)run_game();
        else if (g_event == 1) {
            run_AI(ai);
        }

    }

    // ＤＸライブラリ使用の終了処理

    shig_rep(i, 16) DxLib::DeleteGraph(Gr_mino[i]);
    shig_rep(i, 1) DeleteFontToHandle(f_handle[i]);
    DxLib::DeleteGraph(BG_hndl);
    DxLib::DxLib_End();

    return 0;

}