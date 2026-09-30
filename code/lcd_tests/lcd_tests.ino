/*
  ============================================================
  TESTEUR D'ECRAN LCD 16x2 - MENU A BOUTONS
  Carte : Arduino Uno (fonctionne aussi dans SimulIDE)
  ============================================================

  PRINCIPE
  --------
  Au demarrage, un menu s'affiche sur l'ecran. On choisit un test
  avec les boutons, on le lance, et on peut l'arreter a tout moment.

  BOUTONS (3 boutons poussoirs)
  -----------------------------
  Chaque bouton est branche entre la broche Arduino et GND.
  Pas besoin de resistance : on utilise la resistance interne
  (INPUT_PULLUP). Au repos la broche lit HIGH, appuye elle lit LOW.

    Bouton SUIVANT -> broche D6 : passe au test suivant dans le menu
    Bouton OK      -> broche D7 : lance le test affiche
    Bouton RETOUR  -> broche D8 : arrete le test et revient au menu

  Remarque SimulIDE : si le pull-up interne ne fonctionne pas dans
  ta version, ajoute une resistance de 10k entre chaque broche
  (D6, D7, D8) et le +5V.

  BRANCHEMENT DU LCD (identique aux versions precedentes)
  -------------------------------------------------------
    RS -> D12     E  -> D11
    D4 -> D5      D5 -> D4      D6 -> D3      D7 -> D2
    RW -> GND     VSS -> GND    VDD -> 5V
    V0 -> curseur du potentiometre (contraste)
    (broches D6/D7/D8 de l'Arduino : ne pas confondre avec
     D6/D7 du LCD, ce sont des broches differentes !)

  LISTE DES TESTS
  ---------------
    1. Blocs un par un      : les blocs apparaissent et restent
    2. Bloc qui defile      : une colonne pleine traverse l'ecran
    3. Table ASCII          : affiche les caracteres 32 a 255
    4. Caracteres perso     : coeur, smiley, fleche, cloche
    5. Damier / clignote    : damier, inversion, clignotement
    6. Defilement texte     : texte qui glisse gauche puis droite
    7. Barre progression    : barre de 0 a 100 %
    8. Chrono               : temps ecoule, RETOUR pour arreter
    9. Test boutons         : etat des 3 boutons en direct
                              (maintenir RETOUR 1,5 s pour sortir)

  ASTUCE
  ------
  Les textes n'ont pas d'accents : le LCD HD44780 ne les gere pas.
  Pour changer la vitesse d'un test, modifie les valeurs passees
  a attendre(...) (en millisecondes).
*/

#include <LiquidCrystal.h>

// ------------------------------------------------------------
// CONFIGURATION DES BROCHES
// ------------------------------------------------------------
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int BTN_SUIVANT = 6;   // passe au test suivant
const int BTN_OK      = 7;   // lance le test
const int BTN_RETOUR  = 8;   // quitte le test en cours

// ------------------------------------------------------------
// MENU : noms affiches (16 caracteres maximum par nom)
// ------------------------------------------------------------
const int NB_TESTS = 9;
const char* nomsTests[NB_TESTS] = {
  "Blocs un a un",
  "Bloc qui defile",
  "Table ASCII",
  "Caract. perso",
  "Damier/clignote",
  "Defilement txt",
  "Barre progress.",
  "Chrono",
  "Test boutons"
};

int choix = 0;   // test actuellement selectionne dans le menu

// ------------------------------------------------------------
// CARACTERES PERSONNALISES
// Chaque caractere = 8 lignes de 5 pixels (1 = pixel allume).
// Le LCD ne peut memoriser que 8 caracteres (positions 0 a 7).
// ------------------------------------------------------------
byte blocPlein[8] = {          // position 0 : toujours chargee
  0b11111, 0b11111, 0b11111, 0b11111,
  0b11111, 0b11111, 0b11111, 0b11111
};

byte coeur[8] = {
  0b00000, 0b01010, 0b11111, 0b11111,
  0b11111, 0b01110, 0b00100, 0b00000
};

byte smiley[8] = {
  0b00000, 0b01010, 0b01010, 0b00000,
  0b10001, 0b01110, 0b00000, 0b00000
};

byte fleche[8] = {
  0b00100, 0b01110, 0b11111, 0b00100,
  0b00100, 0b00100, 0b00100, 0b00000
};

byte cloche[8] = {
  0b00100, 0b01110, 0b01110, 0b01110,
  0b11111, 0b00000, 0b00100, 0b00000
};

// ============================================================
// FONCTIONS UTILITAIRES
// ============================================================

// Renvoie true UNE SEULE FOIS quand on appuie sur le bouton.
// - anti-rebond de 30 ms
// - attend que le bouton soit relache avant de rendre la main
bool appui(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(30);                        // anti-rebond
    if (digitalRead(pin) == LOW) {
      while (digitalRead(pin) == LOW) { }   // attend le relachement
      delay(30);                      // anti-rebond au relachement
      return true;
    }
  }
  return false;
}

