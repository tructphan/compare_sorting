#include <iostream>
#include <fstream>
#include <ctime> //for calculating time
#include <chrono> //for high resolution clock
#include <iomanip> //for aligning data
#include <cctype> //for changing lowercase to uppercase

using std::ifstream;
using std::ofstream;
using namespace std::chrono;

//"reading text file" functions
void readFile(std::string[], int, std::string);
int toupper(int c);
std::string changeChar(std::string a);
//sorting functions
void bubbleSort(std::string[], int);
void selectionSort(std::string[], int);
void insertionSort(std::string[], int);
void mergeSort(std::string[], int, int);
void merge(std::string[], int, int, int);
void quickSort (std::string[], int, int);
int partition (std::string[], int, int);
//"printing options for users" functions
int chooseText();
int chooseSort(std::string[], int);
void firstOption(std::string[], int);
void secondOption(std::string[], int);
int afterSort(std::string[], int);
//"calculating and printing execution time" functions
void calcTime1(std::string[], int, void (*)(std::string[], int),
               std::string[], int, void (*)(std::string[], int));
void calcTime2(std::string[], int, void (*)(std::string[], int),
               std::string[], int, int, void (*)(std::string[], int, int));
void calcTime3(std::string[], int, int, void (*)(std::string[], int, int),
               std::string[], int, int, void (*)(std::string[], int, int));
void calcTime4(std::string[], int, void (*)(std::string[], int));
void calcTime5(std::string[], int, int, void (*)(std::string[], int, int));
double calcAverage(double[]);

int main()
{
    ifstream in_file; //read data from text file
    std::string line;
    int count1=0, count2=0, count3=0; //store each text file's size
    int choice1=0, choice2=0; //store users' choices

    in_file.open("shakespeare1.txt"); //"Romeo and Juliet"
    //test for failure of opening the file
    if(in_file.fail()) {
        std::cout << "Error opening file: Romeo and Juliet\n";
        return -1;
    }
    //calculate the size of text file (aka the number of words) to be the size
    //of a dynamically allocated array
    else {while (in_file >> line) {count1++;}}
    in_file.close(); //close current file to open a new one
    
    in_file.open("shakespeare2.txt"); //"All's Well that Ends Well"
    //test for failure of opening the file
    if(in_file.fail()) {
        std::cout << "Error opening file: All's Well that Ends Well\n";
        return -1;
    }
    //calculate the size of text file (aka the number of words) to be the size
    //of a dynamically allocated array
    else {while (in_file >> line) {count2++;}}
    in_file.close();
    
    in_file.open("shakespeare3.txt"); //"Much Ado About Nothing"
    //test for failure of opening the file
    if(in_file.fail()) {
        std::cout << "Error opening file: Much Ado About Nothing\n";
        return -1;
    }
    //calculate the size of text file (aka the number of words) to be the size
    //of a dynamically allocated array
    else {while (in_file >> line) {count3++;}}
    in_file.close();
    
    //dynamically allocated arrays to hold data from text files
    std::string *a = new std::string[count1];
    std::string *b = new std::string[count2];
    std::string *c = new std::string[count3];
    
    //read data from text file into the arrays
    readFile(a, count1, "shakespeare1.txt");
    readFile(b, count2, "shakespeare2.txt");
    readFile(c, count3, "shakespeare3.txt");
    
    //print list of choices for users
    bool check=true; //condition to keep looping menu
    int checkSize=0; //check the limit of sorted data
    while(check) {
        choice1=chooseText(); //ask users which text file to be sorted
        //if the choice is "Romeo and Juliet"
        if (choice1==1) {
            checkSize = chooseSort(a, count1); //print list of sorting algorithms and
                                    //let users decide how to sort data
            //users' choices after data has been sorted
            if (checkSize==1) {choice2=afterSort(a, count1);} //if whole file was sorted
            else {choice2=afterSort(a, 20000);} //if only 20000 elements were sorted
        }
        //if the choice is "All's Well that Ends Well"
        else if (choice1==2) {
            checkSize = chooseSort(b, count2); //print list of sorting algorithms and
                                    //let users decide how to sort data
            //users' choices after data has been sorted
            if (checkSize==1) {choice2=afterSort(b, count2);} //if whole file was sorted
            else {choice2=afterSort(b, 20000);} //if only 20000 elements were sorted
        }
        //if the choice is "Much Ado About Nothing"
        else {
            checkSize = chooseSort(c, count3); //print list of sorting algorithms and
                                    //let users decide how to sort data
            //users' choices after data has been sorted
            if (checkSize==1) {choice2=afterSort(c, count3);} //if whole file was sorted
            else {choice2=afterSort(c, 20000);} //if only 20000 elements were sorted
        }
        //if users wish to quit
        if (choice2==-1) {check=false;}
    } //end while
    return 0;
    delete a, b, c; //delete dynamically allocated arrays when program is finished
}

