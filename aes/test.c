#include <stdio.h>
#include <string.h>
#include "aes.h"

//#define DATA_SIZE 512

void output(unsigned char * p, int n)
{
    int i = 0 ;
    for(i = 0 ; i < n ; i ++)
    {
        if(i && i % 16 == 0 ) printf("\n");
        printf(" 0x%02x ", p[i]);
    }
    printf("\n");
}
int main(int argc, char * argv[])
{
    int DATA_SIZE = 16;//atoi(argv[1]);
    
    printf("DATA_SIZE : %d \n",DATA_SIZE);
    
    char szbuf[DATA_SIZE+1], sztmp[DATA_SIZE+1], sztmp2[DATA_SIZE+1];
    
    char * pkey = "1qazxsw23edcvfr4";
    struct aes_ctx  aes;
    int len ;

    gen_tabs();
    
    if(aes_set_key(&aes, pkey , 16) != 0) {
        printf("can't set key : %s ",pkey);
        return 0;
    }
    
    for(int i = 0; i < 100; ++i)
    {
        memset(szbuf, '\0', DATA_SIZE);
        memset(sztmp, '\0', DATA_SIZE);
        memset(sztmp2, '\0', DATA_SIZE);
        
        for(int j = 0; j < DATA_SIZE; ++j) {
            szbuf[j] = (char)('0' + ((i + j) % 20));
        }
        
        szbuf[DATA_SIZE+1] = '\0';
        
        printf("Original : %s \n", szbuf);
        aes_encrypt(&aes,sztmp,szbuf);
        printf("Encryp Result is :  \n");
        output(sztmp, DATA_SIZE);
        aes_decrypt(&aes,sztmp2,sztmp);
        sztmp2[DATA_SIZE] = '\0';
        printf("Descrpy Result is : %s \n", sztmp2);
        
        printf("comparation result : %s\n", strncmp(sztmp2, szbuf, 16) == 0 ? "yes": "no");
    }

    return 0;
}