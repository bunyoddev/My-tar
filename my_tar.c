#include "base.h"


void fill_null(char *str, int size) {
    int i=0;
    while(i<size) {
        str[i++] = 0;
    }
}
void init_tar(tar_struct* tar) {
    fill_null(tar->name, sizeof(tar->name));
    fill_null(tar->mode, sizeof(tar->mode));
    fill_null(tar->uid, sizeof(tar->uid));
    fill_null(tar->gid, sizeof(tar->gid));
    fill_null(tar->size, sizeof(tar->size));
    fill_null(tar->mtime, sizeof(tar->mtime));
    fill_null(tar->chksum, sizeof(tar->chksum));
    fill_null(tar->linkname, sizeof(tar->linkname));
    fill_null(tar->magic, sizeof(tar->magic));
    fill_null(tar->version, sizeof(tar->version));
    fill_null(tar->uname, sizeof(tar->uname));
    fill_null(tar->gname, sizeof(tar->gname));
    fill_null(tar->devmajor, sizeof(tar->devmajor));
    fill_null(tar->devminor, sizeof(tar->devminor));
    fill_null(tar->prefix, sizeof(tar->prefix));
    fill_null(tar->tes, sizeof(tar->tes));
    tar->typeflag = 0;

}
void revstr(char *str)  {  
    int i, len, temp;  
    len = strlen(str);    
    for (i = 0; i < len/2; i++)  {  
        temp = str[i];  
        str[i] = str[len - i - 1];  
        str[len - i - 1] = temp;  
    }  
}
int chk_sum(char *str) {
    int n = 0;
    for(int i=0; str[i]!='\0'; i++)
        n += str[i];
    // printf(" ----- %d -----\n", n);
    return n;
}
void itoa(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0)  
        n = -n;          
    i = 0;
    do {       
        s[i++] = n % 10 + '0';   
    } while ((n /= 10) > 0);     
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    revstr(s);
}
bool equals(char*s1, char*s2) {
    int i;
    for(i=0; s1[i]!=0;i++) {
        if(s2[i] == '\0')
            return false;
        if(s1[i] != s2[i])
            return false;
    }
    if(s2[i] != '\0')
        return false;
    return true;
}
void my_put(char* str) {
    for(int i=0; str[i] != 0; i++) {
        write(1, &str[i], 1);
    }
    write(1, "\n", 1);
}
long my_pow(int nb, int pow) {
  int	p;
  long	result;
  p = 1;
  result = 1;
  if (pow < 0)
    return (0);
  if (pow == 0)
    return (1);
  while (p <= pow) {
      result = result * nb;
      p = p + 1;
    }
  return (result);
}
long octal_to_decimal(int octalNumber) {
    long decimalNumber = 0;
    int i = 0;
    // printf("octal : %d\n", octalNumber);
    while(octalNumber != 0) {
        decimalNumber += (octalNumber%10) * my_pow(8,i);
        ++i;
        octalNumber/=10;
    }
    i = 1;
    // printf("Size %ld\n", decimalNumber);
    return decimalNumber;
} 

int decimal_to_octal(long int n, char* str) {
    long int i=0, m = n;
    int chk = 0;
    while(m>0) {
        str[i] = m%8 + '0';
        chk += str[i];
        i++;
        m = m/8;
    }
    revstr(str);
    return chk;
}
int set_uname( char* des, struct stat* fs) {
    struct passwd *pw = getpwuid(fs->st_uid);
    strcpy(des, pw->pw_name);
    return chk_sum(des);
}
int set_gname(char*des, struct stat* fs) {
    struct group *grp = getgrgid(fs->st_gid);
    strcpy(des, grp->gr_name);
    return chk_sum(des);
}
long fill_tar(char* fname, tar_struct* tar) {
    init_tar(tar);
    struct stat st;
    stat(fname, &st);
    static int chksum = 0;
    chksum += chk_sum(fname);
    strcpy(tar->name, fname);
    chksum += decimal_to_octal(st.st_mode, tar->mode);
    chksum += decimal_to_octal(st.st_uid, tar->uid);
    chksum += decimal_to_octal(st.st_gid, tar->gid);
    chksum += decimal_to_octal(st.st_size, tar->size);
    chksum += decimal_to_octal(st.st_mtim.tv_sec, tar->mtime);
    tar->typeflag = '0';
    chksum += '0';
    chksum += decimal_to_octal(st.st_nlink, tar->linkname);
    strcpy(tar->magic, "ustar");
    chksum += chk_sum( "ustar");
    strcpy(tar->version, "  ");
    chksum += chk_sum( "  ");
    chksum += set_uname( tar->uname, &st);
    chksum += set_gname( tar->gname, &st);
    decimal_to_octal(chksum, tar->chksum);
    return st.st_size;
} 

void fill_content(char*fname, char* des) {
    int fd = open(fname, O_RDONLY);
    int i = 0;
    char c;
    while(read(fd, &c, 1) > 0) {
        des[i++] = c;
    }
    des[i] = '\0';
    // printf("%s\n", des);
}

