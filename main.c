#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "db_helper.h"

//takes the persons id and returns their login name
char **returnPersonLogin(char* id){
    return getValue("person_ids", id, 0, 1);
}

//takes the persons roleid and returns their resourceid
char **returnResourceRolesID(char* roleId){
    return getValue("resources_roles", roleId, 1, 0);;
}

//takes persons id and returns their roleids
char **returnRoleId(char* id){
    return getValue("person_roles", id, 0, 1);
}

//takes persons id and returns the expiration dates
char **returnExpDate(char* id){
    return getValue("person_roles", id, 0, 2);
}

//takes the resource id and returns the persons role name 
char **returnRolesName(char* resourceID){
    return getValue("roles", resourceID, 0, 1);
}

//takes resource id and returns the resource name
char **returnNameOfResource(char* resourceID){
    return getValue("resources", resourceID, 0, 1);
}

//returns an array of 0 or 1s that signify if a role is expired or not
char **isExpired(char* id){
    char **expired = (char**) malloc(5*10*sizeof(char));
    char **expDate = getValue("person_roles", id, 0, 2);
    
    int index = 0;
    
    //test: display expiration dates
    if (expDate != NULL){
        printf("\n");
      while (index != -1) {
        printf("Expiring Date[%d]: %s\n", index, expDate[index]);
        index = expDate[index+1] != NULL ? index + 1 : -1;
      }
    }
    
    //save the computers clock time
    char *currentDate = malloc(10 * sizeof(char));
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(currentDate, sizeof(currentDate)+1, "%Y%m%d", t);
    
    //convert the curent date to an int
    int *dateInt = malloc(9 * sizeof(int));
    sscanf(currentDate, "%d", dateInt);
    
    //print the date
    printf("Date: %d\n", *dateInt);
    
    int expDateInt = 0;
    
    index = 0;
    
    //go thru the expired dates of the person and compare to the current date
    //in a new char array save if the expiration date being tested against the
    //current date is expired or not: 0 for not expired, 1 for expired
    while(index != -1){
        //current the expiration date to be compared to an int
        expDateInt = atoi (expDate[index]);
        
        //print the dates being compared
        printf("DateCompared: %d, %d\n", expDateInt, *dateInt);
        
        //compare the dates
        int a = (expDateInt - *dateInt);
        
        printf("answer: %d", a);
        
            //if the result is larger than zero than the role is not expired
            if((int) a > (int) 0){
                printf("\nfalse\n");
                expired[index] = "0"; // false
            } else {
                printf("\ntrue\n");
                expired[index] = "1"; // true 
            }
        
        index = expDate[index+1] != NULL ? index + 1 : -1;
    }
    
    return expired;
}

//takes in the TUID and returns an array of strings 
//in the array the first element is the roleID
//the second element is the ExperationDate
//the array continues this pattern for all roles they
//are assigned to
char **returnValidRoles(char* id){
    char **roleId = returnRoleId(id);
    char **expDate = returnExpDate(id);
    char **expiredList = isExpired(id);
    int index = 0;
    
    //test: print role ids
    while(roleId[index]!= NULL){
        printf("roleid: %s\n", roleId[index]);
        index++;
    }
    
    //test: print expire dates
    index = 0;
    while(expDate[index]!= NULL){
        printf("expDate: %s\n", expDate[index]);
        index++;
    }
    
    //test print expired list: 0 or 1
    index = 0;
    while(expiredList[index]!= NULL){
        printf("expList: %s\n", expiredList[index]);
        index++;
    }
    
    //this is the string array to save the valid rolls the
    //person is approved for and is not expired
    char **rollNums = malloc(7*sizeof(char*));

    index = 0;
    int index2 = 0;
    
    if (roleId != NULL){
      while (index != -1) {
          
          //if not expired save the role id and the expiring date
          //saved on alternating lines (1st line: role id, 2nd line expire date)
          if(strncmp (expiredList[index],"0",1) == 0){
              
              //save role id
              rollNums[index2] = roleId[index];
              printf("rollNums: %s\n", rollNums[index2]);
              index2++;
              
              //save expire date
              rollNums[index2] = expDate[index];
              index2++;
          }
        index = expDate[index+1] != NULL ? index + 1 : -1;
      }
      
    }
    
    return rollNums;
}

//returns a string id for the persons full name
char **returnPersonName(char* id){
    char **fullname = (char**) malloc(5*(30*sizeof(char)));
    char **answer = (char**) malloc(4*(30*sizeof(char)));
    
    //array[0]: first, [1] middle, [2] last, [3]prefered
    for(int i = 1; i < 5; i++){
        answer = getValue("person_names", id, 0, i);

        fullname[i-1] = answer[0];
    }
    
    return fullname;
} // end returnPersonName

