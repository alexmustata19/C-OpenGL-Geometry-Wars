/*Tema 1 EGC: Geometry Wars
Student: MUSTATA Alexandru-Ionut
Grupa: 331CB*/

#include "Tema1.h"
#include <vector>
#include <iostream>
#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include "BigBang.h"
#include "Ship.h"
#include "Missle.h"
#include "OneLifeEnemy.h"
#include "TwoLifeEnemy.h"
#include "BonusLife.h"
#include "Freeze.h"
#include "MultipleShot.h"
#include "Invincibility.h"
#include "DividingEnemy.h"
#include "Shield.h"
#include "SlowDown.h"

using namespace std;

//Variabile pentru culori.
glm::vec3 Portocaliu = glm::vec3(1, 0.5, 0);
glm::vec3 Tan = glm::vec3(1, 0.8, 0.6);
glm::vec3 Mov = glm::vec3(0.3, 0, 0.6);
glm::vec3 Albastru = glm::vec3(0, 0, 1);
glm::vec3 Cyan = glm::vec3(0, 1, 1);
glm::vec3 Magenta = glm::vec3(1, 0, 1);
glm::vec3 Verde = glm::vec3(0, 1, 0);
glm::vec3 Galben = glm::vec3(1, 1, 0);
glm::vec3 Roz = glm::vec3(1, 0.5, 0.75);
glm::vec3 Alb = glm::vec3(1, 1, 1);
glm::vec3 Bleu = glm::vec3(0.6, 0.8, 1);
glm::vec3 Seagreen = glm::vec3(0.3, 1, 0.6);
glm::vec3 Sandybrown = glm::vec3(0.95, 0.64, 0.37);

//Timpul de start al jocului.
float globalStartTime = 0;

//Timpi referitori la power-up-uri.
float bonusTimeGet = 3;
float bonusTimeAvability = 3;
float grabedMultipleShot = 0;
float grabedInvincibility = 0;
float grabedFreeze = 0;
float grabedSlowDown = 0;

float lastInvincibility = 0;
float lastMultipleShot = 0;
float lastBonusLife = 0;
float lastFreeze = 0;
float lastBigBang = 0;
float lastShield = 0;
float lastSlowDown = 0;

float bonusTimeAppearanceFreeze = 22;
float bonusTimeAppearanceMultiShot = 15;
float bonusTimeAppearanceBonusLife = 24;
float bonusTimeAppearanceBigBang = 30;
float bonusTimeAppearanceInvincibility = 28;
float bonusTimeAppearanceShield = 18;
float bonusTimeAppearanceSlowDown = 20;

//Variabile de stare pentru bonusuri.
bool notGetBonusLife = true;
bool notGetMultipleShot = true;
bool notGetInvincibility = true;
bool notGetFreeze = true;
bool notGetBigBang = true;
bool notGetShield = true;
bool notGetSlowDown = true;

bool freeze = false;
bool single = true;
bool invincible = false;
bool hasShield = false;
bool areSlowDown = false;
float slowDownFraction = 0.5;

//Variabile ce retin structura bonusurilor.
MultipleShot ms;
BonusLife bl;
Invincibility inv;
Freeze fr;
BigBang bb;
Shield sh;
SlowDown sd;

//Variabile ce retin obiectele jocului.
Ship ship;
std::vector<Missle> missles;
std::vector<OneLifeEnemy> simpleOpponents;
std::vector<TwoLifeEnemy> doubleLifeOpponents;
std::vector<DividingEnemy> dividingOpponents;

//Variabile referitoare la starea de ansamblu a jocului.
int oldScore = 0;
int opponentsKilled = 0;
int powerUpsUsed = 0;
bool gameRunning = true;
float endTime = 0;
float waitTimeToAttack = 2;
float lastEnemyEntry = 0;

/*Mesh-uri pentru desenatul numarului de vieti
si a scutului din jurul navei.*/
Mesh* lifeBar;
Mesh* shieldCircle;

//Variabile pentru memorarea pozitiei mouse-ului.
int txMouse = 0;
int tyMouse = 0;

//Functia returneaza viteza intre 10% si 85% din viteza navei jucator.
float Tema1::getNextOneLifeEnemySpeed() {
	int p = 10 + rand() % 75;
	return p * Ship::speed / 100;
}

/*Functia returneaza viteza intre 10% si 40% din viteza navei jucator.
(Viteza se va dubla cand inamicul va pierde o viata => [20%, 80%].)*/
float  Tema1::getNextTwoLifeEnemySpeed() {
	int p = 10 + rand() % 30;
	return p * Ship::speed / 100;
}

/*Functia returneaza viteza intre 10% si 60% din viteza navei jucator.
(Viteza se va creste cu un sfert cand inamicul se va divide  in doi
inamici mai mici => [12.5%, 75%].)*/
float  Tema1::getNextDividingEnemySpeed() {
	int p = 10 + rand() % 50;
	return p * Ship::speed / 100;
}

Tema1::Tema1(){}

Tema1::~Tema1(){}

void Tema1::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	//Setarea spatiului logic de joc.
	logicSpace.x = -50;		// logic x
	logicSpace.y = -50;		// logic y
	logicSpace.width = 100;	// logic width
	logicSpace.height = 100;	// logic height

	//Instantierea navei.
	ship = Ship(0, 0, Ship::sx, Ship::sy, 0, Ship::color, 3);
	ship.mesh = Object2D::CreateShip("ship", ship.color);

	//Timpul de start al jocului.
	globalStartTime = Engine::GetElapsedTime();
	
	//Instantierea obiectului pentru viata bonus.
	bl = BonusLife(logicSpace.x + rand() % (int)logicSpace.width, logicSpace.y + rand() % (int)logicSpace.height, BonusLife::sx, BonusLife::sy, rand() % 100, BonusLife::color);
	bl.mesh = Object2D::CreateBonusLife("bonusLife", BonusLife::color);

	//Instantierea obiectului pentru mai multe proiectile lansate simultan.
	ms = MultipleShot(logicSpace.x + rand() % (int)logicSpace.width, logicSpace.y + rand() % (int)logicSpace.height, MultipleShot::sx, MultipleShot::sy, rand() % 100, MultipleShot::color);
	ms.mesh = Object2D::CreateMultipleShot("multipleShot", MultipleShot::color);

	//Instantierea obiectului pentru invincibilitate.
	inv = Invincibility(logicSpace.x + rand() % (int)logicSpace.width, logicSpace.y + rand() % (int)logicSpace.height, Invincibility::sx, Invincibility::sy, rand() % 100, Invincibility::color);
	inv.mesh = Object2D::CreateInvincibility("invincibility", Invincibility::color);

	//Instantierea obiectului pentru inghetarea inamicilor.
	fr = Freeze(logicSpace.x + rand() % (int)logicSpace.width, logicSpace.y + rand() % (int)logicSpace.height, Freeze::sx, Freeze::sy, rand() % 100, Freeze::color);
	fr.mesh = Object2D::CreateSnowFlake("freeze", Freeze::color);

	//Instantierea mesh-ului pentru reprezentarea numarului de vieti.
	lifeBar = Object2D::CreateLifeBar("lifeBar", BonusLife::color);

	//Initializarea cercului ce va proteja nava.
	shieldCircle = Object2D::CreateCircle("shield", Shield::color);

	//Instantierea obiectului pentru distrugerea tuturor inamicilor.
	bb = BigBang(logicSpace.x + rand() % (int)logicSpace.width, logicSpace.y + rand() % (int)logicSpace.height, BigBang::sx, BigBang::sy, rand() % 100, BigBang::color);
	bb.mesh = Object2D::CreateBigBang("bigBang", BigBang::color);

	//Initializarea obiectului pentru obtinerea scutului.
	sh = Shield(logicSpace.x + rand() % (int)logicSpace.width, logicSpace.y + rand() % (int)logicSpace.height, Shield::sx, Shield::sy, 0, Shield::color);
	sh.mesh = Object2D::CreateShield("shieldSymbol", Shield::color);

	//Instantierea obiectului pentru incetinirea oponentilor.
	sd = SlowDown(logicSpace.x + rand() % (int)logicSpace.width, logicSpace.y + rand() % (int)logicSpace.height, SlowDown::sx, SlowDown::sy, 0, SlowDown::color);
	sd.mesh = Object2D::CreateSlowDown("slowDown", SlowDown::color);
}

// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void Tema1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema1::Update(float deltaTimeSeconds)
{

	glm::ivec2 resolution = window->GetResolution();

	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	if (gameRunning) {
		//Daca jocul nu s-a terminat background-ul este negru.
		SetViewportArea(viewSpace, glm::vec3(0), true);
	}
	else {
		/*Daca jocul s-a terminat, se determina daca se face tranzitia
		spre rosu a background-ului sau daca acesta este complet rosu.*/
		double thisMoment = Engine::GetElapsedTime();
		if (thisMoment - endTime > 2) {
			SetViewportArea(viewSpace, glm::vec3(1, 0, 0), true);
		}
		else {
			SetViewportArea(viewSpace, glm::vec3((thisMoment - endTime) / 2, 0, 0), true);
		}
	}
	// Compute uniform 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
	DrawScene(visMatrix);
}

void Tema1::FrameEnd(){}

/*Functia sterge din vectorul corespunzator oponentii simpli
ce au fost distrusi.*/
void Tema1::deleteSimpleDeadOpponents() {
	int counter = 0;
	while (counter < simpleOpponents.size()) {
		if (simpleOpponents[counter].lifes == 0) {
			simpleOpponents.erase(simpleOpponents.begin() + counter);
		}
		else {
			counter++;
		}
	}
}

/*Functia sterge oponenti ce avea doua vieti si au fost 
distrusi din vectorul corespunzator.*/
void Tema1::deleteDoubleLifeDeadOpponents() {
	int counter = 0;
	while (counter < doubleLifeOpponents.size()) {
		if (doubleLifeOpponents[counter].lifes == 0) {
			doubleLifeOpponents.erase(doubleLifeOpponents.begin() + counter);
		}
		else {
			counter++;
		}
	}
}

//Functia sterge oponenti ce se divid fara vieti din vectorul corespunzator.
void Tema1::deleteDividingDeadOpponents() {
	int counter = 0;
	while (counter < dividingOpponents.size()) {
		if (dividingOpponents[counter].lifes == 0) {
			dividingOpponents.erase(dividingOpponents.begin() + counter);
		}
		else {
			counter++;
		}
	}
}

/*Functia are rolul de a genera noi oponenti din diferite categorii.*/
void Tema1::generateOpponents(float thisMoment) {
	//Se verifica daca se poate crea un nou inamic.
	if (thisMoment - lastEnemyEntry > waitTimeToAttack) {
		/*Se scade timpul dintre sosirea inamicilor, crescand astfel
		dificultatea jocului (pana la un prag).*/
		if (waitTimeToAttack > 0.5) waitTimeToAttack -= 0.1;
		/*Se alege un unghi random fata de centru spatiului de joc,
		iar distanta se alege suficient de mare pentru ca inamicii
		sa "soseasca" din afara ecranului.*/
		int choice = rand() % 3;
		float alfa = rand() % 360;
		float cos = glm::cos(alfa * TO_RADIANS);
		float sin = glm::sin(alfa * TO_RADIANS);
		switch (choice) {
		case 0:
		{
			OneLifeEnemy ole = OneLifeEnemy(ship.tx + 1.5 * logicSpace.width * cos, ship.ty + 1.5 * logicSpace.height * sin, OneLifeEnemy::sx, OneLifeEnemy::sy, ship.c, ship.s, Tan, getNextOneLifeEnemySpeed());
			ole.mesh = Object2D::CreateShip("oneLifeEnemy", ole.color);
			simpleOpponents.push_back(ole);
		}
		break;

		case 1:
		{
			TwoLifeEnemy tfe = TwoLifeEnemy(ship.tx + 1.5 * logicSpace.width * cos, ship.ty + 1.5 *logicSpace.height * sin, TwoLifeEnemy::sxTwoLifes, TwoLifeEnemy::syTwoLifes, ship.c, ship.s, TwoLifeEnemy::colorTwoLifes, getNextTwoLifeEnemySpeed());
			tfe.mesh = Object2D::CreateShip("twoLifeEnemy", TwoLifeEnemy::colorTwoLifes);
			doubleLifeOpponents.push_back(tfe);
		}
		break;

		case 2:
		{
			DividingEnemy de = DividingEnemy(ship.tx + 1.5 * logicSpace.width * cos, ship.ty + 1.5 *logicSpace.height * sin, DividingEnemy::sx, DividingEnemy::sy, ship.c, ship.s, Cyan, getNextDividingEnemySpeed());
			de.mesh = Object2D::CreateShip("dividingEnemy", DividingEnemy::color);
			dividingOpponents.push_back(de);
		}
		break;
		}

		lastEnemyEntry = thisMoment;
	}
}

//Functia implementeaza rotirea navei dupa mouse.
void Tema1::rotateShip(glm::mat3 visMatrix) {
	float shiptx = (visMatrix*glm::vec3(ship.tx, -ship.ty, 1)).x;
	float shipty = (visMatrix*glm::vec3(ship.tx, -ship.ty, 1)).y;
	float deltaX = shiptx - txMouse;
	float deltaY = shipty - tyMouse;

	ship.c = -deltaX / glm::sqrt(deltaX * deltaX + deltaY * deltaY);
	ship.s = deltaY / glm::sqrt(deltaX * deltaX + deltaY * deltaY);
}

//Functia deseneaza nava jucator.
void Tema1::drawShip(glm::mat3 visMatrix) {
	if (invincible) {
		modelMatrix = visMatrix *  Transform2D::Translate(ship.tx, ship.ty) * Transform2D::Rotate(ship.c, ship.s) * Transform2D::Scale(ship.sx, ship.sy);
		RenderMesh2D(ship.mesh, modelMatrix, ship.invincibleColor);
	}
	else {
		modelMatrix = visMatrix *  Transform2D::Translate(ship.tx, ship.ty) * Transform2D::Rotate(ship.c, ship.s) * Transform2D::Scale(ship.sx, ship.sy);
		RenderMesh2D(ship.mesh, modelMatrix, ship.color);
	}
	if (hasShield) {
		modelMatrix = visMatrix * Transform2D::Translate(ship.tx, ship.ty);
		RenderMesh2D(shieldCircle, modelMatrix, Shield::color);
	}
}