/* This function reads words from a text file into a dynamically
 * allocated array. This function can detect special symbols that
 * are not from abbreviations and omit them from the string
 */
void readFile(std::string a[], int s, std::string fileName)
{
    ifstream in_file; //for reading text file
    in_file.open(fileName.c_str()); //open the text file that was parsed into the function
    std::string word; //store each word in the text file
    int i=0; //store position of word in the dynamically allocated array
    int ascii=0; //store ascii code of a letter
    bool check; //check for meaningful words
    while (in_file >> word) {
        for (int j=0; j<word.length(); j++) { //loop through every character of a word
            check=false; //assume words containing symbols
            ascii = (int)word[j]; //get the ascii code of the character
            //ignore characters that are not from the English alphabet
            while (!((ascii>=65 && ascii<=90) || (ascii>=97 && ascii<=122))
            && j<word.length()) {
                j++; //move on to next character
                ascii = (int)word[j]; //get the ascii code of the next character
            }
            //if a character from the alphabet is found, set its position to "from"
            int from=j;
            //keep reading the word, checking for alphabetical characters, single apostrophy
            //from abbreviation, and single hyphen in hyphenated words - all of those make
            //up meaningful words
            while ((((ascii>=65 && ascii<=90) || (ascii>=97 && ascii<=122)) && j<word.length())
            || ascii==39 || ((int)word[j]==45 && (int)word[j+1]!=45)) {
                check=true; //meaningful words are detected
                j++; //move on to next character
                ascii = (int)word[j]; //get the ascii code of the next character
            }
            int to=j; //finish the word, set position to "to"
            if (check) {
                if (i<s) //make sure position of word is still in the limit of array's size
                    a[i++] = word.substr(from, to-from); //input the word into the array
            }
        }
    }
}

/* This function changes a string to all uppercase letters, which helps sorting
 * functions sort data in a case-insensitive way, this function uses toupper()
 * from library <cctype> to convert lowercase to uppercase letters
 */
std::string changeChar(std::string a)
{
    for (int i=0; i<a.size(); i++) {
        a[i]=std::toupper(a[i]);
    }
    return a;
}

/* This function demonstrates the bubble sort algorithm
 */
void bubbleSort(std::string a[], int s)
{
    bool notsorted=true;
    while(notsorted) {
        notsorted=false;
        for (int i=1; i<s; i++) {
            if (changeChar(a[i]) < changeChar(a[i-1])) {
                std::string temp=a[i];
                a[i]=a[i-1];
                a[i-1]=temp;
                notsorted=true;
            }
        } s--;
    }
}

/* This function demonstrates the selection sort algorithm
 */
void selectionSort(std::string a[], int s)
{
    int minIndex;
    for (int i=0; i<s-1; i++) {
        minIndex=i;
        for (int j=i+1; j<s; j++) {
            if(changeChar(a[minIndex]) > changeChar(a[j]))
                minIndex=j;
            if(minIndex!=i) {
                std::string temp=a[i];
                a[i]=a[minIndex];
                a[minIndex]=temp;
            }
        }
    }
}

/* This function demonstrates the insertion sort algorithm
 */
void insertionSort(std::string a[], int s)
{
    for (int i=1; i<s; i++) {
        std::string next=a[i];
        int j=i;
        while (j>0 && changeChar(a[j-1]) > changeChar(next)) {
            a[j]=a[j-1];
            j--;
        }
        a[j]=next;
    }
}

/* This function demonstrates the merge sort algorithm
 */
void mergeSort(std::string a[], int from, int to)
{
    if (from==to) {return;} //base case
    int mid=(from+to)/2;
    mergeSort(a, from, mid);
    mergeSort(a, mid+1, to);
    merge(a, from, mid, to);
}
/* This function is overloaded in mergeSort function,
 * which merges together sorted data into a backup array
 * and parses back into original array
 */
