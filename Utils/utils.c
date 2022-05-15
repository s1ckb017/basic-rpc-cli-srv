#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

unsigned char* protoseq[] = {
    L"ncacn_np",
    L"ncacn_ip_tcp",
    L"ncacn_http",
    L"ncadg_ip_udp",
    L"ncalrpc"
};


void usage(char *arg0)
{
    printf("Usage: %s <protoID> <endpoint>\n", arg0);
    for (int i = 0; i < sizeof(protoseq) -1; i++)
    {
        printf("\t%d\t%ls\n", i, protoseq[i]);
    }
}

char* get_protocol(char* protoId)
{
    unsigned int id = atoi(protoId);
    if (id >= sizeof(protoseq))
    {
        return NULL;
    }
    return protoseq[id];
}

char* convert_to_wide(char* asciiarg)
{
    size_t convertedChars = 0;
    unsigned char * endpoint = NULL;

    endpoint = malloc((strlen(asciiarg) * 2) + 1);
    if (endpoint != NULL)
        mbstowcs_s(&convertedChars, endpoint, strlen(asciiarg) + 1, asciiarg, _TRUNCATE);
    return endpoint;
}