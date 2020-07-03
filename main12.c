#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "db_helper.h"

char *returnName(){
    char *name = malloc(10 * sizeof(char));
    
    scanf("%s", name);
    
    return name;
}

char **returnPersonName(char* id){
    
      
    char **answer = getValue("person_names", "912086676", 0, 1);
    
    printf("Answer[0]: %s", answer[0]);
    
//    getValue("person_names", id, 0, 2);
//    getValue("person_names", id, 0, 3);
//    getValue("person_names", id, 0, 4);
    
    
} // end returnPersonName

int numPlaces(int a){
   
    if (a < 100000000) return 8;
    if (a < 1000000000) return 9;
    return 10;

}

int *returnID(){
    int *id = malloc(10 * sizeof(int));
    char *garbage = malloc(10 * sizeof(char));
    

    while(!0){
        *id = -1;
        
        printf("Please Enter a TUID:\n");
        
        
        scanf("%s", garbage);
        
        printf("garbage is %s\n", garbage);
        
        sscanf(garbage, "%d", id);
        printf("ID is: %d\n", *id);
        
        if(numPlaces(*id) == 9){
            printf("found dig: ");
            printf("%d\n", *id);
            return id;
        } else {
            printf("something wrong\n");
        }
    }
}



int main(int argc, char **argv) {
  /* The code below is one example of how you might write tests. I've
     also included it to demonstrate how to use the getValue function. */
  char **val = getValue("resources_roles", "19", 1, 0);

  int index = 0;
  if (val != NULL){
    while (index != -1) {
      printf("val[%d]: %s\n", index, val[index]);
      index = val[index+1] != NULL ? index + 1 : -1;
    }
  }
  
  char *dude = returnName();
  printf("dude is %s", dude);
  
  returnID();
  
  /* This demonstrates how to use the newRole function, which is
     implemented in db_helper.c (declared in db_helper.h). */
  char *str = "student";
  Role *role = newRole(atoi(val[0]), "student");
  printf("Constructed the role \"object\" for the role, student.\n");
  return 1;
}

