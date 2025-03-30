#ifndef __CHAINE_H__
#define __CHAINE_H__	

#include <stdio.h>


/* Liste chainee de liaisons */
typedef struct liaison{
  double sig;
  int occ;
  struct liaison* suiv;
} Liaison;

/* Liste chainee de points */
typedef struct cellPoint{
  double x, y;                   /* Coordonnees du point */
  struct cellPoint* suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d'une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint* points;            /* Liste des points de la chaine */
  struct cellChaine* suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine* chaines;          /* La liste chainee des chaines */
} Chaines;

/* Fonctions auxiliaires */
CellPoint* inserer(CellPoint* L, double x, double y);
CellPoint* lecturePoints(char* str, int nbPoints);
CellChaine* lectureChainesAux(FILE* f, int nbChaines, char* buffer);
void detruireChaine(Chaines *C);
void ecrirePoints(CellPoint* L, FILE* f);
int nbPointsChaine(CellChaine* c);
double distanceEuclidienne(CellPoint* p1, CellPoint* p2);

/* Fonctions principales */
Chaines* lectureChaines(FILE *f);
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurChaine(CellChaine *c);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax);


#endif	
