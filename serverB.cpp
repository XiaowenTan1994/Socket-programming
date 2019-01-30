#include"server.h"
#include <iomanip>
using namespace std;
int serverB_socket=socket(AF_INET, SOCK_DGRAM,0);//create the UDP socket


int main()
{
	//abput how to use sendto function, recvfrom is from https://www.youtube.com/watch?v=Emuw71lozdA
	//about how to create UDP socket is from from https://www.youtube.com/watch?v=Emuw71lozdA
	//code line 12-23 is the Reused code from https://www.youtube.com/watch?v=Emuw71lozdA
	struct sockaddr_in serverB_address; //define the serverB address
	serverB_address.sin_family=AF_INET;
	serverB_address.sin_port=htons(22495);
	serverB_address.sin_addr.s_addr=inet_addr("127.0.0.1");

	int udp_socket=socket(AF_INET, SOCK_DGRAM,0);
	struct sockaddr_in udp_address;
	udp_address.sin_family=AF_INET; //define the AWS address
	udp_address.sin_port=htons(24495);
	udp_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    socklen_t addr_len=sizeof (udp_address);


    //bind the socket
    if(bind(serverB_socket,(struct sockaddr*) &serverB_address, sizeof(serverB_address))>-1)
    {
    	cout<<"The Server B is up and runnindg using UDP on port <22495>"<<endl;
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
		if(recvfrom(serverB_socket,&link_id, sizeof(link_id),0, (struct sockaddr*) &udp_address,&addr_len)>-1)
		{
			search("database_b.csv",link_id);// search database, if find a match, flag=1; if not , flag=0
			//cout<<bandwidth<<"   "<<length<<"   "<<velocity<<"   "<<Noise_power<<endl;
			cout<<"The server B received input "<<"<"<<link_id<<">"<<endl;
			sendto(serverB_socket,&m,sizeof(m),0,(struct sockaddr*) &udp_address,addr_len);// tell AWS whether server B found a match	
			if(m)
			{
				cout<<"The Server B has found <1> match"<<endl;
				//send detailed information to AWS server
				sendto(serverB_socket,&bandwidth,sizeof(bandwidth),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverB_socket,&length,sizeof(length),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverB_socket,&velocity,sizeof(velocity),0,(struct sockaddr*) &udp_address,addr_len);
				sendto(serverB_socket,&Noise_power,sizeof(Noise_power),0,(struct sockaddr*) &udp_address,addr_len);
				cout<<"The Server B finished sending the output to AWS"<<endl;
				
			}
			else
			{
				cout<<"The Server B has found <0> match"<<endl;
			}

		}
		else
		{
			cout<<"The serverB failed to receive data"<<endl;
		}
	}
	close(serverB_socket);
	//close(udp_socket);
	return 0;
}