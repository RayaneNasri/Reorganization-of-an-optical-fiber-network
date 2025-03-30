#include <stdio.h>
#include <stdlib.h>

#include "SVGwriter.h"
#include "Reseau.h"

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */

Noeud* creer_noeud(int num, double x, double y){

    Noeud* nd = (Noeud*) malloc(sizeof(Noeud));
    if(nd == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    nd-> num = num;
    nd-> x = x;
    nd-> y = y;
    nd-> voisins = NULL;

    return nd;
}

void inserer_en_tete_noeud(CellNoeud** L, Noeud* nd){
    
    /*
    Insère le noeud nd en tête de la liste de noeuds L.
    */

    CellNoeud* new_cell = (CellNoeud*) malloc(sizeof(CellNoeud));
    if(new_cell == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return;
    }
    new_cell-> nd = nd;
    new_cell-> suiv = (*L);
    (*L) = new_cell;
}

void update_voisins(Noeud* nd1, Noeud* nd2){

    /*
    Met à jour la liste des voisins des noeuds nd1 et nd2,
    sachant qu'ils sont voisins.
    */
    CellNoeud* tmp = nd1-> voisins;
    while(tmp != NULL){
        if( ((tmp-> nd)-> num) == nd2-> num) return;
        tmp = tmp-> suiv;
    }

    inserer_en_tete_noeud(&(nd2-> voisins), nd1);
    inserer_en_tete_noeud(&(nd1-> voisins), nd2);
}

void inserer_en_tete_commodite(CellCommodite** L, Noeud* extrA, Noeud* extrB){
    /*
    Insère la commodite (extrA, extrB) en tête de la liste de commodites L.
    */

    CellCommodite* new_cell = (CellCommodite*) malloc(sizeof(CellCommodite));
    if(new_cell == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return;
    }

    new_cell-> extrA = extrA;
    new_cell-> extrB = extrB;
    new_cell-> suiv = (*L);
    (*L) = new_cell;
}

void detruireReseau(Reseau* res){
    /*
    Libère toute la mémoire associée au réseau res.
    */
    if(res == NULL){
        return;
    }

    // Libération de la mémoire associée aux commodités:
    CellCommodite* tmp_com;
    while(res-> commodites){
        tmp_com = (res-> commodites);
        (res-> commodites) = (res-> commodites)-> suiv;
        free(tmp_com);
    }

    // Libération de la mémoire associée aux noeuds et à leurs voisins:

    CellNoeud* tmp;
    CellNoeud* tmp_voisin;

    while(res-> noeuds){

        tmp = (res-> noeuds);
        tmp_voisin = (tmp-> nd)-> voisins;
        
        while(tmp_voisin){
            (tmp-> nd-> voisins) = (tmp-> nd-> voisins)-> suiv;
            free(tmp_voisin);
            tmp_voisin = tmp-> nd-> voisins;
        }

        (res-> noeuds) = (res-> noeuds)-> suiv;

        free(tmp-> nd);
        free(tmp);
    }

    free(res);

}

/* ~~~~~~ Fonctions principales ~~~~~~ */

Noeud* rechercheCreeNoeudListe(Reseau* R, double x, double y){
    if(R == NULL){
        fprintf(stderr, "Erreur dans les paramètres, le reseau est null !, FILE: %s, LIGNE: %d\n", __FILE__, __LINE__);
        return NULL;
    }

    CellNoeud* tmp = R-> noeuds;
    Noeud* ntmp;

    while(tmp != NULL){
        ntmp = tmp-> nd;
        if((ntmp-> x == x) && (ntmp-> y == y)) return ntmp;
        tmp = tmp-> suiv;
    }

    // Création du nouveau noeud:
    Noeud* new_nd = creer_noeud((R-> nbNoeuds)++, x, y);

    // Ajout du nouveau noeud dans la liste de noeuds du réseau:
    inserer_en_tete_noeud(&(R-> noeuds), new_nd);
    
    return new_nd;
}

Reseau* reconstitueReseauListe(Chaines* C){
    if(C == NULL){
        return NULL;
    }

    Reseau* res = (Reseau*) malloc(sizeof(Reseau));
    if(res == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }
    
    
    res-> nbNoeuds = 0;
    res-> gamma = C-> gamma;

    // On utilise un ensemble de nœuds V qui est initialisé vide: V ← ∅
    res-> noeuds = NULL;

    res-> commodites = NULL;

    CellChaine* tmp_chaine = C-> chaines;

    CellPoint* tmp_point;
    Noeud* tmp_nd;
    Noeud* prec_nd;

    Noeud* extrA;
    Noeud* extrB;

    // On parcourt une à une chaque chaîne:
    while(tmp_chaine){
        prec_nd = NULL;
        tmp_point = tmp_chaine-> points;

        // Pour chaque point p de la chaîne:
        while(tmp_point){

            // Si p no∈ V (on teste si le point n’a pas déjà été rencontré auparavant).
            // On ajoute dans V un nœud correspond au point p.
            tmp_nd = rechercheCreeNoeudListe(res, tmp_point-> x, tmp_point-> y);

            if(prec_nd != NULL){

                // On met à jour la liste des voisins de p et celles de ses voisins.
                update_voisins(tmp_nd, prec_nd);
                
            }
            else{
                extrA = tmp_nd;
            }

            if(tmp_point-> suiv == NULL){
                extrB = tmp_nd;
            }

            prec_nd = tmp_nd;
            tmp_point = tmp_point-> suiv;
        }

        inserer_en_tete_commodite(&(res-> commodites), extrA, extrB);
        tmp_chaine = tmp_chaine-> suiv;
    }
    
    return res;
}

int nbCommodites(Reseau* R){

    int res = 0;
    CellCommodite* tmp = R-> commodites;
    while(tmp != NULL){
        res = res + 1;
        tmp = tmp-> suiv;
    }
    return res;
}

int nbLiaisons(Reseau* R){

    int res = 0;

    CellNoeud* tmp = R-> noeuds;
    CellNoeud* tmp_voisin;

    while(tmp != NULL){
        tmp_voisin = (tmp-> nd)-> voisins;
        while(tmp_voisin != NULL){
            res = res + 1;
            tmp_voisin = tmp_voisin-> suiv;
        }
        tmp = tmp-> suiv;
    }
    return res / 2;

}

void ecrireReseau(Reseau* R, FILE* f){
    fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\nNbCommodites: %d\nGamma: %d\n\n", R-> nbNoeuds, nbLiaisons(R), nbCommodites(R), R-> gamma);

    CellNoeud* tmp = R-> noeuds;
    CellNoeud* tmp_voisin;

    while(tmp != NULL){
        fprintf(f, "v %d %f %f\n", (tmp-> nd)-> num, (tmp-> nd)-> x, (tmp-> nd)-> y);
        tmp = tmp-> suiv;
    }
    fprintf(f, "\n");

    tmp = R-> noeuds;
    int dejaVus[R-> nbNoeuds];

    while(tmp != NULL){
        dejaVus[(tmp-> nd)-> num] = 1;

        tmp_voisin = (tmp-> nd)-> voisins;
        while(tmp_voisin != NULL){
            if(dejaVus[(tmp_voisin-> nd)-> num] == 1){
                tmp_voisin = tmp_voisin-> suiv;
                continue;
            }
            fprintf(f, "l %d %d\n", (tmp_voisin-> nd)-> num, (tmp-> nd)-> num);
            tmp_voisin = tmp_voisin-> suiv;
        }

        tmp = tmp-> suiv;
    }
    fprintf(f, "\n");

    CellCommodite* tmp_com = R-> commodites;
    while(tmp_com != NULL){
        fprintf(f, "k %d %d\n", (tmp_com-> extrA)-> num, (tmp_com-> extrB)-> num);
        tmp_com = tmp_com-> suiv;
    }
}

void afficheReseauSVG(Reseau* R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

