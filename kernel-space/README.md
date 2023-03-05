# Linux Kernel Module to Intercept Outgoing TCP Packets

This is a Linux kernel module that intercepts outgoing TCP packets and sends them out immediately, without waiting for the network stack to buffer them.

## Prerequisite:
1. Linux kernel headers and build tools are installed using: `sudo apt-get install linux-headers-$(uname -r) build-essential`

## Build instructions:
1. Clone the repository to your local machine.
2. Navigate to the directory containing the source code and the Makefile.
3. Run the command `make` to build the kernel module.
4. If the build is successful, a .ko file should be generated in the same directory.

## Testing the module
1. Load the module into the kernel using the `insmod` command: `sudo insmod kmod_netfilter_tcp_send.ko`. This should load the module into the kernel and start intercepting outgoing TCP packets.
2. Open a new terminal window and start a TCP connection from the machine running the module to another machine. For example, you can use the `telnet` command to connect to a remote server: `telnet <remote_server> <port>`. Alternatively, you can start a web browser and visit a website.
3. Open a new terminal window and start `tcpdump` to capture network traffic: `sudo tcpdump -i <interface_name>`.
4. In another terminal window, start a TCP connection from the machine running the module to another machine. For example, you can use the telnet command to connect to a remote server: `telnet <remote_server> <port>`.
5. As you use the TCP connection, you should see outgoing TCP packets being intercepted by the module and sent out immediately. You can observe the effects of the module by comparing the network traffic captured by tcpdump with and without the module loaded. With the module loaded, outgoing TCP packets should appear immediately in tcpdump output, without waiting for the network stack to buffer them. 
6. To unload the module from the kernel, use the command: `sudo rmmod kmod_netfilter_tcp_send`.
