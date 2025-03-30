#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
double time_cpu(int start, int end){
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

/* ~~~~~~ Fonctions principales ~~~~~~ */
void time_diff_structures(){

    Chaines* C = NULL;
    Reseau* R = NULL;
    double start, end;
    char str[51];

    FILE* w = fopen("timeCal.txt", "w");
    if(w == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return;
    }

    
    for(int i = 1; i <= 3; i++){
        sprintf(str, "instance%d.cha", i);
        FILE* f = fopen(str, "r");
        if(f == NULL){
            fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
            return;
        }
        C = lectureChaines(f);
        fclose(f);

        fprintf(w, "Instance %d:\t", i);

        // Calcule du temps cpu pour reconstruire le reseau, pour une liste chainée.
        start = clock();
        R = reconstitueReseauListe(C);
        end = clock();
        detruireReseau(R);
        fprintf(w, "%f\t", time_cpu(start, end));

        // Calcule du temps cpu pour reconstruire le reseau, pour une table de hachage.
        start = clock();
        R = reconstitueReseauHachage(C, 10);
        end = clock();
        detruireReseau(R);
        fprintf(w, "%f\t", time_cpu(start, end));

        // Calcule du temps cpu pour reconstruire le reseau, pour un arbre quaternaire.
        start = clock();
        R = reconstitueReseauArbre(C);
        end = clock();
        detruireReseau(R);
        fprintf(w, "%f\t", time_cpu(start, end));

        fprintf(w, "\n");
    }

    fclose(w);
    detruireChaine(C);
}

void time_hachage_diff_sizes(){
    /*
    Calcule le temps cpu pour reconstruire le reseau pour différentes tailles de table de hachage.
    */

    Chaines* C = NULL;
    double start, end;
    char str[51];
    int size;

    FILE* w = fopen("hachCal.txt", "w");
    if(w == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return;
    }

    for(int i = 1; i <= 3; i++){
        sprintf(str, "instance%d.cha", i);
        FILE* f = fopen(str, "r");
        if(f == NULL){
            fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
            return;
        }
        C = lectureChaines(f);
        fclose(f);

        fprintf(w, "Instance %d:\t", i);
        size = 10;
        while(size <= 50){
            start = clock();
            detruireReseau(reconstitueReseauHachage(C, size));
            end = clock();
            fprintf(w, "%f\t", time_cpu(start, end));

            size += 10;
        }
        fprintf(w, "\n");
    }

    fclose(w);
    detruireChaine(C);
}

void _time(){

    FILE* w = fopen("time_LC.txt", "w");
    if(w == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return;
    }

    Chaines* C = NULL;
    Reseau* R = NULL;

    double start, end;

    int nbPointsChaine = 100;
    int xmax = 5000;
    int ymax = 5000;
    int nbChaines = 500;

    while(nbChaines <= 1000){
        printf("nbPoints: %d\n", nbChaines * nbPointsChaine);
        C = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);

        start = clock();
        // On choisit la structure de données que l'on veut tester !
        R = reconstitueReseauListe(C);
        end = clock();

        fprintf(w, "%d\t%f\n", nbChaines * nbPointsChaine, time_cpu(start, end));

        detruireChaine(C);
        detruireReseau(R);

        nbChaines += 500;
    }

    fclose(w);
}


int main(){
    _time();
    return 0;
}