void merge(std::string a[], int from, int mid, int to)
{
    int n=to-from+1; //size of the array
    std::string *b= new std::string[n];
    int i1=from;
    int i2=mid+1;
    int j=0;
    
    while (i1<=mid && i2<=to) {
        if (changeChar(a[i1]) < changeChar(a[i2])) {b[j]=a[i1]; i1++;}
        else {b[j]=a[i2]; i2++;}
        j++;
    }
    while (i1<=mid) {b[j]=a[i1]; i1++; j++;}
    while (i2<=to) {b[j]=a[i2]; i2++; j++;}
    for(j=0; j<n; j++) {a[from+j]=b[j];}
    delete[] b;
}

/* This function demonstrates the quicksort algorithm
 */
void quickSort (std::string a[], int from, int to)
{
    if (from>=to) {return;}
    int p=partition(a, from, to);
    quickSort(a, from, p);
    quickSort(a, p+1, to);
}
/* This function is overloaded in quickSort function,
 * which calculates the position where data is splitted
 */
int partition (std::string a[], int from, int to)
{
    std::string pivot = a[from];
    int i = from-1;
    int j = to+1;
    while (i<j) {
        i++;
        while (changeChar(a[i]) < changeChar(pivot)) {i++;}
        j--;
        while (changeChar(a[j]) > changeChar(pivot)) {j--;}
        if (i<j) {
            std::string temp=a[i];
            a[i]=a[j];
            a[j]=temp;
        }
    }
    return j;
}

/* This function prints the list of text files and asks user to pick one.
 * The function has the ability to test for invalid input.
 * The function returns user's choice to move on to choosing sorting algorithms.
 */
int chooseText()
{
    int c1=0; //store users' choice
    std::cout << "Enter which text file you want to use (1-3):\n" <<
                        "1. Romeo and Juliet\n" <<
                        "2. All's Well that Ends Well\n" <<
                        "3. Much Ado About Nothing\n";
    std::cin >> c1;
    //test for invalid input
    while (std::cin.fail() || c1<1 || c1>3) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Wrong input. Please enter again:\n";
        std::cin >> c1;
    }
    return c1;
}

/* This function prints all the sorting algorithms and list of choices for users to choose:
 * using one sort over whole file, comparing two sorts over whole file, or using one sort over
 * different elements of file.
 * The function has the ability to test for invalid input.
 */
int chooseSort(std::string a[], int count)
{
    int c2=0;
    std::cout << "Display 5 sorting algorithms:\n" <<
                        "a. Bubble sort\n" <<
                        "b. Selection sort\n" <<
                        "c. Insertion sort\n" <<
                        "d. Merge sort\n" <<
                        "e. Quicksort\n" <<
                        "Choose one option below:\n" <<
                        "1. Sort data using two sorting algorithms\n" <<
                        "2. Sort data using one sorting algorithm over 5000, 10000, 20000 elements\n";
    std::cin >> c2;
    //test for invalid input
    while (std::cin.fail() || c2<1 || c2>2) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Wrong input. Please enter again:\n";
        std::cin >> c2;
    }
    if (c2==1) {firstOption(a, count);} //compare two sorts over whole file
    if (c2==2) {secondOption(a, count);} //one sort over different data sizes
    return c2; //to check the number of sorted data, which is either whole file (c2=1)
                //or only 20000 elements (c2=2)
}

/* This funtion demonstrates user's first choice - comparing two sorts over whole file
 * The function has the ability to test for invalid input.
 * There are ten cases for this option, using probability: 5C2 = 10 (picking 2 distinct
 * sorts over 5 sorts)
 */
