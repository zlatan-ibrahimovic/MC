int nb_sommet(){
return 27;}

int est_adjacent(int u, int v){
 if(0< u && 0<v && u<nb_sommet()+1 && v<nb_sommet()+1){
  int i,j;
  i=u-1;
  j=v-1;
  if((i/3==j/3) && ((i/9==j/9) || (i/9==j%3 && j/9==i%3)))
   return 1; 
  if(i/3==j%3 && i/3==(j/3)%3 && j/3==i%3 && j/3==(i/3)%3)
   return 1;
  }
 return 0;
}
