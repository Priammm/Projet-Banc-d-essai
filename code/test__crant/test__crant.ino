#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Caractère personnalisé : bloc plein (tous les pixels allumés)
byte blocPlein[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, blocPlein); // enregistre le bloc plein en position 0

  testEcran();
}

void loop() {
  // ne fait rien, le test tourne une fois au démarrage
  // (tu peux appeler testEcran() ici si tu veux qu'il se répète en boucle)
}

void testEcran() {
  // --- Message de début ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Debut du test");
  delay(2000);
  lcd.clear();

  // --- Bande pleine hauteur qui défile colonne par colonne ---
  for (int col = 0; col < 16; col++) {
    lcd.clear();
    lcd.setCursor(col, 0);
    lcd.write(byte(0)); // ligne du haut
    lcd.setCursor(col, 1);
    lcd.write(byte(0)); // ligne du bas
    delay(150);
  }

  // --- Message de fin ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fin du test");
  delay(2000);
  lcd.clear();
}
