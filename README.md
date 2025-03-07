# Linux Crypto Demo

This project demonstrates how to use the Linux kernel's cryptographic API with the AF_ALG interface to compute a SHA-256 hash. Inspiration drawn from https://www.kernel.org/doc/html/v4.10/crypto/userspace-if.html. Purpose of this project was to get hands-on experience with low-level Linux cryptography and to practice debugging socket-based applications in C. Google Cloud Shell was used as it supports Linux-specific APIs (like AF_ALG).

## Overview

- **Crypto Demo:** The code creates an AF_ALG socket, binds it to the SHA-256 algorithm, writes a message ("Hello World"), and reads the computed digest.
- **Verification:** The program compares the computed hash with the expected digest (generated using `sha256sum`).

To compile the code:
   ```bash
   gcc -o sha256_demo sha256_demo.c