//Functia realizeaza desenarea proiectilelor pe ecran.
void Tema1::drawMissles(glm::mat3 visMatrix) {
	for (int i = 0; i < missles.size(); i++) {
		modelMatrix = visMatrix * Transform2D::Translate(missles[i].tx, missles[i].ty) * Transform2D::Rotate(missles[i].c, missles[i].s) * Transform2D::Scale(Missle::sx, Missle::sy);
		RenderMesh2D(missles[i].mesh, modelMatrix, missles[i].color);
	}
}

//Functia calculeaza pozitia la urmatorul moment de timp a proiectilelor.
void Tema1::moveMissles() {
	for (int i = 0; i < missles.size(); i++) {
		missles[i].tx += Missle::speed*missles[i].c;
		missles[i].ty += Missle::speed*missles[i].s;
	}
}

//Functia realizeaza eliminarea proiectilelor ce au depasit zona de joc.
void Tema1::deleteFarMissles() {
	int counter = 0;
	while (counter < missles.size()) {
		if (missles[counter].tx < logicSpace.x - logicSpace.width ||
			missles[counter].tx > logicSpace.x + 2 * logicSpace.width ||
			missles[counter].ty < logicSpace.y - logicSpace.height ||
			missles[counter].ty > logicSpace.y + 2 * logicSpace.height) {
			missles.erase(missles.begin() + counter);
		}
		else {
			counter++;
		}
	}
}

/*Functia determina coliziunile dintre oponenti si proiectilele lansate de nava jucator
si modifica starea jocului in mod corespunzator.*/
void Tema1::computeMisslesOpponentsCollisions(float thisMoment) {
	/*Vector ce retine indicele proiectilelor care
	au distrus nave inamice.*/
	std::vector<int> deadMissles;

	for (int i = 0; i < missles.size(); i++) {
		/*Se verifica pe rand coliziunea unui proiectil cu un oponent simplu, cu doua vieti si
		cu oponenti ce se divid. Daca un proiectil a atins un oponent nu se mai ia in calcul
		alta coliziune.*/
		bool missleHit = false;
		for (int j = 0; j < simpleOpponents.size(); j++) {
			float sumOfRadius = OneLifeEnemy::radius* ((OneLifeEnemy::sx + OneLifeEnemy::sy) / 2) + Missle::radius * ((Missle::sx + Missle::sy) / 2);
			float distance = glm::sqrt(glm::pow(simpleOpponents[j].tx - missles[i].tx, 2) + glm::pow(simpleOpponents[j].ty - missles[i].ty, 2));
			if (sumOfRadius >= distance && !missleHit) {
				missleHit = true;
				simpleOpponents[j].lifes--;
				opponentsKilled++;
				deadMissles.push_back(i);
			}
		}
		if (!missleHit) {
			for (int j = 0; j < doubleLifeOpponents.size(); j++) {
				float sx = doubleLifeOpponents[j].lifes == 2 ? TwoLifeEnemy::sxTwoLifes : TwoLifeEnemy::sxOneLife;
				float sy = doubleLifeOpponents[j].lifes == 2 ? TwoLifeEnemy::syTwoLifes : TwoLifeEnemy::syOneLife;
				float sumOfRadius = TwoLifeEnemy::radius* ((sx + sy) / 2) + Missle::radius * ((Missle::sx + Missle::sy) / 2);
				float distance = glm::sqrt(glm::pow(doubleLifeOpponents[j].tx - missles[i].tx, 2) + glm::pow(doubleLifeOpponents[j].ty - missles[i].ty, 2));
				if (sumOfRadius >= distance && !missleHit) {
					missleHit = true;
					opponentsKilled++;
					doubleLifeOpponents[j].lifes--;
					/*Se inregistreaza timpul de start pentru animatia de micsorare,
					accelerare si schimbare de culoare.*/
					doubleLifeOpponents[j].startShrinkAnimationTime = thisMoment;
					deadMissles.push_back(i);
				}
			}
		}
		if (!missleHit) {
			for (int j = 0; j < dividingOpponents.size(); j++) {
				float sx = DividingEnemy::sx;
				float sy = DividingEnemy::sy;
				float sumOfRadius = DividingEnemy::radius* ((sx + sy) / 2) + Missle::radius * ((Missle::sx + Missle::sy) / 2);
				float distance = glm::sqrt(glm::pow(dividingOpponents[j].tx - missles[i].tx, 2) + glm::pow(dividingOpponents[j].ty - missles[i].ty, 2));
				if (sumOfRadius >= distance && !missleHit) {
					missleHit = true;
					opponentsKilled++;
					dividingOpponents[j].lifes--;

					/*Se creeaza doi inamici mai mici (cu o viata) in vecinatatea locului unde a avut loc coliziunea 
					folosind alta culoare si o viteza cu 25% mai mare pentru noile nave.*/
					float alpha = rand() % 360;
					alpha = alpha * TO_RADIANS;
					float beta = alpha + glm::pi<float>();

					OneLifeEnemy ole = OneLifeEnemy(dividingOpponents[j].tx + DividingEnemy::radius * DividingEnemy::sx * glm::cos(alpha),
						dividingOpponents[j].ty + DividingEnemy::radius * DividingEnemy::sy * glm::sin(alpha), OneLifeEnemy::sx, OneLifeEnemy::sy, ship.c, ship.s, Magenta, dividingOpponents[j].speed * 1.25);
					ole.mesh = Object2D::CreateShip("oneLifeEnemy", ole.color);
					simpleOpponents.push_back(ole);

					ole = OneLifeEnemy(dividingOpponents[j].tx + DividingEnemy::radius * DividingEnemy::sx * 2 * glm::cos(beta),
						dividingOpponents[j].ty + DividingEnemy::radius * DividingEnemy::sy * 2 * glm::sin(beta), OneLifeEnemy::sx, OneLifeEnemy::sy, ship.c, ship.s, Magenta, dividingOpponents[j].speed * 1.25);
					ole.mesh = Object2D::CreateShip("oneLifeEnemy", ole.color);
					simpleOpponents.push_back(ole);

					deadMissles.push_back(i);
				}
			}
		}
	}

	//Se sterg proiectilele care au lovit oponenti.
	for (int i = 0; i < deadMissles.size(); i++) {
		missles.erase(missles.begin() + i);
	}

	//Se sterg oponentii distrusi.		
	deleteSimpleDeadOpponents();
	deleteDoubleLifeDeadOpponents();
	deleteDividingDeadOpponents();
}

//Functia realizeaza reorientarea oponentilor simpli dupa nava.
void Tema1::rotateSimpleOpponents() {
	for (int i = 0; i < simpleOpponents.size(); i++) {
		float dx = ship.tx - simpleOpponents[i].tx;
		float dy = ship.ty - simpleOpponents[i].ty;

		float cos = dx / glm::sqrt(dx*dx + dy*dy);
		float sin = dy / glm::sqrt(dx*dx + dy*dy);

		simpleOpponents[i].c = cos;
		simpleOpponents[i].s = sin;
	}
}

