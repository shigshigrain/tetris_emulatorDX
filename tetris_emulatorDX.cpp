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

// DXLib はエラーが発生した場合 -1 を返します
const int dxLibError = -1;
inline bool DXLibError(int result) {
    return result == dxLibError ? true : false;
}

// ちょっとだけ使います
struct Size {
    int width, height;
};

class Window {
    HWND windowHandle = NULL;
    MSG message = {};
public:

    // ウィンドウプロシージャ
    static LRESULT CALLBACK Procedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            // 一応終了処理だけ
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(handle, message, wParam, lParam);
    }

    // ウィンドウハンドルを取得する
    HWND GetHandle() {
        return windowHandle;
    }

    // ウィンドウを生成する
    void Create(string title, string className) {
        // クラス情報を登録する
        WNDCLASS windowClass = {};
        windowClass.lpfnWndProc = Procedure;
        windowClass.hInstance = GetModuleHandle(NULL);
        windowClass.lpszClassName = className.c_str();
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClass(&windowClass);
        // ウィンドウを生成する
        windowHandle = CreateWindowEx(0, className.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, 50, 50, 1536, 800, NULL, NULL, GetModuleHandle(NULL), NULL);

    }

    // ウィンドウのサイズを取得する
    Size GetSize() {
        Size result;
        GetWindowSize(&result.width, &result.height);
        return result;
    }

    // メッセージ処理
    bool Loop() {
        while (true) {
            if (message.message == WM_QUIT) return false;
            if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            else return true;
        }
    }

    // ウィンドウを表示する
    void Show() {
        ShowWindow(windowHandle, SW_SHOW);
    }
};

void act_ref(int i, VI& check) {
    int size = check.size();
    shig_rep(j, size) {
        if (j == 0)continue;
        check[j]++;
        if (check[j] > 10000)check[j] = 1;
    }
    //if (i < size)check[i] = 1;
}

void show_game_state() {

    VI state = get_game_state();

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

void show_field_AI(int flag, shig::shigune_AI &ai) {
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

        bool any_put = false;

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

void run_AI(shig::shigune_AI& ai) {


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

void run_cmd(queue<int>& s_list, shig::shigune_AI& aii) {

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

    Window window;
    window.Create("tetris_emulator_ver3.7.0", "DXlib");
    SetUserWindow(window.GetHandle());
    SetUserWindowMessageProcessDXLibFlag(false);
    SetGraphMode(1920, 1000, 32); // FHD時、↑30px,↓50pxがリボン
    if (DXLibError(DxLib_Init())) return dxLibError;
    window.Show();

    Gr_mino[0] = LoadGraph(_T("tex\\flame_L.bmp"));
    Gr_mino[1] = LoadGraph(_T("tex\\I-mino_100.bmp"));
    Gr_mino[2] = LoadGraph(_T("tex\\J-mino_100.bmp"));
    Gr_mino[3] = LoadGraph(_T("tex\\L-mino_100.bmp"));
    Gr_mino[4] = LoadGraph(_T("tex\\O-mino_100.bmp"));
    Gr_mino[5] = LoadGraph(_T("tex\\S-mino_100.bmp"));
    Gr_mino[6] = LoadGraph(_T("tex\\T-mino_100.bmp"));
    Gr_mino[7] = LoadGraph(_T("tex\\Z-mino_100.bmp"));
    Gr_mino[8] = LoadGraph(_T("tex\\flame_S.bmp"));
    Gr_mino[9] = LoadGraph(_T("tex\\I-mino_90.bmp"));
    Gr_mino[10] = LoadGraph(_T("tex\\J-mino_90.bmp"));
    Gr_mino[11] = LoadGraph(_T("tex\\L-mino_90.bmp"));
    Gr_mino[12] = LoadGraph(_T("tex\\O-mino_90.bmp"));
    Gr_mino[13] = LoadGraph(_T("tex\\S-mino_90.bmp"));
    Gr_mino[14] = LoadGraph(_T("tex\\T-mino_90.bmp"));
    Gr_mino[15] = LoadGraph(_T("tex\\Z-mino_90.bmp"));
    BG_hndl = LoadGraph(_T("tex\\tetris_emulator_background02.bmp"));


    DxLib::SetMouseDispFlag(TRUE);
    DxLib::SetBackgroundColor(0, 0, 0);

    f_handle[0] = CreateFontToHandle(NULL, 10, 2, DX_FONTTYPE_NORMAL);

    set_field();

    shig::shigune_AI ai(1);
    ai.load_ttrp();

    int m_push = 0;

    while (window.Loop()) {
        // メッセージループに代わる処理をする
        //if (ProcessMessage() == -1)break;

        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
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