// asks user to input data and keeps looping till they enter a
// valid TUID (9 characters - only digits and alpha)
// returns the valid ID
char* returnID(){
    char *id = malloc(20 * sizeof(char));
    
    //keeps running until the user enters a valid checkable id
    while(!0){
        
        printf("Please Enter a TUID:\n");
        scanf("%s", id);
        
        int i = 0;
        int boolean = 0;
        int numCount = 0;
        
        // ensure input was correct
        while(id[i] != '\0'){
            
            //check if char is a letter
            if(isalpha(id[i])){
                numCount++;
                
            //check if char is a digit    
            } else if(isdigit(id[i])){
                numCount++;
                
            //else if there is any other symbols boolean becomes 1
            //which means there in another character present and
            //entered id is invalid
            } else {
                boolean = 1;
            } 
            i++;
        }
        
        printf("Num of Chars: %d\n", numCount);
        
        //if the count of alpha and nums is 9 together and there are no
        //other symbols or white space present then the id is valid.
        //return the id or otherwise print something is wrong and restart
        //the loop
        if(numCount == 9 && boolean == 0){
            printf("You entered:%s", id);
            return id;
        } else {
            printf("something wrong\n");
            
        }
    }
}

//gets the user info and writes the persons id and resources to the
//person_resource file
void returnPersonsData(){
    //try to open the file to save the entries too
    FILE *f = fopen("person_resource", "w");
    
    //if file cannot be opened cancel the process
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    //get user input id
    char *id = returnID();
    printf("\nUSER ENTERED ID: %s\n", id);
    
    //check if persons id exists if it does return their login name and
    //continue searching for their information
    char **personLogin = returnPersonLogin(id);
    
    if(personLogin[0] != NULL){
        printf("\nPerson Exists, Persons Login: %s\n", personLogin[0]);
        
        //Get Persons Full Name
        char **personFullName = returnPersonName(id);
        printf("\nFirst Name: %s", personFullName[0]);
        printf("\nMiddle Name: %s", personFullName[1]);
        printf("\nLast Name: %s", personFullName[2]);
        printf("\nPreferred Name: %s\n", personFullName[3]);
        
        //Check what roles are still valid and not expired
        //Return the roleIDs that are not expired and the
        //Dates that those roles do expire 
        char **validRolesIds = returnValidRoles(id);
        
        //if their are no valid roles because they are all expired say no access
        if(validRolesIds[0] != NULL){
            int index = 0;
            int index2 = 0;
            
            char **validRoleNames = malloc(10*sizeof(char*));
            char **validResources = malloc(10*sizeof(char*));
            
            while(validRolesIds[index] != NULL){
                
                
                printf("\nvalidRole ID: %s\n", validRolesIds[index]);
                
                //get the role name associated with the role id
                char **returnedRoleNames = returnRolesName(validRolesIds[index]);
                validRoleNames[index2] = returnedRoleNames[0];
                
                printf("Role Name: %s\n", validRoleNames[index2]);
                
                //get the resources they can use
                char **returnedRoleIds = returnResourceRolesID(validRolesIds[index]);
                
                int index4 = 0;
                
                if(returnedRoleIds[0] != NULL){
                    while(returnedRoleIds[index4] != NULL){
                        char **returnResources = returnNameOfResource(returnedRoleIds[index4]);
                        validResources[index4] = returnResources[0];
                        printf("Resource ID: %s\n", returnedRoleIds[index4]);
                        printf("Resource Name: %s\n", validResources[index4]);
                        
                        //Write their id to the file
                        fprintf(f, "\n%s#", id);
                        
                        //Write their resouce to the file
                        fprintf(f, "%s#", validResources[index4]);
                        
                        //Write their first and lastname to file
                        fprintf(f, "%s%s", personFullName[0],personFullName[2]);
                        
                        index4++;
                    }
                }
                
                index++;
                index2++;
                
                //get the expiring date for the rolesId
                printf("Expire Date For Role: %s\n", validRolesIds[index]);
                index++;
                   
            }
            
        } else{
            printf("PERSON IN DATABASE, THEIR ACCESS TO RESOURCES HAS EXPIRED!");
        }
        
    } else {
        printf("PERSON DOES NOT EXIST IN DATABASE!");
    }
    

}

int main(int argc, char **argv) {
    returnPersonsData();
    return 0;
}