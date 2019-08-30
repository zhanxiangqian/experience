#include <iostream>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int led[3];//r y g
int num;
int stateTime[100][2];
int main(int argc, char** argv) 
{
	
	scanf("%d%d%d",&led[0], &led[1],&led[2]);
	
	
    int allTime = 0;
	
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

int  getAllTime(int stateTime[100][2];)
{
	int allTime = 0;
	for(int i = 0;i < num; i++)
	{
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
	return allTime;
}

void getCrossTime(int currentTime, int ledType, int sateTime, int & newledType, int & newStateTime)
{
	int d = currentTime % (led[0] + led[1] + led[2]);
	if(sateTime - d > 0)
		{
			newStateTime = sateTime - d;
			newledType = ledType;
		}
		else
		{
			if(sateTime + led[2] - d > 0 )
			{
				newStateTime =sateTime + led[2] - d;
				newledType = ledType + 2;
			}
			else
			{
				if(sateTime + led[2] + led[1]- d > 0 )
				{
					newStateTime =sateTime + led[2] + led[1] - d;
					newledType = ledType + 1;
				}
				else
				{
					newStateTime =sateTime + led[2] + led[1] + led[0] - d;
					newledType = type;                           
				}
			}
		}
}
