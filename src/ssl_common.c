/*
 * ssl_common.c
 * Kyle Isom <coder@kyleisom.net>
 * 20100113
 *
 * custom library for initializing / setting up SSL contexts 
 *
 * Released under a BSD-style license. See header for full license.
 */
 
#include <kisom/ssl_common.h>
#define DEBUG               1

// initialize an SSL session simply for the purposes of connecting. see
// overloaded form for keyfile / password inclusion.

/*
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
*/
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
 
int bio_connect(char *hostip, int port, BIO *bp) {
    char *hoststr = malloc(sizeof(hostip) + 7);
    sprintf(hoststr, "%s:%d", hostip, port);
    //int retc = BIO_SUCCESS;         // return code
    
    //bp = BIO_new_socket(bio_sfd, BIO_NOCLOSE);
    
    bp = BIO_new_connect(hoststr);
    
    //retc = BIO_do_connect(bp);
    //if ( retc < 1 ) { printf("error connecting BIO!\n"); return BIO_ERR; }
    free(hoststr);
    return BIO_SUCCESS;
}