void firstOption(std::string a[], int count)
{
    int choice1=0, choice2=0; //hold user's choice of 2 sorts
    std::string p[count]; //temporary array to hold unsorted elements
    for (int i=0; i<count; i++) {p[i]=a[i];} //assign unsorted elements from a to p
    
    std::cout << "Please enter the 2 sorting algorithms you want to use\n" <<
                        "Display 5 sorting algorithms:\n" <<
                        "1. Bubble sort\n" <<
                        "2. Selection sort\n" <<
                        "3. Insertion sort\n" <<
                        "4. Merge sort\n" <<
                        "5. Quicksort\n";
    std::cin >> choice1 >> choice2;
    //test for invalid input
    while(std::cin.fail() || choice1<1 || choice2>5 || choice1<1 ||
    choice2>5 || choice1==choice2) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Wrong input. Please enter again:\n";
        std::cin >> choice1 >> choice2;
    }
    //bubble and selection
    if (choice1==1 && choice2==2 || choice1==2 && choice2==1) {
        std::cout << std::setw(35) << "Bubble sort's execution time" <<
        std::setw(50) << "Selection sort's execution time\n";
        calcTime1 (a, count, bubbleSort, a, count, selectionSort);
    }
    //bubble and insertion
    else if (choice1==1 && choice2==3 || choice1==3 && choice2==1) {
        std::cout << std::setw(35) << "Bubble sort's execution time" <<
        std::setw(50) << "Insertion sort's execution time\n";
        calcTime1 (a, count, bubbleSort, a, count, insertionSort);
    }
    //bubble and merge
    else if (choice1==1 && choice2==4 || choice1==4 && choice2==1) {
        std::cout << std::setw(35) << "Bubble sort's execution time" <<
        std::setw(50) << "Merge sort's execution time\n";
        calcTime2 (a, count, bubbleSort, a, 0, count, mergeSort);
    }
    //bubble and quick
    else if (choice1==1 && choice2==5 || choice1==5 && choice2==1) {
        std::cout << std::setw(35) << "Bubble sort's execution time" <<
        std::setw(50) << "Quicksort's execution time\n";
        calcTime2 (a, count, bubbleSort, a, 0, count, quickSort);
    }
    //selection and insertion
    else if (choice1==2 && choice2==3 || choice1==3 && choice2==2) {
        std::cout << std::setw(35) << "Selection sort's execution time" <<
        std::setw(50) << "Insertion sort's execution time\n";
        calcTime1 (a, count, selectionSort, a, count, insertionSort);
    }
    //selection and merge
    else if (choice1==2 && choice2==4 || choice1==4 && choice2==2) {
        std::cout << std::setw(35) << "Selection sort's execution time" <<
        std::setw(50) << "Merge sort's execution time\n";
        calcTime2 (a, count, selectionSort, a, 0, count, mergeSort);
    }
    //selection and quick
    else if (choice1==2 && choice2==5 || choice1==5 && choice2==2) {
        std::cout << std::setw(35) << "Selection sort's execution time" <<
        std::setw(50) << "Quicksort's execution time\n";
        calcTime2 (a, count, selectionSort, a, 0, count, quickSort);
    }
    //insertion and merge
    else if (choice1==3 && choice2==4 || choice1==4 && choice2==3) {
        std::cout << std::setw(35) << "Insertion sort's execution time" <<
        std::setw(50) << "Merge sort's execution time\n";
        calcTime2 (a, count, insertionSort, a, 0, count, mergeSort);
    }
    //insertion and quick
    else if (choice1==3 && choice2==5 || choice1==5 && choice2==3) {
        std::cout << std::setw(35) << "Insertion sort's execution time" <<
        std::setw(50) << "Quicksort's execution time\n";
        calcTime2 (a, count, insertionSort, a, 0, count, quickSort);
    }
    //merge and quick
    else if (choice1==4 && choice2==5 || choice1==5 && choice2==4) {
        std::cout << std::setw(35) << "Merge sort's execution time" <<
        std::setw(50) << "Quicksort's execution time\n";
        calcTime3 (a, 0, count, mergeSort, a, 0, count, quickSort);
    }
}

/* This funtion demonstrates user's second choice - comparing one sorts over differen
 * data sizes - 5000, 10000, 20000
 * The function has the ability to test for invalid input.
 * There are five cases for five sorting algorithms
 */
