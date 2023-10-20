#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "directs.h"
#include <string.h>
#include <stdlib.h>
#define MAX_PATH_LEN 100

char options[7][2] = {"-a", "-i", "-n", "-o", "-p", "-r", "-v"};
char a_option = 'N';
char n_option = 'N';
char v_option = 'N';
char r_option = 'N';
char p_option = 'N';
char i_option = 'N';
char o_option = 'N';
char *i_pattern;
char *o_pattern;
int input_directories_count = 0;
struct directory *all_directories;

int idx;
int check_inputs(int argc, char const *argv[])
{
    idx = 1;

    while ((idx < argc) && (argv[idx][0] == '-'))
    {

        if (strcmp(argv[idx], "-a") == 0)
        {
            a_option = 'Y';
        }
        else if (strcmp(argv[idx], "-n") == 0)
        {
            n_option = 'Y';
        }
        else if (strcmp(argv[idx], "-v") == 0)
        {
            v_option = 'Y';
        }
        else if (strcmp(argv[idx], "-r") == 0)
        {
            r_option = 'Y';
        }
        else if (strcmp(argv[idx], "-p") == 0)
        {
            p_option = 'Y';
        }
        else if (strcmp(argv[idx], "-i") == 0)
        {
            i_option = 'Y';
            i_pattern = malloc(strlen(argv[idx+1]) + 1);
            strcpy(i_pattern, argv[idx+1]);
            idx++;
        }
        else if (strcmp(argv[idx], "-o") == 0)
        {
            o_option = 'Y';
            o_pattern = malloc(strlen(argv[idx+1]) + 1);
            strcpy(o_pattern, argv[idx+1]);
            idx++;
        }
        else
        {
            printf("Invalid option: %s\n", argv[idx]);
            return -1;
        }
        idx++;
    }
    input_directories_count = 0;
    while (idx < argc) 
    {
        if (input_directories_count == 0)
        {
            all_directories = (struct directory *)malloc(sizeof(struct directory));
            all_directories[input_directories_count].name = malloc(strlen(argv[idx]) + 1);
            all_directories[input_directories_count].isdirect = 'Y';
            strcpy(all_directories[input_directories_count].name, argv[idx]);
            input_directories_count++;
        }
        else
        {
            all_directories = (struct directory *)realloc(all_directories, sizeof(struct directory) * (input_directories_count + 1));
            all_directories[input_directories_count].name = malloc(strlen(argv[idx]) + 1);
            all_directories[input_directories_count].isdirect = 'Y';
            strcpy(all_directories[input_directories_count].name, argv[idx]);
            input_directories_count++;
        }
        idx++;
    }

    if (input_directories_count < 2)
    {
        printf("No input directorys\n");
        return -1;
    }
    return 0;
}

