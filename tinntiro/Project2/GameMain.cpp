//メイン関数

#include "GameMain.h"
int FrameStartTime;        // ６０ＦＰＳ固定用、時間保存用変数

#include "time.h"

/**
* チンチロリン　ルール
* さいころ3つ使用。役を作る
* 張る掛け金を決め、役によってリターンが帰る。
* ピンゾロ　５倍掛け 1/216
* ２～６ゾロ　３倍掛け　5/216
* ４５６　２倍 6/216
* １２３　負け２倍 6/216
* ２２３　３の目など 1目15/216 6つ合わせて90/216
* 目なし　108/216
* 3回とも目が出ない確率1/864
*/

//さいころの目変数
int DiceNum[10];	//使うかわからん10個

//文字表示定数
#define DRAWSTRING_X 100
#define DRAWSTRING_Y 100

//文字色の定数
int Cr_White = GetColor(255, 255, 255);
int Cr_Red = GetColor(255, 0, 0);
int Cr_Blue = GetColor(0, 0, 255);
int Cr_Green = GetColor(0, 255, 0);


//スコア用変数
int MoneyNum = 0;
//掛け金
int MoneyLatch = 0;

//役結果判定
int JudgeNum = 0;
//出目の変数
int Deme = 0;
//目無しのやり直しカウント3
int NoDemeCount = 3;

//ゲームモード定数
enum GAMEMODE {
	gamemode_init = 0,
	gamemode_diceroll,
	gamemode_jugde,
	gamemode_result,
	gamemode_end
};

//ゲームモード切り替え
int GameMode = gamemode_init;

//入力状態 Input:押しっぱなし
int Input;
//入力状態 EdgeInput:1回のみ
int EdgeInput;

//GitHub要修正
//Git修正


//WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//実行ログを出力しない
	SetOutApplicationLogValidFlag(false);

	//ウィンドウの設定
	//ウィンドウモードの設定 false:全画面設定
	ChangeWindowMode(true);

	/*
	// ウインドウモードで起動するか確認する
	if (MessageBox(NULL, "ウインドウモードで起動しますか？", "画面モード確認", MB_YESNO) == IDYES)
	{
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);
	}
	*/

	//ウィンドウのリサイズ
	//Check:実行中に画面の大きさが変更可能か
	SetWindowSizeChangeEnableFlag(true);
	//ウィンドウタイトル
	SetMainWindowText("ゲームテスト");

	//背景色の設定
	SetBackgroundColor(0, 0, 0);
	//DXライブラリ初期化処理
	//check:-1(例外処理)が来た場合は、セットアップに失敗する
	//パソコン本体の機器の確認をしてもらう(音声デバイスがない等。)
	if (DxLib_Init() == -1)return -1;

	//描画先を一番後ろにする
	SetDrawScreen(DX_SCREEN_BACK);

	//-初期設定------------------------------------------

	//初期金額を設定しておく
	MoneyNum = 10000;

	//さいころの初期化
	for (int i = 0; i < 10; i++) {
		DiceNum[i] = 0;
	}

	//乱数シードの設定
	SRand(time(NULL));

	//掛け金は最低100から
	MoneyLatch = 100;

	//-初期設定------------------------------------------

	//メインループ
	while (1) {

		// 画面に描かれているものを一回全部消す
		ClearDrawScreen();

		// １/６０秒立つまで待つ
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}
		// 現在のカウント値を保存
		FrameStartTime = GetNowCount();
		// 入力状態を更新
		{
			int i;
			// パッド１とキーボードから入力を得る
			i = GetJoypadInputState(DX_INPUT_KEY_PAD1);
			// エッジを取った入力をセット
			EdgeInput = i & ~Input;
			// 入力状態の保存
			Input = i;
		}


		//-更新処理------------------------------------------
		DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y -50, Cr_White, "金額：%d" , MoneyNum);
		DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y - 30, Cr_White, "掛け金額：%d", MoneyLatch);

		switch (GameMode) {
		case gamemode_init:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "ボタン１でさいころを振る");
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 50, Cr_White, "上下キーで掛け金の変更");

			//ボタン4を押している状態は青天井モード
			if (Input & PAD_INPUT_4) {
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y+200, Cr_Red, "青天井モード！！！");
				if (EdgeInput & PAD_INPUT_UP) {
					MoneyLatch += 100;
					if (MoneyLatch >= 1000000)MoneyLatch = 1000000;
				}
			}
			else {
				if (EdgeInput & PAD_INPUT_UP) {
					MoneyLatch += 100;
					if (MoneyLatch >= 1000)MoneyLatch = 1000;
				}
			}
		
			if (EdgeInput & PAD_INPUT_DOWN) {
				MoneyLatch -= 100;
				if (MoneyLatch <= 100)MoneyLatch = 100;
			}

			//掛け金以上は掛けられない。
			if (MoneyNum - MoneyLatch >= 0 && EdgeInput & PAD_INPUT_1) {
				GameMode = gamemode_diceroll;
				MoneyNum -= MoneyLatch;
			}

			break;
		case gamemode_diceroll:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "抽選中…");
			DiceNum[0] = GetRand(5) + 1;
			DiceNum[1] = GetRand(5) + 1;
			DiceNum[2] = GetRand(5) + 1;

			//ボタン5を押していると456賽を使用する
			if (Input & PAD_INPUT_5) {
				DiceNum[0] = GetRand(2) + 4;
				DiceNum[1] = GetRand(2) + 4;
				DiceNum[2] = GetRand(2) + 4;
			}
			//ボタン6を押しているとピンゾロ賽を使用する
			if (Input & PAD_INPUT_6) {
				DiceNum[0] = 1;
				DiceNum[1] = 1;
				DiceNum[2] = 1;
				//通るかこんなもん！！！
			}

			//さいころ目の表示
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 50, Cr_White, "さいころの目:%d , %d , %d", DiceNum[0], DiceNum[1], DiceNum[2]);
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White, "ボタン１でさいころを振る");
			if (EdgeInput & PAD_INPUT_1) {
				GameMode = gamemode_jugde;
			}

			break;
		case gamemode_jugde:

			//初期値は役なし
			JudgeNum = 0;

			//役の判定をおこなう
			//３つとも同じ目
			if (DiceNum[0] == DiceNum[1] && DiceNum[1] == DiceNum[2]) {
				//目がすべて１の時
				if (DiceNum[0] == 1 && DiceNum[1] == 1 && DiceNum[2] == 1) {
					JudgeNum = 1;	//ピンゾロ

					MoneyNum += MoneyLatch * 5; //掛け金５倍
					
				}
				else {
					JudgeNum = 2;	//2 ～ 6のゾロ目
					MoneyNum += MoneyLatch * 3; //掛け金３倍
					
				}
			}
			//目を判定する　同じ目2つと異なる目1つで、異なる目が出目となる。
			//同じ目が出たか判定する
			else if (DiceNum[0] == DiceNum[1] || DiceNum[0] == DiceNum[2] ||
				DiceNum[1] == DiceNum[2]) {
				Deme = 0;
				//異なる目を探す
				if (DiceNum[0] == DiceNum[1]) {
					Deme = DiceNum[2];
				}
				if (DiceNum[0] == DiceNum[2]) {
					Deme = DiceNum[1];
				}
				if (DiceNum[1] == DiceNum[2]) {
					Deme = DiceNum[0];
				}

				//役が出来た事を知らせる
				JudgeNum = 5;
				MoneyNum += MoneyLatch * 1; //掛け金1倍
			}
		
			//456の判定
			//足した数が15の時かつ重複無し
			if (DiceNum[0] + DiceNum[1] + DiceNum[2] == 15) {
				if (!(DiceNum[0] == DiceNum[1] || DiceNum[0] == DiceNum[2] ||
					DiceNum[1] == DiceNum[2])) {
					JudgeNum = 3;	//456
					MoneyNum += MoneyLatch * 3; //掛け金３倍
				}
			}

			//123の判定
			//足した数が6の時かつ重複無し
			if (DiceNum[0] + DiceNum[1] + DiceNum[2] == 6) {
				if (!(DiceNum[0] == DiceNum[1] || DiceNum[0] == DiceNum[2] ||
					DiceNum[1] == DiceNum[2])) {
					JudgeNum = 4;	//123
					MoneyNum -= MoneyLatch * 2; //掛け金-2倍
				}
			}

			//判定が終わったので結果表示へ
			GameMode = gamemode_result;


			break;
		case gamemode_result:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "結果表示");
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 50, Cr_White, "さいころの目:%d , %d , %d", DiceNum[0], DiceNum[1], DiceNum[2]);

			//役の発表
			switch (JudgeNum) {
			case 0:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"役なし...");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,"負け");
				break;
			case 1:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"ピンゾロ！！！！");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"５倍付け:%d pt", MoneyLatch * 5);
				break;
			case 2:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"ゾロ目！！！");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"３倍付け:%d pt", MoneyLatch * 3);
				break;
			case 3:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"４５６！！！");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"３倍付け:%d pt", MoneyLatch * 3);
				break;
			case 4:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"１２３...");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"２倍負け…！:- %d pt", MoneyLatch * 2);
				break;
			case 5:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"%dの目！",Deme);
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"1倍付け:%d pt", MoneyLatch);
				break;
			}

			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 200, Cr_White, "ボタン１でさいころを振る");
			if (EdgeInput & PAD_INPUT_1) {

				//手持ちが0の場合ゲームオーバーへ直行
				if (MoneyNum <= 0) {
					GameMode = gamemode_end;
				}
				else {
					GameMode = gamemode_init;
					MoneyLatch = 100;	//掛け金を
				}
			}
			break;

		case gamemode_end:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "ゲームオーバー");
			break;
		}


		//-更新処理------------------------------------------

		//画面の表示順を一番手前から反映する
		ScreenFlip();

		//windowsシステムから来る情報を処理する(例外処理等)
		if (ProcessMessage() == -1)break;

		//ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	//Dxlibの開放
	DxLib_End();

	//ゲームを終了する
	return 0;
}