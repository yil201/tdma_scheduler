This is a Linux kernel module that intercepts outgoing TCP packets and sends them out immediately.

Build instructions:
1. Clone the repository to your local machine.
2. Navigate to the directory containing the source code and the Makefile.
3. Run the command make to build the kernel module.
4. If the build is successful, a .ko file should be generated in the same directory.

Usage:
1. Load the kernel module into the kernel using the command `sudo insmod kmod_netfilter_tcp_send.ko`
2. The module will start intercepting outgoing TCP packets and sending them out immediately.
3. To unload the module from the kernel, use the command `sudo rmmod kmod_netfilter_tcp_send`
