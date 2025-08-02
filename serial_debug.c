void hunt_ghost_bytes(int fd) {
    unsigned char buf[256];
    
    // 1. Limpa a serial
    tcflush(fd, TCIOFLUSH);
    
    // 2. Envia comando conhecido (ex: 'PING')
    write(fd, "PING", 4);
    
    // 3. Captura TUDO com timeout curto
    fd_set fds;
    struct timeval tv = {0, 100000}; // 100ms timeout
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    
    if (select(fd+1, &fds, NULL, NULL, &tv) > 0) {
        ssize_t n = read(fd, buf, sizeof(buf));
        printf("Resposta crua (%zd bytes):\n", n);
        hexdump(buf, n);
        
        // Padrão CRLF?
        if (n >= 2 && buf[n-2] == 0x0D && buf[n-1] == 0x0A) {
            printf("\n>>> ALERTA: CRLF detectado no final! <<<\n");
        }
    }
}
