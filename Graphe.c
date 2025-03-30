#include <stdio.h>
#include <stdlib.h>

#include "Struct_File.h"
#include "Chaine.h"
#include "Reseau.h"
#include "Graphe.h"
#include "TableH.h"

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
void inserer_en_tete_arete(Cellule_arete** L, Cellule_arete* cell_arete){
    /*
    Insére en tête de la liste L la cellule cell_arete.
    */
    cell_arete-> suivant = (*L);
    (*L) = cell_arete;
}

Sommet* sommetNum(Graphe* G, int num){
    for(int i = 0; (G-> nbSom) > i; i++){
        if(((G-> T_som)[i])-> num == num) return (G-> T_som)[i];
    }
    return NULL;
}

void detruireSommet(Sommet* s){
    /*
    Détruit le sommet s.
    */

    Cellule_arete* tmp = NULL;
    Cellule_arete* cour = s-> L_Voisins;

    while(cour != NULL){
        if(cour-> a-> u == -1){
            free(cour-> a);
        }
        else{
            cour-> a-> u = cour-> a-> v = -1;
        }

        tmp = cour;
        cour = cour-> suivant;

        free(tmp);
    }

    free(s);
}

void detruireGraphe(Graphe* G){
    /*
    Libére la mémoire allouée par le graphe G.
    */

    for(int i = 0; (G-> nbSom) > i; i++){
        detruireSommet((G-> T_som)[i]);
    }

    free(G-> T_som);
    free(G-> T_commod);
    free(G);
}

/* ~~~~~~ Fonctions principales ~~~~~~ */
Graphe* creerGraphe(Reseau* R){
    Graphe* G = (Graphe*) malloc(sizeof(Graphe));
    if(G == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    G-> nbSom = R-> nbNoeuds;
    G-> T_som = (Sommet**) calloc((G-> nbSom), sizeof(Sommet*));
    if(G-> T_som == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        free(G);
        return NULL;
    }
    G-> gamma = R-> gamma;
    G-> nbcommod = nbCommodites(R);
    G-> T_commod = (Commod*) calloc((G-> nbcommod), sizeof(Commod));
    if(G-> T_commod == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        free(G-> T_som);
        free(G);
        return NULL;
    }

    // On crée les commodités:
    CellCommodite* tmp_commod = R-> commodites;
    int i = 0;
    while(tmp_commod != NULL){
        
        Commod new_com;
        new_com.e1 = (tmp_commod-> extrA)-> num;
        new_com.e2 = (tmp_commod-> extrB)-> num;
        G-> T_commod[i++] = new_com;

        tmp_commod = tmp_commod-> suiv;
    }

    // On crée les sommets:

    // La table de hachage "tabH" nous permet d'éviter les doublons d'arêtes:
    TableH* H = init_table(500);

    
    CellNoeud* tmp_noeud = R-> noeuds;
    while(tmp_noeud != NULL){
        Sommet* new;
        Noeud* nd = tmp_noeud-> nd;

        // Si on a déja créé le sommet, on le récupère:
        if((G-> T_som[nd-> num]) != NULL){
            new = (G-> T_som)[nd-> num];
        }

        // Si c'est la première fois qu'on rencontre ce sommet, on le crée:
        else{ 
            new = (Sommet*) malloc(sizeof(Sommet));
            if(new == NULL){
                fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
                
                free(H-> tab);
                free(H);
                detruireGraphe(G);
                return NULL;
            }
            (G-> T_som)[nd-> num] = new;
            new-> num = nd-> num;
            new-> x = nd-> x; new-> y = nd-> y;
            new-> L_Voisins = NULL;
        }

        // On crée les arêtes du sommet:
        CellNoeud* tmp_vois = nd-> voisins;
        while(tmp_vois != NULL){
            Noeud* voisin = tmp_vois-> nd;

            Arete* a = in_table(H, nd, voisin);

            Cellule_arete* new_arete = (Cellule_arete*) malloc(sizeof(Cellule_arete));
            if(new_arete == NULL){
                fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);

                free(H-> tab);
                free(H);

                return NULL;
            }

            new_arete-> a = a;
            new_arete-> suivant = NULL;

            inserer_en_tete_arete(&(new-> L_Voisins), new_arete);

            tmp_vois = tmp_vois-> suiv;     
        }

        tmp_noeud = tmp_noeud-> suiv;
    }

    free(H-> tab);
    free(H);

    return G;
}

int min_nombre_arete(Graphe* G, Sommet* u, Sommet* v){
    /*
    Retourne le nombre minimal d'arêtes entre u et v dans le graphe G.
    Sous condition que u et v soient dans le graphe.
    */

    int min = -1;
    int* visiter = (int*) calloc((G-> nbSom), sizeof(int));
    if(visiter == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return -1;
    }

    S_file* file = (S_file*) malloc(sizeof(S_file));
    Init_file(file);
    enfile(file, u-> num);
    visiter[u-> num] = 1;

    int len = 1;
    int tmp_len = 1;

    while(len > 0){
        min = min + 1;
        for(int i = 0; i < len; i++){
            Sommet* som_def = sommetNum(G, defile(file));
            tmp_len--;
            
            if(visiter[som_def-> num] == 0){
                visiter[som_def-> num] = 1;
            }
            if((som_def-> num) == v-> num){
                return min;
            }
        
            Cellule_arete* tmp = som_def-> L_Voisins;
            while(tmp){
                if(visiter[(tmp-> a)-> u] == 0){
                    enfile(file, (tmp-> a)-> u);
                    tmp_len++;
                }
                if(visiter[(tmp-> a)-> v] == 0){
                    enfile(file, (tmp-> a)-> v);
                    tmp_len++;
                }
                
                tmp = tmp-> suivant;
            }
        }
        len = tmp_len;
    }

    free(visiter);
    detruire_file(file);

    return min;
}


