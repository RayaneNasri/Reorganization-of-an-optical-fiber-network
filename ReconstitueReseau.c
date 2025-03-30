#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(int argc, char* argv[]){

    char* nomf = argv[1];
    int i = atoi(argv[2]);

    FILE* fic1 = fopen(nomf, "r");
    if(fic1 == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
        return 0;
    }

    Chaines* C = lectureChaines(fic1);
    fclose(fic1);

    Reseau* res;
    switch(i){
        case(1):
            res = reconstitueReseauListe(C);

            FILE* fic2 = fopen("reseau.txt", "w");
            if(fic2 == NULL){
                fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
                return 0;
            }

            afficheReseauSVG(res, "Affichage_reseau_LC");
            ecrireReseau(res, fic2);
            fclose(fic2);
            break;

        case(2):
            res = reconstitueReseauListe(C);

            fic2 = fopen("reseau.txt", "w");
            if(fic2 == NULL){
                fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
                return 0;
            }

            afficheReseauSVG(res, "Affichage_reseau_Hach");
            ecrireReseau(res, fic2);
            fclose(fic2);
            break;

        case(3):
            res = reconstitueReseauArbre(C);

            fic2 = fopen("reseau.txt", "w");
            if(fic2 == NULL){
                fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'ouverture du fichier.\n", __FILE__, __LINE__);
                return 0;
            }

            afficheReseauSVG(res, "Affichage_reseau_Arbre");
            ecrireReseau(res, fic2);
            fclose(fic2);
            break;

        default:
            break;
    }

    detruireChaine(C);
    detruireReseau(res);

    return 0;
}