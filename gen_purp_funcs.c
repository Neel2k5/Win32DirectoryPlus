/**
  Copyright 2024 Swapnaneel Dutta

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   limitations under the License.

 */


#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "gen_purp_funcs.h"

//path limit hard code, change to 32767 if script deals with extended paths '\\?\'
#define PATH_LIM 260
bool gen_purp_validate_path(char *dir_path){
  size_t dir_path_size = strlen(dir_path), i=0;
  char temp_path[MAX_PATH],*wildcard_token;
  const char invalid[]= "<>/|?";
  bool backslash_presance =false,tokenpresence=false;

  if(dir_path_size==0){//Returns false if path is empty
    return false;
  }
  if(dir_path[dir_path_size-1]=='\\'){
    dir_path[dir_path_size-1]='\0';
    dir_path_size--;
  }
  while (dir_path[i]!='\0'){//Returns false when entire path is invalid
    if(strchr(invalid,dir_path[i++])){
      return false;
    }
    if(strchr(dir_path,'\\')){backslash_presance=true;}
  }
  if(!backslash_presance){//Returns false when entire path is invalid

    return false;
  }
  strncpy(temp_path,dir_path,dir_path_size);
  strrev(temp_path);
  wildcard_token=strtok(temp_path,"\\");
  //printf("(%s)\n",wildcard_token );  //For testing
  if(strstr(wildcard_token,"*.*")!=NULL){//Adds Neecessary wildcard token if not there already
    tokenpresence = true;
  }
  if(!tokenpresence){
    strcat(dir_path,"\\*.*");
  }
  return true;

}

void gen_purp_ext_extension(char *f_name){
  strrev(f_name);
  for(int i=0;i<strlen(f_name);i++){
    if(f_name[i]=='.'){
      f_name[i]='\0';
      break;
    }
  }
  strrev(f_name);
}

void gen_purp_ugdisplay(){
FILE *ug;

ug = fopen("ug_wdp.txt","r");
char buffer;
if(ug == NULL){fprintf(stderr,"\n ERROR : CANT FIND ug_wdp.txt IN DIRECTORY\n");return;}
fprintf(stdout,"\n");
while((buffer=fgetc(ug))!=EOF){
  fprintf(stdout,"%c",buffer);
} fclose(ug);
}


char *gen_purp_striplast(char *dir_path){
  char mutable_dir[PATH_LIM];
  strcpy(mutable_dir,dir_path);
  strrev(mutable_dir);
  char *last=strtok(mutable_dir,"\\");
  if(strcmp(last,"*")==0||strcmp(last,"*.*")==0){
    last=strtok(NULL,"\\");
    }
  strrev(last);
  printf("%s\n",last);//Testing
  return last;
  }