void secondOption(std::string a[], int count)
{
    int choice=0; //hold user's choice of sorting algorithm
    std::string p[count]; //temporary array to hold unsorted elements
    for (int i=0; i<count; i++) {p[i]=a[i];} //assign unsorted elements from a to p

    std::cout << "Please enter the sorting algorithm you want to use\n" <<
                        "Display 5 sorting algorithms:\n" <<
                        "1. Bubble sort\n" <<
                        "2. Selection sort\n" <<
                        "3. Insertion sort\n" <<
                        "4. Merge sort\n" <<
                        "5. Quicksort\n";
    std::cin >> choice;
    //test for invalid input
    while(std::cin.fail() || choice<1 || choice>5){
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Wrong input. Please enter again:\n";
        std::cin >> choice;
    }
    if (choice==1) { //bubble
        std::cout << std::setw(65) << "Bubble sort's execution time:\n";
        std::cout << std::setw(31) << "5000 elements" << std::setw(28) <<
        "10000 elements" << std::setw(29) << "20000 elements\n";
        calcTime4 (a, count, bubbleSort);
    }
    else if (choice==2) { //selection
         std::cout << std::setw(65) << "Selection sort's execution time:\n";
         std::cout << std::setw(31) << "5000 elements" << std::setw(28) <<
         "10000 elements" << std::setw(29) << "20000 elements\n";
         calcTime4 (a, count, selectionSort);
    }
    else if (choice==3) { //insertion
         std::cout << std::setw(65) << "Insertion sort's execution time:\n";
         std::cout << std::setw(31) << "5000 elements" << std::setw(28) <<
         "10000 elements" << std::setw(29) << "20000 elements\n";
         calcTime4 (a, count, insertionSort);
    }
    else if (choice==4) { //merge
         std::cout << std::setw(65) << "Merge sort's execution time:\n";
         std::cout << std::setw(30) << "5000 elements" << std::setw(28) <<
         "10000 elements" << std::setw(29) << "20000 elements\n";
         calcTime5 (a, 0, count, mergeSort);
     }
     else { //quick
         std::cout << std::setw(65) << "Quicksort's execution time:\n";
         std::cout << std::setw(30) << "5000 elements" << std::setw(28) <<
         "10000 elements" << std::setw(29) << "20000 elements\n";
         calcTime5 (a, 0, count, quickSort);
     }
}

/* This function calculates the execution time of two of the following sorts: bubble,
* selection, insertion for 5 times and calculates the average execution time.
* The function then prints execution time on the console.
*/
void calcTime1 (std::string a1[], int count1, void (*f)(std::string a[], int count),
                std::string a2[], int count2, void (*g)(std::string a[], int count))
{
    //temporary array to hold unsorted elements
    std::string p1[count1];
    std::string p2[count2];
    //assign unsorted elements from a to p
    for (int i=0; i<count1; i++) {p1[i]=a1[i];}
    for (int i=0; i<count2; i++) {p2[i]=a2[i];}
    
    //arrays holding the time for each time data gets sorted
    double timeRecorded1[5], timeRecorded2[5];
    double timeAverage1=0, timeAverage2=0;
    
    for (int i=1; i<6; i++) {
            //reserve original array for re-sorting
            for (int j=0; j<count1; j++) {a1[j]=p1[j];}
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            f(a1, count1);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
            timeRecorded1[i-1]=time_span1.count();
        
            for (int j=0; j<count2; j++) {a2[j]=p2[j];}
            high_resolution_clock::time_point t3 = high_resolution_clock::now();
            g(a2, count2);
            high_resolution_clock::time_point t4 = high_resolution_clock::now();
            duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);
            timeRecorded2[i-1]=time_span2.count();
        
            std::cout << i << "." << std::right << std::setw(20) << timeRecorded1[i-1] <<
            " seconds" << std::right << std::setw(40) << timeRecorded2[i-1] << " seconds"
            << std::endl;
    }
    timeAverage1=calcAverage(timeRecorded1);
    timeAverage2=calcAverage(timeRecorded2);
    std::cout << "Average time:  " << timeAverage1 << " seconds" << std::right <<
    std::setw(40) << timeAverage2 << " seconds\n";
}

/* This function calculates the execution time of one of the following sorts: bubble,
* selection, insertion for 5 times; and one of the following: merge, quick for 5 times.
* Then it calculates the average execution time.
* The function then prints execution time on the console.
*/
void calcTime2 (std::string a1[], int count, void (*f)(std::string a[], int count),
                std::string a2[], int from, int to, void (*g)(std::string a[], int from, int to))
{
    //temporary array to hold unsorted elements
    std::string p1[count];
    std::string p2[to];
    //assign unsorted elements from a to p
    for (int i=0; i<count; i++) {p1[i]=a1[i];}
    for (int i=0; i<to; i++) {p2[i]=a2[i];}
    
    //arrays holding the time for each time data gets sorted
    double timeRecorded1[5], timeRecorded2[5];
    double timeAverage1=0, timeAverage2=0;
    
    for (int i=1; i<6; i++) {
            //reserve original array for re-sorting
            for (int j=0; j<count; j++) {a1[j]=p1[j];}
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            f(a1, count);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
            timeRecorded1[i-1]=time_span1.count();
        
            for (int j=0; j<to; j++) {a2[j]=p2[j];}
            high_resolution_clock::time_point t3 = high_resolution_clock::now();
            g(a2, from, to);
            high_resolution_clock::time_point t4 = high_resolution_clock::now();
            duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);
            timeRecorded2[i-1]=time_span2.count();
        
            std::cout << i << "." << std::right << std::setw(20) << timeRecorded1[i-1] <<
            " seconds" << std::right << std::setw(40) << timeRecorded2[i-1] << " seconds"
            << std::endl;
    }
    timeAverage1=calcAverage(timeRecorded1);
    timeAverage2=calcAverage(timeRecorded2);
    std::cout << "Average time:  " << timeAverage1 << " seconds" << std::right <<
    std::setw(40) << timeAverage2 << " seconds\n";
}

