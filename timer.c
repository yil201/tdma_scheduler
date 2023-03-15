#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/time.h>

volatile int stop=0;

void sigalrm_handler( int sig )
{
    stop = 1;
}

int main(int argc, char **argv)
{
    struct sigaction sact;
    // int num_sent = 0;
    sigemptyset(&sact.sa_mask);
    sact.sa_flags = 0;
    sact.sa_handler = sigalrm_handler;
    sigaction(SIGALRM, &sact, NULL);

    /*
    alarm(60);  // Request SIGALRM in 60 seconds //
    while (!stop) {
        send_a_packet();
        num_sent++;
    }
    */
    
    send_packet();
    time_t start = time(NULL); // return current time in seconds

    receive_packet();
    time_t end = time(NULL); 

    double diff = difftime(start, end);

    

    printf("send and receive this packet time: %f\n", diff);

    // The following can be used to count # of packets sent //
    
    /*
    int numPackets = 0;
    
    while (time() - start < 60)
    {
        send_packet()
        numPackets++;
        
    }

    printf("Sent %d packets\n", numPackets);
    */
    

    exit(0);
}