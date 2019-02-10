#include<iostream>
#include<stack>
#include<string>
#include<fstream>
#include<cctype>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sstream>
using namespace std;

#define MAX_TREE_HT 52


struct HuffNode
{
    int freq;
    char data;
    struct HuffNode *left;
    struct HuffNode *right;
};

struct HuffTree
{
    int size;
    int capacity;
    struct HuffNode **array;
};


struct HuffNode* newNode(char data, int freq)
{
    struct HuffNode* temp =
          (struct HuffNode*) malloc(sizeof(struct HuffNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}


struct HuffTree* newHeap(int capacity)
{
    struct HuffTree* HuffTree =
         (struct HuffTree*) malloc(sizeof(struct HuffTree));
    HuffTree->size = 0;
    HuffTree->capacity = capacity;
    HuffTree->array =
     (struct HuffNode**)malloc(HuffTree->capacity * sizeof(struct HuffNode*));
    return HuffTree;
}

void swapStruct(struct HuffNode** a, struct HuffNode** b)
{
    struct HuffNode* temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(struct HuffTree* HuffTree, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < HuffTree->size && HuffTree->array[left]->freq < HuffTree->array[smallest]->freq)
      smallest = left;

    if (right < HuffTree->size && HuffTree->array[right]->freq < HuffTree->array[smallest]->freq)
      smallest = right;

    if (smallest != idx)
    {
        swapStruct(&HuffTree->array[smallest], &HuffTree->array[idx]);
        heapify(HuffTree, smallest);
    }
}

int isSizeOne(struct HuffTree* HuffTree)
{
    return (HuffTree->size == 1);
}

struct HuffNode* extractMin(struct HuffTree* HuffTree)
{
    struct HuffNode* temp = HuffTree->array[0];
    HuffTree->array[0] = HuffTree->array[HuffTree->size - 1];
    --HuffTree->size;
    heapify(HuffTree, 0);
    return temp;
}

void insertMinHeap(struct HuffTree* HuffTree, struct HuffNode* HuffNode)
{
    ++HuffTree->size;
    int i = HuffTree->size - 1;
    while (i && HuffNode->freq < HuffTree->array[(i - 1)/2]->freq)
    {
        HuffTree->array[i] = HuffTree->array[(i - 1)/2];
        i = (i - 1)/2;
    }
    HuffTree->array[i] = HuffNode;
}

void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}


struct HuffTree* buildHeap(char data[], int freq[], int size)
{
    struct HuffTree* HuffTree = newHeap(size);
    for (int i = 0; i < size; ++i)
        HuffTree->array[i] = newNode(data[i], freq[i]);
    HuffTree->size = size;

    int n = HuffTree->size - 1;
    int j;
    for (j = (n - 1) / 2; j >= 0; --j)
    heapify(HuffTree, j);
    return HuffTree;
}

struct HuffNode* buildHuffmanTree(char data[], int freq[], int size)
{
    struct HuffNode *left, *right, *top;
    struct HuffTree* HuffTree = buildHeap(data, freq, size);

    while (!isSizeOne(HuffTree))
    {
        left = extractMin(HuffTree);
        right = extractMin(HuffTree);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(HuffTree, top);
    }

    return extractMin(HuffTree);
}


void printCodes(struct HuffNode* root, int arr[], int top)
{
start:

    if (root->left)
    {
        arr[top] = 1;
        //root = root->left; top++; goto start;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right)
    {
        arr[top] = 0;
        root = root->right; top++; goto start;
        //printCodes(root->right, arr, top + 1);
    }


    if ((!(root->left) && !(root->right)))
    {
       cout << root->data << ": "; //char
       printArr(arr, top);         //code
    }

     /*   stringstream strStream (stringstream::in | stringstream::out);
        for(i=0;i<top;i++)
        {
            strStream << arr[i];
        }
        code[inc] = strStream.str();
         cout << code[inc] << endl;
        inc++; */
}




void HuffmanCodes(char data[], int freq[], int size)
{

   struct HuffNode* root = buildHuffmanTree(data, freq, size);

   int arr[MAX_TREE_HT];
   int top = 0;
   printCodes(root, arr, top);
}

void decodeFile(string filecode, string code[], char ch[], int size)
{
    int i,count;
    unsigned int j = 0;
    string dummy;
    string test;
    while(j < filecode.length())
    {
        top:
        count = 0;
        if (j + 1 > filecode.length()) break;
        dummy = filecode.at(j);

        top2:


        for(i=0;i<size;i++)
        {
            if(dummy == code[i])
            {
                cout << ch[i];
                j = j + 1 + count;
                goto top;
            }
        }

        test = filecode.at(j + 1 + count);
        dummy = dummy + test;

        count++;
        goto top2;
    }
    return;

}

int main()
{
    // Part 1
    string str;
    int size, i;

    ifstream inFile;
    inFile.open("a4-1.txt");
    getline(inFile,str);
    size = atoi(str.c_str()); // convert str to int


    string strs[size];
    string substrs[size];


    //char chars[size];
    char arr[size];
    int freq[size];


    for(i=0;i<size;i++)
    {
        getline(inFile,strs[i]);
        substrs[i] = strs[i].substr(strs[i].find(" ") + 1);
        freq[i] = atoi(substrs[i].c_str());
        arr[i] = strs[i].at(0);
    }

    cout << "CodeBook:" << endl;
    HuffmanCodes(arr, freq, size);
    cout << endl;


    // Part 2
    inFile.close();
    string filecode;

    inFile.open("a4-2.txt");
    getline(inFile,filecode);
    cout << "Binary Code: " << endl << filecode << endl << endl;
    cout << "Translation:" << endl;


    string binarycodearray[size] = {"111","11011","11010","1100","1011","101011","101010","10100","1001","10001","10000111","10000110","1000010","100000","0"};
    char characterarray[size] = {'i','n','e','g','j','c','k','a','h','d','l','b','m','f','p'};
    decodeFile(filecode, binarycodearray, characterarray, size);


    cout << endl;








}
