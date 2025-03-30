#include <stdlib.h>
#include <stdio.h>
#include "Struct_File.h"

void Init_file(S_file *f){
  f->tete=NULL;
  f->dernier=NULL;
}

int estFileVide(S_file *f){
  return f->tete == NULL;
}

void enfile(S_file * f, int donnee){
 Cellule_file *nouv=(Cellule_file *) malloc(sizeof(Cellule_file));
  nouv->val=donnee;
  nouv->suiv=NULL;
  if (f->tete==NULL)
    f->tete=nouv;
  else
    f->dernier->suiv=nouv;
  f->dernier=nouv;
}


int defile(S_file *f){
  int v=f->tete->val;
  Cellule_file *temp=f->tete;
  if (f->tete==f->dernier)
    f->dernier=NULL;
  f->tete=f->tete->suiv;
  free(temp);
  return v;

}

int length(S_file *f){
  int i=0;
  Cellule_file *temp=f->tete;
  while(temp!=NULL){
    i++;
    temp=temp->suiv;
  }
  return i;
}

void enfile_tete(S_file* f, int donnee){
  Cellule_file* new = (Cellule_file*) malloc(sizeof(Cellule_file));
  if(new == NULL){
    fprintf(stderr, "FILE: %s, LINE: %d, Erreur lors de l'allocation memoire.\n", __FILE__, __LINE__);
    return;
  }
  new-> val = donnee;
  new-> suiv = NULL;
  if(f-> tete == NULL){
    f-> tete = f-> dernier = new;
  }
  else{
    new-> suiv = f-> tete;
    f-> tete = new;
  }
}

void afficher_file(S_file* f){
  Cellule_file* tmp = f-> tete;
  while(tmp != NULL){
    printf("%d ", tmp-> val);
    tmp = tmp-> suiv;
  }
  printf("\n");
}

void detruire_file(S_file* f){
  while(! estFileVide(f)){
    defile(f);
  }
  free(f);
}
