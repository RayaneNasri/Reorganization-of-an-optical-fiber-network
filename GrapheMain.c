#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Chaine.h"
#include "Struct_File.h"
#include "Graphe.h"
#include "TableH.h"


int main(int argc, char** argv){

    char* nomf = argv[1];

    FILE* f = fopen(nomf, "r");

    if(f == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }
    
    Chaines* C = lectureChaines(f);
    if(C == NULL){
        fclose(f);
        return EXIT_FAILURE;
    }

    Reseau* R = reconstitueReseauListe(C);
    if(R == NULL){
        fclose(f);
        detruireChaine(C);
        return EXIT_FAILURE;
    }

    Graphe* G = creerGraphe(R);

    plus_court_chemin(G, sommetNum(G, 1), sommetNum(G, 6));

    detruireChaine(C);
    detruireReseau(R);

    fclose(f);

    return EXIT_SUCCESS;
}
