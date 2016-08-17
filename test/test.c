#include "../src/lidar.h"
#include "../src/xsens.h"

#include "../src/raspi.h"
#include "../src/helper.h"

#include "../src/serial.h"

#include "../src/repository.h"

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>

int test_repository (int argc, char * argv[]) {
     Configuration * configurations = read_configuration_file("./main.ini", 5);
    
    const char * ins_device = (const char *)get_configuration(configurations, "repository.root");
    
    debug("ins_device : %s\n", ins_device);
    
    REPOSITORY * repo = repository_init(ins_device);
    
    REPO_ITEM * item = repository_new_item(repo);
    
    repository_close_item(item);
    
    FILE * fp = repository_open_write_file(item, "lidar");
    
    debug("fp : %p\n", fp);
    
    fprintf(fp, "%s\n", repo->root_pathname);
    
    fclose(fp);
    
    repository_dispose(repo);
    
    dispose_configuration(configurations);
    
    return 0;
}

int test_encoder_decoder(int argc, char * argv[]) {
    //char buff[1024];
    
    //format_gprmc(buff, sizeof(buff), 2016, 6, 5, 10, 59, 00);
    
    //puts(buff);
    
    //blink(10);
    
    gen_tabs();
    
    char * src = "Hello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\nHello This is a test.\n";
    
    debug("strlen(src) : %d\n", strlen(src));
    
    char encoded[1024];
    
    char decoded[1024];
    
    int len = encode_data(encoded, src, strlen(src));
    
    debug("len : %d\n", len);
    
    int len2 = decode_data(decoded, encoded, len);
    
    debug("len2 : %d\n", len2);
    
    decoded[len2] = '\0';
    
    debug("--1--\n");
    
    debug("decoded : %s\n", decoded);
    
    debug("strlen(decoded) : %d\n", strlen(decoded));
    
    debug("src and decoded are %s.\n", strcmp(src, decoded) == 0 ? "equal" : "not equal");
    
    return 0;
}

int test_xsens(int argc, char * argv[]) {
    Configuration * configurations = read_configuration_file("./main.ini", 5);
    
    // for(int i = 0; i < configurations->length; ++i) {
    //     debug("[%s=%s]\n", (configurations->pConfig)[i].key, (configurations->pConfig)[i].value);
    // }
    
    const char * ins_device = (const char *)get_configuration(configurations, "ins.device");
    
    debug("ins_device : %s\n", ins_device);
    
    readXsensData(ins_device, xsensDataProcessor, (void *) NULL);
    
    dispose_configuration(configurations);
    
    return 0;
}
