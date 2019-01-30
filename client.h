#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string>
#include<errno.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<math.h>
#include<iostream>
using namespace std;
float convert_stringtofloat(string s)
{
	float num=0;
	int dot_position;
	int a = s.find('.'); //find the position of '.'
	if (a > -1)   //for the data form xxxx.xxx
	{
		int integar_position = 0;
		int decimal_position = 1;
		int minus=s.find('-');
		if(minus==-1)
		{
			for (int i = a - 1; i >= 0; i--)
			{
				num = num + (s[i] - '0')*pow(10, integar_position);
				integar_position++;
			}
			for (int i = a + 1; i < s.length(); i++)
			{
				num = num + (s[i] - '0')*pow(0.1, decimal_position);
				decimal_position++;
			}
		}
		else
		{
			for (int i = a - 1; i >= 1; i--)
			{
				num = num + (s[i] - '0')*pow(10, integar_position);
				integar_position++;
			}
			for (int i = a + 1; i < s.length(); i++)
			{
				num = num + (s[i] - '0')*pow(0.1, decimal_position);
				decimal_position++;
			}
			num=-num;
		}
	}
	else  //for the data form xxxxx
	{
		int position = 0;
		int minus=s.find('-');
		if(minus==-1)
		{
			for (int i = s.length()-1; i >= 0; i--)
			{
				num = num + (s[i] - '0')*pow(10,position);
					position++;
			}
		}
		else
		{
			for (int i = s.length()-1; i >= 1; i--)
			{
				num = num + (s[i] - '0')*pow(10,position);
				position++;
			}
			num=-num;
		}

	}
	return num;
}
long int convert_stringtolongint(string s)
{
	long int num=0;
	int position=0;
	for (int i = s.length()-1; i >= 0; i--)
	{
		num = num + (s[i] - '0')*pow(10,position);
		position++;
	}
	return num;

}