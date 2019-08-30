#include <iostream>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) 
{
	int led[3];//r y g
	scanf("%d%d%d",&led[0], &led[1],&led[2]);
	
	int stateTime[100][2];
    int allTime = 0;
	int num;
	scanf("%d",&num);
	for(int i = 0;i < num; i++)
	{
		scanf("%d%d",&stateTime[i][0],&stateTime[i][1]);
		if (stateTime[i][0] == 0)
		{
			allTime += stateTime[i][1];
		}
		else if(stateTime[i][0] == 1)
		{
			allTime += stateTime[i][1];
		}
		else if(stateTime[i][0] == 2)
		{
			allTime += stateTime[i][1];
			allTime += led[0];
		}
	}
	printf("%d",allTime);
	return 0;
}
