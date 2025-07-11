#include"tool.h"


int userInput = 0;
int frameCount = 0;
int size = 3;
int maintitlereturn = -1, titleselect = 1;
int FPS = 0;
char text[99];
char p1movement = 0;
char p2movement = 0;

int pagetransfer = 1;
char pageinput = 0;
int pageenter = 0;

int cursorx = 154;
int cursory = 30;

int tutorialback = 0;

int Wp1 = 1, Wp2 = 1;

int i, j, x, y;
int Mbullet = 6, bullet = 0, bulletActive[30] = { 0, 0, };
int isSkip;

struct BossAttack {
	int x, y;
	int isActive;
	int type;
	int special;
};
struct Boss {
	float hp;
	float maxhp;
	int x, y;
	int attack;
	int timer;
	int next;
};
struct Player
{
	int Hp;
	int MaxHp;
	int WeaponCd;
	int maxWeaponCd;
	int Damage;
	int SkillCd;
	int maxSkillCd;
	int x;
	int y;
	int Upgrade;
	int MoveSpeed;
	int OnAttack;
	int custom;
	int costume;
	int AttackSpeed;
	int moveCd;
};
struct Bullets {
	int x;
	int y;
	int speed;
	int Damage;
	int type;
};

struct Bullets Bullet[30];

struct Player p1;
struct Player p2;

struct Boss boss1;

struct BossAttack bossAttack[270];


void Shot(int x, int y, int type) {
	bossAttack[i].isActive = 1;
	bossAttack[i].x = boss1.x + x * 2;
	bossAttack[i].y = boss1.y + y;
	bossAttack[i].type = type;
	i += 270;
}

int fpstest(int a) {
	return FPS % a == 0;
}

void skillmanager() {
	if (p1.SkillCd > 0) {
		p1.SkillCd--;
	}
	//p1.SkillCd = p1.maxSkillCd;

	if (p1.WeaponCd > 0) {
		p1.WeaponCd--;
	}
	//p1.WeaponCd = p1.maxWeaponCd;

	if (p2.SkillCd > 0) {
		p2.SkillCd--;
	}
	//p2.SkillCd = p2.maxSkillCd;

	if (p2.WeaponCd > 0 && Mbullet > bullet) {
		p2.WeaponCd--;
		if (p2.SkillCd > 185) {
			p2.WeaponCd -= 3;
		}
		if (p2.WeaponCd <= 0) {
			p2.WeaponCd = p2.maxWeaponCd;
			if (Mbullet > bullet) {
				bullet++;
			}
		}
	}
}

void AttackManager() {
	p2.OnAttack--;
	if (kbhit()) {//키감지 - 공격 상호작용 if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		if (p1.Hp > 0) {//자동 공격, 근거리 공격
			if (p1.WeaponCd == 0) {
				p1.WeaponCd = p1.maxWeaponCd;
				p1.OnAttack = p1.AttackSpeed;
			}
		}
		if (GetAsyncKeyState(0x4F) & 0x8000 && p2.Hp > 0) {//O, 원거리 사격
			if (bullet > 0 && p2.OnAttack < 1) {
				bullet -= 1;
				for (int i = 0; i < 30; i++) {
					if (bulletActive[i] == 0) {
						Bullet[i].x = p2.x;
						Bullet[i].y = p2.y;
						Bullet[i].type = p2.custom;//기본 장비: 권총
						Bullet[i].Damage = p2.Damage;
						Bullet[i].speed = 1;
						bulletActive[i] = 1;
						p2.OnAttack = p2.AttackSpeed;
						break;
					}
				}
			}
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && p1.Hp > 0) {//근거리 스킬, 스페이스로 발동, 탄을 전부 제거하고, 보스를 기절시킴
			if (p1.SkillCd == 0) {
				p1.SkillCd = p1.maxSkillCd;
				boss1.timer += 45;
				boss1.next += 45;
				boss1.hp -= 10;
			}
		}
		if (p2.SkillCd == 185) {
			Mbullet /= 2;
			if (Mbullet < bullet) {
				bullet = Mbullet;
			}
		}
		if (GetAsyncKeyState(0x50) & 0x8000 && p2.Hp > 0) {//원거리 스킬, P로 발동, 탄창을 잠시 2배로 늘리고, 탄 재장전을 잠시 4배 빠르게 함.
			if (p2.SkillCd == 0) {
				p2.SkillCd = p2.maxSkillCd;
				Mbullet *= 2;
			}
		}
	}

	for (int i = 0; i < 30; i++) {
		if (bulletActive[i] == 1) {
			if (fpstest(Bullet[i].speed)) Bullet[i].y -= 1;
			if (Bullet[i].x <= boss1.x + 2 && Bullet[i].y <= boss1.y + 1 && Bullet[i].x >= boss1.x - 2 && Bullet[i].y >= boss1.y - 1) {
				boss1.hp -= Bullet[i].Damage;
				bulletActive[i] = 0;
			}
			if (Bullet[i].y < 1) {
				bulletActive[i] = 0;
				continue;
			}
			if (Bullet[i].type == 1) ScreenPrint(Bullet[i].x, Bullet[i].y, "ㅁ");
			if (Bullet[i].type == 2) ScreenPrint(Bullet[i].x, Bullet[i].y, "◎");
			if (Bullet[i].type == 3) ScreenPrint(Bullet[i].x, Bullet[i].y, "＊");
		}
	}
}

