#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
	Edip Kemal SARDOGAN
	240201026
	this code compiled with gnu99
*/
int mutex=1;
int favoriesSize=0;

struct product{
	char *product_name; // (phone, tshirt, coke etc.)
	char *product_type; //(electronicDevice, clothing, market etc.)
	int price; // (can be integer between 1-1000)
	struct product *next;
};

typedef struct{ //parameter struct for type 1 thread job(cut and paste)
	struct product *list1; //one of the product lists
	struct product *list2; //favories list
	char* product_type; // type of the favorite product
	char* listName; //name of the list
}cutPasteParam;

typedef struct{ //parameter struct for type 2 thread job(price increasing)
	struct product* productList; //one of the product lists
	char* listName; //name of the list
}priceIncreasingParam;

typedef struct{ //parameter struct for type 3 thread job(product offering)
	struct product* productList; //favories list
	int budget; //budget of user
}productOfferParam;

struct product* fillProductLists(int listNumber); //takes the number of list and creates items for this number
void addProduct(struct product **head, char* product_name,char* product_type,int price);
void showProducts(struct product* productList, char* listName);
void signal(int* value);
void wait(int* value);
void* cutAndPaste(void *param); // type 1 thread job
void* priceIncreasing(void *param); //type 2 thread job
void* productOffering(void *param); // type 3 thread job
int findSize(struct product *productList); 

int main(){
	srand(time(NULL)); 
	pthread_t th1,th2,th3,th4;

	struct product *products1;

	products1=fillProductLists(1);
	struct product *products2;
	products2=fillProductLists(2);
	struct product *favories=(struct product*)malloc(sizeof(struct product));
	showProducts(products1,"Products 1");
	showProducts(products2,"Products 2");
	
	int list_1_size,list_2_size;

	while(1){
		wait(&mutex);
		printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
		showProducts(favories,"\nFavories");
		list_1_size=findSize(products1);
		list_2_size=findSize(products2);

		if(list_1_size<=1 & list_2_size <=1){
			printf("Products1 and Products2 list are empty..\n");
			break;
		}

		printf("Please enter your favorite product_type (in lower case):\n");
		char favProduct[20];
		scanf("%s",favProduct);
		printf("Product_type: %s\n",favProduct);
		printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");

		printf("\nPlease enter your budget for shopping:\n");
		int budget;
		scanf("%d",&budget);
		printf("Budget: %d\n",budget);
		printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
		signal(&mutex);

		if(list_1_size!=0){
			cutPasteParam *param1=(cutPasteParam*)malloc(sizeof(cutPasteParam));
			param1->list1=products1;
			param1->list2=favories;
			param1->product_type=favProduct;
			param1->listName="Products 1";
			pthread_create(&th1,NULL,cutAndPaste,(void*)param1);
		
		}
		if(list_1_size!=0){
			cutPasteParam* param2=(cutPasteParam*)malloc(sizeof(cutPasteParam));
			param2->list1=products2;
			param2->list2=favories;
			param2->listName="Products 2";
			param2->product_type=favProduct;
			pthread_create(&th2,NULL,cutAndPaste,(void*)param2);
		}
		
		int listNumber=rand()%3; //random list number selection for switch case
		priceIncreasingParam* param3=(priceIncreasingParam*)malloc(sizeof(priceIncreasingParam));

		switch(listNumber){
			case 0://product list 1
				param3->productList=products1;
				param3->listName="Products 1";
				break;
			case 1: //product list 2
				param3->productList=products2;
				param3->listName="Products 2";
				break;
			case 2: //favories
				param3->productList=favories;
				param3->listName="Favories";
				break;
			default:
				printf("Invalid Number!");
				exit(1);
		}
		
		productOfferParam* param4=(productOfferParam*)malloc(sizeof(productOfferParam));
		param4->budget=budget;
		param4->productList=favories;

		pthread_create(&th3,NULL,priceIncreasing,(void*)param3);
		pthread_create(&th4,NULL,productOffering,(void*)param4);
		
		pthread_join(th1,NULL);
		pthread_join(th2,NULL);
		pthread_join(th3,NULL);
		pthread_join(th4,NULL);
	}
	pthread_exit(NULL);
}

void* cutAndPaste(void *param){
	printf("Type 1 thread is performing.\n");
	wait(&mutex);

	cutPasteParam *parameter;
	parameter=(cutPasteParam*) param;

	struct product *productList=parameter->list1;
	struct product *favories=parameter->list2;
	struct product *new_list=(struct product*)malloc(sizeof(struct product));

	char* type=parameter->product_type;
	char* listName=parameter->listName;
	int count,new_list_size;
	count=0;
	new_list_size=0;
	for(;productList!=NULL;productList=productList->next){
		if(!(strcmp(type,productList->product_type)) & count!=2){//types matched
			if(favoriesSize==0){//favories list empty, create first element
				favories->product_name=productList->product_name;
				favories->product_type=productList->product_type;
				favories->price=productList->price;
				favories->next=NULL;
			}else{
				addProduct(&favories,productList->product_name,productList->product_type,productList->price);
			}
			count++;
			favoriesSize++;
		}else{ //types mismatched add item to new list
			if(new_list_size==0){//if there is no element, create the first element manually
				new_list->product_name=productList->product_name;
				new_list->product_type=productList->product_type;
				new_list->price=productList->price;
				new_list->next=NULL;
			}else{// if there is already some element, add the next element with method
				addProduct(&new_list,productList->product_name,productList->product_type,productList->price);
			}
			new_list_size++;
		}
	}
	printf("%d products are cut from %s and pasted to favories.\n",count,listName);
	*(parameter->list1)=*new_list;

	signal(&mutex);
	pthread_exit(NULL);
}

