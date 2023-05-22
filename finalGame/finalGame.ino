#include <Gamebuino-Meta.h>
#include "Sprite-Perso.h"

// Bombs
const int bombsNumber = 10;
int bombsX[bombsNumber] = {};
int bombsY[bombsNumber] = {};

const int bombsWidth = 2;
const int bombsHeigth = 3;
int bombsSpeed = 1;

// Medikit
const int maxNumberMedikits = 8;
int medikitNumber = 0;
int medikitX[maxNumberMedikits] = {};
int medikitY[maxNumberMedikits] = {};

const int medikitWidth = 3;
const int medikitHeigth = 5;

// Player
int playerX = 20;  
const int playerWidth = 16;  
const int playerHeight = 16;  
const int playerY = gb.display.height() - playerHeight;  
int life = 3;

int score = 0;
int highscore = 0;

Image currentImage = IMAGE_DATA_STAND;
bool keyPressDown = false;

void setup() {
  gb.begin();
  bombsSpeed = buildMenu();
  for (int i = 0; i < bombsNumber; i += 1) {
    bombsX[i] = random(0, gb.display.width());
    bombsY[i] = random(-40, -bombsHeigth); 
  }
}

void loop() {
  while (!gb.update());
  entrees();
  miseAJour();
  affichage();
}

void entrees() {
  if (gb.buttons.repeat(BUTTON_LEFT, 0) && playerX > 0) {
    playerX -= 2;
    currentImage = IMAGE_DATA_LEFT;
    keyPressDown = false;
  }
  else if (gb.buttons.repeat(BUTTON_RIGHT, 0) && playerX+playerWidth < gb.display.width()) {
    playerX += 2;
    currentImage = IMAGE_DATA_RIGHT;
    keyPressDown = false;
  }
  else if (gb.buttons.repeat(BUTTON_DOWN, 0)) {
    currentImage = IMAGE_DATA_MOOVE_CHARGE;
    keyPressDown=true;
  }
  else if (gb.buttons.repeat(BUTTON_UP, 0)) {
    currentImage = IMAGE_DATA_MOOVE_JUMP;
    keyPressDown = false;

  }
  else{
    currentImage = IMAGE_DATA_STAND;
    keyPressDown = false;
  }
}

void miseAJour() {
  score += 1;
  
  if (score > highscore) {
        highscore = score;
  }
  
  if (life <= 0) { 
    medikitNumber = 0;  
    score = 0;  
    life = 3;
    bombsSpeed = 1;
    for (int i = 0; i < bombsNumber; i += 1) {
      bombsX[i] = random(0, gb.display.width());
      bombsY[i] = random(-40, -bombsHeigth); 
    }
  }

  for (int i = 0; i < bombsNumber; i += 1) {
    bombsY[i] += bombsSpeed;

    if (gb.collide.rectRect(playerX, playerY, playerWidth, playerHeight, bombsX[i], bombsY[i], bombsWidth, bombsHeigth)) {
      life -= 1;
      gb.sound.fx({Gamebuino_Meta::Sound_FX_Wave::NOISE,0,70,0,0,224,1});
      
      medikitNumber = 3 - life;

      bombsX[i] = random(0, gb.display.width());
      bombsY[i] = random(-40, -bombsHeigth); 
    }

    if (bombsY[i] >= gb.display.height()) {
      bombsX[i] = random(0, gb.display.width());
      bombsY[i] = random(-40, -bombsHeigth); 
    }
  }

  for (int i = 0; i < medikitNumber; i += 1) {
    medikitY[i] += 2;
   

    if (gb.collide.rectRect(playerX, playerY, playerWidth, playerHeight, medikitX[i], medikitY[i], medikitWidth, medikitHeigth)) {
      life += 1; 
      gb.sound.playOK();
      medikitNumber = 3 - life;
      
      medikitX[i] = random(0, gb.display.width());
      medikitY[i] = random(-20, -medikitHeigth);
    }

    if (medikitY[i] >= gb.display.height()) {
      medikitX[i] = random(0, gb.display.width());
      medikitY[i] = random(-20, medikitHeigth);  
    }
  }

}

void affichage() {
  gb.display.clear();

  gb.display.setColor(GRAY);
  for (int i = 0; i < bombsNumber; i += 1) {
    gb.display.fillRect(bombsX[i], bombsY[i], bombsWidth, bombsHeigth);
  }

  gb.display.setColor(PINK);
  for (int i = 0; i < medikitNumber; i += 1) {
    gb.display.fillRect(medikitX[i], medikitY[i], medikitWidth, medikitHeigth);
  }

  gb.display.setColor(WHITE);
  gb.display.drawImage(playerX, playerY, currentImage);

  gb.display.setColor(WHITE);
  gb.display.setCursor(gb.display.width() - 16, 0);
  gb.display.print(score);
  
  gb.display.setColor(RED);
  gb.display.setCursor(gb.display.width() - 16, 8);
  gb.display.print(highscore);
  
  gb.display.setColor(WHITE);
  gb.display.setCursor(gb.display.width() - 16, 16);
  gb.display.print(life);
}


int buildMenu() {
  const char* items[] = {
    "eazy",
    "medium",
    "hard"
  };

  const uint8_t indexItem = gb.gui.menu("Menu", items);
  int choice = 1;
  if(items[indexItem] == "eazy") {
    choice = 1;
  } else if(items[indexItem] == "medium") {
    choice = 2;
  } else if(items[indexItem] == "hard") {
    choice = 3;
    }
  return choice;
}

