#include <iostream>
#include <fstream>                                        //for file operations.
#include <string>                                         //to use variable string, .find() etc.  
#include <chrono>                                         //to keep track of elapsed time.
 
struct Node {                                             //Declaring node structure containing 3 variables.
 std::string key;                                         //String variable 'key' to hold file name of each file.  
 int visitCount;                                          //Integer variable 'visitCount' to hold visit count of each file.  
 Node* next;                                              //Pointer 'next' to be able to point to another node(next node in linked list).  
    
    Node(const std::string& fname, int vcount)            //Constructor for the Node structure.                                       ||
    {                                                     //                                                                          || 
     key = fname;                                         //Assign 'fname' parameter value to 'key' member.                           ||Initializing members.
     visitCount = vcount;                                 //Assign 'vcount' parameter value to 'visitCount' member.                   || 
     next = nullptr;                                      //Assign 'next' member to  nullptr so node initially points to nothing.     ||
    }         
};

class HashTable {                                         //Class declaration for hash table.

 private: 
     static const int hashTableSize = 1100;               //Declaring size of the hash table.*By trial and error I calculated 1088 is the size required for this log file.*      
 public:
 
    HashTable()                                           //Default constructor for hashTable class.(because it doesnt take arguments its default const)
    {
        for (int i = 0; i < hashTableSize; i++)           //Iterates through each index for the size of hash table.
        {
         pointerArray[i] = nullptr;                       //Sets all index on the array to point nullptr.Initially every array element is empty but present in memory.We have our buckets.
        }
    }

    Node* pointerArray[hashTableSize];                    //Declaring an array (hashtable) made up of node pointers with size of hashTableSize.This array can be called the hashTable but this naming is easier for me to visualise the hash table.
                                                 
    int htsize() const                                    //Declaring function named 'htsize' to be able to access and use hash table size.          
    {                                   
     return hashTableSize;                                //Returns hash table size as an integer.Constant to state hash table value wont be modified.  
    }

    int hashValue(const std::string& key)                 //Declaring hashValue function.Constant referencing 'key'(type string) to only read. Hashing logic.
    {
     int keySum = 0;                                      //Declaring an integer value to store the summation of ascii values of each filenames characters.Equals to zero initially. 
     for (int i = 0; i < key.length(); ++i)               //For loop to iterate through the character lenght of the currently processed key which is filename.
        {
         int asciiValue = key[i];                         //Declaring an integer value to store each characters ascii value one by one troughout the for loop.
         keySum = keySum + asciiValue ;                   //Storing total ascii value of each character from the 'key' string into keySum by addition trough the loop.                  
        }
     return (keySum % hashTableSize);                     //Takning mod of keySum by size of the hast table to get the hash value to be used for the current key(filename).                     
    }

    void insertVisit(const std::string& key)             //Function declaration for inserting filenames and updating visits if the filename exists.
    {
     int i = hashValue(key);                             //Declaring integer value, to store ascii value of 'key'. Key is the filename inserted on the iteration.
     Node* current = pointerArray[i];                    //Pointer 'current' points to the head of the linked list on array index.

        while (current != nullptr)                       //While loop iterates untill current points to nullptr(end of linked list on array index).
        {
            if (current->key == key)                     //If currently pointed node holds same filename as the filename that is iserted(line 200 'fileName').  
            {                                             
             current->visitCount++;                      //Increment visit count member of pointed node by one.
             return;                                     //When filename is matched with the node that holds the same filename exit function. 
            }
            else                                         //If currently pointed node doesnt hold same filename else work on iteration.  
            {                                       
             current = current->next;                    //Iterate trough nodes untill end of the linked list.
            }                                            //As insertVisit function works we end up with linked lists and the current points to the of the list head first.
        }                                                //So nodes are already set up to point at each other with'next'.
         
        if (current == nullptr)                          //If current is pointing at the end of a linked list on any array index or array index has no nodes present,this part works.
        {
         Node* newNode = new Node(key, 1);               //Pointer 'newNode'points to a new node. Allocates memory for new node and creates a node with filename and one visit.
         newNode->next = pointerArray[i];                //On the array index, points newNodes 'next' pointer, to point to head of linked list present there(array index).
         pointerArray[i] = newNode;                      //Current iterations index of pointer array points to the newNode pointer.Meaning it is the pointer to the head node.
        }
    }      

    ~HashTable()                                         //Destructor for class HashTable.Because we used 'new' to generate new node we have dynamically allocated memory that needs deletion.
    {
        for (int i = 0; i < hashTableSize; i++)          //Iterate over the array.
        {
         Node* current = pointerArray[i];                //Declare pointer 'current' and point to indexed node on pointer array.               
            while (current != nullptr)                   //While there is still nodes left in the linked list while loop iterates.
            {                                            //(Every index at hash table array may have different amount of nodes on their linked lists so we iterate untill they are all deleted)
             Node* temp = current;                       //Declare temporary pointer to use for the iteration.
             current = current->next;                    //current->next of the initial node points to the next node on the linked list.Whith this line our new current is the next node in the linked list.
             delete temp;                                //Deleting node and deallocating memory.(dynamically allocated memory)
            }
         pointerArray[i] = nullptr;                      //Resetting pointer of the hash table index.Prior head of the linked list if the linked list was not deleted whithin for while loop.
       }
    }                                          
};                                   
 
