#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#include "feature_funcs.h"
#include "gen_purp_funcs.h"

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BLUE "\033[34m"

//path limit hard code, change to 32767 if script deals with extended paths '\\?\'
#define PATH_LIM 260 

/* org_funs_organise_dir()
{{{IMP NOTE: the directory name must be processed after input scanning before being passed to avoid any winapi error}}}
This function 
-> checks all the contents of the directory
-> If content is not a directory
    -> it's transferred to a folder labelled with its extension (if directory does not exist, it is created)
*/
void feature_organise_dir(char *dir_path) {
    char dir_path_mutable[PATH_LIM],
         filename[PATH_LIM],
         new_dir_name[PATH_LIM],
         OLDfilepath[PATH_LIM],
         NEWfilepath[PATH_LIM],
         temporary_filename_buffer[PATH_LIM]; // Used to strip and store extension from filename

    BOOL dir_existance_check, Movement_check;

    strcpy(dir_path_mutable, dir_path); // Copying to avoid mutation of original input

    if (dir_path_mutable[strlen(dir_path_mutable)] == '\n') { // Strips newline left by input (if any)
        dir_path_mutable[strlen(dir_path_mutable)] = '\0';
    }

    bool validity = gen_purp_validate_path(dir_path_mutable); // Validates path for WinAPI functions if it's a valid dir path
    // printf("\n(%s)\n\n", dir_path_mutable); // For testing

    if (!validity) { // Early return if path is not a valid dir path by checks
        fprintf(stderr, "\n%sPATH ERROR : CODE 3  %s\n", RED, RESET);
        return;
    }

    WIN32_FIND_DATA container; // To store filenames
    HANDLE search_handle;
    search_handle = FindFirstFileA(dir_path_mutable, &container);

    if (search_handle == INVALID_HANDLE_VALUE) { // If Handle Fails
        fprintf(stderr, "\n%sHANDLE ERROR : CODE %lu %s\n", RED, GetLastError(), RESET);
        return;
    }

    while (1) {
        if (FindNextFileA(search_handle, &container) == 0) { // FindNextFile returns 0 on failure
            break;
        }

        if (!(container.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { // File is not a directory
            if (strcmp(container.cFileName, "..") == 0||container.cFileName[0]=='.') {
                continue;
            }

            fprintf(stdout, "\n%s%s%s\n", YELLOW, container.cFileName, RESET);
            strcpy(temporary_filename_buffer, container.cFileName);
            gen_purp_ext_extension(temporary_filename_buffer);
            // fprintf(stdout, "\n%s", temporary_filename_buffer); // For testing

            sprintf(new_dir_name, "%s\\%s_Files", dir_path, temporary_filename_buffer);
            // fprintf(stdout, "%s\n", new_dir_name); // For testing

            dir_existance_check = CreateDirectoryA(new_dir_name, NULL);

            if (dir_existance_check != 0) {
                fprintf(stdout, "%s[%s_Files directory does not exist. Was newly created.]%s", MAGENTA, temporary_filename_buffer, RESET);
            }

            sprintf(OLDfilepath, "%s\\%s", dir_path, container.cFileName);
            sprintf(NEWfilepath, "%s\\%s", new_dir_name, container.cFileName);
            Movement_check = MoveFile(OLDfilepath, NEWfilepath);

            if (Movement_check != 0) { // To check if file was moved
                fprintf(stdout, "%s\n%s moved \n[%s->%s]\n%s", GREEN, container.cFileName, dir_path, new_dir_name, RESET);
            } else {
                fprintf(stderr, "%s\nMOVEMENT ERROR : CODE %lu \n%s", RED, GetLastError(), RESET);
            }
        }
    }

    if (GetLastError() == ERROR_NO_MORE_FILES) {
        FindClose(search_handle);
        fprintf(stdout, "\nEND OF DIRECTORY - ALL THE ABOVE CHANGES WERE MADE TO ORGANISE");
    }
}

void feature_searching(char *dir_path, char *token,bool *flag) {
    if (token == NULL) { // If no token is provided -> early return
        fprintf(stderr, "\nTOKEN IS NULL\n%sSEARCH ABORTED...%s", RED, RESET);
        return;
    }

    if(*flag){return;}
    char dir_path_mutable[PATH_LIM],subdir[PATH_LIM],dpm_copy[PATH_LIM];
    int matches = 0;
    
    strcpy(dir_path_mutable, dir_path); // Copying path to avoid original value mutation

    if (dir_path_mutable[strlen(dir_path_mutable) - 1] == '\n') { // Strips newline left by input (if any)
        dir_path_mutable[strlen(dir_path_mutable) - 1] = '\0';
    }

    // puts(dir_path); // testing
    bool validity = gen_purp_validate_path(dir_path_mutable);
    // fprintf(stdout, validity ? "true\n" : "false\n"); // testing

    fprintf(stdout, "\nSEARCHING IN: %s%s%s\nTOKEN: %s%s%s\n", CYAN, dir_path, RESET, YELLOW, token, RESET);

    if (!validity) { // Early return if path is not a valid dir path by checks
        fprintf(stderr, "\n%sPATH ERROR : CODE 3  %s\n", RED, RESET);
        return;
    }

    WIN32_FIND_DATA container; // To store filenames
    HANDLE search_handle;
    search_handle = FindFirstFileA(dir_path_mutable, &container);

    if (search_handle == INVALID_HANDLE_VALUE) { // If Handle Fails
        fprintf(stderr, "\n%sHANDLE ERROR : CODE %lu %s\n", RED, GetLastError(), RESET);
        return;
    }
    while (1) {
        if (FindNextFileA(search_handle, &container) == 0) { // FindNextFile returns 0 on failure
            break;
        }
      
        if (strcmp(container.cFileName, "..") == 0 || strcmp(container.cFileName, ".") == 0||container.cFileName[0]=='.') {
            continue;
        } 
        else if(container.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY){// If directory is enumerated, recurse inside
        strcpy(dpm_copy,dir_path_mutable);
        dpm_copy[strlen(dpm_copy)-4]='\0';
        sprintf(subdir,"%s\\%s",dpm_copy,container.cFileName);
        feature_searching(subdir,token,flag);}
        else if (strcmp(container.cFileName, token) == 0) {
          fprintf(stdout,"\n\n------------------------------------[%sexact match found%s]------------------------------------\n",GREEN,RESET);
            fprintf(stdout, "\n%s%s%s\n%s%s\\%s%s\n", GREEN, container.cFileName, RESET, CYAN, dir_path, container.cFileName, RESET);
            matches++;
            fprintf(stdout, "\n-------------------------------------------------------------------------------------------\n",dir_path, matches);
      
            *flag=true;
            continue;
        } else if (strstr(container.cFileName, token) != NULL) {
            fprintf(stdout, "\n%s%s%s\n%s%s\\%s%s\n", YELLOW, container.cFileName, RESET, CYAN, dir_path, container.cFileName, RESET);
            matches++;
        }
        
    }
    if(*flag){
      return;
    }
    if (GetLastError() == ERROR_NO_MORE_FILES) {
      
      if(matches>0){
        fprintf(stdout,"\n%spossible instance present%s\n",GREEN,RESET);
      }
        fprintf(stdout, "\nEND OF DIRECTORY [%s]\n%d match(es) found\n-------------------------------------------------------------------------------------------\n",dir_path, matches);
      
    } 
    
    else {
        fprintf(stdout, "\n%sERROR : CODE %lu%s\n%d match(es) found till error\n", RED, GetLastError(), RESET, matches);
    }

    FindClose(search_handle);
    return;
}

void feature_dir_sync(char *src_dir,char *dest_dir,char comm){
  char src_mutable[PATH_LIM],dest_mutable[PATH_LIM],temp_dest_path[PATH_LIM],temp_src_path[PATH_LIM];
  bool src_valid,dest_valid;
  int time_comparison;//To check difference in filewrite times
  //to avoid argument mutation
  strcpy(src_mutable,src_dir);
  strcpy(dest_mutable,dest_dir);
  src_valid = gen_purp_validate_path(src_mutable);
  if(!src_valid){
    fprintf(stderr,"\n%sPATH ERROR: Invalid path of source directory%s\n",RED,RESET);
    return;
  }
  dest_valid = gen_purp_validate_path(dest_mutable);
  if(!src_valid){
    fprintf(stderr,"\n%sPATH ERROR: Invalid path of destination directory%s\n",RED,RESET);
  }
  BOOL copystatus,obj_status;
  DWORD dest,src,if_in_dest;
  HANDLE search_handle1;
  WIN32_FIND_DATAA container,dest_timestore;
  //Done to reuse validate path and remove unwanted mutation
  src_mutable[strlen(src_mutable)-4]='\0';
  dest_mutable[strlen(dest_mutable)-4]='\0';
  src = GetFileAttributesA(src_mutable);
  dest = GetFileAttributesA(dest_mutable);

  if(src==INVALID_FILE_ATTRIBUTES){
     fprintf(stdout, "\n%sPATH ERROR : SOURCE DOES NOT EXIST%s\n", RED,RESET);
     return;
            
  }
  if(dest==INVALID_FILE_ATTRIBUTES){
    fprintf(stdout, "\n%sDESTINATION DOES NOT EXIST%s\n", RED,RESET);
    if(comm =='u'){
    if(CreateDirectoryA(dest_mutable,NULL)==0){
      fprintf(stderr,"%s\nNON EXISTING DESTINATION CREATION ERROR : CODE : %lu%s\n",RED,RESET);
      return;   
    }
    fprintf(stdout, "\nCreated new directory : %s%s\n %s\n", CYAN,dest_mutable,RESET);
  
    }
  }
  gen_purp_validate_path(src_mutable);//Setting up src_mutable for FindFirsstFile()
  search_handle1 = FindFirstFileA(src_mutable,&container);
  src_mutable[strlen(src_mutable)-4]='\0';
  if(search_handle1==INVALID_HANDLE_VALUE){
    fprintf(stderr,"\n%sHANDLE ERROR : CODE %lu\n%s",RED,GetLastError(),RESET);
    return;
  }
  while (1) {
        if (FindNextFileA(search_handle1, &container) == 0) { // FindNextFile returns 0 on failure
            break;
        }
        else if(strcmp(container.cFileName,"..")==0||strcmp(container.cFileName,".")==0||container.cFileName[0]=='.')
          {
           continue;
         }
        
        sprintf(temp_dest_path,"%s\\%s",dest_mutable,container.cFileName);
        sprintf(temp_src_path,"%s\\%s",src_mutable,container.cFileName);
        if_in_dest = GetFileAttributesA(temp_dest_path);
        if (if_in_dest == INVALID_FILE_ATTRIBUTES) {//If file is entirely not found
            fprintf(stdout,"\n%s%s%s \nDoes not exist in destination",YELLOW,container.cFileName,RESET);
            if(comm=='u'){
              //LOGIC FOR UPDATE
              //printf("\n UPDATE LOGIC\n");//TESTING
              if(!(container.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
                //Not a dir
                if(CopyFile(temp_src_path,temp_dest_path,FALSE)==0){
                  fprintf(stderr,"\n%sCOPYING ERROR : CODE %lu%s\n",RED,GetLastError(),RESET);
                  continue;
                  }
                  fprintf(stdout,"\n%sSucessfully coppied %s%s%s%s \nto : %s%s%s\n",GREEN,RESET,YELLOW,container.cFileName,
                      RESET,CYAN,temp_dest_path,RESET);
                  }
                else {//Logic for dir copying
                  if(comm =='u'){
                  if(CreateDirectoryA(temp_dest_path,NULL)==0){
                    fprintf(stderr,"%s\nDESTINATION CREATION ERROR : CODE : %lu%s\n",RED,RESET);
                    return;   
                  }
                    fprintf(stdout, "\nCreated new directory in destination : %s%s\n %s\n", CYAN,temp_dest_path,RESET);
                    feature_dir_sync(temp_src_path,temp_dest_path,comm);
                }
                  }
                } 
                else {//If command is just s, no changes are to be made
                   continue;
                  }
        }
        else {
          if(!(container.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
            if(FindFirstFileA(temp_dest_path,&dest_timestore)==INVALID_HANDLE_VALUE){
              fprintf(stderr,"\n%sDEST HANDLE ERROR : CODE %lu%s\n");
              continue;
            }       
            time_comparison = CompareFileTime(&container.ftLastWriteTime,&dest_timestore.ftLastWriteTime);//Comparison by last edit time
            if(time_comparison>0){
              fprintf(stdout,"\n%s%s%s \nPossible edits at source that are absent in destination",YELLOW,container.cFileName,RESET);
              if(comm == 'u'){
                if(DeleteFile(temp_dest_path)==0&CopyFile(temp_src_path,temp_dest_path,FALSE)==0){
                  fprintf(stderr,"\n%sCOMMIT ERROR : CODE %lu%s\n",RED,GetLastError(),RESET);
                  continue;
                }
                fprintf(stdout,"\n%sChanges in source are commited to destination Sucessfully%s\n",GREEN,RESET);
              }
            }

          }
          else if(container.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
          //printf("(%s\n%s)\n",temp_src_path,temp_dest_path);//testing

          feature_dir_sync(temp_src_path,temp_dest_path,comm);}
          
        }

       }
  if (GetLastError() == ERROR_NO_MORE_FILES) {//base case for recursion
      return;
    } 
  }

void feature_dir_tree(char *dir_path,int depth){
  char dir_path_mutable[PATH_LIM],
       sub_dir[PATH_LIM];

  strcpy(dir_path_mutable,dir_path);
  if (dir_path_mutable[strlen(dir_path_mutable)] == '\n') { // Strips newline left by input (if any)
       dir_path_mutable[strlen(dir_path_mutable)] = '\0';
   }
   bool validity = gen_purp_validate_path(dir_path_mutable); // Validates path for WinAPI functions if it's a valid dir path
    //printf("\n(%s)\n\n", dir_path_mutable); // For testing

   if (!validity) { // Early return if path is not a valid dir path by checks
       fprintf(stderr, "\n%sPATH ERROR : CODE 3  %s\n", RED, RESET);
       return;
   }

    WIN32_FIND_DATA container; // To store filenames
    HANDLE search_handle;
    search_handle = FindFirstFileA(dir_path_mutable, &container);
    while(1){
      if (FindNextFileA(search_handle, &container) == 0) { // FindNextFile returns 0 on failure
            break;
        }
      if((strcmp(container.cFileName,"..")==0)&&depth==1){
          fprintf(stdout,"%s%s%s\n",BLUE,gen_purp_striplast(dir_path_mutable),RESET);
          continue;
      }
      else if((strcmp(container.cFileName,"..")==0)&&depth !=1){
           continue;
       }
      for(int i=0;i<depth;i++){
        fprintf(stdout,"%s|%s",BLUE,RESET);
        }
      if(!(container.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
        fprintf(stdout,"%s--%s",BLUE,RESET);
        fprintf(stdout,"%s%s%s\n",YELLOW,container.cFileName,RESET);
      }
      else{
        
        fprintf(stdout,"%s%s%s\n",BLUE,container.cFileName,RESET);
        //printf("(%s)\n",dir_path_mutable);//testing
        
         if(container.cFileName[0]=='.'){continue;}
        //Extra validation for standardisation
        gen_purp_validate_path(dir_path_mutable);
        //Removal of validity token so subdir can be constructed
        dir_path_mutable[strlen(dir_path_mutable)-3]='\0';
        sprintf(sub_dir,"%s\\%s",dir_path_mutable);
        //printf("(%s)\n",sub_dir);//testing
        feature_dir_tree(sub_dir,depth+1);
      }
      }
      if (GetLastError() == ERROR_NO_MORE_FILES) {
        FindClose(search_handle);
       
      }
  }

void feature_tc(char *dir_path,unsigned short offset,char comm,int depth){//offset is the number of months, depth is to be initially set to 1 ,comm - s(status),u(update)
    


    char dir_path_mutable[PATH_LIM], trashbin[PATH_LIM],newpath[PATH_LIM],
       sub_dir[PATH_LIM],ext[PATH_LIM];
  strcpy(dir_path_mutable,dir_path);
  if (dir_path_mutable[strlen(dir_path_mutable)-1] == '\n') { // Strips newline left by input (if any)
       dir_path_mutable[strlen(dir_path_mutable)-1] = '\0';
   }
   bool validity = gen_purp_validate_path(dir_path_mutable); // Validates path for WinAPI functions if it's a valid dir path
    //printf("\n(%s)\n\n", dir_path_mutable); // For testing

   if (!validity) { // Early return if path is not a valid dir path by checks
       fprintf(stderr, "\n%sPATH ERROR : CODE 3  %s\n", RED, RESET);
       return;
   }

    
   if(comm=='u'&&depth==1){//Extra confirmation
    char ch[5];

    //printf("\n%d",depth);//Testing
    fprintf(stdout,"\nConfirm mass movement of files to trashbin? %sThis action is irreversable.%s (YYY/n)\n>>>",RED,RESET);
    fgets(ch,5,stdin);
    ch[3]='\0';
    if(!(strcmp(ch,"YYY")==0)){
      fprintf(stdout, "\n%sABORTED%s",RED,RESET);
      return;
    }
    BOOL dir_existance_check;
     dir_path_mutable[strlen(dir_path_mutable)-4]='\0';
    sprintf(trashbin,"%s\\trashbin",dir_path_mutable);
    dir_existance_check = CreateDirectoryA(trashbin, NULL);
    if (dir_existance_check != 0) {
        fprintf(stdout, "%s[trashbin does not exist. Was newly created.]%s", MAGENTA, RESET);
    }
    gen_purp_validate_path(dir_path_mutable);
  }

   SYSTEMTIME current,file;
   WORD diff_in_months;

   GetSystemTime(&current);
   
  WIN32_FIND_DATA container; // To store filenames and otherinfo
    HANDLE search_handle;
    search_handle = FindFirstFileA(dir_path_mutable, &container);
    if(depth==1){
    fprintf(stdout,"\nFILES OLDER THAN %d MONTHS:",offset);
  }
    while(1){
      if (FindNextFileA(search_handle, &container) == 0) { // FindNextFile returns 0 on failure
            break;
        }

        gen_purp_validate_path(dir_path_mutable);

        dir_path_mutable[strlen(dir_path_mutable)-4]='\0';
        sprintf(sub_dir,"%s\\%s",dir_path_mutable,container.cFileName);
        strcpy(ext,sub_dir);
        gen_purp_ext_extension(ext);

      if(strcmp(container.cFileName,"..")==0||strcmp(container.cFileName,".")==0||strcmp(container.cFileName,"trashbin")==0||container.cFileName[0]=='.'||strcmp(ext,"exe")==0||strcmp(ext,"o")==0){
        continue;
      }

      if(!(container.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
        if(FileTimeToSystemTime(&container.ftLastWriteTime,&file)==FALSE){
            fprintf(stderr, "\n%sTIME FETCHING  ERROR FOR %S : CODE 3  %s\n", RED,container.cFileName, RESET);
            continue;
        }
        diff_in_months=(current.wMonth+(current.wYear*12)+(current.wDay/30))-(file.wMonth+(file.wYear*12)+(file.wDay/30));
        if(diff_in_months>=offset){

            fprintf(stdout,"\n%s%s%s\n(%s%s%s)",YELLOW,container.cFileName,RESET,CYAN,sub_dir,RESET);
          //printf("%s\n",sub_dir);//Testing

          if(comm=='u'){
            //Move in trash sub_dir->newpath

            sprintf(newpath,"%s\\%s",trashbin,container.cFileName);
            BOOL Movement_check = MoveFile(sub_dir,newpath);
            if (Movement_check != 0) { // To check if file was moved
                fprintf(stdout, "\nmoved to trashbin\n[%s->%s]\n", dir_path, trashbin);
            } else {
                fprintf(stderr, "%s\nMOVEMENT ERROR : CODE %lu \n%s", RED, GetLastError(), RESET);
            }
          }
        }

      }
      else{
        feature_tc(sub_dir,offset,comm,depth+1);
      }
    }
    if (GetLastError() == ERROR_NO_MORE_FILES) {
        FindClose(search_handle);
       
      }


}


