#ifndef GEN_PURP_FUNCS
#define GEN_PURP_FUNCS
#include <stdbool.h>
/**## GEN_PURP_FUNS_VALIDATE_PATH
 * This Function validates the path string for winapi FindFirstFile function call.
 *A valid path should resemble a directory path ending with "*" or "*.*" for FindFirstFile function
 *
 *>checks if path is valid as a directory path-> if not, returns false
 *>checks if "*" or "*.*" is added-> if not, adds it validating the path 
 *								-> else returns true
 *valid directory path examples:
 *C:\Program Files\MyApp
 *D:\Documents\Projects\2024
 *C:\Users\JohnDoe\Desktop
 *
 *invalid directory path examples: 
 *C:\Program Files\MyApp.txt
 *randomtext
 *
 *valid directory path examples for utilisation of FindFirstFile according to need of the script:
 *C:\Program Files\MyApp\*
 *D:\Documents\Projects\2024\*.*
 *C:\Users\JohnDoe\Desktop\*
 *
 **/
bool gen_purp_validate_path(char *dir_path);
void gen_purp_ugdisplay();
void gen_purp_ext_extension(char *f_name);

#endif
