#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<fstream> 
#include<vector>
#include<sstream>
#include<string>
#include<iostream>
#include<math.h>
#include <iomanip>
using namespace std;
int serverC_socket=socket(AF_INET, SOCK_DGRAM,0); //create UDP socket


int main()
{
	//abput how to use sendto function, recvfrom is from https://www.youtube.com/watch?v=Emuw71lozdA
	//about how to create UDP socket is from from https://www.youtube.com/watch?v=Emuw71lozdA
	//code line 24-35 is the Reused code from https://www.youtube.com/watch?v=Emuw71lozdA
	struct sockaddr_in serverC_address;//define server C IP address and port number
	serverC_address.sin_family=AF_INET;
	serverC_address.sin_port=htons(23495);
	serverC_address.sin_addr.s_addr=inet_addr("127.0.0.1");

	//int udp_socket=socket(AF_INET, SOCK_DGRAM,0);
	struct sockaddr_in udp_address;// defien AWS server IP address and port number
	udp_address.sin_family=AF_INET;
	udp_address.sin_port=htons(24495);
	udp_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    socklen_t addr_len=sizeof (udp_address);

    //bind the UDP socket
    if(bind(serverC_socket,(struct sockaddr*) &serverC_address, sizeof(serverC_address))>-1)
    {
    	cout<<"The Server C is up and runnindg using UDP on port <23495>"<<endl;
    }
    else
    {
    	cout<<"bind error"<<endl;
    }
    long int link_id,size;
	float power_db;
	bool flag_A, flag_B,flag_AB;
    float Tt, Tp,delay;
	float bandwidth,length, velocity, Noise_power_db;
	float Noise_power, power,capacity;

	while(1)
	{
		cout<<fixed<<setprecision(2);
		//receive detailed information from AWS server
		if((recvfrom(serverC_socket,&flag_AB, sizeof(flag_AB),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
			&&(recvfrom(serverC_socket,&link_id, sizeof(link_id),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
			&&(recvfrom(serverC_socket,&bandwidth, sizeof(bandwidth),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
			&&(recvfrom(serverC_socket,&size, sizeof(size),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
			&&(recvfrom(serverC_socket,&power_db, sizeof(power),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
			&&(recvfrom(serverC_socket,&velocity, sizeof(velocity),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
			&&(recvfrom(serverC_socket,&length, sizeof(length),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
			&&(recvfrom(serverC_socket,&Noise_power_db, sizeof(Noise_power_db),0, (struct sockaddr*) &udp_address,&addr_len)>-1))
		{

			if(flag_AB)
			{	//if there is a match
				cout<<"The Server C received link information of link <"<<link_id<<">, file size <"<<size<<"> bit, and signal power <"<<power_db<<">dB"<<endl;
				power=pow(10,power_db/10)/1000;
				Noise_power=pow(10,Noise_power_db/10)/1000;
				//cout<<link_id<<"   "<<size<<"   "<<length<<"    "<<power<<"   "<<bandwidth<<"   "<<velocity<<"    "<<Noise_power<<endl;
	
				//calculation Tt, Tp,delay
				capacity=bandwidth*log(1+power/Noise_power)/log(2)*1000000;
				Tt=size/capacity*1000;
				Tp=(length*1000)/(velocity*10000000)*1000;
				delay=Tt+Tp;
				//send calculation result to AWS server
				cout<<"The Server C finished the calculation for link <"<<link_id<<">"<<endl;
				sendto(serverC_socket,&Tt,sizeof(Tt),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverC_socket,&Tp,sizeof(Tp),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverC_socket,&delay,sizeof(delay),0,(struct sockaddr*) &udp_address,addr_len);
				cout<<"The Server C finished sending the output to AWS"<<endl;
				
			}
			else
			{
				//cout<<"Since there is no match in server A and server B, there is no result for server C to send"<<endl;
			}

		}
		else
		{
			cout<<"serverC failed to receive information from AWS"<<endl;
		}
	}
	close(serverC_socket);
	//close(udp_socket);
	return 0;
}