//finds the number of elements
int findSize(struct product *productList){
	int size=0;
	for(;productList!=NULL;productList=productList->next){
		size++;
	}
	return size;
}

void* priceIncreasing(void *param){
	printf("Type 2 thread is performing.\n");
	wait(&mutex);
	
	priceIncreasingParam* parameter=(priceIncreasingParam*)param;

	struct product *productList=parameter->productList;
	char* listName=parameter->listName;

	int i,size;
	size=findSize(productList);

	int product_index=rand()%size;	//random number between 0 and size of the list

	for(i=0;i<product_index;i++){//go to random indexed element
		productList=productList->next;
	}
	productList->price+=productList->price*0.2; // price increased %20

	printf("\nProduct price increased in %s: %s, %d\n\n",listName,productList->product_name,productList->price);
	
	signal(&mutex);
	pthread_exit(NULL);
}


void* productOffering(void *param){
	printf("Type 3 thread is performing.\n");
	wait(&mutex);

	int i,j, budget;
	j=0;
	productOfferParam* parameter=(productOfferParam*)param;
	struct product* favories=parameter->productList;
	budget=parameter->budget;

	char productNames[favoriesSize][20];

	for(i=0;i<favoriesSize;i++){
		if(favories->price<=budget){//product is cheaper than budget
			strcpy(productNames[j],favories->product_name);
			j++;
		}
		favories=favories->next;
	}
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("You can buy products which are less than your budget: ");
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");

	for(i=0;i<j;i++){
		printf("%s",productNames[i]);
		if(i!=j-1){//dont put comma after last element
			printf(", ");
		}
	}
	printf("\n");
	signal(&mutex);
	pthread_exit(NULL);
}

void showProducts(struct product* productList, char* listName){
	printf("%s:\n\n",listName);
	while(productList!=NULL){
		if(!strcmp(listName,"Favories")&favoriesSize==0){
			break;//favories list created but there is no element, values are null, so break
		}
		printf("product_name: %s, product_type: %s, price: %d\n",productList->product_name,productList->product_type,productList->price);
		productList=productList->next;
	}
	printf("\n%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
}

void signal(int* value){
	while(*value==1);
	*value=*value+1;
}

void wait(int* value){
	while(*value==0);
	*value=*value-1;
}

void addProduct(struct product **head, char* product_name,char* product_type,int price){
	struct product *new_product=(struct product*)malloc(sizeof(struct product));
	
	if(new_product!=NULL){
		//create a new product with values
		new_product->product_name=product_name;
		new_product->product_type=product_type;
		new_product->price=price;
		new_product->next=NULL;
		while(*head !=NULL){//while there is a next element
			head=&(*head)->next;//go to next element
		}
		*head=new_product;
	}
}


struct product* fillProductLists(int listNumber){
	struct product p;
	struct product *productHead;
	productHead=(struct product*)malloc(sizeof(struct product));

	switch(listNumber){
		case 1://create products for product list 1
			productHead->product_name="iphoneX";
			productHead->product_type="electronicdevice";
			productHead->price=1000;
			productHead->next=NULL;

			addProduct(&productHead,"shoes","clothing",550);
			addProduct(&productHead,"powerbank","electronicdevice",100);
			addProduct(&productHead,"detergent","market",20);
			addProduct(&productHead,"coke","market",5);
			addProduct(&productHead,"tshirt","clothing",70);
			addProduct(&productHead,"puding","market",3);
			addProduct(&productHead,"scarf","clothing",10);
			addProduct(&productHead,"mouse","electronicdevice",50);
			addProduct(&productHead,"usb","electronicdevice",20);
			addProduct(&productHead,"water","market",2);
			addProduct(&productHead,"belt","clothing",15);
			break;
		case 2://create products for product list 2
			productHead->product_name="jean";
			productHead->product_type="clothing";
			productHead->price=220;
			productHead->next=NULL;

			addProduct(&productHead,"dress","clothing",300);
			addProduct(&productHead,"hairDrier","electronicdevice",300);
			addProduct(&productHead,"blender","electronicdevice",500);
			addProduct(&productHead,"coffee","market",75);
			addProduct(&productHead,"tea","market",30);
			addProduct(&productHead,"tea","clothing",30);
			addProduct(&productHead,"tea","market",30);
			addProduct(&productHead,"tea","clothing",30);
			addProduct(&productHead,"tea","electronicdevice",30);
			addProduct(&productHead,"tea","market",30);
			addProduct(&productHead,"tea","electronicdevice",30);
			break;
		default:
			printf("Invalid list number!");
			exit(1);
	}
	return productHead;
}