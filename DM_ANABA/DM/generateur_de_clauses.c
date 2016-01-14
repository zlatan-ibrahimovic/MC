#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include <string.h>

FILE * fichier; //input for glucose
 

void generateur_de_clause(int k){
  int nb_var= nb_sommet() * k;
  int nb_clause=0;
  
  //première ligne du fichier de glucose
  fprintf(fichier,"%s %s %d %s\n","p","cnf",nb_var,"              ");
  
  //chaque_position au moins un sommet
  for(int j= 1 ; j<=k ; j++) {
    for(int i = 0; i<nb_sommet()*k; i=i+k){    
      fprintf(fichier,"%d ",i+j);
    }
    fprintf(fichier,"0 \n");
    nb_clause++;
  }

  //au moins une extrêmité par arrête
  for(int i=1; i<=nb_sommet(); i++) {
    for(int j=1; j<=nb_sommet(); j++){

      if(est_adjacent(i,j)){
	for(int t=0;t<k*nb_sommet();t=t+nb_sommet()){
	  fprintf(fichier,"%d %d ",i+t,j+t);
	}
	fprintf(fichier,"0\n");
	nb_clause++;
      }
  
      //pas deux sommets différents a la même position
      for(int j= 1 ; j<=nb_sommet()*k ; j=j+nb_sommet()) {
	for(int t=j; t<=j+nb_sommet()-1; t++){
	  for(int i=t+1; i<=j+nb_sommet()-1; i++){
	    fprintf(fichier,"-%d -%d 0\n",t,i);
	    nb_clause++;
	  }
	}
      }
  
    }
  }
  

  //remplissage de l'entête du fichier input de glucose
  fseek(fichier, 0, SEEK_SET);
  fprintf(fichier,"%s %s %d %d","p","cnf",nb_var,nb_clause);
  
}

int main(int argc, char* argv[]){
 
 fichier = fopen("input.in", "w");
  int k = atoi(argv[1]);
  generateur_de_clause(k);
  fclose(fichier);

return 0;
}
