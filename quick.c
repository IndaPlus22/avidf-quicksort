//Acknowledgments to ChatGPT and mr Hoare
//And how the hell can this be done faster????????????

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//Functions 
//I have used pointers in this task, so indeed everything we learn comes together!

static void insert_sort(int *left, int *right);
static void sort(int *arr, int len);
static void qusort(int* left, int* right);
static int* partition(int *left, int *right);
static int parse();

//Operators

#define less(a, b) ((a) < (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define swap(a, b) do { int tmp = a; a = b; b = tmp; } while(0)
#define rotate(a, b, c) do { int tmp = a; a = b; b = c; c = tmp; } while(0)


int main() {
  int size = parse();
  int array[size];
  int *ptr = array;
  int c_val = size, c_char;
  do {
    c_char = parse();
    *ptr++ = c_char;    
  } while(--c_val > 0);
  
  sort(array, size);

  for(int i = 0; i < size; i++)
    printf("%d ", array[i]);
  return 0;
}   


//Insertion sort using pointers and references instead of arrays 



static 
void insert_sort(int *left, int *right) {
    
  //Gets smallest value and swaps to first index 
  for (int *index_pointer = left + 1; index_pointer <= right; index_pointer++)
    
        if (less(*index_pointer, *left))
        swap(*index_pointer, *left);


  //Move the value
  for (int *index_pointer = left + 2; index_pointer <= right; index_pointer++) {
        int h = *index_pointer;
        int *previous = index_pointer - 1;
        
        while (less(h, *previous)) {
        *(previous + 1) = *previous;
        previous -= 1;
        }

        //Place the element when there are no smaller elements
        *(previous + 1) = h;
  }
}



/*Using Hoare's partition scheme (Thanks to the great psuedo codes of wikipedia!), 
there is a good gif on there to get some nice intuition for whats going on */

//Gets the left and right most index 
//Gets the middle value 

static 
int* partition(int* left, int* right) {

    int *first_left = left;
    int pivot_left = *left;
    left += 1;

    int pivot_right = *right;

    int* middle = first_left + ((right - first_left) >> 1);    
    int pivot = *middle;


    //Get the median of the three 
    if(less(pivot, pivot_left)) { 

        if(less(pivot_right, pivot_left)) {   

            if(less(pivot_right, pivot)) 
            swap(pivot_left, pivot_right);    

            else rotate(pivot_left, pivot, pivot_right);          
        } 

        else swap(pivot_left, pivot); 
    }

    else { 
        if(less(pivot_right, pivot))  

            if(less(pivot_right, pivot_left)) 
            rotate(pivot_right, pivot, pivot_left);  

            else swap(pivot, pivot_right);                 
    }  



    // Update and set new pivots
    *right = pivot_right;
    *middle = *left;
    *first_left = pivot_left;
    *left = pivot;


    //Move right and left pivot towards the middle
    //Breaks if left is larger than right, otherwise swaps
    
    while (1) {
        do left += 1; while (less(*left, pivot));
        do right -= 1; while (less(pivot, *right));

        /* When left is larger than right, break */
        if (left >= right) break;

        /* If not swap value and repeat babyyyyyy */
        swap(*left, *right);
    }

    *(first_left + 1) = *right;
    *right = pivot;
    return right;
}


//Get characters from the stdin 
static 
int parse() {
    register int c, n, m = 0;
    n = getchar_unlocked(); //This here is like nitro put it in and run your code hottt! and unsafe (not thread safe), 
                            //I have been holding counsel with chatGPT and it has taught me the dark arts of C
    
    if(n == EOF) return n;


    //If character is - make m a true 
    //If not subtract 0 from value
    if(n == '-') { m = 1; n = 0; }
    else n -= '0';

    //keep reading until value is less than 0
    //Multiply by 10 and add 
    while((c = getchar_unlocked()) >= '0')
        n = ((n << 3) + (n << 1)) + c - '0';

    return (m) ? -n : n;
}



//Here is where the magic happens 
// Make a stack for your sorting and use a stack pointer 
//If the array is below 32 elements it dose an insertion sort instead

static 
void sort(int *data, int length) {


    int *stack[32];
    register int s_pointer = 0;

    //set values for right and left pointers 
    int *left = data;
    int *right = data + length - 1;

    while (1) {

        //Breaks if the stack is empty
        //If not keeps going

        //Once more using mr Hoars partitining 
        if (32 > right - left) {
            insert_sort(left, right);
            
            if (s_pointer == 0) break;

            s_pointer -= 2;
            left = stack[s_pointer];
            right = stack[s_pointer + 1];
            
            } 

            else {
            
                int *mid = partition(left, right);
                
                if (mid < left + ((right - left) >> 1)) {
                    stack[s_pointer] = mid + 1;
                    stack[s_pointer + 1] = right;
                    right = mid - 1;
                } 
                
                else {
                    stack[s_pointer] = left;
                    stack[s_pointer + 1] = mid - 1;
                    left = mid + 1;
                }
                
                s_pointer += 2;
            }
  }
}

