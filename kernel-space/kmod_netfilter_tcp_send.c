#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

static struct nf_hook_ops nfho;

// TCP packet sending function
int send_tcp_packet(void)
{
    struct socket *sock;
    struct sockaddr_in sin;
    struct msghdr msg;
    struct iovec iov;
    int len, ret;

    // Create a socket
    sock = (struct socket *)sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &ret);
    if (!sock) {
        printk(KERN_ALERT "Error creating socket\n");
        return -1;
    }

    // Fill in the destination address
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // example: send to loopback interface
    sin.sin_port = htons(80); // example: send to port 80

    // Connect to the destination address
    ret = sock->ops->connect(sock, (struct sockaddr *)&sin, sizeof(sin), 0);
    if (ret < 0) {
        printk(KERN_ALERT "Error connecting to destination address\n");
        sock_release(sock);
        return -1;
    }

    // Prepare the TCP packet
    len = sizeof(struct iphdr) + sizeof(struct tcphdr) + 1; // example: TCP packet size
    char buf[len];
    memset(buf, 0, len);
    struct iphdr *ip_header = (struct iphdr *)buf;
    struct tcphdr *tcp_header = (struct tcphdr *)(buf + sizeof(struct iphdr));
    ip_header->protocol = IPPROTO_TCP;
    ip_header->saddr = in_aton("192.168.1.1"); // example: source IP address
    ip_header->daddr = sin.sin_addr.s_addr;
    tcp_header->source = htons(1234); // example: source port
    tcp_header->dest = sin.sin_port;
    tcp_header->seq = htonl(1); // example: sequence number
    tcp_header->ack_seq = 0;
    tcp_header->doff = sizeof(struct tcphdr) / 4;
    tcp_header->fin = 0;
    tcp_header->syn = 1;
    tcp_header->rst = 0;
    tcp_header->psh = 0;
    tcp_header->ack = 0;
    tcp_header->urg = 0;
    tcp_header->window = htons(65535); // example: window size
    tcp_header->check = 0;
    tcp_header->urg_ptr = 0;

    // Send the TCP packet
    iov.iov_base = buf;
    iov.iov_len = len;
    msg.msg_flags = 0;
    msg.msg_name = &sin;
    msg.msg_namelen = sizeof(sin);
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    ret = sock_sendmsg(sock, &msg, len);
    if (ret < 0) {
        printk(KERN_ALERT "Error sending TCP packet\n");
        sock_release(sock);
        return -1;
    }

    sock_release(sock);

    return 0;
}

// Netfilter hook function
unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    if (send_tcp_packet() < 0) {
        printk(KERN_ALERT "Error sending TCP packet\n");
    }

    return NF_ACCEPT;
}

static int __init init_module(void)
{
    printk(KERN_INFO "TCP packet sender kernel module loaded\n");

    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_POST_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;
    nf_register_hook(&nfho);

    return 0;
}

static void __exit cleanup_module(void)
{
    printk(KERN_INFO "TCP packet sender kernel module unloaded\n");

    nf_unregister_hook(&nfho);
}

module_init(init_module);
module_exit(cleanup_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Enoch Li");
MODULE_DESCRIPTION("Kernel module for sending a TCP packet out");

