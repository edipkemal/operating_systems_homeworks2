// This code compiled with C18
//Edip Kemal Sardogan 240201026
#include <stdlib.h>
#include <stdio.h>
#define PRODUCT_COUNT 10


typedef struct{
	char *name;
	double price;
	int quantity;
}Product;

Product productArray[PRODUCT_COUNT]; //product storage
Product selectedProducts[PRODUCT_COUNT]; //products which are selected by user
int numberOfSelectedProducts=0;

Product createProduct(char *name, double price, int quantity);
int getLen(const char* str);
void listProducts();
void showMenu();
void storeProducts();
int buyProduct();
void shoppingSummary();
int productAlreadyExists(Product product);

int main(){
	int selection;

	storeProducts();
	showMenu();

	while(selection!=4){
		printf("Enter the choice:");
		scanf("%d",&selection);

		switch(selection){
			case 1:
				buyProduct();
				break;
			case 2:
				listProducts();
				break;
			case 3:
				shoppingSummary();
				break;
			case 4:
				break;
			default:
				printf("\nPlease enter a valid number!\n");
				continue;
		}
		printf("\n");
	}
	return 0;
}

int buyProduct(){
	int i,j,equal,arraySize,inputSize,quantity;
	Product product;
	arraySize=5;
	char userInput[50];
	printf("Name of product:");
	scanf(" %[^\n]%*c",userInput);
	printf("Quantity of product:");
	scanf("%d",&quantity);
	inputSize=getLen(userInput);//size of the input string

	while(1){
		if(inputSize>arraySize){ //if user input size greater than dynamic array length
			arraySize=arraySize*2; //size x 2
		}else{
			break;
		}
	}
	char *dynamicArray; //user input will be coppied into this array
	dynamicArray=(char*) malloc(arraySize*sizeof(char));

	for(i=0;i<inputSize;i++){ //copying process
		dynamicArray[i]=userInput[i];
	}
	dynamicArray[i]='\0';//adding null character at the end of the array

	for(i=0;i<PRODUCT_COUNT;i++){
		equal=0;//boolean result of equality of strings
		if(productArray[i].name[0]==dynamicArray[0]){ //if first characters are equal
			equal=1; //equality started here
			j=1;//index count after first character
			while(dynamicArray[j]!='\0'){
				if(dynamicArray[j]!=productArray[i].name[j]){//if next characters are different, equal=0 and break the loop
					equal=0;//strings are not equal
					break;
				}
				j++;
			}
			if(equal==1){//search string matches with a product
				if(productArray[i].quantity<quantity){//no enough product
					printf("There is not enough %s!\n",productArray[i].name);
					return 0;
				}else{//there is enough product

					/*this new product will be stored in selectedProducts array*/
					product.name=productArray[i].name;
					product.price=productArray[i].price;
					product.quantity=quantity;//selected quantity by user
					productArray[i].quantity-=quantity;//quantity decrease of product in storage

					int productIndex=productAlreadyExists(product);//gives the index of the product in selected array if it is already exists, else it gives returns -1
					if(productIndex>=0){//if selectedProducts contains the product, just increase the quantity
						selectedProducts[productIndex].quantity+=quantity;
					}else{
						selectedProducts[numberOfSelectedProducts]=product;
						numberOfSelectedProducts++;
					}
					printf("You bought %d %s!\n",product.quantity,product.name );
					return 1;
				}
			}
		}
	}
	/*no match*/
	printf("Product not found!\n");
	return 0;
}

int productAlreadyExists(Product product){
	int i;
	for(i=0;i<numberOfSelectedProducts;i++){
		if(selectedProducts[i].name==product.name){
			return i;//returns the index of the product in the selectedProducts array
		}
	}
	return -1;
}

void shoppingSummary(){
	char *name;
	double totalPrice,price;
	int i,quantity;
	printf("------ Shopping Summary ------\n");
	for(i=0;i<numberOfSelectedProducts;i++){
		name=selectedProducts[i].name;
		price=selectedProducts[i].price;
		quantity=selectedProducts[i].quantity;
		totalPrice+=quantity*price;
		printf("Product name: %s, Unit price: %.2f, Quantity: %d\n",name,price,quantity);
	}
	printf("------------------------------\n");
	printf("Total price: %.2f\n",totalPrice);

}


/*creates products and add them into productArray*/
void storeProducts(){
	productArray[0]=createProduct("Bread",3.5,10);
	productArray[1]=createProduct("Egg",2.82,30);
	productArray[2]=createProduct("Pasta",2.79,15);
	productArray[3]=createProduct("Fish",15.98,25);
	productArray[4]=createProduct("Milk",10.26,10);
	productArray[5]=createProduct("Water Small",1.55,35);
	productArray[6]=createProduct("Hand Sanitizer",20.55,20);
	productArray[7]=createProduct("Toothpaste",10.83,15);
	productArray[8]=createProduct("Pizza",15.3,10);
	productArray[9]=createProduct("Chocolate",5.21,40);
}

/*creates product with given attributes*/
Product createProduct(char *name, double price, int quantity){
	Product p;
	p.name=name;
	p.price=price;
	p.quantity=quantity;
	return p;
}

/*gives length of a string*/
int getLen(const char* str){
	int i=0;
	while(str[i] != '\0'){
		i++;
	}
	return i;
}

void listProducts(){
	int i;
	char *name;
	double price;
	int quantity;
	printf("----------- Product List -----------\n");
	for(i=0; i<PRODUCT_COUNT ;i++){
		name=productArray[i].name;
		price=productArray[i].price;
		quantity=productArray[i].quantity;

		printf("Product name: %s, Unit price: %.2f, Quantity: %d\n",name,price,quantity);
	}
}

void showMenu(){
	printf("---- MAIN MENU ----\n");
	printf("1- Buy a product\n");
	printf("2- List all products\n");
	printf("3- Shopping summary\n");
	printf("4- Exit\n");
	printf("-------------------\n");
}