/* This function calculates the execution time of merge sort and quicksort.
* The function then prints execution time on the console.
*/
void calcTime3 (std::string a1[], int from1, int to1, void (*f)(std::string a[], int from1, int to1),
                std::string a2[], int from2, int to2, void (*g)(std::string a[], int from2, int to2))
{
    //temporary array to hold unsorted elements
    std::string p1[to1];
    std::string p2[to2];
    //assign unsorted elements from a to p
    for (int i=0; i<to1; i++) {p1[i]=a1[i];}
    for (int i=0; i<to2; i++) {p2[i]=a2[i];}
    
    //arrays holding the time for each time data gets sorted
    double timeRecorded1[5], timeRecorded2[5];
    double timeAverage1=0, timeAverage2=0;
    
    for (int i=1; i<6; i++) {
            //reserve original array for re-sorting
            for (int j=0; j<to1; j++) {a1[j]=p1[j];}
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            f(a1, from1, to1);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
            timeRecorded1[i-1]=time_span1.count();
        
            for (int j=0; j<to2; j++) {a2[j]=p2[j];}
            high_resolution_clock::time_point t3 = high_resolution_clock::now();
            g(a2, from2, to2);
            high_resolution_clock::time_point t4 = high_resolution_clock::now();
            duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);
            timeRecorded2[i-1]=time_span2.count();
        
            std::cout << i << "." << std::right << std::setw(21) << timeRecorded1[i-1] <<
            " seconds" << std::right << std::setw(40) << timeRecorded2[i-1] << " seconds"
            << std::endl;
    }
    timeAverage1=calcAverage(timeRecorded1);
    timeAverage2=calcAverage(timeRecorded2);
    std::cout << "Average time:  " << timeAverage1 << " seconds" << std::right <<
    std::setw(40) << timeAverage2 << " seconds\n";
}

/* This funtion calculates the execution time of one of the following sorts: bubble,
* selection, insertion for 5 times, over 5000, 10000, and 20000 elements.
* Then it calculates the average execution time for each data size.
* The function then prints execution time on the console.
*/
void calcTime4 (std::string a[], int count, void (*f)(std::string a[], int count))
{
    //temporary array to hold unsorted elements
    std::string p[count];
    //assign unsorted elements from a to p
    for (int i=0; i<count; i++) {p[i]=a[i];}
    
    //arrays holding the time for each time data gets sorted
    double timeRecorded1[5], timeRecorded2[5], timeRecorded3[5];
    double timeAverage1=0, timeAverage2=0, timeAverage3=0;
    for (int i=1; i<6; i++) {
            //reserve original array for re-sorting
            for (int j=0; j<count; j++) {a[j]=p[j];}
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            f(a, 5000);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
            timeRecorded1[i-1]=time_span1.count();
        
            for (int j=0; j<count; j++) {a[j]=p[j];}
            high_resolution_clock::time_point t3 = high_resolution_clock::now();
            f(a, 10000);
            high_resolution_clock::time_point t4 = high_resolution_clock::now();
            duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);
            timeRecorded2[i-1]=time_span2.count();
        
            for (int j=0; j<count; j++) {a[j]=p[j];}
            high_resolution_clock::time_point t5 = high_resolution_clock::now();
            f(a, 20000);
            high_resolution_clock::time_point t6 = high_resolution_clock::now();
            duration<double> time_span3 = duration_cast<duration<double>>(t6 - t5);
            timeRecorded3[i-1]=time_span3.count();
        
            std::cout << i << "." << std::right << std::setw(22) << timeRecorded1[i-1] <<
            " seconds" << std::right << std::setw(20) << timeRecorded2[i-1] << " seconds"
            << std::right << std::setw(20) << timeRecorded3[i-1] << " seconds\n";
    }
    timeAverage1=calcAverage(timeRecorded1);
    timeAverage2=calcAverage(timeRecorded2);
    timeAverage3=calcAverage(timeRecorded3);
    std::cout << "Average time:   " << timeAverage1 << " seconds" << std::right <<
    std::setw(20) << timeAverage2 << " seconds" << std::right << std::setw(20)
    << timeAverage3 << " seconds\n";
}

