# Projet Banc d'essai – Écran LCD Hitachi

Banc d'essai permettant de tester et de valider le fonctionnement d'un écran LCD Hitachi : carte électronique dédiée, code de test embarqué et simulation du montage.

![Statut](https://img.shields.io/badge/Statut-Finish-green)

---

## Sommaire

- [Présentation](#présentation)
- [Fonctionnalités](#fonctionnalités)
- [Structure du dépôt](#structure-du-dépôt)
- [Matériel nécessaire](#matériel-nécessaire)
- [Installation et utilisation](#installation-et-utilisation)
- [Simulation](#simulation)
- [PCB](#pcb)
- [Feuille de route](#feuille-de-route)
- [Auteur](#auteur)

---

## Présentation

Ce projet a pour objectif de concevoir un **banc d'essai pour écran LCD Hitachi**. Il permet de vérifier rapidement qu'un afficheur fonctionne correctement (initialisation, affichage des caractères, rétroéclairage, contraste, etc.) grâce à une série de tests automatisés.

Le dépôt regroupe les trois volets du projet :

1. **Le code** des programmes de test de l'écran LCD ;
2. **Le PCB** (carte électronique) du banc d'essai ;
3. **La simulation** du montage avant réalisation.

## Fonctionnalités

- Initialisation et configuration de l'écran LCD
- Affichage de caractères et de chaînes de test
- Test de toutes les cases / lignes de l'afficheur
- Contrôle du contraste et du rétroéclairage
- Carte dédiée pour brancher facilement l'écran à tester

## Structure du dépôt

```
Projet-Banc-d-essai/
├── code/
│   └── lcd_tests/      # Programmes de test de l'écran LCD
├── documentation/      # Document en lien avec le projet
├── pcb/                # Fichiers de conception de la carte électronique
├── simulation/         # Fichiers de simulation du montage
└── README.md
```

| Dossier | Contenu |
|---|---|
| `code/lcd_tests` | Code source des tests de l'écran LCD |
| `documentation/fiche-maintenance` | Fiche pour la maintenance du pcb |
| `pcb` | Schéma électrique, routage et fichiers de fabrication |
| `simulation` | Projet de simulation du circuit |

## Matériel nécessaire


- Écran LCD Hitachi (ex. 16×2, HD44780)
- Microcontrôleur / carte de développement : *à préciser*
- Potentiomètre de contraste (~10 kΩ)
- Résistance de limitation pour le rétroéclairage
- Alimentation 5 V
- Fils / connecteurs ou PCB du dossier `pcb/`

## Installation et utilisation

1. **Cloner le dépôt**

   ```bash
   git clone https://github.com/Priammm/Projet-Banc-d-essai.git
   cd Projet-Banc-d-essai
   ```

2. **Réaliser le montage** à partir du PCB (voir [PCB](#pcb)) ou d'une plaque d'essai en suivant le schéma.

3. **Compiler et téléverser le code** présent dans `code/lcd_tests/` :

   ```text
   # Indiquez ici votre environnement (Arduino IDE, PlatformIO, MPLAB, STM32CubeIDE...)
   # et les étapes de compilation / téléversement.
   ```

4. **Alimenter le banc** : les tests s'exécutent et les résultats s'affichent sur l'écran.

## Simulation

Le dossier `simulation/` contient le projet de simulation du montage (outil utilisé : Simulide).

Pour l'utiliser :

1. Ouvrir le fichier de simulation avec Simulide ;
2. Charger le programme compilé (`.hex`) dans le microcontrôleur simulé ;
3. Lancer la simulation.

## PCB

Le dossier `pcb/` contient les fichiers de conception de la carte (outil utilisé : Kicad 9) :

- Schéma électrique
- Routage de la carte
- Fichiers de fabrication (Gerber), si disponibles

## Feuille de route

- [x] Structure du projet
- [x] Finaliser les tests de l'écran LCD
- [x] Valider la simulation
- [x] Réaliser et tester le PCB
- [x] Documenter les résultats

## Auteur

**Priamm** – [@Priammm](https://github.com/Priammm)
