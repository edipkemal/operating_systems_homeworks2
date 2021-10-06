#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//Edip Kemal Sardoðan 240201026

typedef struct{
	char *name;
	int totalPoint;	
}Player;


const char* getUserName(){
	char *userName;
	printf("Please enter username :");
	scanf("%[^\n]%*c",userName);
	printf("\nUser registered as \"%s\"\n", userName);
	
	return userName;
}

int rollDices(Player player){
	int d1,d2,d3,score; //dices
	d1=rand()%6+1;
	d2=rand()%6+1;
	d3=rand()%6+1;
	int dices[]={d1,d2,d3};
	
	int temp;
	// sorting dices 
	while(dices[0]>dices[1] || dices[1]>dices[2] || dices[0]>dices[2]){  
		if(dices[0]>dices[1]){
			temp=dices[0];
			dices[0]=dices[1];
			dices[1]=temp;
		}
		if(dices[1]>dices[2]){
			temp=dices[1];
			dices[1]=dices[2];
			dices[2]=temp;
		}
	}
	printf("%s rolled: %d-%d-%d\n",player.name,dices[0],dices[1],dices[2]);
	
	score=calculateScore(dices);
	if(score<0){
		printf("%s lost %d points.\n",player.name,score*(-1));
	}else{
		printf("%s gained %d points.\n",player.name,score);
	}
	player.totalPoint+=score;
	
	return score;
}


int calculateScore(int dices[]){
	int d1,d2,d3,score;
	
	d1=dices[0]; //smallest
	d2=dices[1];
	d3=dices[2]; //largest
	
	if(d1==d3){ //all equal since d1 largest d3 smallest
		if(d1==1){
			score=-10;
		}else if(d1==6){
			score=10;
		}else{
			score=5;
		}
	}else if(d1==1 && d2==2 && d3==3){
		score=-3;
	}else if(d1==4 && d2==5 && d3==6){
		score=3;
	}else if(d1==d2){
		if(d1>3){
			score=d3;
		}else if(d1<4){
			score=-1*d3;
		}
	}else if(d2==d3){
		if(d2>3){
			score=d1;	
		}else if(d2<4){
			score=-1*d1;
		}
	}else{
		score=0;
	}
	
	return score;
}

int main(){
	
	Player p1;
	Player p2;
	Player p3;
	Player pUser; // players
	p1.totalPoint=0;
	p2.totalPoint=0;
	p3.totalPoint=0;
	pUser.totalPoint=0;
	printf("test");
	p1.name="Player1";
	p2.name="Player2";
	p3.name="Player3";
	pUser.name=getUserName();
	
	
	char start[5];
	printf("\nType \"start\" to begin the game:");
	scanf(" %s",start);
	
	if(strcmp(start,"start")==0){ // game starts
		char roll;
		
		int roundCount;
	
		roundCount=1;
		srand(time(NULL));
		
		while(p1.totalPoint<20 && p2.totalPoint<20 && p3.totalPoint<20 && pUser.totalPoint<20){
			printf("%d. Round\n",roundCount);
			
			rollDices(p1);
			
			printf("Roll the dice(y/n)?:");
			scanf(" %c",&roll);
			if(roll=='y'){
		
				printf("\nPoints Table after %d. round:\n",roundCount);
				
				
				
				
			}else{
				break;
			}
			roundCount++;
		}
		printf("exit");
		
	}else{
		printf("Bye");
	}
	
	return 0;
}
