#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>

static struct nf_hook_ops nfho; // netfilter hook function to intercept incoming packets

unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *ip_header = (struct iphdr *) skb_network_header(skb);
    struct tcphdr *tcp_header = (struct tcphdr *) skb_transport_header(skb);

    // check if the packet is a TCP packet
    if (ip_header->protocol == IPPROTO_TCP)
    {
        // do something with the packet
        // ...

        // send out the packet immediately
        skb_dst_drop(skb);
        nf_send(skb, state->net, state->pf, 0);

        return NF_STOLEN;
    }

    return NF_ACCEPT;
}

// module init
static int __init init_hook(void)
{
    // netfilter hook init
    nfho.hook = hook_func;                  
    nfho.hooknum = NF_INET_LOCAL_IN;        
    nfho.pf = PF_INET;                      
    nfho.priority = NF_IP_PRI_FIRST;        

    // register the hook
    nf_register_hook(&nfho);               

    return 0;
}

// module cleanup 
static void __exit cleanup_hook(void)
{
    nf_unregister_hook(&nfho);              
}

module_init(init_hook);                     
module_exit(cleanup_hook);                  