#include <iostream>
#include <fstream>                                        //for file operations.
#include <string>                                         //to use variable string, .find() etc.  
#include <chrono>                                         //to keep track of elapsed time.
#include <unordered_map>                                  //to be able to use unordered_map.

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
            
 std::unordered_map<std::string, int> fileVisits;                                  //Initializing the unordered_map with a string and a respective integer value to that file name.String will hold file names.Integer will hold visit counts.                      
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
             fileVisits[fileName]++;                                               //Everytime a filename is accessed on the map increment its value by one.Initial value is 0.
            }                                                                      //If its the first time of a file it will get added to the map and be given visit count of 1.      
        }                                                                        
     logFile.close();                                                              //When while loop ends close the file.
    }


 MaxHeap maxHeap;                                                            //Declaring maxHeap of class MaxHeap.
       
    for (std::pair<const std::string, int>& pair : fileVisits)               //Range-based for loop to iterate over unordered_map container.
    {
     std::string fileName = pair.first;                                      //String 'fileName is' initialized with the first value of the pair from the unordered_map.Which is the filename string.          
     int visitCount = pair.second;                                           //Integer 'visitCount' is initialized with the second value of the pair from the unordered_map.Which is the visit count integer.                  

     Node* newNode = new Node(fileName, visitCount);                         //Allocate memory for new Node and pass the values.Assign its memory adress to 'newNode' pointer.           
     maxHeap.heapInsert(newNode);                                            //Call on the '.insert' member function of MaxHeap class to instert the Node into heap.           
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
 std::cout << "  |       unordered_map        |";   
 return 0;
}