/* This funtion calculates the execution time of one of the following sorts: merge,
* quick for 5 times, over 5000, 10000, and 20000 elements.
* Then it calculates the average execution time for each data size.
* The function then prints execution time on the console.
*/
void calcTime5 (std::string a[], int from, int to, void (*f)(std::string a[], int from, int to))
{
    //temporary array to hold unsorted elements
    std::string p[to];
    //assign unsorted elements from a to p
    for (int i=0; i<to; i++) {p[i]=a[i];}
    
    //arrays holding the time for each time data gets sorted
    double timeRecorded1[5], timeRecorded2[5], timeRecorded3[5];
    double timeAverage1=0, timeAverage2=0, timeAverage3=0;
    
    for (int i=1; i<6; i++) {
            //reserve original array for re-sorting
            for (int j=0; j<to; j++) {a[j]=p[j];}
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            f(a, 0, 5000);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
            timeRecorded1[i-1]=time_span1.count();
        
            for (int j=0; j<to; j++) {a[j]=p[j];}
            high_resolution_clock::time_point t3 = high_resolution_clock::now();
            f(a, 0, 10000);
            high_resolution_clock::time_point t4 = high_resolution_clock::now();
            duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);
            timeRecorded2[i-1]=time_span2.count();
        
            for (int j=0; j<to; j++) {a[j]=p[j];}
            high_resolution_clock::time_point t5 = high_resolution_clock::now();
            f(a, 0, 20000);
            high_resolution_clock::time_point t6 = high_resolution_clock::now();
            duration<double> time_span3 = duration_cast<duration<double>>(t6 - t5);
            timeRecorded3[i-1]=time_span3.count();
        
            std::cout << i << "." << std::right << std::setw(22) << timeRecorded1[i-1] <<
            " seconds" << std::right << std::setw(20) << timeRecorded2[i-1] << " seconds"
            << std::right << std::setw(20) << timeRecorded3[i-1] << " seconds\n";
    }
    timeAverage1=calcAverage(timeRecorded1);
    timeAverage2=calcAverage(timeRecorded2);
    timeAverage3=calcAverage(timeRecorded3);
    std::cout << "Average time:  " << timeAverage1 << " seconds" << std::right <<
    std::setw(20) << timeAverage2 << " seconds" << std::right << std::setw(20)
    << timeAverage3 << " seconds\n";
}

/* This function reads in an array of size 5 and calculates the average of all the
 * elements in the array, then returns the average.
 */
double calcAverage(double a[])
{
    double timeAverage=0;
    for (int i=0; i<5; i++) {timeAverage += a[i];}
    timeAverage = timeAverage/5;
    return timeAverage;
}

/* This function gives user list of choices after data has been sorted:
 * seeing first 50 elements, last 50 elements, returning to main menu
 * (choosing text file), or quitting.
 * The function has the ability to test for invalid input.
 */
int afterSort(std::string a[], int s)
{
    bool check=true; //condition for printing menu
    int choice=0; //store users' choice
    while(check) {
        std::cout << "1. See first 50 elements of sorted list\n" <<
                            "2. See last 50 elements of sorted list\n" <<
                            "3. Return to main menu\n" <<
                            "4. Quit\n";
        std::cin >> choice;
        //test for invalid input
        while (std::cin.fail() || choice<1 || choice>4) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Wrong input. Please enter again:\n";
            std::cin >> choice;
        }
        //if users choose to see first 50 elements
        if (choice==1) {
            for (int i=0; i<50; i++) {std::cout << a[i] << std::endl;}
        }
        //if users choose to see last 50 elements
        else if (choice==2) {
            for (int i=(s-50); i<(s-1); i++) {std::cout << a[i] << std::endl;}
        }
        //if users want to come back to main menu
        else if (choice==3) {return 1;}
        //if users want to quit the program
        else {
            check=false; //quit current while loop
            return -1; //return -1 to quit main menu as well
        }
    }
}







