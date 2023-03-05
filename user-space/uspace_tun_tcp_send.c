#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <linux/if_tun.h>

#define BUFSIZE 2000 // maximum packet size to read from TUN device

int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);

    return fd;
}

int main(int argc, char *argv[])
{
    int tun_fd, nbytes, sock_fd;
    char buf[BUFSIZE];
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;
    struct sockaddr_in sin;

    // create TUN device
    char tun_name[IFNAMSIZ];
    memset(tun_name, 0, IFNAMSIZ);
    tun_alloc(tun_name);

    // create socket for sending packets
    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock_fd < 0) {
        return 1;
    }

    // initialize socket address structure
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(0);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket to address structure
    if (bind(sock_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind() failed");
        return 1;
    }

    // read packets from TUN device and send them out immediately
    while (1) {
        nbytes = read(tun_fd, buf, BUFSIZE);
        if (nbytes < 0) {
            return 1;
        }

        // parse IP and TCP headers
        ip_header = (struct iphdr *)buf;
        if (ip_header->protocol == IPPROTO_TCP) {
            tcp_header = (struct tcphdr *)(buf + ip_header->ihl * 4);

            // modify packet as necessary
            // ...

            // send out the packet immediately
            sin.sin_addr.s_addr = ip_header->daddr;
            sendto(sock_fd, buf, nbytes, 0, (struct sockaddr *)&sin, sizeof(sin));
        }
    }

    return 0;
}