// int read_input(int argc, char const *argv[])
// {
//     for (int i = 1; i < argc; i++)
//     {
//         if (input_directories_count == 0)
//         {
//             all_directories = (struct directory *)malloc(sizeof(struct directory));
//         }
//         else
//         {
//             all_directories = (struct directory *)realloc(all_directories, sizeof(struct directory) * (input_directories_count + 1));
//         }
//         all_directories[input_directories_count].n_files = -1;
//         if (strcmp(argv[i], "-a") == 0)
//         {
//             a = 'Y';
//         }
//         else if (strcmp(argv[i], "-i") == 0)
//         {
//             all_directories[input_directories_count].name = malloc(strlen(argv[i + 1]) + 1);
//             all_directories[input_directories_count].isdirect = 'Y';
//             strcpy(all_directories[input_directories_count].name, argv[i + 1]);
//             all_directories[input_directories_count].switch_type = 'I';
//             input_directories_count++;
//         }
//         else if (strcmp(argv[i], "-n") == 0)
//         {
//             n = 'Y';
//             v = 'Y';
//         }
//         else if (strcmp(argv[i], "-o") == 0)
//         {
//             all_directories[input_directories_count].name = malloc(strlen(argv[i + 1]) + 1);
//             all_directories[input_directories_count].isdirect = 'Y';
//             strcpy(all_directories[input_directories_count].name, argv[i + 1]);
//             all_directories[input_directories_count].switch_type = 'O';
//             input_directories_count++;
//         }
//         else if (strcmp(argv[i], "-p") == 0)
//         {
//         }
//         else if (strcmp(argv[i], "-r") == 0)
//         {
//             r_option = 'Y';
//         }
//         else if (strcmp(argv[i], "-v") == 0)
//         {
//             v_option = 'Y';
//         }
//         else
//         {
//             all_directories[input_directories_count].name = malloc(strlen(argv[i]) + 1);
//             strcpy(all_directories[input_directories_count].name, argv[i]);
//             all_directories[input_directories_count].isdirect = 'Y';
//             all_directories[input_directories_count].switch_type = 'N';
//             input_directories_count++;
//         }
//     }
//     return 0;
// }
char is_it_real(char *name)
{
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        return 'N';
    }
    else
    {
        return 'Y';
    }
}
char is_in_other(struct directory d1, struct directory *d2, int d2_size)
{
    for (int i = 0; i < d2_size; i++)
    {
        if (strcmp(d1.name, d2[i].name) == 0)
        {
            if (d1.mod_time > d2[i].mod_time)
            {
                return 'O';
            }
            return 'Y';
        }
    }
    return 'N';
}
char which_is_newer(struct directory d1, struct directory *d2, int d2_size)
{
    if (d1.mod_time > d2->mod_time)
    {
        return 'Y';
    }
    return 'N';
}
char is_it_hidden(char *path)
{
    if (path[0] == '.')
    {
        return 'Y';
    }
    else
    {
        return 'N';
    }
}
void compare_directs(struct directory *d1)
{
    for (int j = 0; j < d1[0].n_files; j++)
    {
        char compare = is_in_other(d1[0].directs[j], d1[1].directs, d1[1].n_files);
        if (compare == 'Y')
        {
            printf("%s is in both\n", d1[0].directs[j].name);
        }
        else if (compare == 'O')
        {
            printf("%s is in %s but is older in other\n", d1[0].directs[j].name, d1[1].name);
        }
        else
        {
            printf("%s is in %s but not in other\n", d1[0].directs[j].name, d1[1].name);
        }
    }
}
void read_in_direcotry(struct directory *outer_directories, int input_directories_count)
{
    for (int i = 0; i < input_directories_count; i++)
    {
        DIR *dirp;
        struct dirent *dp;
        int inner_count = 0;
        if (inner_count == 0)
        {
            outer_directories[i].directs = (struct directory *)malloc(sizeof(struct directory));
        }
        else
        {
            outer_directories[i].directs = (struct directory *)realloc(outer_directories[i].directs, sizeof(struct directory) * (inner_count + 1));
        }
        // printf("name is: %s, is it directory: %c, i: %d\n",outer_directories[i].name,outer_directories[i].isdirect,i);
        if (outer_directories[i].isdirect == 'Y')
        {
            outer_directories[i].n_files = -1;
            dirp = opendir(outer_directories[i].name);
            char fullpath[MAXPATHLEN];
            if (dirp == NULL)
            {
                ///////////perror( progname );
                //////printf("error oppening directory\n");
                ////exit(EXIT_FAILURE);
            }
            else
            {
                while ((dp = readdir(dirp)))
                {
                    struct stat stat_buffer;
                    stat(dp->d_name, &stat_buffer);
                    if (is_it_real(dp->d_name) == 'N')
                    {
                        continue;
                    }
                    if (a_option == 'N' && is_it_hidden(dp->d_name) == 'Y')
                    {
                        continue;
                    }
                    outer_directories[i].directs[inner_count].mod_time = stat_buffer.st_mtime;
                    if (dp->d_type == DT_DIR)
                    {
                        //////printf("2.%s\n",dp->d_name);
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name) + 1);
                        strcpy(outer_directories[i].directs[inner_count].name, dp->d_name);
                        outer_directories[i].n_files += 1;
                        // printf("directory:%s\n",outer_directories[i].directs[inner_count].name);
                        // printf("date:%ld\n",outer_directories[i].directs[inner_count].mod_time);
                        outer_directories[i].directs[inner_count].isdirect = 'Y';
                        if (r_option == 'Y')
                        {
                            read_in_direcotry(outer_directories[i].directs, inner_count);
                        }
                    }
                    else if (dp->d_type == DT_REG)
                    {
                        outer_directories[i].directs[inner_count].name = malloc(strlen(dp->d_name) + 1);
                        strcpy(outer_directories[i].directs[inner_count].name, dp->d_name);
                        outer_directories[i].n_files += 1;
                        // printf("file:%s\n",outer_directories[i].directs[inner_count].name);
                        // printf("date:%ld\n",outer_directories[i].directs[inner_count].mod_time);
                        outer_directories[i].directs[inner_count].isdirect = 'N';
                    }
                    else
                    {
                        if (v_option == 'Y')
                        {
                            printf("%s is unknown!\n", dp->d_name);
                        }
                        else
                        {
                            printf("unknown file!\n");
                        }
                    }
                    inner_count += 1;
                }
                closedir(dirp);
            }
        }
    }
}

void usage()
{
    printf("prompt> ./mysync  [options]  directory1  directory2  [directory3  ...]\n");
    printf("The program's options are:\n");
    printf("-a\tBy default, 'hidden' and configuration files (those beginning with a '.' character) are not synchronised because they do not change very frequently. The -a option requests that all files, regardless of whether they begin with a '.' or not, should be considered for synchronisation.\n");
    printf("-i pattern\tFilenames matching the indicated pattern should be ignored; all other (non-matching) filenames should be considered for synchronisation. The -i option may be provided multiple times.\n");
    printf("-n\tBy default, mysync determines what files need to be synchronised and then silently performs the necessary copying. The -n option requests that any files to be copied are identified, but they are not actually synchronised. Setting the -n option also sets the -v option.\n");
    printf("-o pattern\tOnly filenames matching the indicated pattern should be considered for synchronisation; all other (non-matching) filenames should be ignored. The -o option may be provided multiple times.\n");
    printf("-p\tBy default, after a file is copied from one directory to another, the new file will have the current modification time and default file permissions. The -p option requests that the new copy of the file have the same modification time and the same file permissions as the old file.\n");
    printf("-r\tBy default, only files found immediately within the named directories are synchronised. The -r option requests that any directories found within the named directories are recursively processed.\n");
    printf("-v\tBy default, mysync performs its actions silently. No output appears on the stdout stream, although some errors may be reported on the stderr stream. The -v option requests that mysync be more verbose in its output, reporting its actions to stdout. There is no required format for the (your) debug printing (it will be ignored during marking).\n");
}

int main(int argc, char const *argv[])
{

    if(check_inputs(argc, argv) == -1){
        usage();
    }

    //read_input(argc, argv);
    for (int i = 0; i < input_directories_count; i++)
    {
        printf("%s\n", all_directories[i].name);
    }
    read_in_direcotry(all_directories, input_directories_count);
    compare_directs(all_directories);
}