// Pause de 'ms' millisecondes, INTERROMPUE si on appuie sur RETOUR.
// Renvoie true si la pause est allee au bout,
// false si l'utilisateur a appuye sur RETOUR.
// Utilisation dans un test : if (!attendre(500)) return;
bool attendre(unsigned long ms) {
  unsigned long debut = millis();
  while (millis() - debut < ms) {
    if (appui(BTN_RETOUR)) return false;
  }
  return true;
}

// Affiche un texte sur une ligne en effacant le reste de la ligne
void ligne(int numero, const char* texte) {
  lcd.setCursor(0, numero);
  lcd.print("                ");   // 16 espaces
  lcd.setCursor(0, numero);
  lcd.print(texte);
}

// ============================================================
// MENU
// ============================================================
void afficherMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">");                  // curseur devant le test choisi
  lcd.print(nomsTests[choix]);
  lcd.setCursor(0, 1);
  lcd.print(choix + 1);            // numero du test
  lcd.print("/");
  lcd.print(NB_TESTS);
  lcd.print(" OK=lancer");
}

// ============================================================
// LES TESTS
// Chaque test peut etre arrete avec le bouton RETOUR.
// ============================================================

// ---- TEST 1 : blocs un par un, ils restent en place ----
void testBlocs() {
  lcd.clear();
  ligne(0, "Blocs un a un");
  if (!attendre(1500)) return;
  lcd.clear();

  for (int col = 0; col < 16; col++) {
    lcd.setCursor(col, 0);
    lcd.write(byte(0));            // bloc ligne du haut
    lcd.setCursor(col, 1);
    lcd.write(byte(0));            // bloc ligne du bas
    if (!attendre(500)) return;    // vitesse : 500 ms par colonne
  }
  attendre(2000);                  // ecran plein, on laisse voir
}

// ---- TEST 2 : une colonne pleine qui traverse l'ecran ----
void testDefilementBloc() {
  for (int col = 0; col < 16; col++) {
    lcd.clear();                   // efface l'ancien bloc
    lcd.setCursor(col, 0);
    lcd.write(byte(0));
    lcd.setCursor(col, 1);
    lcd.write(byte(0));
    if (!attendre(250)) return;    // vitesse : 250 ms par colonne
  }
}

// ---- TEST 3 : table ASCII (caracteres 32 a 255) ----
// 32 caracteres par page (16 par ligne), 7 pages.
void testASCII() {
  for (int debut = 32; debut < 256; debut += 32) {
    lcd.clear();
    lcd.setCursor(0, 0);
    for (int c = debut; c < debut + 16; c++) lcd.write((uint8_t)c);
    lcd.setCursor(0, 1);
    for (int c = debut + 16; c < debut + 32; c++) lcd.write((uint8_t)c);
    if (!attendre(2500)) return;   // 2,5 s par page
  }
}

// ---- TEST 4 : caracteres personnalises ----
void testPerso() {
  // On charge les 4 caracteres dans les positions 1 a 4
  lcd.createChar(1, coeur);
  lcd.createChar(2, smiley);
  lcd.createChar(3, fleche);
  lcd.createChar(4, cloche);

  lcd.clear();
  ligne(0, "Caract. perso :");
  lcd.setCursor(0, 1);
  lcd.write(byte(1));  lcd.print(" ");
  lcd.write(byte(2));  lcd.print(" ");
  lcd.write(byte(3));  lcd.print(" ");
  lcd.write(byte(4));
  attendre(5000);
}

// ---- TEST 5 : damier, inversion, clignotement ----
void testDamier() {
  // Damier : bloc, espace, bloc, espace...
  for (int rep = 0; rep < 4; rep++) {          // 4 inversions
    lcd.clear();
    for (int col = 0; col < 16; col++) {
      for (int lig = 0; lig < 2; lig++) {
        lcd.setCursor(col, lig);
        if ((col + lig + rep) % 2 == 0) lcd.write(byte(0));
        else lcd.print(" ");
      }
    }
    if (!attendre(700)) return;
  }

  // Ecran plein qui clignote (display / noDisplay)
  lcd.clear();
  for (int col = 0; col < 16; col++) {
    lcd.setCursor(col, 0); lcd.write(byte(0));
    lcd.setCursor(col, 1); lcd.write(byte(0));
  }
  for (int i = 0; i < 5; i++) {
    lcd.noDisplay();                // ecran eteint (contenu conserve)
    if (!attendre(300)) { lcd.display(); return; }
    lcd.display();                  // ecran rallume
    if (!attendre(300)) return;
  }
}

