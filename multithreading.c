/*
Beyza Aydeniz
150200039
*/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define Num_of_Customers 3
#define Num_of_Products 5


pthread_mutex_t lock[Num_of_Products];
pthread_mutex_t writelock;

struct Node{ //node to keep ordered item's and purchased item's data
    int product_ID;
    int product_Quantity;
};

typedef struct Customer {
    int customer_ID;
    double customer_Balance;
    struct Node ordered_Items[Num_of_Products];
    struct Node purchased_Items[Num_of_Products];
    int ordered_item_type_quantity;
    int purchased_item_count; //counts number of purchased items, will be used in loops
    double initial_balance;

    pthread_attr_t attr;
}Customer;

struct Product {
    int product_ID;
    double product_Price;
    int product_Quantity;
};

struct Product Products[Num_of_Products];

void order_product(int product_ID, int product_Quantity){

    Products[product_ID-1].product_Quantity = (Products[product_ID-1].product_Quantity - product_Quantity);

}

void* order_products(void * ptr){

    Customer *args = (Customer*) ptr;
    int ordered_quantity; //will be "Customer->ordered_Items[i].product_Quantity", used to make the code more readable
    int available_quantity; //will be "Products[Customer->ordered_Items[i].product_ID-1].product_Quantity", used to make the code more readable
    double cost; //will be "(Customer->ordered_Items[i].product_Quantity)*(Products[Customer->ordered_Items[i].product_ID-1].product_Price)", used to make the code more readable
    int initial_product_quantity[Num_of_Products];


    for(int i = 0; i<args->ordered_item_type_quantity; i++){

        
        pthread_mutex_lock(&lock[args->ordered_Items[i].product_ID-1]); //locks the product by checking its product ID. Product x is accessed by &lock(x-1).

        ordered_quantity = args->ordered_Items[i].product_Quantity;
        available_quantity = Products[args->ordered_Items[i].product_ID-1].product_Quantity;
        cost = (args->ordered_Items[i].product_Quantity)*(Products[args->ordered_Items[i].product_ID-1].product_Price);

        if(ordered_quantity<= available_quantity && cost<=args->customer_Balance){ //if there are enough money and quantity to purchase

            for(int j = 0; j<Num_of_Products; j++){
                initial_product_quantity[j]=Products[j].product_Quantity; //first it keeps the initial number of items to print later
            }

            order_product(args->ordered_Items[i].product_ID, ordered_quantity);
            

            args->purchased_Items[args->purchased_item_count]=args->ordered_Items[i]; //if the purchase is successful, it adds it to the array of purchases

            args->purchased_item_count++;
            args->customer_Balance -= cost;
            

            pthread_mutex_lock(&writelock); //write_lock to not interrupt outputs
            printf("Customer%d SHOPPING\n", args->customer_ID);
            printf("Initial Products \n");
            printf("Product ID  Quantity    Price \n");
            for(int j = 0; j <Num_of_Products; j++){
                printf("    %d          %d        %f\n", Products[j].product_ID, initial_product_quantity[j], Products[j].product_Price);
            }

            printf("Bought %d of product %d for %f. \n", ordered_quantity, args->ordered_Items[i].product_ID, cost);
            printf("\n");
            printf("Updated Products \n");
            printf("Product ID  Quantity    Price \n");
            for(int j = 0; j <Num_of_Products; j++){
                printf("    %d          %d        %f\n", Products[j].product_ID, Products[j].product_Quantity, Products[j].product_Price);
            }
            pthread_mutex_unlock(&writelock);
        }
        
        else if(ordered_quantity>available_quantity){ //if there is not enough stock to purchase
            pthread_mutex_lock(&writelock); //write_lock to not interrupt outputs
            printf("Fail! Customer %d cannot buy %d of product %d. Only %d left in stock.\n", args->customer_ID, ordered_quantity, args->ordered_Items[i].product_ID, available_quantity);
            pthread_mutex_unlock(&writelock);
        }
        else if(cost>args->customer_Balance){ //if there is not enough money to purchase
            pthread_mutex_lock(&writelock); //write_lock to not interrupt outputs
            printf("Fail! Customer %d cannot buy %d of product %d. Insufficient funds.\n", args->customer_ID, ordered_quantity, args->ordered_Items[i].product_ID);
            pthread_mutex_unlock(&writelock); 
        }
        pthread_mutex_unlock(&lock[args->ordered_Items[i].product_ID-1]); //unlocks the product so that other costumers can purchase too
        
    }
    
}


int main(){

    clock_t start_time, end_time;
    double cpu_time_used;

    // Record the start time
    start_time = clock();

    srand(time(NULL));
    Customer* Customers=malloc(sizeof(Customer)*Num_of_Customers); //dynamic memory is allocated so that customers' data is not lost after pthread_join

    for (int i = 0; i < Num_of_Products; i++) {
        Products[i].product_ID = i+1;
        Products[i].product_Price = ((rand() % 30) + 1); //give 30 to check outputs easily
        Products[i].product_Quantity = ((rand() % Num_of_Products) + 1);
    }
    for (int i = 0; i < Num_of_Customers; i++) {
        Customers[i].customer_ID = i+1;
        Customers[i].customer_Balance = (rand() % 201); //balance is between 0 and 200
        Customers[i].ordered_item_type_quantity = ((rand() % 5) + 1); //ordered item type quantity can be max 5 according to the given limit
        for (int j = 0; j < Customers[i].ordered_item_type_quantity; j++){ 
            Customers[i].ordered_Items[j].product_ID= ((rand() % 5) + 1);
            Customers[i].ordered_Items[j].product_Quantity= ((rand() % 5) + 1); //a customer can purchase max 5 type of products according to the given limit
        }
        Customers[i].purchased_item_count=0;
        Customers[i].initial_balance=Customers[i].customer_Balance;
        pthread_attr_init(&Customers[i].attr);
    }


    pthread_t threads_customer [Num_of_Customers];

    for(int i = 0; i<Num_of_Customers; i++){ //threads
        pthread_create(&threads_customer[i], &Customers[i].attr, &order_products, &Customers[i]);
    }

    for(int i = 0; i<Num_of_Customers; i++){ //waits for the termination of another thread
        pthread_join(threads_customer[i], NULL);
    }


    for(int i = 0; i<Num_of_Customers; i++){ //to print customer's information
        printf("\n");
        printf("Customer%d Information\n", Customers[i].customer_ID);
        printf("----------------------\n");
        printf("Initial Balance: $ %f \n", Customers[i].initial_balance);
        printf("Updated Balance: $ %f \n", Customers[i].customer_Balance);
        printf("Ordered Products \n");
        printf("ID      Quantity \n");
        for(int j = 0; j<Customers[i].ordered_item_type_quantity; j++){ //so that the loop continues for the number of ordered items
            printf("%d      %d \n", Customers[i].ordered_Items[j].product_ID, Customers[i].ordered_Items[j].product_Quantity);
        }
        printf("Purchased Products \n");
        printf("ID      Quantity \n");
        if(Customers[i].purchased_item_count==0){
                printf("-       -\n");
        }
        else{
            for(int j = 0; j<Customers[i].purchased_item_count; j++){
                printf("%d      %d\n", Customers[i].purchased_Items[j].product_ID, Customers[i].purchased_Items[j].product_Quantity);
            }
        }
    }

    free(Customers); //to avoid memory leak

    for(int i = 0; i < Num_of_Customers; i++){
        pthread_attr_destroy(&Customers[i].attr);
    }

    // Record the end time
    end_time = clock();

    // Calculate the CPU time used
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("CPU time used for multithreading: %f seconds\n", cpu_time_used);
}