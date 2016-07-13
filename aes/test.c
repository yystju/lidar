#include <stdio.h>
#include <string.h>
#include "aes.h"


void output(unsigned char * p)
{
    int i = 0 ;
    for(i = 0 ; p[i] ; i ++)
    {
        if(i && i % 16 == 0 ) printf(" ");
        printf(" 0x%02x ", p[i]);
    }
    printf(" ");
}
int main(int argc, char * argv[])
{
    char szbuf[1024], sztmp[1024];;
    char *pkey = "iamvingo and live";
    struct aes_ctx  aes;
    int len ;

    gen_tabs();
    if(aes_set_key(&aes, pkey , 16) != 0) {
        printf("can't set key : %s ",pkey);
        return 0;
    }
    printf("Please Input string to encrypt : ");
    while(gets(szbuf) > 0 )
    {
        len = strlen(szbuf);
        szbuf[len] = '\0';
        printf("Input is : %s ",szbuf);
        aes_encrypt(&aes,sztmp,szbuf);
        printf("Encryp Result is :  ");
        output(sztmp);
        aes_decrypt(&aes,szbuf,sztmp);
        printf("Descrpy Result is : %s ", szbuf);
        printf("Please Input string to encrypt : ");
    }

    return 0;
}