int __cf(int ac, char** av, int index) {
    if(access( av[index], F_OK ) != -1) {
        if(remove(av[index])!=0)
            printf("my_tar: cannot creat, %s already exist\n", av[index]);
    }
    tar_struct tar;

    creat(av[index], 0000644);
    
    int fd = open(av[index], O_WRONLY | O_APPEND);
    if(fd == -1) {
        err_open(av[index]);
        exit(-1);
    }
    for(int i=index+1; i < ac; i++) {
        long size, content_size;
        content_size = fill_tar(av[i], &tar);
        // printf("< - - %s\n", av[i]);
        size = (content_size / 513 + 1) * 512;
        char content[size];
        fill_null(content, size);
        fill_content(av[i], content);
        if(write(fd, &tar, sizeof(tar))  < 0) {
            return 1;
        } 
        if(write(fd, content, size) < 0){
            return 1;
        };
        // return 1;
    };
    return 0;
}

int __rf(int ac, char**av, int index) {

    tar_struct tar;
    // init_tar(&tar);
    int fd = open(av[index],O_WRONLY | O_APPEND);
    if(fd == -1) {
        err_open(av[2]);
        exit(-1);
    }
    for(int i=index+1; i<ac; i++) {
        long size, content_size;
        content_size = fill_tar(av[i], &tar);
        // printf("< - - %s\n", av[i]);
        size = (content_size / 513 + 1) * 512;
        char content[size];
        fill_null(content, size);
        fill_content(av[i], content);
        if(write(fd, &tar, sizeof(tar)) < 0 ){
            return 1;
        } 
        if(write(fd, content, size) < 0){
            return 1;
        }

    }
    return 0;
}

void read_tar(char*entry) {
   int fd = open(entry, O_RDONLY);
   if(fd == -1) {
       err_open(entry);
       exit(-1);
   }
   tar_struct tar;
   while(read(fd, &tar, sizeof(tar_struct)) > 0) {
        my_put(tar.name);
        long c_size = octal_to_decimal(atoi(tar.size));
        long size = (c_size / 513 + 1) * 512;
        lseek(fd, size, 1);
   } 
}
int __tf(char**av, int index) {
    read_tar(av[index]);
    return 0;
}

int __uf(char** av, int index) {
    
    int fd = open(av[index], O_RDWR | O_APPEND);
    if(fd == -1) {
        printf("my_tar: %s: Cannot open: No such file or directory\n", av[2]);
        exit(1);
    }
    struct stat fs;
    if(stat(av[index+1], &fs) == -1){
        return printf("my_tar: %s: Cannot stat: No such file or directory\n", av[index+1]);
    };
    tar_struct tar;
    char inside_mtim[12];
    while(read(fd, &tar, sizeof(tar_struct)) > 0) {
        if(equals(tar.name, av[index+1])) {
        //     printf("file found %s - mtime %s\n", tar.name, tar.mtime);
            strcpy(inside_mtim, tar.mtime);
        }
        struct stat cs;
        stat(tar.name, &cs);
        long c_size = (cs.st_size / 513 + 1) * 512;
        lseek(fd, c_size, SEEK_CUR);
    }
    if(fs.st_mtim.tv_sec != octal_to_decimal(atoi(inside_mtim))) {
                tar_struct new;
                fill_tar(av[index+1], &new);
                // printf("write file %s\n", new.name);
                long size = (fs.st_size / 513 + 1) * 512;
                char content[size];
                fill_null(content, size);
                fill_content(av[index+1], content);
                if(write(fd, &new, sizeof(tar_struct)) < 0) {
                    return 1;
                } 
                if(write(fd, content, size) < 0){
                    return 1;
                }
                return 0;
            }
    puts("my_tar: file not found");
    return 1;
}

int creat_write(char* fn, char* mode, char* content, long c_size, long mtim){
    // printf("content size: %ld\n", c_size);
    creat(fn, octal_to_decimal(atoi(mode)));

    struct stat foo;
    // time_t mtime;
    struct utimbuf new_times;
    stat(fn, &foo);
    // mtime = foo.st_mtime; /* seconds since the epoch */
    new_times.actime = foo.st_atime; /* keep atime unchanged */
    new_times.modtime = mtim;    /* set mtime to current time */
    utime(fn, &new_times);
    /* set mod time to file */
    foo.st_mtim.tv_sec = mtim;

    int fd = open(fn, O_WRONLY);
    if(fd < 0){
        err_open(fn);
        exit(-1);
    }
    if(write(fd, content, c_size) >= 0)
        return 0;
    return -1;

}

int __xf(char** av, int index) {
    
    int fd = open(av[index], O_RDONLY);
    if(fd == -1) {
        err_open(av[index]);
        exit(-1);
    }
    tar_struct tar;
    while(read(fd, &tar, sizeof(tar_struct)) > 0) {
            long c_size = octal_to_decimal(atoi(tar.size));
            char content[c_size];
            read(fd, content, c_size);
            creat_write(tar.name, tar.mode, content, c_size,
                octal_to_decimal(atoi(tar.mtime)));
            long size = (c_size / 513 + 1) * 512 - c_size;

            lseek(fd, size, SEEK_CUR);
    } 
    return 0;
}
