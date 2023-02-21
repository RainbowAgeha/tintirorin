//���C���֐�

#include "GameMain.h"
int FrameStartTime;        // �U�O�e�o�r�Œ�p�A���ԕۑ��p�ϐ�

#include "time.h"

/**
* �`���`�������@���[��
* ��������3�g�p�B�������
* ����|���������߁A���ɂ���ă��^�[�����A��B
* �s���]���@�T�{�|�� 1/216
* �Q�`�U�]���@�R�{�|���@5/216
* �S�T�U�@�Q�{ 6/216
* �P�Q�R�@�����Q�{ 6/216
* �Q�Q�R�@�R�̖ڂȂ� 1��15/216 6���킹��90/216
* �ڂȂ��@108/216
* 3��Ƃ��ڂ��o�Ȃ��m��1/864
*/

//��������̖ڕϐ�
int DiceNum[10];	//�g�����킩���10��

//�����\���萔
#define DRAWSTRING_X 100
#define DRAWSTRING_Y 100

//�����F�̒萔
int Cr_White = GetColor(255, 255, 255);
int Cr_Red = GetColor(255, 0, 0);
int Cr_Blue = GetColor(0, 0, 255);
int Cr_Green = GetColor(0, 255, 0);


//�X�R�A�p�ϐ�
int MoneyNum = 0;
//�|����
int MoneyLatch = 0;

//�����ʔ���
int JudgeNum = 0;
//�o�ڂ̕ϐ�
int Deme = 0;
//�ږ����̂�蒼���J�E���g3
int NoDemeCount = 3;

//�Q�[�����[�h�萔
enum GAMEMODE {
	gamemode_init = 0,
	gamemode_diceroll,
	gamemode_jugde,
	gamemode_result,
	gamemode_end
};

//�Q�[�����[�h�؂�ւ�
int GameMode = gamemode_init;

//���͏�� Input:�������ςȂ�
int Input;
//���͏�� EdgeInput:1��̂�
int EdgeInput;

//GitHub�v�C��
//Git�C��


//WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//���s���O���o�͂��Ȃ�
	SetOutApplicationLogValidFlag(false);

	//�E�B���h�E�̐ݒ�
	//�E�B���h�E���[�h�̐ݒ� false:�S��ʐݒ�
	ChangeWindowMode(true);

	/*
	// �E�C���h�E���[�h�ŋN�����邩�m�F����
	if (MessageBox(NULL, "�E�C���h�E���[�h�ŋN�����܂����H", "��ʃ��[�h�m�F", MB_YESNO) == IDYES)
	{
	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);
	}
	*/

	//�E�B���h�E�̃��T�C�Y
	//Check:���s���ɉ�ʂ̑傫�����ύX�\��
	SetWindowSizeChangeEnableFlag(true);
	//�E�B���h�E�^�C�g��
	SetMainWindowText("�Q�[���e�X�g");

	//�w�i�F�̐ݒ�
	SetBackgroundColor(0, 0, 0);
	//DX���C�u��������������
	//check:-1(��O����)�������ꍇ�́A�Z�b�g�A�b�v�Ɏ��s����
	//�p�\�R���{�̂̋@��̊m�F�����Ă��炤(�����f�o�C�X���Ȃ����B)
	if (DxLib_Init() == -1)return -1;

	//�`������Ԍ��ɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//-�����ݒ�------------------------------------------

	//�������z��ݒ肵�Ă���
	MoneyNum = 10000;

	//��������̏�����
	for (int i = 0; i < 10; i++) {
		DiceNum[i] = 0;
	}

	//�����V�[�h�̐ݒ�
	SRand(time(NULL));

	//�|�����͍Œ�100����
	MoneyLatch = 100;

	//-�����ݒ�------------------------------------------

	//���C�����[�v
	while (1) {

		// ��ʂɕ`����Ă�����̂����S������
		ClearDrawScreen();

		// �P/�U�O�b���܂ő҂�
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}
		// ���݂̃J�E���g�l��ۑ�
		FrameStartTime = GetNowCount();
		// ���͏�Ԃ��X�V
		{
			int i;
			// �p�b�h�P�ƃL�[�{�[�h������͂𓾂�
			i = GetJoypadInputState(DX_INPUT_KEY_PAD1);
			// �G�b�W����������͂��Z�b�g
			EdgeInput = i & ~Input;
			// ���͏�Ԃ̕ۑ�
			Input = i;
		}


		//-�X�V����------------------------------------------
		DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y -50, Cr_White, "���z�F%d" , MoneyNum);
		DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y - 30, Cr_White, "�|�����z�F%d", MoneyLatch);

		switch (GameMode) {
		case gamemode_init:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "�{�^���P�ł��������U��");
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 50, Cr_White, "�㉺�L�[�Ŋ|�����̕ύX");

			//�{�^��4�������Ă����Ԃ͐V�䃂�[�h
			if (Input & PAD_INPUT_4) {
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y+200, Cr_Red, "�V�䃂�[�h�I�I�I");
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

			//�|�����ȏ�͊|�����Ȃ��B
			if (MoneyNum - MoneyLatch >= 0 && EdgeInput & PAD_INPUT_1) {
				GameMode = gamemode_diceroll;
				MoneyNum -= MoneyLatch;
			}

			break;
		case gamemode_diceroll:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "���I���c");
			DiceNum[0] = GetRand(5) + 1;
			DiceNum[1] = GetRand(5) + 1;
			DiceNum[2] = GetRand(5) + 1;

			//�{�^��5�������Ă����456�΂��g�p����
			if (Input & PAD_INPUT_5) {
				DiceNum[0] = GetRand(2) + 4;
				DiceNum[1] = GetRand(2) + 4;
				DiceNum[2] = GetRand(2) + 4;
			}
			//�{�^��6�������Ă���ƃs���]���΂��g�p����
			if (Input & PAD_INPUT_6) {
				DiceNum[0] = 1;
				DiceNum[1] = 1;
				DiceNum[2] = 1;
				//�ʂ邩����Ȃ���I�I�I
			}

			//��������ڂ̕\��
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 50, Cr_White, "��������̖�:%d , %d , %d", DiceNum[0], DiceNum[1], DiceNum[2]);
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White, "�{�^���P�ł��������U��");
			if (EdgeInput & PAD_INPUT_1) {
				GameMode = gamemode_jugde;
			}

			break;
		case gamemode_jugde:

			//�����l�͖��Ȃ�
			JudgeNum = 0;

			//���̔���������Ȃ�
			//�R�Ƃ�������
			if (DiceNum[0] == DiceNum[1] && DiceNum[1] == DiceNum[2]) {
				//�ڂ����ׂĂP�̎�
				if (DiceNum[0] == 1 && DiceNum[1] == 1 && DiceNum[2] == 1) {
					JudgeNum = 1;	//�s���]��

					MoneyNum += MoneyLatch * 5; //�|�����T�{
					
				}
				else {
					JudgeNum = 2;	//2 �` 6�̃]����
					MoneyNum += MoneyLatch * 3; //�|�����R�{
					
				}
			}
			//�ڂ𔻒肷��@������2�ƈقȂ��1�ŁA�قȂ�ڂ��o�ڂƂȂ�B
			//�����ڂ��o�������肷��
			else if (DiceNum[0] == DiceNum[1] || DiceNum[0] == DiceNum[2] ||
				DiceNum[1] == DiceNum[2]) {
				Deme = 0;
				//�قȂ�ڂ�T��
				if (DiceNum[0] == DiceNum[1]) {
					Deme = DiceNum[2];
				}
				if (DiceNum[0] == DiceNum[2]) {
					Deme = DiceNum[1];
				}
				if (DiceNum[1] == DiceNum[2]) {
					Deme = DiceNum[0];
				}

				//�����o��������m�点��
				JudgeNum = 5;
				MoneyNum += MoneyLatch * 1; //�|����1�{
			}
		
			//456�̔���
			//����������15�̎����d������
			if (DiceNum[0] + DiceNum[1] + DiceNum[2] == 15) {
				if (!(DiceNum[0] == DiceNum[1] || DiceNum[0] == DiceNum[2] ||
					DiceNum[1] == DiceNum[2])) {
					JudgeNum = 3;	//456
					MoneyNum += MoneyLatch * 3; //�|�����R�{
				}
			}

			//123�̔���
			//����������6�̎����d������
			if (DiceNum[0] + DiceNum[1] + DiceNum[2] == 6) {
				if (!(DiceNum[0] == DiceNum[1] || DiceNum[0] == DiceNum[2] ||
					DiceNum[1] == DiceNum[2])) {
					JudgeNum = 4;	//123
					MoneyNum -= MoneyLatch * 2; //�|����-2�{
				}
			}

			//���肪�I������̂Ō��ʕ\����
			GameMode = gamemode_result;


			break;
		case gamemode_result:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "���ʕ\��");
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 50, Cr_White, "��������̖�:%d , %d , %d", DiceNum[0], DiceNum[1], DiceNum[2]);

			//���̔��\
			switch (JudgeNum) {
			case 0:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"���Ȃ�...");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,"����");
				break;
			case 1:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"�s���]���I�I�I�I");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"�T�{�t��:%d pt", MoneyLatch * 5);
				break;
			case 2:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"�]���ځI�I�I");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"�R�{�t��:%d pt", MoneyLatch * 3);
				break;
			case 3:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"�S�T�U�I�I�I");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"�R�{�t��:%d pt", MoneyLatch * 3);
				break;
			case 4:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"�P�Q�R...");
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"�Q�{�����c�I:- %d pt", MoneyLatch * 2);
				break;
			case 5:
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 100, Cr_White,
					"%d�̖ځI",Deme);
				DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 150, Cr_White,
					"1�{�t��:%d pt", MoneyLatch);
				break;
			}

			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y + 200, Cr_White, "�{�^���P�ł��������U��");
			if (EdgeInput & PAD_INPUT_1) {

				//�莝����0�̏ꍇ�Q�[���I�[�o�[�֒��s
				if (MoneyNum <= 0) {
					GameMode = gamemode_end;
				}
				else {
					GameMode = gamemode_init;
					MoneyLatch = 100;	//�|������
				}
			}
			break;

		case gamemode_end:
			DrawFormatString(DRAWSTRING_X, DRAWSTRING_Y, Cr_White, "�Q�[���I�[�o�[");
			break;
		}


		//-�X�V����------------------------------------------

		//��ʂ̕\��������Ԏ�O���甽�f����
		ScreenFlip();

		//windows�V�X�e�����痈�������������(��O������)
		if (ProcessMessage() == -1)break;

		//ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	//Dxlib�̊J��
	DxLib_End();

	//�Q�[�����I������
	return 0;
}