S_file* plus_court_chemin(Graphe* G, Sommet* u, Sommet* v){
    /*
    Retourne une liste d'entiers, correspondants au plus court chemin entre u et v.
    */
    
    // Création du tableau predecesseurs qui nous permettera de stocker le plus court chemin.
    int predecesseurs[G-> nbSom];
    for(int i = 0; (G-> nbSom) > i; i++){
        predecesseurs[i] = -1;
    }
    predecesseurs[u-> num] = -2;

    // Création de la liste résulat à retourner.
    S_file* res = (S_file*) malloc(sizeof(S_file));
    if(res == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }
    Init_file(res);

    // Création de la file.
    S_file* f = (S_file*) malloc(sizeof(S_file));
    if(f == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        free(res);
        return NULL;
    }
    Init_file(f);
    enfile(f, u-> num);

    int numero;
    Sommet* som_def;
    Cellule_arete* tmp;

    while(!estFileVide(f)){
        numero = defile(f);
        if(numero == v-> num){
            while(numero != u-> num){
                enfile_tete(res, numero);
                numero = predecesseurs[numero];
            }
            enfile_tete(res, u-> num);
        }
        som_def = sommetNum(G, numero);
        tmp = som_def-> L_Voisins;
        while(tmp != NULL){
            if(predecesseurs[(tmp-> a)-> u] == -1){
                enfile(f, (tmp-> a)-> u);
                predecesseurs[(tmp-> a)-> u] = som_def-> num;
            }
            if(predecesseurs[(tmp-> a)-> v] == -1){
                enfile(f, (tmp-> a)-> v);
                predecesseurs[(tmp-> a)-> v] = som_def-> num;
            }
            tmp = tmp-> suivant;
        }
    }

    free(f);
    return res;
}

int reorganiseReseau(Reseau* r){
    /*
    La fonction reorganise Reseau:
        - Crée le graphe correspondant au réseau.
        - Calcule la plus courte chaı̂ne pour chaque commodité.
        - Retourne vrai si pour toute arête du graphe, le nombre de chaı̂nes qui passe par cette arête est inférieur à γ, et faux sinon.
    */

    Graphe* G = creerGraphe(r);
    if(G == NULL){
        return 0;
    }

    S_file** T_commodite = (S_file**) calloc((G-> nbcommod), sizeof(S_file*));
    if(T_commodite == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        detruireGraphe(G);
        return 0;
    }
    for(int i = 0; (G-> nbcommod) > i; i++){
        T_commodite[i] = plus_court_chemin(G, sommetNum(G, ((G-> T_commod[i]).e1)), sommetNum(G, ((G-> T_commod[i]).e2)));
        if(T_commodite[i] == NULL){
            for(int j = 0; j < i; j++){
                detruire_file(T_commodite[j]);
            }
            free(T_commodite);
            detruireGraphe(G);
            return 0;
        }
    }

    int** matrix = (int**) calloc((G-> nbSom), sizeof(int*));
    if(matrix == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        for (int i = 0; (G-> nbcommod) > i; i++){
            detruire_file(T_commodite[i]);
        }
        free(T_commodite);
        detruireGraphe(G);
        return 0;
    }

    for(int i = 0; (G-> nbSom) > i; i++){
        matrix[i] = (int*) calloc((G-> nbSom), sizeof(int));
        if(matrix[i] == NULL){
            fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
            for(int j = 0; j < i; j++){
                free(matrix[j]);
            }
            free(matrix);
            for(int j = 0; j < G-> nbcommod; j++){
                detruire_file(T_commodite[j]);
            }
            free(T_commodite);
            detruireGraphe(G);
            return 0;
        }
    }

    for(int i = 0; (G-> nbcommod) > i; i++){
        if(estFileVide(T_commodite[i])){
            fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors du calcule de la plus courte chaine.\n", __FILE__, __LINE__);
            for(int j = 0; j < G-> nbcommod; j++){
                detruire_file(T_commodite[j]);
            }
            free(T_commodite);
            for(int j = 0; (G-> nbSom) > j; j++){
                free(matrix[j]);
            }
            free(matrix);
            detruireGraphe(G);
            return 0;
        }

        Cellule_file* tmp_file = (T_commodite[i])-> tete;
        int i = -1;
        int j = -1;

        while(tmp_file-> suiv != NULL){
            i = tmp_file-> val;
            j = (tmp_file-> suiv)-> val;

            (matrix[i][j])++; (matrix[j][i])++;

            if(matrix[i][j] > (G-> gamma)) {
                for(int j = 0; j < G-> nbcommod; j++){
                    detruire_file(T_commodite[j]);
                }
                free(T_commodite);
                for(int j = 0; (G-> nbSom) > j; j++){
                    free(matrix[j]);
                }
                free(matrix);
                detruireGraphe(G);
                return 0;
            }

            tmp_file = tmp_file-> suiv;
        }
    }

    /* On libère toute la mémoire allouée dynamiquement */

    for(int i = 0; (G-> nbSom) > i; i++){
        free(matrix[i]);
    }
    free(matrix);

    for (int i = 0; (G-> nbcommod) > i; i++){
        detruire_file(T_commodite[i]);
    }
    free(T_commodite);

    detruireGraphe(G);

    return 1;
}

int main(int argc, char** argv){

    char* nomf = "instance1.cha";

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

    //afficher_file(plus_court_chemin(G, sommetNum(G, 10), sommetNum(G, 9)));

    detruireChaine(C);
    detruireReseau(R);
    detruireGraphe(G);

    fclose(f);

    return EXIT_SUCCESS;
}
