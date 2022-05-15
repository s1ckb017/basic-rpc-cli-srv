/* file: hellos.c */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <windows.h>
#include "interfaces.h"
#include "utils.h"

void HelloProc(
    /* [string][in] */ unsigned char* pszString)
{
    printf(pszString);
}

void Shutdown(void)
{
    printf("Shutdown\n");
}

void main(int argc, char** argv)
{
    
    RPC_STATUS status;
    unsigned char* pszSecurity = NULL;
    unsigned int    cMinCalls = 10;
    unsigned int    fDontWait = FALSE;
    unsigned char* protocol = NULL;
    unsigned char* endpoint = 0;
    RPC_BINDING_VECTOR* pbindingVector = 0;

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

    status = RpcServerUseProtseqEp(protocol,
        1,
        endpoint,
        NULL);

    if (status) exit(status);
    printf("[+] Intialized Server RPC PROTO\n");

    status = RpcServerRegisterIf(interfaces_v1_0_s_ifspec,
        NULL,
        NULL);
    if (status) exit(status);
    printf("[+] Server RPC Registered\n");
    status = RpcServerInqBindings(&pbindingVector);
    if (status != RPC_S_OK)
    {
        return 1;
    }
    status = RpcEpRegister(interfaces_v1_0_s_ifspec,   // from rpcsvc.h
        pbindingVector,
        0,
        0);

    if (status != RPC_S_OK)
    {
        return;
    }
    // Enable NT LM Security Support Provider (NtLmSsp service)
    printf("[+] Server RPC Registered\n");
    status = RpcServerRegisterAuthInfo(0,
        RPC_C_AUTHN_WINNT,
        0,
        0
    );
    if (status != RPC_S_OK)
    {
        return;
    }

    printf("[+] Going to wait a client\n");
    status = RpcServerListen(cMinCalls,
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,
        fDontWait);
    free(endpoint);

    if (status) exit(status);
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