# Online Shopping Routine Using the Principles of Multiprocessing and Multithreading in Operating Systems

The goal of this assignment is to develop an online shopping routine using the principles of multiprocessing and multithreading in operating systems. Since multiple customers may order simultaneously, this can lead to a race condition issue. Thus, a proper synchronization mechanism must be implemented to manage the flow of customers purchasing the same product at the same time, and prevent any race conditions. This assignment requires implementing a fully functional online shopping routine using mutexes to ensure proper synchronization.

Except for the main part, the other parts of my code are almost the same. The first function takes a void pointer as input, which is then cast into a pointer of type Customer using (Customer*) ptr. This means that the input argument can be of any type, and the function will try to interpret it as a pointer to a Customer struct. On the other hand, the second function takes a pointer to a Custom r struct as input, which means that the input argument must be a pointer to a Customer struct.
In multithreading I allocated dynamic memory so that customers’ data is not lost after using pthread join. I used mutex to perform a properly working online shopping routine. I created a for loop and lock the product by checking its product ID. Product x is locked by lock(x-1). Also I created write lock so that the texts do not interfere with each other.

To provide a synchronous online shopping experience, I first created a shared memory segment and attached it. Once that was done, I implemented a child process using fork to create three customers. After that, I waited for the child processes to complete. To prevent any issues with multiple customers trying to purchase the same product simultaneously, I used a mutex. I created a for loop to iterate over the products and locked each product by checking its product ID. In this way, I ensured that product x was locked by lock(x-1).

These are the sample outputs:

<p align="center">
<img height="600" alt="çıktı1" src="https://github.com/beyzaaydeniz/Multiprocessing-and-Multithreading-in-Operating-Systems/assets/131685199/5896da8c-aeb2-496f-8c76-2b4dda234626">
<img height="600" alt="çıktı2" src="https://github.com/beyzaaydeniz/Multiprocessing-and-Multithreading-in-Operating-Systems/assets/131685199/0ab0b4f1-3a52-4d04-b525-b8b9fc165b13">
</p>
