/*
 * ssl_common.h
 * Kyle Isom <coder@kyleisom.net>
 * 20100113
 *
 * custom library for initializing / setting up SSL contexts
 * a large portion of this was derived from work by Eric Rescorla.
 * I found the following IBM developerworks article to be useful as well:
 * http://www.ibm.com/developerworks/linux/library/l-openssl.html
 *
 * released under a BSD license as follows:
 *
 *  Copyright 2010 Kyle Isom. All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:
 *  
 *     1. Redistributions of source code must retain the above copyright notice, 
 *        this list of conditions and the following disclaimer.
 *  
 *     2. Redistributions in binary form must reproduce the above copyright 
 *        notice, this list of conditions and the following disclaimer in the 
 *        documentation and/or other materials provided with the distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED BY KYLE ISOM ``AS IS'' AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
 *  EVENT SHALL KYLE ISOM OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 *  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 *  The views and conclusions contained in the software and documentation are 
 *  those of the authors and should not be interpreted as representing official 
 *  policies, either expressed or implied, of Kyle Isom. 
 */
 
 #include <openssl/bio.h>
 #include <openssl/err.h>
 #include <openssl/ssl.h>
 #include <errno.h>
 #include <signal.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 
 #define SSL_SUCCESS                0
 #define CONNECT_ERROR              1
 #define CLEAN_SSL_DISCONNECT       2
 #define UNCLEAN_SSL_DISCONNECT     3
 
 #define SSL_TIMEOUT                1
     
 
 SSL_CTX* initialize_ctx(void);
 //SSL_CTX* init_ctx(char *keyfile, char *pass);    // need to work on authenticating SSL
 int ssl_connect(char *hostip, int port, BIO *bp, SSL_CTX *ctx, SSL *sslp);
 int ssl_initialize(char *host, int port, BIO *bp, SSL_CTX *ctx, SSL *sslp);
 int destroy_ssl(BIO *bp, SSL_CTX *ctx, SSL *sslp);
 char *get_hoststr(char* host, int port);
 int process_SSL_error(SSL *sslr);
