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


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "feature_funcs.h"
#include "gen_purp_funcs.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        gen_purp_ugdisplay();
        return 0;
    }
    fprintf(stdout, "\n----------------------------------------------\nargs passed (exc appname and command): %d\nfor command: %s\n", argc - 2, argv[1]);

    if (strcmp(argv[1], "help") == 0) {
//         printf("\nreaches c1"); // Testing
  gen_purp_ugdisplay();
    } else if (strcmp(argv[1], "org") == 0) {
        if (argc < 3) {
            fprintf(stderr, "\nTOO LITTLE ARGUMENTS FOR org\n");
            return 0;
        } else if (argc > 3) {
            fprintf(stderr, "\nTOO MANY ARGUMENTS FOR org\n");
            return 0;
        }
        char dir_path[strlen(argv[2]) + 1];
        strcpy(dir_path, argv[2]);
        for (int i = 0; i < strlen(argv[2]); i++) {
            if (dir_path[i] == '\n') {
                dir_path[i] = '\0';
                break;
            }
        }
        feature_organise_dir(dir_path);
    } else if (strcmp(argv[1], "tks") == 0) {
      bool found_condition =false;//Return condition for recursive search
        if (argc < 3) {
            fprintf(stderr, "\nTOO LITTLE ARGUMENTS FOR tks\n");
            return 0;
        } else if (argc > 4) {
            fprintf(stderr, "\nTOO MANY ARGUMENTS FOR tks\n");
            return 0;
        }
        char dir_path[strlen(argv[2]) + 1];
        char token[strlen(argv[3]) + 1];
        strcpy(dir_path, argv[2]);
        if (argc == 4) {
            strcpy(token, argv[3]);
            for (int i = 0; i < strlen(argv[3]); i++) {
                if (token[i] == '\n') {
                    token[i] = '\0';
                    break;
                }
            }
            feature_searching(dir_path, token,&found_condition);
        } else {
            fprintf(stderr, "\nTOO MANY ARGUMENTS FOR OPERATION\n");
            // HELP FUN GOES HERE
        }
    }
    else if(strcmp(argv[1], "ds") == 0){
      if (argc < 5) {
            fprintf(stderr, "\nTOO LITTLE ARGUMENTS FOR ds\n");
            return 0;
        } else if (argc > 5) {
            fprintf(stderr, "\nTOO MANY ARGUMENTS FOR ds\n");
            return 0;
        }
        else if(!(strcmpi(argv[4],"-s")==0||strcmpi(argv[4],"-u")==0)){
          fprintf(stderr, "\nINVALID LAST ARGUMENT FOR ds\n");
            return 0;
        }
        feature_dir_sync(argv[2],argv[3],tolower(argv[4][1]));
    }

    else if(strcmp(argv[1],"dt")==0){
        if(argc<3){
           fprintf(stderr, "\nTOO LITTLE ARGUMENTS FOR dt\n");
           return 0;
        }
        else if(argc>3){
           fprintf(stderr, "\nTOO MANY ARGUMENT FOR dt\n");
          return 0;
        }
        else
          feature_dir_tree(argv[2],1);
    }

    else if(strcmp(argv[1],"tc")==0){// ./wdp.exe tc <dir_path> -ol <months> <-flag>

        if(argc<6){
           fprintf(stderr, "\nTOO LITTLE ARGUMENTS FOR dt\n");
           return 0;
        }
        else if(argc>6){
           fprintf(stderr, "\nTOO MANY ARGUMENT FOR dt\n");
          return 0;
        }
        else{
            if(!(strcmpi(argv[5],"-s")==0||strcmpi(argv[5],"-u")==0)/*||strcmpi(argv[3],"-ol")==0*/){
          fprintf(stderr, "\nINVALID ARGUMENT FOR tc\n");
            return 0;
        }
       
        feature_tc(argv[2],atoi(argv[4]),tolower(argv[5][1]),1);
        }
    }

    
    fprintf(stdout, "\n----------------------------------------------\n");
    return 0;
}
