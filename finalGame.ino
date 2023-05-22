#include <Gamebuino-Meta.h>

// Bombs
const int bombsNumber = 10;
int bombsX[bombsNumber] = {};
int bombsY[bombsNumber] = {};

const int bombsWidth = 2;
const int bombsHeigth = 3;

// Medikit
const int maxNumberMedikits = 8;
int medikitNumber = 0;
int medikitX[maxNumberMedikits] = {};
int medikitY[maxNumberMedikits] = {};

const int medikitWidth = 3;
const int medikitHeigth = 5;

// Player
int playerX = 20;  // Le panier peut bouger horizontalement, donc ce n'est pas une constante
const int playerY = gb.display.height() - 4;  // Constante car le panier est fixe selon l'axe Y
int playerWidth = 4;  // Pas constant car la taille dépend de la nourriture restante
const int playerHeight = 3;  // Constante

int score = 0;
int highscore = 0;
int nourriture;  // Plus de nourriture est mieux. 0 nourriture = game over

void setup() {
  gb.begin();
  // Placer les oeufs aléatoirement au dessus de l'écran
  for (int i = 0; i < bombsNumber; i += 1) {
    bombsX[i] = random(0, gb.display.width());
    bombsY[i] = random(-40, -bombsHeigth);  // Au dessus de l'écran
  }
  nourriture = 300;
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
  }
  else if (gb.buttons.repeat(BUTTON_RIGHT, 0) && playerX < gb.display.width()) {
    playerX += 2;
  }
}

void miseAJour() {
  // La faim fait doucement baisser notre nourriture dans la panier
  nourriture--;
  if (nourriture <= 0) {  // Somme nous mort de faim??
    medikitNumber = 0;  // Reset les carottes
    score = 0;  // Recommencer le jeu
    nourriture = 300;  // Reset nourriture
    for (int i = 0; i < bombsNumber; i += 1) {
      bombsX[i] = random(0, gb.display.width());
      bombsY[i] = random(-40, -bombsHeigth);  // Au dessus de l'écran
    }
  }

  // Eggs
  for (int i = 0; i < bombsNumber; i += 1) {
    bombsY[i] += 1;

    // Collisions
    if (gb.collide.rectRect(playerX - playerWidth / 2, playerY, playerWidth, playerHeight, bombsX[i], bombsY[i], bombsWidth, bombsHeigth)) {
      score += 1;
      if (score > highscore) {
        highscore = score;
      }
      nourriture = nourriture + 20;  // MMmmmmm des oeufs
      // Ajouter des carrotes. Sans dépasser le plafond
      medikitNumber = min(score / 5, maxNumberMedikits);

      // Reset l'oeuf
      bombsX[i] = random(0, gb.display.width());
      bombsY[i] = random(-40, -bombsHeigth);  // Au dessus de l'écran
    }

    // Verifier que la carotte ne soit pas sortie de l'écran
    if (bombsY[i] >= gb.display.height()) {
      // Reset l'oeuf
      bombsX[i] = random(0, gb.display.width());
      bombsY[i] = random(-40, -bombsHeigth);  // Au dessus de l'écran
    }
  }

  // Carottes
  for (int i = 0; i < medikitNumber; i += 1) {
    medikitY[i] += 2;

    // Collisions avec le joueur
    if (gb.collide.rectRect(playerX - playerWidth / 2, playerY, playerWidth, playerHeight, medikitX[i], medikitY[i], medikitWidth, medikitHeigth)) {

      nourriture -= 40;  // Allergie => moins de nourriture :(

      // Reset la carotte
      medikitX[i] = random(0, gb.display.width());
      medikitY[i] = random(-20, -medikitHeigth);  // Au dessus de l'écran
    }

    // Verifier que la carotte ne soit pas sortie de l'écran
    if (medikitY[i] >= gb.display.height()) {
      // Reset la carotte
      medikitX[i] = random(0, gb.display.width());
      medikitY[i] = random(-20, medikitHeigth);  // Au dessus de l'écran
    }
  }

}

void affichage() {
  gb.display.clear();

  // Eggs
  gb.display.setColor(BROWN);
  for (int i = 0; i < bombsNumber; i += 1) {
    gb.display.fillRect(bombsX[i], bombsY[i], bombsWidth, bombsHeigth);
  }

  // Carrots
  gb.display.setColor(ORANGE);
  for (int i = 0; i < medikitNumber; i += 1) {
    gb.display.fillRect(medikitX[i], medikitY[i], medikitWidth, medikitHeigth);
  }

  // Player
  gb.display.setColor(WHITE);
  gb.display.fillRect(playerX - playerWidth / 2, playerY, playerWidth, playerHeight);

  // Score
  gb.display.setColor(WHITE);
  gb.display.setCursor(gb.display.width() - 8, 0);
  gb.display.print(score);
  gb.display.setColor(RED);
  gb.display.setCursor(gb.display.width() - 8, 8);
  gb.display.print(highscore);
}