void IngameManager() {

	if (p1.Hp < 0 && p2.Hp < 0) {
		//함수 게임오버
	}

	if (kbhit() && fpstest(p1.MoveSpeed))
	{
		if (p1.moveCd == 0) {
			if (GetAsyncKeyState(0x57)) p1.y--;
			if (GetAsyncKeyState(0x41)) p1.x -= 2;
			if (GetAsyncKeyState(0x53)) p1.y++;
			if (GetAsyncKeyState(0x44)) p1.x += 2;
		}
		if (GetAsyncKeyState(0x1B)) {
			tutorialback = getch();
			maintitlereturn = -1;
			FPS = 0;
		}
		if (GetAsyncKeyState(VK_UP)) p2.y--;
		if (GetAsyncKeyState(VK_LEFT)) p2.x -= 2;
		if (GetAsyncKeyState(VK_DOWN)) p2.y++;
		if (GetAsyncKeyState(VK_RIGHT)) p2.x += 2;

	}

	if (p1.x == 266)
	{
		p1.x -= 2;
	}
	if (p2.x == 266)
	{
		p2.x -= 2;
	}
	if (p1.x == 52)
	{
		p1.x += 2;
	}
	if (p2.x == 52)
	{
		p2.x += 2;
	}

	if (p1.y == 0)
	{
		p1.y += 1;
	}
	if (p2.y == 0)
	{
		p2.y += 1;
	}
	if (p1.y == 68)
	{
		p1.y -= 1;
	}
	if (p2.y == 68)
	{
		p2.y -= 1;
	}
	if (p1.Hp > 0) {
		switch (p1.costume)
		{
		case 1: ScreenPrint(p1.x, p1.y, "웃"); break;
		case 2: ScreenPrint(p1.x, p1.y, "①"); break;
		case 3: ScreenPrint(p1.x, p1.y, "▣"); break;
		case 4: ScreenPrint(p1.x, p1.y, "P1"); break;
		case 5: ScreenPrint(p1.x, p1.y, "뭇"); break;
		}
	}
	else p1.x = 400;
	if (p2.Hp > 0) {
		switch (p2.costume)
		{
		case 1: ScreenPrint(p2.x, p2.y, "웃"); break;
		case 2: ScreenPrint(p2.x, p2.y, "②"); break;
		case 3: ScreenPrint(p2.x, p2.y, "▣"); break;
		case 4: ScreenPrint(p2.x, p2.y, "P2"); break;
		case 5: ScreenPrint(p2.x, p2.y, "뭇"); break;
		}
	}
	else p2.x = 400;


	if (p1.OnAttack > 0) {
		p1.OnAttack--;
		if (p1.custom == 1) {
			ScreenPrint(p1.x, p1.y - 1, "╀");
			ScreenPrint(p1.x, p1.y - 2, "┃");
			ScreenPrint(p1.x, p1.y - 3, "┃");
			ScreenPrint(p1.x, p1.y - 4, "┃");
			if (p1.x <= boss1.x + 2 && p1.x >= boss1.x - 2 && (p1.y >= boss1.y && p1.y - 5 <= boss1.y)) {
				boss1.hp -= p1.Damage;
				p1.OnAttack = 0;
			}
		}
		if (p1.custom == 2) {
			ScreenPrint(p1.x, p1.y - 1, "╀");
			ScreenPrint(p1.x, p1.y - 2, "┃");
			if (p1.x <= boss1.x + 2 && p1.x >= boss1.x - 2 && (p1.y >= boss1.y && p1.y - 3 <= boss1.y)) {
				boss1.hp -= p1.Damage;
				p1.OnAttack = 0;
			}
		}
	}
}

