#include"server.h"
#include <iomanip>
using namespace std;
int serverA_socket=socket(AF_INET, SOCK_DGRAM,0);

 //define the server address
struct sockaddr_in serverA_address;

int main()
{
	//abput how to use sendto function, recvfrom is from https://www.youtube.com/watch?v=Emuw71lozdA
	//about how to create UDP socket is from from https://www.youtube.com/watch?v=Emuw71lozdA
	//code line 15-26 is the Reused code from https://www.youtube.com/watch?v=Emuw71lozdA
	//define the serverA address
	serverA_address.sin_family=AF_INET;
	serverA_address.sin_port=htons(21495);
	serverA_address.sin_addr.s_addr=inet_addr("127.0.0.1");

	//int udp_socket=socket(AF_INET, SOCK_DGRAM,0);
	//define the AWS address
	struct sockaddr_in udp_address;
	udp_address.sin_family=AF_INET;
	udp_address.sin_port=htons(24495);
	udp_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    socklen_t addr_len=sizeof (udp_address);


    //bind the socket
    if(bind(serverA_socket,(struct sockaddr*) &serverA_address, sizeof(serverA_address))>-1)
    {
    	cout<<"The Server A is up and runnindg using UDP on port <21495>"<<endl;
    }
    else
    {
    	cout<<"bind error"<<endl;
    }

    long int link_id;

	while(1)
	{
		cout<<fixed<<setprecision(2);
		//receive link_id from AWS server
		if(recvfrom(serverA_socket,&link_id, sizeof(link_id),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
		{
			search("database_a.csv",link_id);//search the database, if there is a match, flag=1; if not, flag=0
			cout<<"The server A received input "<<"<"<<link_id<<">"<<endl;
			sendto(serverA_socket,&m,sizeof(m),0,(struct sockaddr*) &udp_address,addr_len);	
			if(m)
			{
				cout<<"The Server A has found <1> match"<<endl;
				//after finding a match, send detailed information to AWS server
				sendto(serverA_socket,&bandwidth,sizeof(bandwidth),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverA_socket,&length,sizeof(length),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverA_socket,&velocity,sizeof(velocity),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverA_socket,&Noise_power,sizeof(Noise_power),0,(struct sockaddr*) &udp_address,addr_len);
				cout<<"The Server A finished sending the output to AWS"<<endl;
				
			}
			else
			{
				cout<<"The Server A has found <0> match"<<endl;
			}

		}
		//cout<<"1"<<endl;
	}
	close(serverA_socket);
	//close(udp_socket);
	return 0;
}