// ---- TEST 6 : texte qui defile gauche puis droite ----
void testDefilementTexte() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Defilement texte");
  lcd.setCursor(0, 1);
  lcd.print("SimulIDE + UNO !");
  if (!attendre(1500)) return;

  for (int i = 0; i < 16; i++) {    // vers la gauche
    lcd.scrollDisplayLeft();
    if (!attendre(250)) { lcd.clear(); return; }
  }
  for (int i = 0; i < 16; i++) {    // retour vers la droite
    lcd.scrollDisplayRight();
    if (!attendre(250)) { lcd.clear(); return; }
  }
  lcd.clear();                      // remet le decalage a zero
}

// ---- TEST 7 : barre de progression de 0 a 100 % ----
void testBarre() {
  lcd.clear();
  ligne(0, "Progression");
  for (int p = 0; p <= 16; p++) {
    // Ligne du haut : p blocs pleins
    lcd.setCursor(0, 0);
    for (int i = 0; i < 16; i++) {
      if (i < p) lcd.write(byte(0));
      else lcd.print(" ");
    }
    // Ligne du bas : pourcentage
    lcd.setCursor(0, 1);
    lcd.print(p * 100 / 16);
    lcd.print("%    ");
    if (!attendre(300)) return;
  }
  attendre(2000);
}

// ---- TEST 8 : chrono (s'arrete avec RETOUR) ----
void testChrono() {
  lcd.clear();
  ligne(0, "Chrono (RETOUR)");
  unsigned long debut = millis();
  long dernier = -1;

  while (true) {
    long s = (millis() - debut) / 1000;
    if (s != dernier) {             // met a jour seulement si ca change
      dernier = s;
      lcd.setCursor(0, 1);
      lcd.print(s / 60);
      lcd.print("min ");
      lcd.print(s % 60);
      lcd.print("s    ");
    }
    if (appui(BTN_RETOUR)) return;
  }
}

// ---- TEST 9 : etat des boutons en direct ----
// Affiche 1 = appuye, 0 = relache.
// Pour quitter : maintenir RETOUR pendant 1,5 seconde.
void testBoutons() {
  lcd.clear();
  ligne(0, "S:  O:  R:");
  ligne(1, "Tenir R 1,5s=fin");
  unsigned long debutRetour = 0;

  while (true) {
    bool s = (digitalRead(BTN_SUIVANT) == LOW);
    bool o = (digitalRead(BTN_OK)      == LOW);
    bool r = (digitalRead(BTN_RETOUR)  == LOW);

    lcd.setCursor(2, 0);  lcd.print(s ? "1" : "0");
    lcd.setCursor(6, 0);  lcd.print(o ? "1" : "0");
    lcd.setCursor(10, 0); lcd.print(r ? "1" : "0");

    // Sortie : RETOUR maintenu 1500 ms
    if (r) {
      if (debutRetour == 0) debutRetour = millis();
      if (millis() - debutRetour > 1500) {
        while (digitalRead(BTN_RETOUR) == LOW) { }   // attend relachement
        delay(30);
        return;
      }
    } else {
      debutRetour = 0;
    }
  }
}

// ============================================================
// LANCEMENT D'UN TEST SELON LE NUMERO CHOISI
// ============================================================
void lancerTest(int numero) {
  switch (numero) {
    case 0: testBlocs();           break;
    case 1: testDefilementBloc();  break;
    case 2: testASCII();           break;
    case 3: testPerso();           break;
    case 4: testDamier();          break;
    case 5: testDefilementTexte(); break;
    case 6: testBarre();           break;
    case 7: testChrono();          break;
    case 8: testBoutons();         break;
  }
}

// ============================================================
// SETUP : execute une seule fois au demarrage
// ============================================================
void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, blocPlein);     // bloc plein en position 0

  // Boutons en entree avec resistance de rappel interne
  pinMode(BTN_SUIVANT, INPUT_PULLUP);
  pinMode(BTN_OK,      INPUT_PULLUP);
  pinMode(BTN_RETOUR,  INPUT_PULLUP);

  // Petit message d'accueil
  lcd.setCursor(0, 0);
  lcd.print("Testeur LCD");
  lcd.setCursor(0, 1);
  lcd.print("Demarrage...");
  delay(1500);

  afficherMenu();
}

// ============================================================
// LOOP : le menu tourne en boucle
// ============================================================
void loop() {
  // SUIVANT : passe au test suivant (revient au 1er apres le dernier)
  if (appui(BTN_SUIVANT)) {
    choix = (choix + 1) % NB_TESTS;
    afficherMenu();
  }

  // OK : lance le test choisi, puis reaffiche le menu a la fin
  if (appui(BTN_OK)) {
    lcd.clear();
    lancerTest(choix);
    lcd.createChar(0, blocPlein);   // securite : recharge le bloc plein
    afficherMenu();
  }
}