int maintitle() {


	p1.x = 140; p1.y = 40;
	p2.x = 180; p2.y = 40;
	boss1.x = 160, boss1.y = 12;
	boss1.maxhp = 600;
	boss1.hp = boss1.maxhp;//테스트용 /2 삭제해야함
	p1.MaxHp = 16;
	p2.MaxHp = 13;
	p1.Hp = p1.MaxHp;
	p2.Hp = p2.MaxHp;


	for (int x = 0; x <= 160; x++) {
		for (int y = 0; y <= 84; y++) {
			if ((x == 50 || x == 110) && y < 70 && y > 14) {//Y축 그리기
				ScreenPrint(x * 2, y, "■");
			}
			if ((y == 70 || y == 14) && x > 49 && x < 111) {//X축 그리기
				ScreenPrint(x * 2, y, "■");
			}
		}
	}
	ScreenPrint(cursorx, cursory, "→");
	ScreenPrint(158, 30, "게임 시작");
	ScreenPrint(158, 45, "게임 설명");
	ScreenPrint(158, 60, "설정");

	if (kbhit() && fpstest(8))
	{
		if (GetAsyncKeyState(VK_UP)) {
			pagetransfer--;
			if (pagetransfer == 0) {
				pagetransfer = 3;
			}
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			pagetransfer++;
			if (pagetransfer == 4) {
				pagetransfer = 1;
			}
		}
		if (GetAsyncKeyState(VK_RETURN)) {
			pageenter = 1;
		}
		cursory = 15 + (pagetransfer * 15);
	}

	if (pageenter)
	{
		pageenter = 0;
		return pagetransfer;
	}


	return -1;
}

void tutorial()
{
	ScreenPrint(p1.x, p1.y, "웃");
	if (kbhit() && fpstest(p1.MoveSpeed))
	{
		if (GetAsyncKeyState(0x57)) p1.y--;
		if (GetAsyncKeyState(0x41)) p1.x -= 2;
		if (GetAsyncKeyState(0x53)) p1.y++;
		if (GetAsyncKeyState(0x44)) p1.x += 2;
		if (GetAsyncKeyState(0x1B)) {
			tutorialback = getch();
			maintitlereturn = -1;
			FPS = 0;
		}
		if (GetAsyncKeyState(VK_UP)) p2.y--;
		if (GetAsyncKeyState(VK_LEFT)) p2.x -= 2;
		if (GetAsyncKeyState(VK_DOWN)) p2.y++;
		if (GetAsyncKeyState(VK_RIGHT)) p2.x += 2;
	}
	ScreenPrint(140 - 10, 44 - 1, "W");
	ScreenPrint(140 - 2 - 10, 44, "A");
	ScreenPrint(140 - 10, 44 + 1, "S");
	ScreenPrint(140 + 2 - 10, 44, "D");
	ScreenPrint(140, 44, "스킬: SPACE");


	ScreenPrint(p2.x, p2.y, "웃");

	ScreenPrint(180 - 10, 44 - 1, "↑");
	ScreenPrint(180 - 2 - 10, 44, "←");
	ScreenPrint(180 - 10, 44 + 1, "↓");
	ScreenPrint(180 + 2 - 10, 44, "→");
	ScreenPrint(180, 44 - 1, "공격: O");
	ScreenPrint(180, 44 + 1, "스킬: P");


}

void custom() {//무기 바꾸기, 플레이어 외형 바꾸기
	bullet = 0;
	if (kbhit() && fpstest(10)) {
		if (GetAsyncKeyState(0x44)) {
			Wp1++;
			if (Wp1 == 3) Wp1 = 1;
		}
		if (GetAsyncKeyState(0x41)) {
			Wp1--;
			if (Wp1 == 0) Wp1 = 2;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			Wp2++;
			if (Wp2 == 4) Wp2 = 1;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			Wp2--;
			if (Wp2 == 0) Wp2 = 3;
		}
		if (GetAsyncKeyState(0x57)) {
			p1.costume--;
			if (p1.costume == 0) p1.costume = 5;
		}
		if (GetAsyncKeyState(0x53)) {
			p1.costume++;
			if (p1.costume == 6) p1.costume = 1;
		}
		if (GetAsyncKeyState(VK_UP)) {
			p2.costume--;
			if (p2.costume == 0) p2.costume = 5;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			p2.costume++;
			if (p2.costume == 6) p2.costume = 1;
		}
		if (GetAsyncKeyState(0x1B)) {
			tutorialback = getch();
			maintitlereturn = -1;
			FPS = 0;
		}
	}

	switch (Wp1)
	{
	case 1:
		p1.maxWeaponCd = 16;
		p1.Damage = 15;
		p1.AttackSpeed = 5;
		p1.custom = 1;
		ScreenPrint(100, 45, "대검");
		break;
	case 2:
		p1.maxWeaponCd = 3;
		p1.Damage = 6;
		p1.AttackSpeed = 2;
		p1.custom = 2;
		ScreenPrint(100, 45, "암살 단검");
		break;
	}
	switch (Wp2)
	{
	case 1:
		p2.maxWeaponCd = 14;
		p2.Damage = 5;
		p2.AttackSpeed = 2;
		p2.custom = 1;
		Mbullet = 6;
		ScreenPrint(210, 45, "권총");
		break;
	case 2:
		p2.maxWeaponCd = 34;
		p2.Damage = 23;
		p2.AttackSpeed = 8;
		p2.custom = 2;
		Mbullet = 2;
		ScreenPrint(210, 45, "유탄발사기");
		break;
	case 3:
		p2.maxWeaponCd = 4;
		p2.Damage = 2;
		p2.AttackSpeed = 0;
		p2.custom = 3;
		Mbullet = 12;
		ScreenPrint(210, 45, "기관총");
		break;
	}
	switch (p1.costume)
	{
	case 1: ScreenPrint(100, 50, "옷"); break;
	case 2: ScreenPrint(100, 50, "①"); break;
	case 3: ScreenPrint(100, 50, "▣"); break;
	case 4: ScreenPrint(100, 50, "P1"); break;
	case 5: ScreenPrint(100, 50, "뭇"); break;
	}
	switch (p2.costume)
	{
	case 1: ScreenPrint(210, 50, "옷"); break;
	case 2: ScreenPrint(210, 50, "②"); break;
	case 3: ScreenPrint(210, 50, "▣"); break;
	case 4: ScreenPrint(210, 50, "P2"); break;
	case 5: ScreenPrint(210, 50, "뭇"); break;
	}
	ScreenPrint(90, 45, "장비: ");
	ScreenPrint(90, 44, "A/D로 변경");
	ScreenPrint(200, 45, "장비: ");
	ScreenPrint(200, 44, "←/→로 변경");
	ScreenPrint(90, 50, "아이콘: ");
	ScreenPrint(90, 49, "W/S로 변경");
	ScreenPrint(200, 50, "아이콘: ");
	ScreenPrint(200, 49, "↑/↓로 변경");
	ScreenPrint(95, 40, "P1");
	ScreenPrint(205, 40, "P2");
}