//Functia realizeaza reorientarea oponentilor cu doua vieti dupa nava.
void Tema1::rotateDoubleLifeOpponents() {
	for (int i = 0; i < doubleLifeOpponents.size(); i++) {
		float dx = ship.tx - doubleLifeOpponents[i].tx;
		float dy = ship.ty - doubleLifeOpponents[i].ty;

		float cos = dx / glm::sqrt(dx*dx + dy*dy);
		float sin = dy / glm::sqrt(dx*dx + dy*dy);

		doubleLifeOpponents[i].c = cos;
		doubleLifeOpponents[i].s = sin;
	}
}

//Functia realizeaza reorientarea oponentilor ce se divid dupa nava.
void Tema1::rotateDividingOpponents() {
	for (int i = 0; i < dividingOpponents.size(); i++) {
		float dx = ship.tx - dividingOpponents[i].tx;
		float dy = ship.ty - dividingOpponents[i].ty;

		float cos = dx / glm::sqrt(dx*dx + dy*dy);
		float sin = dy / glm::sqrt(dx*dx + dy*dy);

		dividingOpponents[i].c = cos;
		dividingOpponents[i].s = sin;
	}
}

//Functia realizeaza desenarea oponentilor simpli pe ecran.
void Tema1::drawSimpleOpponents(glm::mat3 visMatrix) {
	for (int i = 0; i < simpleOpponents.size(); i++) {
		modelMatrix = visMatrix * Transform2D::Translate(simpleOpponents[i].tx, simpleOpponents[i].ty)
			* Transform2D::Rotate(simpleOpponents[i].c, simpleOpponents[i].s)
			* Transform2D::Scale(OneLifeEnemy::sx, OneLifeEnemy::sy);
		RenderMesh2D(simpleOpponents[i].mesh, modelMatrix, simpleOpponents[i].color);
	}
}

//Functia realizeaza desenarea oponentilor ce se multiplica.
void Tema1::drawDividingOpponents(glm::mat3 visMatrix) {
	for (int i = 0; i < dividingOpponents.size(); i++) {
		modelMatrix = visMatrix * Transform2D::Translate(dividingOpponents[i].tx, dividingOpponents[i].ty)
			* Transform2D::Rotate(dividingOpponents[i].c, dividingOpponents[i].s)
			* Transform2D::Scale(DividingEnemy::sx, DividingEnemy::sy);
		RenderMesh2D(dividingOpponents[i].mesh, modelMatrix, DividingEnemy::color);
	}
}

//Functia calculeaza urmatoarea pozitie a oponentilor simpli.
void Tema1::moveSimpleOpponents() {
	float fraction = 1;
	if (areSlowDown) fraction = slowDownFraction;
	for (int i = 0; i < simpleOpponents.size(); i++) {
		simpleOpponents[i].tx += fraction*simpleOpponents[i].speed*simpleOpponents[i].c;
		simpleOpponents[i].ty += fraction*simpleOpponents[i].speed*simpleOpponents[i].s;
	}
}

//Functia calculeaza urmatoarea pozitie a oponentilor ce creeaza alti oponenti.
void Tema1::moveDividingOpponents() {
	float fraction = 1;
	if (areSlowDown) fraction = slowDownFraction;
	for (int i = 0; i < dividingOpponents.size(); i++) {
		dividingOpponents[i].tx += fraction*dividingOpponents[i].speed*dividingOpponents[i].c;
		dividingOpponents[i].ty += fraction*dividingOpponents[i].speed*dividingOpponents[i].s;
	}
}

//Functia realizeaza desenarea oponentilor cu doua vieti pe ecran.
void Tema1::drawDoubleLifeOpponents(glm::mat3 visMatrix, float thisMoment) {
	for (int i = 0; i < doubleLifeOpponents.size(); i++) {
		if (doubleLifeOpponents[i].lifes == 2) {
			/*Daca oponentul are doua vieti se cunoaste culoarea si
			scalarea acestuia si se poate desena direct.*/
			modelMatrix = visMatrix * Transform2D::Translate(doubleLifeOpponents[i].tx, doubleLifeOpponents[i].ty)
				* Transform2D::Rotate(doubleLifeOpponents[i].c, doubleLifeOpponents[i].s)
				* Transform2D::Scale(TwoLifeEnemy::sxTwoLifes, TwoLifeEnemy::syTwoLifes);
			RenderMesh2D(doubleLifeOpponents[i].mesh, modelMatrix, TwoLifeEnemy::colorTwoLifes);
		}
		else {
			/*Daca oponentul mai are doar o viata, trebuie sa se verifice daca s-a terminat
			sau nu animatia prin care accelereaza, se micsoreaza si isi schimba culoarea.*/
			float auxSX = TwoLifeEnemy::sxOneLife;
			float auxSY = TwoLifeEnemy::syOneLife;

			glm::vec3 auxColor = TwoLifeEnemy::colorOneLife;

			if (thisMoment - doubleLifeOpponents[i].startShrinkAnimationTime < TwoLifeEnemy::shrinkTime) {
				float fraction = (thisMoment - doubleLifeOpponents[i].startShrinkAnimationTime) / TwoLifeEnemy::shrinkTime;
				auxSX = TwoLifeEnemy::sxTwoLifes - (TwoLifeEnemy::sxTwoLifes - TwoLifeEnemy::sxOneLife) * fraction;
				auxSY = TwoLifeEnemy::syTwoLifes - (TwoLifeEnemy::syTwoLifes - TwoLifeEnemy::syOneLife) * fraction;

				auxColor = TwoLifeEnemy::colorTwoLifes - (TwoLifeEnemy::colorTwoLifes - TwoLifeEnemy::colorOneLife) * fraction;
			}

			modelMatrix = visMatrix * Transform2D::Translate(doubleLifeOpponents[i].tx, doubleLifeOpponents[i].ty)
				* Transform2D::Rotate(doubleLifeOpponents[i].c, doubleLifeOpponents[i].s)
				* Transform2D::Scale(auxSX, auxSY);
			RenderMesh2D(doubleLifeOpponents[i].mesh, modelMatrix, auxColor);
		}
	}
}

//Functia calculeaza urmatoarea pozitie a oponentilor cu doua vieti.
void Tema1::moveDoubleLifeOpponents(float thisMoment) {
	float f = 1;
	if (areSlowDown) f = slowDownFraction;
	for (int i = 0; i < doubleLifeOpponents.size(); i++) {
		if (doubleLifeOpponents[i].lifes == 2) {
			/*Daca oponentul are doua vieti se cunoaste viteza acestuia si
			se poate deduce pozitia la urmatorul moment de timp.*/
			doubleLifeOpponents[i].tx += f*doubleLifeOpponents[i].speedTwoLifes*doubleLifeOpponents[i].c;
			doubleLifeOpponents[i].ty += f*doubleLifeOpponents[i].speedTwoLifes*doubleLifeOpponents[i].s;
		}
		else {
			/*Daca oponentul mai are doar o viata, trebuie sa se verifice daca s-a terminat
			sau nu animatia prin care accelereaza, se micsoreaza si isi schimba culoarea.*/
			float fraction = 1;
			if (thisMoment - doubleLifeOpponents[i].startShrinkAnimationTime < TwoLifeEnemy::shrinkTime) {
				fraction = (thisMoment - doubleLifeOpponents[i].startShrinkAnimationTime) / TwoLifeEnemy::shrinkTime;
			}
			float speed = doubleLifeOpponents[i].speedTwoLifes + (doubleLifeOpponents[i].speedOneLife - doubleLifeOpponents[i].speedTwoLifes) * fraction;
			doubleLifeOpponents[i].tx += f*speed*doubleLifeOpponents[i].c;
			doubleLifeOpponents[i].ty += f*speed*doubleLifeOpponents[i].s;
		}
	}
}

