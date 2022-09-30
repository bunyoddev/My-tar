#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <utime.h>
#include <pwd.h>
#include <grp.h>
#define CF "-cf"
#define RF "-rf"
#define TF "-tf"
#define UF "-uf"
#define XF "-xf"
#define err_f_not_found(n) (prinf("my_tar: %s: Cannot stat: No such file or directory\n", n))
#define err_open(n) (printf("my_tar: Cannot open %s\n", n))

#define err_cf printf("my_tar: Refusing to write archive contents to terminal (missing -f option?)\n")
#define err_rf printf("my_tar: Options '-Aru' are incompatible with '-f -'\n")
#define err_tf printf("my_tar: Refusing to read archive contents from terminal (missing -f option?)\n")
#define err_uf printf("my_tar: Options '-Aru' are incompatible with '-f -'\n")
#define err_xf printf("my_tar: Refusing to read archive contents from terminal (missing -f option?)\n")
typedef struct posix_header
{                              /* byte offset */
  char name[100];               /*   0 */
  char mode[8];                 /* 100 */
  char uid[8];                  /* 108 */
  char gid[8];                  /* 116 */
  char size[12];                /* 124 */
  char mtime[12];               /* 136 */
  char chksum[8];               /* 148 */
  char typeflag;                /* 156 */
  char linkname[100];           /* 157 */
  char magic[6];                /* 257 */
  char version[2];              /* 263 */
  char uname[32];               /* 265 */
  char gname[32];               /* 297 */
  char devmajor[8];             /* 329 */
  char devminor[8];             /* 337 */
  char prefix[155];             /* 345 */
  char tes[12];                 /* 500 */
} tar_struct;

bool equals(char*, char*);
int __cf(int , char**, int);
int __rf(int , char**, int );
int __tf(char**, int );
int __uf( char** , int );
int __xf(char** , int );
