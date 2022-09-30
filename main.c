#include "base.h"

int main(int ac, char** av) {
    if (ac > 2) {
        if(equals(av[1], CF))
            goto go_cf;
        if(equals(av[1], RF))
            goto go_rf;
        if(equals(av[1], TF))
            goto go_tf;
        if(equals(av[1], UF))
            goto go_uf;
        if(equals(av[1], XF))
            goto go_xf;
    }
    if(ac > 3) {
       if(equals(av[1], "-c")) {
           if(equals(av[2], "-f"))
                return __cf(ac, av, 3);
            return err_cf;
       } else if(equals(av[1], "-r")){
           if(equals(av[2], "-f"))
                return __rf(ac, av, 3);
            return err_rf;
       } else if(equals(av[1], "-t")) {
           if(equals(av[2], "-f"))
                return __tf( av, 3);
            return err_tf;
       } else if(equals(av[1], "-u")) {
           if(equals(av[2], "-f"))
                return __uf( av, 3);
            return err_uf;
       } else if(equals(av[1], "-x")) {
           if(equals(av[2], "-f"))
                return __xf( av, 3);
            return err_xf;
       }
    }

    go_cf:
        if(ac > 3)
            __cf(ac, av, 2);
        else 
            printf("my_tar: Cowardly refusing to create an empty archive\n");
        return 0;
    go_rf:
        if(ac > 3)
            __rf(ac, av, 2);
        else
            printf("my_tar: agrument less\n");
        return 0;
    go_tf:
        return __tf( av, 2);
    go_uf:
        if(ac > 3)
            __uf(av, 2);
        else  
            printf("my_tar: agrument less\n");
        return 0;
    go_xf:
        __xf( av, 2);
    
    return 0;
}