void UiSword()
{
	for (int i = 0; i < 23; i++)
	{
		ScreenPrint(4 + i * 2, 12, "■");
	}
	for (int i = 0; i < 16; i++)
	{
		ScreenPrint(6 + i * 2, 11, "■");
	}
	for (int i = 0; i < 16; i++)
	{
		ScreenPrint(6 + i * 2, 13, "■");
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ScreenPrint(34 + j * 2, 10 + i, "■");
		}
	}
}

void UiGun()
{
	int GunArray[9][25] =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			if (GunArray[i][j] == 1)
			{
				ScreenPrint(268 + j * 2, 8 + i, "■");
			}

		}
	}

}

void UI() {
	for (int x = 0; x < 160; x++) {
		for (int y = 0; y < 84; y++) {
			if (x == 0 || x == 26 || x == 159 || x == 133) {//기반 세로줄
				ScreenPrint(x * 2, y, "■");
			}
			if (y == 0 || y == 83) {//기반 가로줄
				ScreenPrint(x * 2, y, "■");
			}
			if (y == 69 && x > 26 && x < 133) {
				ScreenPrint(x * 2, y, "■");
			}
			if ((y == 6 || y == 19 || y == 32 || y == 17 || y == 30) && (x < 26 || x > 133)) {
				ScreenPrint(x * 2, y, "■");
			}
			if ((x == 22 || x == 137) && (y > 32)) {
				ScreenPrint(x * 2, y, "■");
			}
		}
	}
	ScreenPrint(26, 3, "P1");
	ScreenPrint(292, 3, "P2");

	ScreenPrint(48, 81, "HP");
	for (y = 79; 79 - y < p1.MaxHp; y--) {
		ScreenPrint(48, y, "□");
	}for (y = 79; 79 - y < p1.Hp; y--) {
		ScreenPrint(48, y, "■");
	}
	ScreenPrint(270, 81, "HP");
	for (y = 79; 79 - y < p2.MaxHp; y--) {
		ScreenPrint(270, y, "□");
	}for (y = 79; 79 - y < p2.Hp; y--) {
		ScreenPrint(270, y, "■");
	}

	for (int i = 25; i > p1.SkillCd * 25 / p1.maxSkillCd; i--) {
		if (p1.SkillCd == 0) {
			ScreenPrint(52 - i * 2, 31, "■");
		}
		else ScreenPrint(52 - i * 2, 31, "▩");
	}
	for (int i = 25; i > p1.WeaponCd * 25 / p1.maxWeaponCd; i--) {
		if (p1.WeaponCd == 0) {
			ScreenPrint(52 - i * 2, 18, "■");
		}
		else ScreenPrint(52 - i * 2, 18, "▩");

	}
	for (int i = 25; i > p2.SkillCd * 25 / p2.maxSkillCd; i--) {
		if (p2.SkillCd == 0) {
			ScreenPrint(318 - i * 2, 31, "■");
		}
		else ScreenPrint(318 - i * 2, 31, "▩");

	}
	for (int i = 25; i > p2.WeaponCd * 25 / p2.maxWeaponCd; i--) {
		if (p2.WeaponCd == 0) {
			ScreenPrint(318 - i * 2, 18, "■");
		}
		else ScreenPrint(318 - i * 2, 18, "▩");

	}

	for (int i = 0; i < 96; i++) {
		ScreenPrint(64 + i * 2, 77, "□");
	}
	for (int i = 0; i < boss1.hp * 96 / boss1.maxhp; i++) {
		ScreenPrint(64 + i * 2, 77, "■");
		if (i == 48) {
			ScreenPrint(64 + i * 2, 77, "▣");
		}
	}
	ScreenPrint(155, 74, "차원 방랑자");

	for (int i = 0; i < Mbullet; i++) {
		if (i >= bullet) {
			ScreenPrint(268 + i * 2, 7, "□");
		}
		else ScreenPrint(268 + i * 2, 7, "▣");
	}

	UiSword();
	UiGun();
}

