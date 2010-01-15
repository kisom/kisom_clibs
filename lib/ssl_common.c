/*
 * ssl_common.c
 * Kyle Isom <coder@kyleisom.net>
 * 20100113
 *
 * custom library for initializing / setting up SSL contexts 
 *
 * Released under a BSD-style license. See header for full license.
 */
 
#include "ssl_common.h"
#define DEBUG               1

char hoststr[22];

// initialize an SSL session simply for the purposes of connecting. see
// overloaded form for keyfile / password inclusion.

SSL_CTX* initialize_ctx(void) {
     BIO *bio_err = 0;
     SSL_METHOD *sslm;
     SSL_CTX *ctx;
     
     if (!bio_err) {
         SSL_library_init();
         SSL_load_error_strings();
         
         // SSL error writing context
         bio_err=BIO_new_fp(stderr,BIO_NOCLOSE);
     }
     
     // set up signal handler
     // signal(SIGPIPE, sigpipe_h);
     

     // create SSL context
     sslm=SSLv23_method();
     ctx=SSL_CTX_new(sslm);

     return ctx;
}
/* 
SSL_CTX* init_ctx(char *keyfile, char *pass) {
    SSL_CTX *ctx = initialize_ctx();
    
   // load keys and certs
   if (! (SSL_CTX_use_certificate_chain_file(ctx, keyfile)) ) 
       { berr_exit("error reading certificate file!"); }

    SSL_CTX_set_default_passwd_cb(ctx, pass_cb);
    if  (! (SSL_CTX_use_PrivateKey_file(ctx, keyfile,SSL_FILETYPE_PEM)) )
        { berr_exit("error reading key file!"); }

    // load trusted CAs
    if (! (SSL_CTX_load_verify_locations(ctx, CA_LIST,0)) )
        { berr_exit("error reading CA list!"); }
    
    #if (OPENSSL_VERSION_NUMBER < 0x0090600fL)
    SSL_CTX_set_verify_depth(ctx,1);
    #endif

    return ctx;
}
*/
 
int ssl_connect(char *hostip, int port, BIO *bp, SSL_CTX *ctx, SSL *sslp) {
    int sslr = 0, b_sock;
    
    BIO_set_conn_hostname(bp, hostip);
    BIO_set_conn_int_port(bp, &port);
    bp = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bp, &sslp);
    
    bp = BIO_new_socket(b_sock, BIO_NOCLOSE);
    
    if ( BIO_do_connect(bp) <= 0 ) { 
        sslr = destroy_ssl(bp, ctx, sslp);
        switch (sslr) {
            default:
                return CONNECT_ERROR;
        }

        return CONNECT_ERROR;
    }
 
    #ifdef DEBUG
    printf("initiating SSL handshake...");
    #endif
    sslr = SSL_do_handshake(sslp);
    
    if ( sslr == 0 ) { return CLEAN_SSL_DISCONNECT; }
    else if ( sslr < 0 ) { return UNCLEAN_SSL_DISCONNECT; }
    
    #ifdef DEBUG
    printf("success!\n");
    #endif
    
    return SSL_SUCCESS;
}

int ssl_initialize(char *host, int port, BIO *bp, SSL_CTX *ctx, SSL *sslp) {
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    SSL_library_init();
    strncpy(hoststr, get_hoststr(host, port),  22);
    #ifdef DEBUG
    printf("got hoststr %s\n", hoststr);
    #endif
    
    sslp = SSL_new(ctx);
    SSL_set_mode(sslp, SSL_MODE_AUTO_RETRY);
    

    //BIO_set_conn_hostname(bp, &hoststr);
    return SSL_SUCCESS;
}

int destroy_ssl(BIO *bp, SSL_CTX *ctx, SSL *sslp) {
    printf("shutting down SSL connection\n");
    
    BIO_free_all(bp);
    
    // shutdown SSL connection
    if ( sslp != NULL ) {
        int sslr = SSL_shutdown(sslp);
        if ( sslr < 0 ) { return UNCLEAN_SSL_DISCONNECT; }
        else if ( sslr == 0 ) { return CLEAN_SSL_DISCONNECT; }
        else { return SSL_SUCCESS; }
    }
    
    SSL_CTX_free(ctx);              // free SSL context
    
    return SSL_SUCCESS;
}


char *get_hoststr(char* host, int port) {
    // create a host:port string that is used in the SSL
    // connections in several instances.
    
    // length of hostip string, colon, and max 5 digits for the number
    sprintf(hoststr, "%s:%d", host, port);

    #ifdef DEBUG
    printf("returning host string...\n");
    #endif    
    
    return hoststr;
}

int process_SSL_error(SSL *sslr) {
    return EXIT_FAILURE;
}
