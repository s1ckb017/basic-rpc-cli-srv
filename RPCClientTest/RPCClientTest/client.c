#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "interfaces.h"
#include "utils.h"


void main(int argc, char ** argv)
{
    RPC_STATUS status;
    unsigned char* pszUuid = NULL;
    unsigned char* protocol;
    unsigned char* pszNetworkAddress = NULL;
    unsigned char* endpoint;
    unsigned char* pszOptions = NULL;
    unsigned char* pszStringBinding = NULL;
    unsigned char* pszString = "hello, world";
    unsigned long ulCode;

    if (argc < 3)
    {
        usage(argv[0]);
        return 1;
    }
    endpoint = convert_to_wide(argv[2]);
    protocol = get_protocol(argv[1]);
    if (endpoint == NULL || protocol == NULL) {
        printf("[-] Error getting proto or endpoint %p %p\n", endpoint, protocol);
        usage(argv[0]);
        return 1;
    }
    printf("[+] Protocol Seq choosen: %ls Endpoint: %ls\n", protocol, endpoint);


    status = RpcStringBindingCompose(pszUuid,
        protocol,
        pszNetworkAddress,
        endpoint,
        pszOptions,
        &pszStringBinding);
    if (status) exit(status);

    printf("[+] Intialized Client RPC PROTO\n");


    status = RpcBindingFromStringBinding(pszStringBinding, &hello_IfHandle);

    if (status) exit(status);
    printf("[+] Client RPC Binded\n");

    status = RpcBindingSetAuthInfo(hello_IfHandle,
                0,
                RPC_C_AUTHN_LEVEL_CONNECT,
                RPC_C_AUTHN_WINNT,
                0,
                0);
    if (status != RPC_S_OK)
    {
        printf("RpcBindingSetAuthInfo failed - %d\n", status);
        return(1);
    }
    printf("[+] Client RPC run on server!\n");
    RpcTryExcept
    {
        HelloProc(pszString);
        Shutdown();
    }
        RpcExcept(1)
    {
        ulCode = RpcExceptionCode();
        printf("Runtime reported exception 0x%lx = %ld\n", ulCode, ulCode);
    }
    RpcEndExcept

        status = RpcStringFree(&pszStringBinding);

    if (status) exit(status);

    status = RpcBindingFree(&hello_IfHandle);

    if (status) exit(status);

    exit(0);
}

/******************************************************/
/*         MIDL allocate and free                     */
/******************************************************/

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr)
{
    free(ptr);
}