/*Functia determina coliziunile dintre oponenti si nava jucator si modifica
starea jocului in mod corespunzator.*/
void Tema1::computePlayerOpponentsCollisions(float thisMoment) {
	if (!invincible) {
		/*In cazul lipsei invincibilitatii, jucatorul este cel care pierde
		vieti si este distrus. Se calculeaza coliziunile cu fiecare tip de oponenti:
		cu o viata, cu doua vieti si cei care se despart in doi oponenti mici.
		Daca nava ramane fara vieti se semnaleaza sfarsitul jocului.*/
		for (int i = 0; i < simpleOpponents.size(); i++) {
			float sumOfRadius = OneLifeEnemy::radius* ((OneLifeEnemy::sx + OneLifeEnemy::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
			float distance = glm::sqrt(glm::pow(simpleOpponents[i].tx - ship.tx, 2) + glm::pow(simpleOpponents[i].ty - ship.ty, 2));
			if (sumOfRadius >= distance) {
				simpleOpponents[i].lifes = 0;
				ship.lifes--;

				if (hasShield) {
					hasShield = false;
					opponentsKilled++;
					ship.lifes++;
				}

				if (ship.lifes == 0) {
					gameRunning = false;
					endTime = thisMoment;
				}
			}
		}
		for (int i = 0; i < doubleLifeOpponents.size(); i++) {
			float sx = doubleLifeOpponents[i].lifes == 2 ? TwoLifeEnemy::sxTwoLifes : TwoLifeEnemy::sxOneLife;
			float sy = doubleLifeOpponents[i].lifes == 2 ? TwoLifeEnemy::syTwoLifes : TwoLifeEnemy::syOneLife;
			float sumOfRadius = TwoLifeEnemy::radius* ((sx + sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
			float distance = glm::sqrt(glm::pow(doubleLifeOpponents[i].tx - ship.tx, 2) + glm::pow(doubleLifeOpponents[i].ty - ship.ty, 2));
			if (sumOfRadius >= distance) {
				doubleLifeOpponents[i].lifes = 0;
				ship.lifes--;

				if (hasShield) {
					hasShield = false;
					opponentsKilled++;
					ship.lifes++;
				}

				if (ship.lifes == 0) {
					gameRunning = false;
					endTime = thisMoment;
				}
			}
		}
		for (int i = 0; i < dividingOpponents.size(); i++) {
			float sumOfRadius = DividingEnemy::radius* ((DividingEnemy::sx + DividingEnemy::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
			float distance = glm::sqrt(glm::pow(dividingOpponents[i].tx - ship.tx, 2) + glm::pow(dividingOpponents[i].ty - ship.ty, 2));
			if (sumOfRadius >= distance) {
				dividingOpponents[i].lifes = 0;
				ship.lifes--;

				if (hasShield) {
					hasShield = false;
					opponentsKilled++;
					ship.lifes++;
				}

				if (ship.lifes == 0) {
					gameRunning = false;
					endTime = thisMoment;
				}
			}
		}
	}
	else {
		/*In cazul prezentei invincibilitatii, oponentii sunt cei care pierd
		vieti si sunt distrusi. Se calculeaza coliziunile cu fiecare tip de oponenti:
		cu o viata, cu doua vieti si cei care se despart in doi oponenti mici.*/
		for (int i = 0; i < simpleOpponents.size(); i++) {
			float sumOfRadius = OneLifeEnemy::radius* ((OneLifeEnemy::sx + OneLifeEnemy::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
			float distance = glm::sqrt(glm::pow(simpleOpponents[i].tx - ship.tx, 2) + glm::pow(simpleOpponents[i].ty - ship.ty, 2));
			if (sumOfRadius >= distance) {
				simpleOpponents[i].lifes = 0;
				opponentsKilled++;
			}
		}
		for (int i = 0; i < doubleLifeOpponents.size(); i++) {
			float sx = doubleLifeOpponents[i].lifes == 2 ? TwoLifeEnemy::sxTwoLifes : TwoLifeEnemy::sxOneLife;
			float sy = doubleLifeOpponents[i].lifes == 2 ? TwoLifeEnemy::syTwoLifes : TwoLifeEnemy::syOneLife;
			float sumOfRadius = TwoLifeEnemy::radius* ((sx + sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
			float distance = glm::sqrt(glm::pow(doubleLifeOpponents[i].tx - ship.tx, 2) + glm::pow(doubleLifeOpponents[i].ty - ship.ty, 2));
			if (sumOfRadius >= distance) {
				doubleLifeOpponents[i].lifes = 0;
				opponentsKilled++;
			}
		}
		for (int i = 0; i < dividingOpponents.size(); i++) {
			float sumOfRadius = DividingEnemy::radius* ((DividingEnemy::sx + DividingEnemy::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
			float distance = glm::sqrt(glm::pow(dividingOpponents[i].tx - ship.tx, 2) + glm::pow(dividingOpponents[i].ty - ship.ty, 2));
			if (sumOfRadius >= distance) {
				dividingOpponents[i].lifes = 0;
				opponentsKilled++;
			}
		}
	}

	//Se elimina oponentii distrusi in aceasta iteratie.
	deleteSimpleDeadOpponents();
	deleteDoubleLifeDeadOpponents();
	deleteDividingDeadOpponents();
}

/*Functia implementeaza logica referitoare la bonusul ce imobilizeaza
oponenti pentru un timp limitat, lasand nava jucator sa obtina un avantaj.*/
void Tema1::computeFreezeLogic(glm::mat3 visMatrix, float thisMoment) {

	/*Se verifica daca a trecut suficient timp de la ultima aparitie
	a acestui power-up. Daca da, i se atribuie o locatie random.*/
	if (thisMoment - lastFreeze > bonusTimeAppearanceFreeze && opponentsKilled % 6 == 0) {
		lastFreeze = thisMoment;
		fr.r = rand() % 100;
		fr.tx = logicSpace.x + rand() % (int)logicSpace.width;
		fr.ty = logicSpace.y + rand() % (int)logicSpace.height;
		notGetFreeze = true;
	}

	/*Daca timpul de obtinere a bonusului nu a trecut si acesta nu a
	fost cules, se verifica daca nava se intersecteaza cu acesta. Daca
	nava obtine bonusul, atunci se ingheata oponentii.*/
	if (thisMoment - lastFreeze < bonusTimeGet && notGetFreeze) {
		modelMatrix = visMatrix *  Transform2D::Translate(fr.tx, fr.ty) * Transform2D::Rotate(fr.r) * Transform2D::Scale(Freeze::sx, Freeze::sy);
		RenderMesh2D(fr.mesh, modelMatrix, Freeze::color);
		float sumOfRadius = Freeze::radius* ((Freeze::sx + Freeze::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
		float distance = glm::sqrt(glm::pow(ship.tx - fr.tx, 2) + glm::pow(ship.ty - fr.ty, 2));
		if (sumOfRadius >= distance) {
			freeze = true;
			notGetFreeze = false;
			grabedFreeze = thisMoment;
			powerUpsUsed++;
		}
	}
	else {
		notGetFreeze = false;
	}

	//Daca timpul de valabilitate a expirat, atunci bonusul este dezactivat.
	if (thisMoment - grabedFreeze > bonusTimeAvability) {
		freeze = false;
	}
}


/*Functia implementeaza logica referitoare la bonusul ce activeaza un
timp de invincibilitate pentru nava. In acest timp nava poate distruge
oponenti direct prin coliziuni cu ei.*/
void Tema1::computeInvincibilityLogic(glm::mat3 visMatrix, float thisMoment) {

	/*Se verifica daca a trecut suficient timp de la ultima aparitie
	a acestui power-up. Daca da, i se atribuie o locatie random.*/
	if (thisMoment - lastInvincibility > bonusTimeAppearanceInvincibility) {
		lastInvincibility = thisMoment;
		inv.r = rand() % 100;
		inv.tx = logicSpace.x + rand() % (int)logicSpace.width;
		inv.ty = logicSpace.y + rand() % (int)logicSpace.height;
		notGetInvincibility = true;
	}

	/*Daca timpul de obtinere a bonusului nu a trecut si acesta nu a
	fost cules, se verifica daca nava se intersecteaza cu acesta. Daca
	nava obtine bonusul, atunci se activeaza imunitatea pentru un timp limitat.*/
	if (thisMoment - lastInvincibility < bonusTimeGet && notGetInvincibility) {
		modelMatrix = visMatrix *  Transform2D::Translate(inv.tx, inv.ty) * Transform2D::Rotate(inv.r) * Transform2D::Scale(Invincibility::sx, Invincibility::sy);
		RenderMesh2D(inv.mesh, modelMatrix, Invincibility::color);
		float sumOfRadius = Invincibility::radius* ((Invincibility::sx + Invincibility::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
		float distance = glm::sqrt(glm::pow(ship.tx - inv.tx, 2) + glm::pow(ship.ty - inv.ty, 2));
		if (sumOfRadius >= distance) {
			invincible = true;
			notGetInvincibility = false;
			grabedInvincibility = thisMoment;
			powerUpsUsed++;
		}
	}
	else {
		notGetInvincibility = false;
	}

	//Daca timpul de valabilitate a expirat, atunci bonusul este dezactivat.
	if (thisMoment - grabedInvincibility > bonusTimeAvability) {
		invincible = false;
	}
}

/*Functia implementeaza logica referitoare la bonusul ce activeaza
4 tunuri ce vor trage simultan.*/
void Tema1::computeMultiShotLogic(glm::mat3 visMatrix, float thisMoment) {

	/*Se verifica daca a trecut suficient timp de la ultima aparitie
	a acestui power-up. Daca da, i se atribuie o locatie random.*/
	if (thisMoment - lastMultipleShot > bonusTimeAppearanceMultiShot) {
		lastMultipleShot = thisMoment;
		ms.r = rand() % 100;
		ms.tx = logicSpace.x + rand() % (int)logicSpace.width;
		ms.ty = logicSpace.y + rand() % (int)logicSpace.height;
		notGetMultipleShot = true;
	}

	/*Daca timpul de obtinere a bonusului nu a trecut si acesta nu a
	fost cules, se verifica daca nava se intersecteaza cu acesta.
	Daca nava obtine bonusul, atunci va trage cate 4 proiectile deodata.*/
	if (thisMoment - lastMultipleShot < bonusTimeGet && notGetMultipleShot) {
		modelMatrix = visMatrix *  Transform2D::Translate(ms.tx, ms.ty) * Transform2D::Rotate(ms.r) * Transform2D::Scale(ms.sx, ms.sy);
		RenderMesh2D(ms.mesh, modelMatrix, MultipleShot::color);
		float sumOfRadius = MultipleShot::radius* ((MultipleShot::sx + MultipleShot::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
		float distance = glm::sqrt(glm::pow(ship.tx - ms.tx, 2) + glm::pow(ship.ty - ms.ty, 2));
		if (sumOfRadius >= distance) {
			single = false;
			notGetMultipleShot = false;
			grabedMultipleShot = thisMoment;
			powerUpsUsed++;
		}
	}
	else {
		notGetMultipleShot = false;
	}

	//Daca timpul de valabilitate a expirat, atunci bonusul este dezactivat.
	if (thisMoment - grabedMultipleShot > bonusTimeAvability) {
		single = true;
	}
}

/*Functia implementeaza logica referitoare la bonusul de obtinere
a unei vieti suplimentare.*/
void Tema1::computeBonusLifeLogic(glm::mat3 visMatrix, float thisMoment) {

	/*Se verifica daca a trecut suficient timp de la ultima aparitie
	a acestui power-up. Daca da, i se atribuie o locatie random.*/
	if (thisMoment - lastBonusLife > bonusTimeAppearanceBonusLife) {
		lastBonusLife = thisMoment;
		bl.r = rand() % 100;
		bl.tx = logicSpace.x + rand() % (int)logicSpace.width;
		bl.ty = logicSpace.y + rand() % (int)logicSpace.height;
		notGetBonusLife = true;
	}

	/*Daca timpul de obtinere a bonusului nu a trecut si acesta nu a
	fost cules, se verifica daca nava se intersecteaza cu acesta.
	Daca nava obtine bonusul, atunci se incrementeaza numarul de vieti.*/
	if (thisMoment - lastBonusLife < bonusTimeGet && notGetBonusLife) {
		modelMatrix = visMatrix *  Transform2D::Translate(bl.tx, bl.ty) * Transform2D::Rotate(bl.r) * Transform2D::Scale(BonusLife::sx, BonusLife::sy);
		RenderMesh2D(bl.mesh, modelMatrix, BonusLife::color);
		float sumOfRadius = BonusLife::radius* ((BonusLife::sx + BonusLife::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
		float distance = glm::sqrt(glm::pow(ship.tx - bl.tx, 2) + glm::pow(ship.ty - bl.ty, 2));
		if (sumOfRadius >= distance) {
			ship.lifes++;
			notGetBonusLife = false;
			powerUpsUsed++;
		}
	}
	else {
		notGetBonusLife = false;
	}
}

/*Functia implementeaza logica referitoare la bonusul ce distruge toti
oponenti prezenti in jocul la momentul cand este obtinut.*/
void Tema1::computeBigBangLogic(glm::mat3 visMatrix, float thisMoment) {

	/*Se verifica daca a trecut suficient timp de la ultima aparitie
	a acestui power-up. Daca da, i se atribuie o locatie random.*/
	if (thisMoment - lastBigBang > bonusTimeAppearanceBigBang) {
		lastBigBang = thisMoment;
		bb.r = rand() % 100;
		bb.tx = logicSpace.x + rand() % (int)logicSpace.width;
		bb.ty = logicSpace.y + rand() % (int)logicSpace.height;
		notGetBigBang = true;
	}

	/*Daca timpul de obtinere a bonusului nu a trecut si acesta nu a
	fost cules, se verifica daca nava se intersecteaza cu acesta.
	Daca nava obtine bonusul, atunci se sterg toti oponenti prezenti in
	joc in acest moment.*/
	if (thisMoment - lastBigBang < bonusTimeGet && notGetBigBang) {
		modelMatrix = visMatrix *  Transform2D::Translate(bb.tx, bb.ty) * Transform2D::Rotate(bb.r) * Transform2D::Scale(BigBang::sx, BigBang::sy);
		RenderMesh2D(bb.mesh, modelMatrix, BigBang::color);
		float sumOfRadius = BigBang::radius* ((BigBang::sx + BigBang::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
		float distance = glm::sqrt(glm::pow(ship.tx - bb.tx, 2) + glm::pow(ship.ty - bb.ty, 2));
		if (sumOfRadius >= distance) {
			opponentsKilled += simpleOpponents.size() + doubleLifeOpponents.size() + dividingOpponents.size();
			simpleOpponents.clear();
			doubleLifeOpponents.clear();
			dividingOpponents.clear();
			notGetBigBang = false;
			powerUpsUsed++;
		}
	}
	else {
		notGetBigBang = false;
	}
}

/*Functia implementeaza logica referitoare la bonusul de obtinere
a unui scut ce rezista la o coliziune cu un inamic.*/
void Tema1::computeShieldLogic(glm::mat3 visMatrix, float thisMoment) {

	/*Se verifica daca a trecut suficient timp de la ultima aparitie
	a acestui power-up. Daca da, i se atribuie o locatie random.*/
	if (thisMoment - lastShield > bonusTimeAppearanceShield) {
		lastShield = thisMoment;
		sh.tx = logicSpace.x + rand() % (int)logicSpace.width;
		sh.ty = logicSpace.y + rand() % (int)logicSpace.height;
		notGetShield = true;
	}

	/*Daca timpul de obtinere a bonusului nu a trecut si acesta nu a
	fost cules, se verifica daca nava se intersecteaza cu acesta.
	Daca nava obtine bonusul, atunci se activeaza scutul.*/
	if (thisMoment - lastShield < bonusTimeGet && notGetShield) {
		modelMatrix = visMatrix *  Transform2D::Translate(sh.tx, sh.ty) * Transform2D::Rotate(sh.r) * Transform2D::Scale(Shield::sx, Shield::sy);
		RenderMesh2D(sh.mesh, modelMatrix, Shield::color);
		float sumOfRadius = Shield::radius* ((Shield::sx + Shield::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
		float distance = glm::sqrt(glm::pow(ship.tx - sh.tx, 2) + glm::pow(ship.ty - sh.ty, 2));
		if (sumOfRadius >= distance) {
			hasShield = true;
			notGetShield = false;
			powerUpsUsed++;
		}
	}
	else {
		notGetShield = false;
	}
}

/*Functia implementeaza logica referitoare la bonusul ce activeaza
incetinirea inamicilor.*/
void Tema1::computeSlowDownLogic(glm::mat3 visMatrix, float thisMoment) {

	/*Se verifica daca a trecut suficient timp de la ultima aparitie
	a acestui power-up. Daca da, i se atribuie o locatie random.*/
	if (thisMoment - lastSlowDown > bonusTimeAppearanceSlowDown) {
		lastSlowDown = thisMoment;
		sd.tx = logicSpace.x + rand() % (int)logicSpace.width;
		sd.ty = logicSpace.y + rand() % (int)logicSpace.height;
		notGetSlowDown = true;
	}

	/*Daca timpul de obtinere a bonusului nu a trecut si acesta nu a
	fost cules, se verifica daca nava se intersecteaza cu acesta.
	Daca nava obtine bonusul, atunci inamici vor fi incetiniti.*/
	if (thisMoment - lastSlowDown < bonusTimeGet && notGetSlowDown) {
		modelMatrix = visMatrix *  Transform2D::Translate(sd.tx, sd.ty) * Transform2D::Rotate(sd.r) * Transform2D::Scale(sd.sx, sd.sy);
		RenderMesh2D(sd.mesh, modelMatrix, SlowDown::color);
		float sumOfRadius = SlowDown::radius* ((SlowDown::sx + SlowDown::sy) / 2) + Ship::radius * ((Ship::sx + Ship::sy) / 2);
		float distance = glm::sqrt(glm::pow(ship.tx - sd.tx, 2) + glm::pow(ship.ty - sd.ty, 2));
		if (sumOfRadius >= distance) {
			areSlowDown = true;
			notGetSlowDown = false;
			grabedSlowDown = thisMoment;
			powerUpsUsed++;
		}
	}
	else {
		notGetSlowDown = false;
	}

	//Daca timpul de valabilitate a expirat, atunci bonusul este dezactivat.
	if (thisMoment - grabedSlowDown > bonusTimeAvability) {
		areSlowDown = false;
	}
}

/*Functia realizeaza reprezentare numarului de vieti a navei prin desenarea
numarului de dreptunghiuri in coltul din dreapta sus al ecranului.*/
void Tema1::drawLifes(glm::mat3 visMatrix) {
	for (int i = 0; i < ship.lifes; i++) {
		RenderMesh2D(lifeBar, visMatrix * Transform2D::Translate(logicSpace.x + logicSpace.width - 2 * (i + 1), logicSpace.y + logicSpace.height - 2) * Transform2D::Rotate(glm::half_pi<double>()) * Transform2D::Scale(0.75, 0.75), BonusLife::color);
	}
}

/*Functia realizeaza desenarea bonusurilor ce erau disponibile la sfarsitul jocului,
dar nu au fost culese deoarece jucatorul a ramas fara vieti.*/
void Tema1::bonusAtEndOfGame(glm::mat3 visMatrix) {
	if (notGetBonusLife) {
		modelMatrix = visMatrix *  Transform2D::Translate(bl.tx, bl.ty) * Transform2D::Rotate(bl.r) * Transform2D::Scale(BonusLife::sx, BonusLife::sy);
		RenderMesh2D(bl.mesh, modelMatrix, BonusLife::color);
	}

	if (notGetInvincibility) {
		modelMatrix = visMatrix *  Transform2D::Translate(inv.tx, inv.ty) * Transform2D::Rotate(inv.r) * Transform2D::Scale(Invincibility::sx, Invincibility::sy);
		RenderMesh2D(inv.mesh, modelMatrix, Invincibility::color);
	}

	if (notGetMultipleShot) {
		modelMatrix = visMatrix *  Transform2D::Translate(ms.tx, ms.ty) * Transform2D::Rotate(ms.r) * Transform2D::Scale(ms.sx, ms.sy);
		RenderMesh2D(ms.mesh, modelMatrix, MultipleShot::color);
	}
	if (notGetFreeze) {
		modelMatrix = visMatrix *  Transform2D::Translate(fr.tx, fr.ty) * Transform2D::Rotate(fr.r) * Transform2D::Scale(Freeze::sx, Freeze::sy);
		RenderMesh2D(fr.mesh, modelMatrix, Freeze::color);
	}
	if (notGetBigBang) {
		modelMatrix = visMatrix *  Transform2D::Translate(bb.tx, bb.ty) * Transform2D::Rotate(bb.r) * Transform2D::Scale(BigBang::sx, BigBang::sy);
		RenderMesh2D(bb.mesh, modelMatrix, BigBang::color);
	}
	if (notGetShield) {
		modelMatrix = visMatrix *  Transform2D::Translate(sh.tx, sh.ty) * Transform2D::Rotate(sh.r) * Transform2D::Scale(Shield::sx, Shield::sy);
		RenderMesh2D(sh.mesh, modelMatrix, Shield::color);
	}
	if (notGetSlowDown) {
		modelMatrix = visMatrix *  Transform2D::Translate(sd.tx, sd.ty) * Transform2D::Rotate(sd.r) * Transform2D::Scale(sd.sx, sd.sy);
		RenderMesh2D(sd.mesh, modelMatrix, SlowDown::color);
	}
}

void Tema1::DrawScene(glm::mat3 visMatrix)
{
	/*Se inregistreaza timpul pentru aceasta iteratie de 
	calcul si desenare.*/
	float thisMoment = Engine::GetElapsedTime();
	
	if (gameRunning) {
		/*Daca jocul este inca in desfasurare se efectueaza
		calcule si se executa desenari pe suprafata de afisare.*/

		/*Daca nu a fost obtinut power-up de "inghetare" inamici,
		atunci se pot genera inamici, iar cei existenti deja se
		pot misca.*/
		if (!freeze) {
			generateOpponents(thisMoment);
			rotateSimpleOpponents();
			rotateDividingOpponents();
			rotateDoubleLifeOpponents();
			moveSimpleOpponents();
			moveDividingOpponents();
			moveDoubleLifeOpponents(thisMoment);
		}

		//Desenarea inamicilor.
		drawSimpleOpponents(visMatrix);
		drawDoubleLifeOpponents(visMatrix, thisMoment);
		drawDividingOpponents(visMatrix);

		//Efectuarea rotatie navei si desenarea acesteia.
		rotateShip(visMatrix);
		drawShip(visMatrix);

		/*Desenarea proiectilelor, aplicarea unui pas de miscare
		si eliminarea proiectilelor ce au depasi limitele impuse
		de suprafata jocului.*/
		drawMissles(visMatrix);
		moveMissles();
		deleteFarMissles();

		/*Calcularea coliziunilor dintre proiectile si oponenti,
		cat si a celor dintre nava jucator si oponenti.*/
		computeMisslesOpponentsCollisions(thisMoment);
		computePlayerOpponentsCollisions(thisMoment);

		/*Calculul pentru desenarea bonusurilor, pentru disponibilitatea,
		obtinerea si valabilitatea acestora.*/
		computeFreezeLogic(visMatrix, thisMoment);
		computeInvincibilityLogic(visMatrix, thisMoment);
		computeMultiShotLogic(visMatrix, thisMoment);
		computeBonusLifeLogic(visMatrix, thisMoment);
		computeBigBangLogic(visMatrix, thisMoment);
		computeShieldLogic(visMatrix, thisMoment);
		computeSlowDownLogic(visMatrix, thisMoment);

		//Desenarea vietilor jucatorului in partea din dreapta sus a ecranului.
		drawLifes(visMatrix);

		//Daca scorul s-a modificat se printeaza la consola.
		if (oldScore != opponentsKilled) {
			cout << "Scor: " << opponentsKilled << "\n";
			oldScore = opponentsKilled;
		}

		//Afisarea statisticii jocului ce s-a terminat.
		if (endTime != 0) {
			cout << "\nJocul s-a sfarsit!\n";
			cout << "S-au distrus " << opponentsKilled << " inamici.\n";
			if (powerUpsUsed == 0) {
				cout << "Nu s-au folosit power-up-uri.\n";
			}
			else if (powerUpsUsed == 1) {
				cout << "S-a folosit un power-up.\n";
			}
			else {
				cout << "S-au folosit " << powerUpsUsed << " power-up-uri.\n";
			}
			cout << "Jocul a durat " << endTime << " secunde.\n\n";
		}
	}
	else {
		//Se realizeaza redesenarea scenei de la sfarsitul jocului.
		
		//Se deseneaza nava.
		drawShip(visMatrix);

		//Se deseneaza oponentii ramasi.
		drawSimpleOpponents(visMatrix);
		drawDoubleLifeOpponents(visMatrix, thisMoment);
		drawDividingOpponents(visMatrix);

		/*Se deseneaza proiectilele care nu au atins tinte si nici nu
		au parasit limitele de desenare.*/
		drawMissles(visMatrix);

		/*Se deseneaza power-up-urile care nu au fost inca "culese",
		dar care erau disponibile la sfarsitul jocului.*/
		bonusAtEndOfGame(visMatrix);
	}
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (gameRunning) {
		//Se realizeaza miscarea navei.
		if (window->KeyHold(GLFW_KEY_W)) {
			ship.ty += Ship::speed;
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			ship.ty -= Ship::speed;
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			ship.tx -= Ship::speed;
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			ship.tx += Ship::speed;
		}
	}
}

void Tema1::OnKeyPress(int key, int mods) {}

void Tema1::OnKeyRelease(int key, int mods) {}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (gameRunning) {
		//Se inregistreaza coordonatele mouse-ului.
		txMouse = mouseX;
		tyMouse = mouseY;
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_2 && gameRunning) {
		/*Se detecteaza daca este activ bonusul de MultipleShot pentru a se
		"lansa" un proiectil pe directia navei sau 4 proiectile simultan.*/
		if (single) {
			Missle aux = Missle(ship.tx, ship.ty, 1, 1, ship.c, ship.s, Missle::color);
			aux.mesh = Object2D::CreateMissle("missle", Missle::color);
			missles.push_back(aux);
		}
		else {
			Missle aux = Missle(ship.tx, ship.ty, 1, 1, ship.c, ship.s, Missle::color);
			aux.mesh = Object2D::CreateMissle("missle", Missle::color);
			missles.push_back(aux);

			aux = Missle(ship.tx, ship.ty, 1, 1, -ship.s, ship.c, Missle::color);
			aux.mesh = Object2D::CreateMissle("missle", Missle::color);
			missles.push_back(aux);

			aux = Missle(ship.tx, ship.ty, 1, 1, ship.s, -ship.c, Missle::color);
			aux.mesh = Object2D::CreateMissle("missle", Missle::color);
			missles.push_back(aux);

			aux = Missle(ship.tx, ship.ty, 1, 1, -ship.c, -ship.s, Missle::color);
			aux.mesh = Object2D::CreateMissle("missle", Missle::color);
			missles.push_back(aux);
		}
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}