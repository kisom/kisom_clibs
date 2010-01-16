/*
 * hostipn.c
 * Kyle Isom <coder@kyleisom.net>
 * version 0.1: 20100112
 *
 * a group of functions handy for distinguishing a char* input between an IPv4
 * address and a hostname.
 *
 * Released under a BSD license. See header for full license.
 */

#include <kisom/hostipn.h>

int is_host_ip(const char *host) {
    int i = 0, j = 0;
    int s_host = strlen(host);
    if (s_host > 15) return 1;      // 15 is the most characters in a dotted quad
    int dot = 0;
    const char numarray[] = "0123456789";
    const int s_numarray = strlen(numarray);
    
    for (i = 0; i < s_host; ++i) {
        if (host[i] == '.') { ++dot; }
        for (j = 0; j < s_numarray; ++j) {
            // matched a numeric character
            if ( host[i] == numarray[j] ) { break; }
            
            // non-numeric character detected
            if ( j == (s_numarray) ) { return 1; }  
        }
    }
    
    // there should be three dots in an IPv4 address
    if (dot != 3) { return 1; }    
    
    return 0;
}

int gethostip(const char *host, int proto, int port, char *hostip) {
    // note that gethostbyname and any other functions based on the
    // host_ent struct are deprecated. The Right Thing to do now is 
    // to use getaddrinfo() and addrinfo structures.
	
    //int host_s = strlen(host);
    char *anyip = "0.0.0.0";
    char c_port[16];
    struct addrinfo *result;		// host_ent is deprecated
    struct addrinfo *rp;
    struct addrinfo hints;
    struct sockaddr_in *rp_a;

    
    sprintf(c_port, "%d", port);
    
    
    // stolen from the getaddrinfo man page
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = (int) proto;    /* use the right socktype */ 
    hints.ai_flags = 0;
    hints.ai_protocol = 0;              /* Any protocol */

    int s = getaddrinfo(host, c_port, &hints, &result);
    
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        rp_a = (struct sockaddr_in *) (rp->ai_addr);
        if ( strncmp(anyip, inet_ntoa(rp_a->sin_addr), 7) != 0 ) { break; }
        if (rp->ai_next == NULL) { return EXIT_FAILURE; }
    }
    
    sprintf(hostip, "%s", inet_ntoa(rp_a->sin_addr));
    
    return EXIT_SUCCESS;     
}

