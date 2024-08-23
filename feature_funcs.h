#ifndef FEATURE_FUNCS_H
#define FEATURE_FUNCS_H
#include <stdbool.h>
/**## FEATURE_ORGANISE_DIR ##
 * Directory path is coppied to further mutate for winapi functions and usecase.
 * 
 * IMP>>> Directory path passed must not have an endline character as that is not checked later and will throw errors
 * 
 *Primary mutation includes a series of validity checks by  org_funs_validate_path.
 * 
 *The path is further used by winapi functions to fetch data from the directory for processing.
 * 
 * Each filename is fetched and a directory is created with a combination of the directory name 
 * and suffixed with the extension fetched by org_funs_ext_extension if it already does not exist.
 * 
 * The File paths are then renamed accordingly to move them in their respective sub directories.(Respective outputs and errors will be displayed in streams)
 **/
void feature_organise_dir(char *dir_path);

/**## FEATURE_SEARCHING ##
 * Takes path of a directory and a token and searches for all 
 * instances similar to the token in that directory and its sub directories (token is a subset of file/dir name).
 * 
 * Path is validated and processed before feeding into windows.h functions
 * 
 * 
 */
void feature_searching(char *dir_path, char *token,bool *flag);
/**## FEATURE_DIR_SYNC ##
 * Takes path of a source and a destination and recursively checks if all the files and subdirectories in 
 * source are present in destination and are upto date or not.
 * 			-> If not: Does the necessary changes
 * IMP>>> COMPARISON IF EXISTS IS BASED OFF LASTWRITEDATE ATTRIBUTE OF WIN32_FIND_DATA TYPE STRUCTURE
 */
void feature_dir_sync(char *src_dir,char *dest_dir,char comm);
#endif
