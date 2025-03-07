// sha256_demo.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <linux/if_alg.h>

// Helper function
void print_hex(const unsigned char *buf, size_t len) {
    for (size_t i = 0; i < len; i++)
        printf("%02x", buf[i]);
    printf("\n");
}


int verify_digest(const unsigned char *computed, const unsigned char *expected, size_t len) {
    if (memcmp(computed, expected, len) == 0) {
        printf("Computed digest matches the expected digest.\n");
        return 1;
    } else {
        printf("Computed digest does not match the expected digest :( \n");
        return 0;
    }
}

// Function to perform the crypto demo using AF_ALG.
void crypto_demo() {
    int sockfd, opfd, ret;
	//Set up the sockaddr_alg structure 
    struct sockaddr_alg sa = {
        .salg_family = AF_ALG,
        .salg_type = "hash",
        .salg_name = "sha256"
    };

    // Create a socket for the crypto API.
    sockfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
    if (sockfd == -1) {
        perror("socket fail");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to algo SHA-256
    ret = bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    if (ret == -1) {
        perror("binding error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Accept an operation socket
    opfd = accept(sockfd, NULL, 0);
    if (opfd == -1) {
        perror("accepting failure");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    const char *message = "Hello World";
    size_t msg_len = strlen(message);
    ret = write(opfd, message, msg_len);
    if (ret == -1) {
        perror("writing fail");
        close(opfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // SHA-256 produces 32 bytes
    unsigned char digest[32];
    ret = read(opfd, digest, sizeof(digest));
    if (ret == -1) {
        perror("reading fail");
        close(opfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //Output computed digest in hexa
    printf("Computed SHA-256 digest: ");
    print_hex(digest, sizeof(digest));

   // This expected digest is generated using:
    //   echo -n "Hello World" | sha256sum
    unsigned char expected_digest[32] = {
        0xa5, 0x91, 0xa6, 0xd4, 0x0b, 0xf4, 0x20, 0x40,
        0x4a, 0x01, 0x17, 0x33, 0xcf, 0xb7, 0xb1, 0x90,
        0xd6, 0x2c, 0x65, 0xbf, 0x0b, 0xcd, 0xa3, 0x2b,
        0x57, 0xb2, 0x77, 0xd9, 0xad, 0x9f, 0x14, 0x6e
    };

    verify_digest(digest, expected_digest, sizeof(digest));

    // close sockets.
    close(opfd);
    close(sockfd);
}

int main(void) {
    crypto_demo();
    return 0;
}
