#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include <string.h>

// rajoute la clause pour qu'au moins une extrêmité (i ou j)
// soit dans la couverture 
int representer_arrete(int i, int j,int k,FILE* fichier){
  for(int t=0;t<k*nb_sommet();t=t+nb_sommet()){
    fprintf(fichier,"%d %d ",i+t,j+t);
  }
  fprintf(fichier,"0\n");
  return 1;
}


// algo principal de génération des clauses 
FILE* formula_for_if_exist_sizeof_k_corver(int k){
  FILE * fichier;
  
  //fichier d'entrée du SOLVER 
  fichier = fopen("SATformula.in", "w");
  int nb_var= nb_sommet() * k;
  int nb_clause=0;
  
  //on laisse de l'espace pour mettre nb_clause après l'écriture 
  //de toutes les clauses
  fprintf(fichier,"%s %s %d %s\n","p","cnf",nb_var,"              ");
  
  /*********************************************************
   il y a au moins un sommet qui est le j ème
   sommet du graphe: 
   les couples (i,j) ==> i+[(j-1) * nb_sommet()]
   (1,1) | (2,1) | (3,1) | (4,1) | (5,1) ...... (n,1) &
   (1,2) | (2,2) | (3,2) | (4,2) | (5,2) ...... (n,2) &
                        :
                        :
                        :
   (1,k) | (2,k) | (3,k) | (4,k) | (5,k) ...... (n,k) 
  ********************************************************/
  for(int j= 1 ; j<=k ; j++) {
    for(int i = 0; i<nb_sommet()*k; i=i+k){    
      fprintf(fichier,"%d ",i+j);
    }
    fprintf(fichier,"0 \n");
    nb_clause++;
  }

  
  /**********************************************************
   Dans la couverture, il  n' y a pas deux sommets différents  
   qui ont la même position.
   -(1,1) | -(2,1) &
   -(1,1) | -(3,1) &
   -(1,1) | -(4,1) &
   -(1,1) | -(5,1) &
          :
          :
   -(1,1) | -(n,1) &
   -(2,1) | -(3,1) &
          :
          :
   -(2,1) | -(n,1) &
          :
          :
          :
 -(n-1,1) | -(n,1) &
   -(1,2) | -(2,2) &
          :
 -(n-1,2) | -(n,2) &
          :
          :
 -(n-1,k) | -(n,k) 
 *******************************************************/
  for(int j= 1 ; j<=nb_sommet()*k ; j=j+nb_sommet()) {
    int debut= j;
    int fin = j+nb_sommet()-1;
    for(int t=debut; t<=fin; t++){
      for(int i=t+1; i<=fin; i++){
	fprintf(fichier,"-%d -%d 0\n",t,i);
	nb_clause++;
      }
    }
  }
  
  /*************************************************************
    clauses qui vérifient que toutes les arêtes sont couvertes:
   
    si i ---- j est une arrête alors 
    soit i soit j appartient à la couverture:
    i ou j à la t ième position t € (1,2,..k)

    (i,1) | (j,1) &
    (i,2) | (j,2) &
          :
          :
    (i,k) | (j,k) &


  **************************************************************/
  for(int i=1; i<=nb_sommet(); i++) {
    for(int j=1; j<=nb_sommet(); j++){

      if(est_adjacent(i,j)){
	nb_clause+=representer_arrete(i,j,k,fichier);      
      }
    }
  }



  fseek(fichier, 0, SEEK_SET);
  fprintf(fichier,"%s %s %d %d","p","cnf",nb_var,nb_clause);
  
  


  return fichier;
}

int main(int argc, char* argv[]){
 
  FILE *fichier;
  if(!strcmp(argv[1],"-a")){
    printf("\t>Mode automatique\n");
    
    //éditions des liens avec le bon grphe
    char *commande=malloc(100*sizeof(*commande)); ;
    strcpy(commande,"gcc -std=c99 -Wall -Werror algo.c  ");
    strcat(commande,argv[2]);
    strcat(commande," -o tmp");
    printf("%s\n", commande);
    system(commande);
    
    char *cmd=malloc(20*sizeof(*cmd));
       
    for(int i = 1; i<nb_sommet(); i++) { 
      //nettoyage des fichiers nécessaires 
      system("rm SATformula.in");
      system("touch SATformula.in");
      system("rm tmp.out");
      system("touch tmp.out");
      int fin=5;
     
      //cmd= ./tmp
      cmd[0]='.';cmd[1]='/';cmd[2]='t';cmd[3]='m';cmd[4]='p';
      
      //cmd= ./tmp -m i    
      cmd[fin]=' ';cmd[fin+1]='-';cmd[fin+2]='m';cmd[fin+3]=' ';
      cmd[fin+4]=(char)((int)'0'+i);cmd[fin+5]='\n';
    
      printf("%s",cmd);
      system(cmd);
      
      //lancement du solveur glucose
      system("glucose-syrup/simp/./glucose SATformula.in > tmp.out");
      
      //lecture du résultat
      FILE * fichier_tempon = fopen("tmp.out","r");
      char str[15];
      fseek(fichier_tempon,-13,SEEK_END);
      fscanf(fichier_tempon, "%s",str);
    
      printf("result: %d %s\n",i,str);

    if(!strcmp(str,"SATISFIABLE")){
      system("clear");
      printf("\t*****************************************\n");
      printf("\t*         >taille min: %d                *\n",i);
      printf("\t*****************************************\n");
      i=nb_sommet();
    }
    fclose(fichier_tempon);
    }
    free(commande);
    free(cmd);
   
  } 
  if(!strcmp(argv[1],"-h")){
    system("clear");
    
    printf("----------------------------------------------------------------\n");
    printf("\t>usage \n");
    printf("\t./autoSAT [OPTION]  [FILE_GRAPH] ou [SIZEOF_CORVER]\n");
    printf("\t OPTION : \n");
    printf("\t\t> -m : Mode manuel: [SIZEOF_CORVER] \n");
    printf("\t\t> -a : Mode automatique [FILE_GRAPH]\n");
    printf("\n\n\t FILE_GRAPH : \n");
    printf("\t\tIndique le nom du fichier .c contenant le  graphe\n");
    printf("\t\t> EX: K7_11.c C10.c ...\n");
     printf("\n\n\t SIZEOF_CORVER : \n");
    printf("\t\tIndique la taile de la couverture pour le teste\n");
    printf("\t\t> EX: 1 2 3 ...\n");
    
    printf("---------------------------------------------------------------\n");
    
  }
  else{
   printf("\t>Mode manuel\n");
   int k = atoi(argv[2]);
   printf("nb sommet : %d\n",nb_sommet());
   fichier = formula_for_if_exist_sizeof_k_corver(k);
   fclose(fichier);
  }
  return 0;
}