void Win()
{
	ScreenPrint(160, 42, "YOU WIN");
}

void Lose()
{
	ScreenPrint(160, 42, "YOU LOSE");

}

void BossManager() {
	if (boss1.hp < 0) {
		//다음 보스전 준비 함수 (미구현)
		//게임 승리 화면을 무한으로 송출할 함수 Win();
		maintitlereturn = 4;
		return;
	}
	if (p1.Hp <= 0 && p2.Hp <= 0)
	{
		maintitlereturn = 5;
		return;
	}
	boss1.timer -= 1;
	boss1.next -= 1;

	if (boss1.next == 0) {
		isSkip = rand() % 4 + 5;
		if (boss1.hp / boss1.maxhp <= 0.5) {//보스의 체력이 절반 이하로 내려가면 궁극기를 시전하기 시작
			isSkip = rand() % 4;
		}

		if (boss1.attack != 11) {
			boss1.attack = 11;
			if (isSkip < 5) {
				boss1.next = 1;
			}
			else boss1.next = 3;

		}
		else boss1.attack = rand() % 11;
		boss1.timer = 0;
	}

	if (boss1.timer <= 0) {
		if (boss1.attack == 0) {//좌우 탄막 난사
			if (boss1.next <= 0) boss1.next = 6;//최초 실행시 패턴 수행 시간
			boss1.y += (rand() % 6) * 1 - 1;
			Prison(60, 260, 5, 67);
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 0);
				}
			}
			if (isSkip > 4) {
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, -1, 0);
						boss1.timer = 2;
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 1, 0);
					}
				}
			}
			if (isSkip < 5) {

				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 3);
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 2);
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 6);
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 7);
					}
				}
			}
		}
		if (boss1.attack == 1) {//탄막 분사 패턴
			if (boss1.next <= 0) boss1.next = 11;//최초 실행시 패턴 수행 시간
			boss1.x += (rand() % 3) * 2 - 2;
			Prison(60, 260, 5, 33);
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 1);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(2, -1, 1);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(-2, -1, 1);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 2);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 3);
					}
				}
			}
			if (isSkip < 5) {

				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(1, 0, 2);
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						if (bossAttack[i].isActive == 0) {
							Shot(-1, 0, 3);
						}
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(-1, 0, 2);
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						if (bossAttack[i].isActive == 0) {
							Shot(1, 0, 3);
						}
					}
				}
			}
			boss1.timer = 2;
		}
		if (boss1.attack == 2) {//이동
			if (boss1.next <= 0) {
				boss1.next = 10;
				Prison(60, 260, 12, 22);
			}
			boss1.x += 2;
			Prison(60, 260, 5, 52);
			boss1.timer = 1;
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0 && boss1.next % 3 == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 2);
					}
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0 && boss1.next % 3 == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(2, 0, 2);
					}
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0 && boss1.next % 3 == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(-2, 0, 2);
					}
				}
			}
		}
		if (boss1.attack == 3) {//이동
			if (boss1.next <= 0) {
				boss1.next = 10;
				Prison(60, 260, 12, 42);
			}
			boss1.x -= 2;
			Prison(60, 260, 5, 52);
			boss1.timer = 1;
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0 && boss1.next % 3 == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(-2, 0, 3);
					}
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0 && boss1.next % 3 == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 3);
					}
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0 && boss1.next % 3 == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(2, 0, 3);
					}
				}
			}
		}
		if (boss1.attack == 4) {//이동
			if (boss1.next <= 0) {
				boss1.next = 10;
				Prison(150, 170, 12, 22);
				if (isSkip < 5) boss1.next = 2;
			}
			boss1.x += (rand() % 5) * 2 - 4;
			boss1.y += (rand() % 3) * 1 - 1;
			Prison(60, 260, 5, 67);
			boss1.timer = 6;
		}
		if (boss1.attack == 5 && isSkip <= 1) {//강력한 차징 공격
			if (boss1.next <= 0) {
				boss1.next = 180;
				boss1.x = 160;
				boss1.y = 35;
			}
			if ((boss1.next >= 62 && boss1.next <= 95) && boss1.next % 2 == 0) {
				do {
					boss1.x = 160 + rand() % 101 * 2 - 100;
					boss1.y = 30 + rand() % 50 - 20;
				} while ((p1.x > -11 && p1.x < 11 && p1.y > -6 && p1.y < 6) || (p2.x > -11 && p2.x < 11 && p2.y > -6 && p2.y < 6));
				Prison(60, 260, 5, 67);
				for (j = 1; j < 9; j++) {
					for (i = 0; i < 270; i++) {
						if (bossAttack[i].isActive == 0) {
							Shot(0, 0, j);
						}
					}
				}
				for (j = 1; j < 9; j++) {
					for (i = 0; i < 270; i++) {
						if (bossAttack[i].isActive == 0) {
							Shot(rand() % 3 + 1, rand() % 2 - 2, j);
						}
					}
				}
				for (j = 1; j < 9; j++) {
					for (i = 0; i < 270; i++) {
						if (bossAttack[i].isActive == 0) {
							Shot(rand() % 3 - 3, rand() % 2 + 1, j);
						}
					}
				}
			}
		}
		else if (boss1.attack == 5) boss1.next = 1;
		if (boss1.attack == 7 && isSkip < 4) {//약한 차징 공격
			if (boss1.next == 0) {
				boss1.next = 36;
				if (rand() % 2 == 0) {
					if (p1.Hp > 0) {
						boss1.x = p1.x;
						boss1.y = p1.y - 1;
					}
					else {
						boss1.x = p2.x;
						boss1.y = p2.y - 1;
					}
				}
				else {
					if (p2.Hp > 0) {
						boss1.x = p2.x;
						boss1.y = p2.y - 1;
					}
					else {
						boss1.x = p1.x;
						boss1.y = p1.y - 1;
					}
				}
			}
			if (boss1.next < 22 && boss1.next > 17) {
				boss1.x += (rand() % 11) * 2 - 11;
				boss1.y += (rand() % 9) * 1 - 4;
				Prison(60, 260, 5, 67);
				for (j = 1; j < 9; j++) {
					for (i = 0; i < 270; i++) {
						if (bossAttack[i].isActive == 0) {
							Shot(0, 0, j);
						}
					}
				}
				for (j = 1; j < 9; j++) {
					for (i = 0; i < 270; i++) {
						if (bossAttack[i].isActive == 0) {
							Shot(rand() % 2 * 2 + 2, rand() % 2 - 2, j);
						}
					}
				}
				for (j = 1; j < 9; j++) {
					for (i = 0; i < 270; i++) {
						if (bossAttack[i].isActive == 0) {
							Shot(rand() % 2 * 2 - 2, rand() % 2 + 1, j);
						}
					}
				}
			}
		}
		else if (boss1.attack == 7) boss1.next = 1;

		if (boss1.attack == 6) {//상단 탄막 분사 패턴
			if (boss1.next <= 0) boss1.next = 17;//최초 실행시 패턴 수행 시간
			boss1.x += (rand() % 5) * 2 - 2;
			boss1.y += (rand() % 2);
			Prison(60, 260, 57, 67);
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 6);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(2, 0, 8);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(-2, 0, 8);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 7);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					if (bossAttack[i].isActive == 0) {
						Shot(0, 0, 8);
					}
				}
			}
			if (isSkip < 5) {

				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(2, 0, 7);
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						if (bossAttack[i].isActive == 0) {
							Shot(-2, 0, 8);
						}
					}
				}
				boss1.timer = 2;
			}
			boss1.timer += 3;
		}
		if (boss1.attack == 8) {//플레이어 x 추적 비탄
			if (boss1.next <= 0) {
				boss1.next = 14;//최초 실행시 패턴 수행 시간
				if (rand() % 2 == 0 && p1.Hp > 0) {
					boss1.x = p1.x + rand() % 3 * 2 - 2;
				}
				else if (p2.Hp > 0) {
					boss1.x = p2.x + rand() % 3 - 1;
				}
				else boss1.x = p1.x + rand() % 3 * 2 - 2;
			}

			boss1.x += (rand() % 5) * 2 - 4;
			boss1.y += (rand() % 2) * 1 - 1;
			Prison(60, 260, 5, 33);
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 1);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(rand() % 3 * 2 - 6, 0, 1);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(rand() % 3 * 2 + 2, 0, 1);
				}
			}
			if (isSkip < 5) {

				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(rand() % 3 * 2 - 10, 0, 1);
					}
				}
				for (i = 0; i < 270; i++) {
					if (bossAttack[i].isActive == 0) {
						Shot(rand() % 3 * 2 + 6, 0, 1);
					}
				}
			}
			boss1.timer = 3;


		}
		if (boss1.attack == 9) {//혼돈 탄막 발사
			if (boss1.next <= 0) {
				boss1.next = 17;//최초 실행시 패턴 수행 시간
				if (rand() % 2 == 0 && p1.Hp > 0) {
					boss1.x = p1.x + rand() % 3 * 2 - 2;
				}
				else if (p2.Hp > 0) {
					boss1.x = p2.x + rand() % 3 - 1;
				}
				else boss1.x = p1.x + rand() % 3 * 2 - 2;
			}
			Prison(60, 260, 5, 33);
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 10);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 11);
				}
			}
			for (i = 0; i < 270; i++) {
				if (bossAttack[i].isActive == 0) {
					Shot(0, 0, 12);
				}
			}
			if (isSkip < 5) {
				boss1.timer = 3;
			}
			else boss1.timer = 6;
		}
		if (boss1.attack == 10 && isSkip >= 8) {//휴식, 2페이즈에선 사용하지 않음.
			if (boss1.next <= 0) {
				boss1.next = 80;
			}
			if (boss1.hp < boss1.maxhp) boss1.hp += 0.1;
		}
		else if (boss1.attack == 10 && isSkip < 8) boss1.next = 1;



	}
	//if (x == 0 || x == 26 || x == 159 || x == 133) {//기반 세로줄
	//if (y == 0 || y == 83) {//기반 가로줄

	for (int i = 0; i < 270; i++) {
		if (bossAttack[i].isActive == 1) {
			if (bossAttack[i].type == 0) {
				if (i % 2 == 0) {
					bossAttack[i].x -= 2;
				}
				else bossAttack[i].x += 2;

				if (p1.SkillCd > 266)//탄 소멸 스킬 발동시 탄 삭제
				{
					bossAttack[i].isActive = 0;
					continue;
				}if (p1.x == bossAttack[i].x && p1.y == bossAttack[i].y) {
					p1.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (p2.x == bossAttack[i].x && p2.y == bossAttack[i].y) {
					p2.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (bossAttack[i].x > 264 || bossAttack[i].x < 54 || bossAttack[i].y > 67 || bossAttack[i].y < 1) {
					bossAttack[i].isActive = 0;
					continue;
				}

				ScreenPrint(BOSS "↔");

			}
			if (bossAttack[i].type == 1 || bossAttack[i].type == 2 || bossAttack[i].type == 3) {
				bossAttack[i].y++;
				if (bossAttack[i].type == 2) bossAttack[i].x -= 2;
				if (bossAttack[i].type == 3) bossAttack[i].x += 2;

				if (p1.SkillCd > 266)//탄 소멸 스킬 발동시 탄 삭제
				{
					bossAttack[i].isActive = 0;
					continue;
				}if (p1.x == bossAttack[i].x && p1.y == bossAttack[i].y) {
					p1.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (p2.x == bossAttack[i].x && p2.y == bossAttack[i].y) {
					p2.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (bossAttack[i].x > 264 || bossAttack[i].x < 54 || bossAttack[i].y > 67 || bossAttack[i].y < 1) {
					bossAttack[i].isActive = 0;
					continue;
				}

				if (bossAttack[i].type == 1) ScreenPrint(BOSS "↓");
				if (bossAttack[i].type == 2) ScreenPrint(BOSS "↙");
				if (bossAttack[i].type == 3) ScreenPrint(BOSS "↘");
			}
			if (bossAttack[i].type >= 4 || bossAttack[i].type <= 7) {
				if (bossAttack[i].type == 4) bossAttack[i].x -= 2;
				if (bossAttack[i].type == 5) bossAttack[i].x += 2;
				if (bossAttack[i].type == 6) {
					bossAttack[i].x += 2;
					bossAttack[i].y -= 1;
				}
				if (bossAttack[i].type == 7) {
					bossAttack[i].x -= 2;
					bossAttack[i].y -= 1;
				}
				if (bossAttack[i].type == 8) bossAttack[i].y -= 1;

				if (p1.SkillCd > 266)//탄 소멸 스킬 발동시 탄 삭제
				{
					bossAttack[i].isActive = 0;
					continue;
				}if (p1.x == bossAttack[i].x && p1.y == bossAttack[i].y) {
					p1.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (p2.x == bossAttack[i].x && p2.y == bossAttack[i].y) {
					p2.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (bossAttack[i].x > 264 || bossAttack[i].x < 54 || bossAttack[i].y > 67 || bossAttack[i].y < 1) {
					bossAttack[i].isActive = 0;
					continue;
				}

				if (bossAttack[i].type == 4) ScreenPrint(BOSS "←");
				if (bossAttack[i].type == 5) ScreenPrint(BOSS "→");
				if (bossAttack[i].type == 6) ScreenPrint(BOSS "↗");
				if (bossAttack[i].type == 7) ScreenPrint(BOSS "↖");
				if (bossAttack[i].type == 8) ScreenPrint(BOSS "↑");
			}
			if (bossAttack[i].type == 10 || bossAttack[i].type == 11 || bossAttack[i].type == 12) {
				bossAttack[i].y++;
				if (bossAttack[i].type == 10) {
					bossAttack[i].y += rand() % 2 - 1;
					bossAttack[i].x += rand() % 3 * 2 - 1;
				}
				if (bossAttack[i].type == 11) bossAttack[i].x += rand() % 2 * 2;
				if (bossAttack[i].type == 12) bossAttack[i].x += rand() % 2 * -2;

				if (p1.SkillCd > 266)//탄 소멸 스킬 발동시 탄 삭제
				{
					bossAttack[i].isActive = 0;
					continue;
				}if (p1.x == bossAttack[i].x && p1.y == bossAttack[i].y) {
					p1.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (p2.x == bossAttack[i].x && p2.y == bossAttack[i].y) {
					p2.Hp -= 1;
					bossAttack[i].isActive = 0;
					continue;
				}if (bossAttack[i].x > 264 || bossAttack[i].x < 54 || bossAttack[i].y > 67 || bossAttack[i].y < 1) {
					bossAttack[i].isActive = 0;
					continue;
				}

				ScreenPrint(BOSS "◈");
			}
		}
	}

	switch (boss1.attack)
	{
	case 5: ScreenPrint(boss1.x - 2, boss1.y - 1, "■ ◆ ■");
		ScreenPrint(boss1.x - 2, boss1.y, "□ ■ □");
		ScreenPrint(boss1.x - 2, boss1.y + 1, "■ ■ ■");
		break;
	case 7: ScreenPrint(boss1.x - 2, boss1.y - 1, "■ ◆ ■");
		ScreenPrint(boss1.x - 2, boss1.y, "□ ■ □");
		ScreenPrint(boss1.x - 2, boss1.y + 1, "■ ■ ■");
		break;
	case 10: ScreenPrint(boss1.x - 2, boss1.y - 1, "■ ◇ ■");
		ScreenPrint(boss1.x - 2, boss1.y, "□ ■ □");
		ScreenPrint(boss1.x - 2, boss1.y + 1, "■ ■ ■");
		break;
	case 11: ScreenPrint(boss1.x - 2, boss1.y - 1, "■ ◈ ■");
		ScreenPrint(boss1.x - 2, boss1.y, "▣ ■ ▣");
		ScreenPrint(boss1.x - 2, boss1.y + 1, "■ 〓■");
		break;
	default: ScreenPrint(boss1.x - 2, boss1.y - 1, "■ ◈ ■");
		ScreenPrint(boss1.x - 2, boss1.y, "▣ ■ ▣");
		ScreenPrint(boss1.x - 2, boss1.y + 1, "■ ㅁ■");
		break;
	}
	//임시 이미지
}

void Prison(int x1, int x2, int y1, int y2) { // 맵 이탈 방지
	if (boss1.x < x1) boss1.x = x1;
	if (boss1.x > x2) boss1.x = x2;
	if (boss1.y < y1) boss1.y = y1;
	if (boss1.y > y2) boss1.y = y2;
	if (boss1.x % 2 == 1) {
		boss1.x -= 1;
	}
}

void gamestart() {
	skillmanager();
	UI();
	IngameManager();
	AttackManager();
	BossManager();
}

void startOption() {
	setWindowSize(320, 84);
	setFont(12);
	ScreenInit();//초기값 설정x
	p2.maxWeaponCd = 14;
	p2.Damage = 6;
	p2.AttackSpeed = 2;
	p2.custom = 1;
	Mbullet = 6;
	p1.maxWeaponCd = 16;
	p1.Damage = 15;
	p1.AttackSpeed = 5;
	p1.custom = 1;
	p1.maxSkillCd = 320;
	p2.maxSkillCd = 230;

	p1.SkillCd = 1;
	p1.WeaponCd = 1;
	p2.SkillCd = 1;
	p2.WeaponCd = 1;
	p1.MoveSpeed = 1;
	p2.MoveSpeed = 1;
	p1.costume = 2;
	p2.costume = 2;
	boss1.next = 0;
	boss1.timer = 0;
	srand(time(NULL));
	for (int i = 0; i < 270; i++) {
		bossAttack[i].isActive = 0;
	}
}

int main() {
	startOption();
	while (1)
	{
		ScreenClear();//다음 스크린 지우기
		FPS += 1;

		if (maintitlereturn == -1) {//maintitlereturn이 -1이라면 함수를 실행
			maintitlereturn = maintitle(); //maintitle을 실행해 값을 리턴에 넣는다. (-1: 플레이어가 선택하지 않음, 재실행) (1: 게임 시작) (2: 게임 설명)
			//1이나 2를 선택한 후엔 플레이어가 되돌아오기 전까지 함수를 비활성화 한다.
			
			if (FPS == 25) FPS = 0;
			for (int i = 0; i < FPS; i++) {
				ScreenPrint(136 + i * 2, 0, "■");
			}
			
		}
		else if (maintitlereturn == 1) {//게임 시작
			gamestart();
		}
		else if (maintitlereturn == 2) {//게임 설명
			tutorial();
		}
		else if (maintitlereturn == 3) {//커스터마이즈
			custom();
		}
		else if (maintitlereturn == 4)
		{
			Win();
		}
		else if (maintitlereturn == 5)
		{
			Lose();
		}


		Sleep(1);//생명 주기 (딜레이, 1000당 1초)
		ScreenFlipping();//출력
	}

	ScreenRelease();

	return 0;
}