class MaxHeap {    
private:

     static const int heapSize = 12000;                    //Maximum size of heap array. Trial and error. 11847 is the number required for this file.(if not considering success code, in our this number is even smaller as we consider success code.)                    
     Node* heap[heapSize];                                 //Declaring an array made up of node pointers with size equal to the heap size.
    int size;                                              //This is supposed to keep track of number of nodes in heap array.Used in insert and pop member functions.

    void swap(Node*& c, Node*& p)                          //Swap function declaration to swap pointers of first and second pointer refences fed to the function.(our case child and parent node)
    {
     Node* temp = c;                                       //Pointer temp store address pointed by f reference.
     c = p;                                                //Address pointed by second element gets assigned to first.
     p = temp;                                             //Adress stored in temp(initial first) gets assigned to second element.
    }

    void heapifyUp(int index)                                   //heapifyUp function takes in the index of currently inserted node and checks if it should be upper in the tree by visitCount by comparing it with its parent node.
    {   
        if (index == 0)                                         //if current parent is the root of the tree;
        {
         return;                                                //void funnction returns(terminates function).
        }

        int parent = (index - 1);                               //find the index value of the parent of current node.In a linear (array-based) heap parent is (child index - 1)
        if (heap[parent]->visitCount < heap[index]->visitCount) //compare visitCount of node at the index with its parent node.Works if visitCount of node at current index is greater than its parent.
        {
         swap(heap[parent], heap[index]);                       //Change index positions of parent and current node.
         heapifyUp(parent);                                     //Recursive function.This function calls on itself to check if the node is greater than its new parent.
        }                                                       //Doesnt stop moving the node higher in the tree untill it faces a greater parent.This is a linear tree no left/right child.
    }
    
public:

    MaxHeap()                                                   //Default constructor for MaxHeap class.(because it doesnt take arguments its default const)
    {
        for (int i = 0; i < heapSize; i++)                      //Iterates through each index for the size of hash table. 
        {
         heap[i] = nullptr;                                     //Sets all index on the array to point nullptr.Now every array element is initially empty but present in memory.We have our buckets.
        }
    }                                                 

    void heapInsert(Node* node)                                 //Function to insert into the pointer array(linear tree for our case).
    {
        if (size == heapSize)                                   //Check for available space.(Not necesarry in this case as we have a single logfile with a known size but good to have.)
        {
         std::cerr << "Heap is full, insertion failed." << std::endl;   //Error output. The last possible index on our array is (heapSize-1) so if the size is equal to heapsize it is not possible to insert.
         return;                                                //Terminate function.
        }
     
     heap[size] = node;                                         //If there is available space,current node is assigned the current size as index for the array.
     heapifyUp(size++);                                         //We want to check if the newly inserted node has greater visitCount than its parent so we call the heapifyUp function.
    }                                                           //Because we inserted a node size is incremented by one after the iteration inside the function is done. 

    Node* heapRetrieve(int x)                                   //'heapRetrieve' function declared.Function can access any point on the tree.We know that our max heap will store the highest visit count on index 0.
    {                                                           //Taking integer x, as the index to be used to reach that point on the array.(In our case we can reach top 10 by feeding the function x=0 to x=9).
        if (size == 0)                                          //Check if the tree is empty.Size represents amount of nodes in the array(or tree).
        {                                                       //As we only have one logfile and we already know that the array wont be empty there isnt much use of checking if the array is empty.
         std::cerr << "Heap is empty, nothing to retrieve" << std::endl;        //Error message. 
         return nullptr;                                        //Return nullptr because function is defined as a pointer to a node.
        }

     Node* retrievedNode = heap[x];                             //Pointer 'retrievedNode' points to the node on the array index.Index is the provided integer value for the funnction.
     return retrievedNode;                                      //Function returns the node that is requested.
    }

    //Inside our 'MaxHeap' class we dont have dynamically allocated memory.We didnt use 'new' to create new nodes like we did in the Hash table class.
    //For that reason we dont need to define a destructor. Default destructor generated by the compiler is enough.    
};

class Timer {         //Class containing functions to calculate elapsed time.Theres actually not much going on inside this class but chrono library and its syntax took some time to learn. 
private:
 std::chrono::steady_clock::time_point timerBegin;           //In 'chrono' namespace using clock type 'steady_clock', 'timerBegin' is a point in time of this clock type.
 std::chrono::steady_clock::time_point timerEnd;             //In 'chrono' namespace using clock type 'steady_clock', 'timerEnd' is a point in time of this clock type.
public:

    void markBegin() 
    {
     timerBegin = std::chrono::steady_clock::now();          //Marking current time that the function is called to timerBegin timepoint.
    }
    void markEnd() 
    {
     timerEnd = std::chrono::steady_clock::now();            //Marking current time that the function is called to timerEnd timepoint.
    }
    double elapsedTime()                                                       
    {
     std::chrono::duration<double, std::milli> elapsedTime = timerEnd - timerBegin;   //In chrono namespace 'duration' is a template class.'std::milli'Indicates duration is measured in milliseconds.
     return elapsedTime.count();                                               //'duration' use double to store value but its still a 'std::chrono::duration' object so we need '.count'function to retrieve the double value.
    }
};


int main() { 
 Timer timer;                //Creating object 'timer' of class 'Timer'. 
 timer.markBegin();          //Marking current time as beginning time.
            
 HashTable fileVisits;       //Creating object 'fileVisits' of class 'HashTable'.                                                 
 std::ifstream logFile("access_log");                                              //Oppening access_log and declaring 'logFile' object for input file stream.
     
    if (logFile.is_open())                                                         //Checking if acces_log is succesfully opened.
    {
        std::string line;                                                          //Declaring a string variable to hold string of characters from the log file to be processed in the while loop.
        while (std::getline(logFile, line))                                        //Reading from the ifstream object 'logFile' with getline funtion line by line and storing into the string variable 'line'.
        {                                                                          //If getline function reaches the end of the log it can not perform, using this as a boolean for the while loop.
         int GETPos = line.find("GET");                                            //Declaring integer value for the position of string'GET', within currently processed string which is our line.
         int HTTPPos = line.find("HTTP");                                          //Declaring integer value for the position of string'HTTP', within currently processed string which is our line. 
         int successCode = line.find("\" 200");                                    //HTTP status code for successful requests is 200 so we search for 200 to understand if the request was successful.
            if (GETPos != std::string::npos && HTTPPos != std::string::npos && successCode != std::string::npos)  //Checking if GETPos,HTTPPos,successCode values are valid,if 'GET','HTTP' and '" 200' are both found in current line, line is processed.
            {                                                                      //Declaring a string variable to store filenames and using substr function to extract the string(filename in that string).
             std::string fileName = line.substr(GETPos+4, HTTPPos-(GETPos+5));     //Extraction starts from get position+4 to not include get, extracts the lenght of the filename which is equal to position of the end minus start.
             fileVisits.insertVisit(fileName);                                     //Using insertVisit function we declared outside of the main function, to insert the filename we substracted into the hash table,if filename is present function increments visit count by one.
            }                                                                   
        }                                                                        
     logFile.close();                                                              //When while loop ends close the file.
    }

 MaxHeap maxHeap;                                                       //Creating object 'maxHeap' of class 'MaxHeap'.
    for (int i = 0; i < fileVisits.htsize(); i++)                       //For loop iterates through the hash table for the whole size of it.  
    {
     Node* current = fileVisits.pointerArray[i];                        //Node named current is declared using the node struct, points to the hash table index of the current iteration.        
        while (current != nullptr)                                      //While loop checks that if current is still pointing to a node.   
        {
         maxHeap.heapInsert(current);                                   //Member function '.heapInsert' is called to insert node pointer 'current' into the maxHeap.
         current = current->next;                                       //Points current to the next node pointer in the hash table (traversing linked list on the hash table index).
        }
    }

 std::cout << "  |----|Most|Visited|Files|----|" << std::endl << std::endl;       //Output 
 std::cout << "  |File  Name|   |Total  Visits|"<< std::endl<< std::endl;         //Output                                          
    for (int i = 0; i < 10; i++)                                                  //For loop iterates 10 times so we access only 10 elements from the max heap.
    {
     Node* fileInfo = maxHeap.heapRetrieve(i);                  //For the first iteration 'fileInfo' pointer is initialized the node at index 0, which is the root. 'maxHeap.heapRetrieve' function returns the node.Next iterations access the nodes according to value for 'i' in for loop.
     std::string fileNameOut = fileInfo->key;                   //fileNameOut string is given the filename of the current node processed.
     int visitCountOut = fileInfo->visitCount;                  //Integer visitCountOut is given the visitcount of the current node processed.
     std::string numOfDigits = std::to_string(visitCountOut);   //To find the number of digits of visitCountOut, turn the integer value to string so we can use .lenght function and use that to have a better looking output.                                   
     std::cout << "  |" << fileNameOut << std::string(10 - fileNameOut.length(), ' ') <<"|   |"<< visitCountOut <<std::string(6 - numOfDigits.length(), ' ') <<" visits|" << std::endl;  //Output                                          
    }

 timer.markEnd();                                                                                  //Marking current time as end time.                           
 std::cout << std::endl << "  |Elapsed time  -  " << timer.elapsedTime() << "  ms|" << std::endl;  //'timer.elapsedTime()' is the important part and rest is aesthetics.
 std::cout << "  |own HashTable implementation|";   
 return 0;
}