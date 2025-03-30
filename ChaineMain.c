#include <stdio.h>
#include <stdlib.h>

#include "Chaine.h"

int main(int argc, char* argv[]){
    FILE* f = fopen(argv[1], "r");
    Chaines* C = lectureChaines(f);
    fclose(f);

    afficheChainesSVG(C, "Affichage_Chaine");

    detruireChaine(C);
    return 0;
}