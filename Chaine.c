#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>


#include "Chaine.h"
#include "SVGwriter.h"
 

/* ~~~~~~ Fonctions auxiliaires ~~~~~~ */
void inserer_en_tete_point(CellPoint** L, double x, double y){
    CellPoint* new = malloc(sizeof(CellPoint));
    if(new == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return;
    }

    new-> x = x;
    new-> y = y;
    new-> suiv = *L;
    *L = new;
}

CellPoint* inserer_en_queue_point(CellPoint* L, double x, double y){
    CellPoint* new = malloc(sizeof(CellPoint));
    if(new == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    new-> x = x;
    new-> y = y;
    new-> suiv = NULL;

    CellPoint* tmp = L;
    if(tmp == NULL){
        return new;
    }
    while(tmp-> suiv){
        tmp = tmp-> suiv;
    }

    tmp-> suiv = new;
    return L;
}

CellPoint* lecturePoints(char* str, int nbPoints){
    
    CellPoint* res = NULL;
    double x;
    double y;

    while(nbPoints--){

        x = 0;
        y = 0;

        // Lecture de x:
        while(*str != ' '){ 

            if(*str == '.'){
                str++;
                double dec = 0.1;
                while(*str != ' '){
                    x = x + (*str - '0') * dec;
                    dec = dec / 10.0;
                    str++;
                }
            }
            if(*str == ' '){
                break;
            }

            x = x * 10 + (*str - '0');
            str++;
        }

        // Saut de l'espace entre x et y:
        str++;

        // Lecture de y:
        while(*str != ' ' && *str != '\0' && *str != '\n'){    
            if(*str == '.'){
                str++;
                double dec = 0.1;
                while(*str != ' ' && *str != '\0' && *str != '\n'){
                    y = y + (*str - '0') * dec;
                    dec = dec / 10.0;
                    str++;
                }
            }

            if(*str == ' ' || *str == '\0' || *str == '\n'){
                break;
            }

            y = y * 10 + (*str - '0');
            str++;
        }

        // Ajout du point dans la liste:
        res = inserer_en_queue_point(res, x, y);
        str++;
    }
    return res;
}

CellChaine* lectureChainesAux(FILE *f, int nbChaines, char* buffer){

    int numChaine = 0;
    int nbPoints = 0;

    CellChaine* res = NULL; // La liste chainée à retourner.
    CellChaine* last = NULL; // Pour une insertion plus rapide à la fin.

    while((feof(f) == 0) && (nbChaines-- > 0)){
        fgets(buffer, 100000, f);

        char* tmp = buffer;
        // Lecture du numéro de la chaine:
        while(*tmp != ' '){
            numChaine = numChaine*10 + (*tmp - '0');
            tmp++;
        }
        // Saut de l'espace entre le numéro de la chaine et le nombre de points:
        tmp++;

        // Lecture du nombre de points:
            while(*tmp != ' '){
            nbPoints = nbPoints*10 + (*tmp - '0');
            tmp++;
        }
        // Saut de l'espace entre le numéro de la chaine et le nombre de points:
        tmp++;

        // Lecture des points:
        CellPoint* points = lecturePoints(tmp, nbPoints);

        // Création du nouvel élement:
        CellChaine* new = malloc(sizeof(CellChaine));
        if(new == NULL){
            fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
            return NULL;
        }
        new-> numero = numChaine;
        new-> points = points;
        new-> suiv = NULL;

        // Insertion du nouvel élement:
        if(last == NULL){
            res = new;
            last = new;
        }
        else{
            last-> suiv = new;
            last = new;
        }

        numChaine = 0;
        nbPoints = 0;
    }
    return res;
}

void detruireChaine(Chaines *C){

    CellChaine* tmp = C-> chaines;
    CellChaine* next = NULL;
    while(tmp){
        next = tmp-> suiv;
        CellPoint* ptmp = tmp-> points;
        CellPoint* pnext = NULL;
        while(ptmp){
            pnext = ptmp-> suiv;
            free(ptmp);
            ptmp = pnext;
        }

        free(tmp);
        tmp = next;
    }
    free(C);
}

void ecrirePoints(CellPoint* L, FILE *f){
    CellPoint* tmp = L;
    while(tmp){
        fprintf(f, "%.2f %.2f ", tmp-> x, tmp-> y);
        tmp = tmp-> suiv;
    }
}

int nbPointsChaine(CellChaine* L){
    int res = 0;
    CellPoint* tmp = L-> points;
    while(tmp){
        res++;
        tmp = tmp-> suiv;
    }
    return res;
}

double distanceEuclidienne(CellPoint* p1, CellPoint* p2){
    double dx = (p2-> x - p1-> x) * (p2-> x - p1-> x);
    double dy = (p2-> y - p1-> y) * (p2-> y - p1-> y);
    return sqrt(dx + dy);
}

double roundToTwoDecimalPlaces(double x){
    return roundf(x * 100) / 100;
}

double signature_de_liaison(double x1, double y1, double x2, double y2){
    /*
    Retourne une clé à priori unique de liaison entre deux points.
    */
    double cle1 = y1 + ((x1 + y1)*(x1 + y1 + 1) / 2);
    double cle2 = y2 + ((x2 + y2)*(x2 + y2 + 1) / 2);

    return cle1 * cle2;
}

int ajout_de_liaison(Liaison** L, double sig){
    /*
    Ajoute une liaison de signature sig si elle n'existe pas, met à jour son occurence sinon.
    Retourne le nombre d'occurence de la liaison.
    */
    Liaison* tmp = *L;
    while(tmp){
        if(tmp-> sig == sig){
            (tmp-> occ)++;
            return (tmp-> occ);
        }
        tmp = tmp-> suiv;
    }

    // Ajout de la liaison:
    Liaison* new = malloc(sizeof(Liaison));
    if(new == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return -1;
    }
    new-> sig = sig;
    new-> occ = 1;
    new-> suiv = *L;
    *L = new;

    return 1;
}

void detruireLiaison(Liaison* L){
    Liaison* tmp = L;
    Liaison* next = NULL;
    while(tmp){
        next = tmp-> suiv;
        free(tmp);
        tmp = next;
    }
}

/* ~~~~~~ Fonctions principales ~~~~~~ */

// Fonction de lecture:
Chaines* lectureChaines(FILE *f){

    Chaines* res = malloc(sizeof(Chaines));
    if(res == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }
    
    char* buffer = malloc(sizeof(char) * 100000);

    // Lecture du nombre de chaines:
    fgets(buffer, 100000, f);
    sscanf(buffer, "NbChain: %d", &(res-> nbChaines));

    // Lecture de la valeure de Gamma:
    fgets(buffer, 100000, f);
    sscanf(buffer, "Gamma: %d", &(res-> gamma));

    // Lecture des chaines:
    res-> chaines = lectureChainesAux(f, res-> nbChaines, buffer);

    free(buffer);
    return res;
}

// Fonction d'écriture:
void ecrireChaines(Chaines *C, FILE *f){
    fprintf(f, "NbChain: %d\n", C-> nbChaines);
    fprintf(f, "Gamma: %d\n", C-> gamma);

    CellChaine* tmp = C-> chaines;
    while(tmp){
        fprintf(f, "%d %d ", tmp-> numero, nbPointsChaine(tmp));
        ecrirePoints(tmp-> points, f);
        fprintf(f, "\n");
        tmp = tmp-> suiv;
    }
}

// Représentation graphique des instances:
void afficheChainesSVG(Chaines* C, char* nomInstance){
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

// La longeur physique d'une chaine:
double longueurChaine(CellChaine* C){
    if(C == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur chaine vide !.\n", __FILE__, __LINE__);
        return -1;
    }

    if(C-> points == NULL){
        return 0;
    }
    
    double res = 0.;
    CellPoint* tmp = C-> points;

    while(tmp-> suiv != NULL){
        res += distanceEuclidienne(tmp-> suiv , tmp);
        tmp = tmp-> suiv;
    }

    return res;
}

// La longeur totale des chaines:
double longueurTotale(Chaines* C){
    if(C == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur ensemble de chaines null !.\n", __FILE__, __LINE__);
        return -1;
    }

    double res = 0;
    CellChaine* tmp = C-> chaines;
    while(tmp){
        res += longueurChaine(tmp);
        tmp = tmp-> suiv;
    }

    return res;
}

int comptePointsTotal(Chaines* C){
    if(C == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur ensemble de chaines null !.\n", __FILE__, __LINE__);
        return -1;
    }

    int res = 0;
    CellChaine* tmp = C-> chaines;
    while(tmp){
        res += nbPointsChaine(tmp);
        tmp = tmp-> suiv;
    }

    return res;
}

// Géneration de chaines aléatoire:
Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax, int ymax){
    
    // Allocation et initialisation de la structure Chaines:
    Chaines* C = malloc(sizeof(Chaines));
    if(C == NULL){
        fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
        return NULL;
    }

    C-> nbChaines = nbChaines;
    C-> chaines = NULL;
    C-> gamma = 1;

    // Déclaration des variables temporaires:
    int num = nbChaines - 1;

    // CellPoint* last_point;

    // double sig = 0.;
    double x = 0.;
    double y = 0.;

    // Liaison* liaison = NULL; // Liste de liaison pour mettre à jour la valeur de gamma.
    // int tgamma = 0;

    for(int i = 0; i < nbChaines; i++){
        // Création d'une nouvelle chaine:
        CellChaine* new_chaine = malloc(sizeof(CellChaine));
        if(new_chaine == NULL){
            fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
            return NULL;
        }
        new_chaine-> numero = num--;
        new_chaine-> points = NULL;
        new_chaine-> suiv = C-> chaines;

        for(int j = 0; j < nbPointsChaine; j++){
            x = roundToTwoDecimalPlaces(((rand() * 1.0) / RAND_MAX) * xmax);
            y = roundToTwoDecimalPlaces(((rand() * 1.0) / RAND_MAX) * ymax);

            // Ajout du point dans la chaine:
            inserer_en_tete_point(&(new_chaine-> points), x, y);

            // // Ajout de la liaison:
            // if(last_point != NULL){
            //     sig = signature_de_liaison(last_point-> x, last_point-> y, x, y);
            //     tgamma = ajout_de_liaison(&liaison, sig);
            //     (C-> gamma) = (tgamma > (C-> gamma)) ? tgamma : (C-> gamma);
            // }

            // last_point = new_chaine-> points;       
        }

        // Ajout de la chaine dans l'ensemble:
        new_chaine-> suiv = C-> chaines;
        C-> chaines = new_chaine;
    }

    // Libération de la mémoire allouée pour la liste de liaison:
    // detruireLiaison(liaison);

    return C;
}








