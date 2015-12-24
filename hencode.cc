/////////////////////////////////////////
// Huffman 
// Muhammad Najib
// hencode.cc
// CS 111 Data Structures
// May 11 2015
//
// The program takes an input file and produces
// an output file by encoding characters taken  
// from the input file. The program takes the length
// of original file and makes a minheap data structure
// based on the frequency of characters in the file.
// Using that minheap it prints file size, tree header
// and the characters after converting them from bit
// strings made from huffman tree.

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctype.h>
#include <string>
#include <math.h>
using namespace std;

//two arrays to store frequency
//and bit string of path in huffman
//tree for each character in file.
size_t counts[256] = { 0 };
string codes[256];

// a node to contain a character from file
// as data and its frequency as count.
// left and right node pointers point to 
// two children
struct node {
  unsigned char data;
  int count;
  node *left, *right;
  node(char data, size_t count) : 
    data(data), count(count), 
    left(NULL), right(NULL) {}
  node(size_t count, node* left, node* right) :
    count(count),left(left),right(right){}
};

class minheap
{
 public:
  //constructor
  minheap() 
  {
    N = 0;
    for(int i =0; i < 256; i++)
      data[i] = NULL; 
  }
  
  //destructor
  ~minheap()
  {
    while(size() != 0){
      node* dead = remove();
      delete dead;
    }
  }

  //returns size of minheap
  size_t size(){
    return N;
  }

  //returns array subsript of parent 
  //of current node
  size_t parent(size_t k){
    return (k-1)/2;
  }

  //returns array subsript of left_child 
  //of current node
  size_t left_child(size_t k){
    return 2*k+1;
  }
 
  //returns array subsript of right_child 
  //of current node
  size_t right_child(size_t k){
    return 2*k+2;
  } 
  
  //adds a new node to minheap
  //reheap_up function used to 
  //sort the minheap to its correct order
  void add(node* new_data)
  {
    data[N] = new_data;
    N++;
    reheap_up(N-1);
  }
 
  //removes a node from minheap
  //reheap_down sorts the heap 
  //as per minheap structure.
  //removes the first node and 
  //replaces it with last one.
  node * remove()
  {
    node* result = data[0]; 
    data[0] = data[N-1];
    N--;
    reheap_down(1);
    return result;
  }

  //sorts the heap after an item
  //is removed
  void reheap_down(size_t k)
  {
    //return if out of range
    if(k >= N)
      return;
       
    //store the index of left child
    size_t child = left_child(k);

    //if right child exists and count of right child
    //is less than count of left child then
    //assign child the value of right child
    if(right_child(k) < N and 
    data[right_child(k)]->count < data[child]-> count)
      child = right_child(k);
     
    //return if count of current node is less than that of 
    //its child(assigned left or right previously
    if(data[k]-> count <= data[child]->count)
      return;

    //swap the current node with its child
    //if current node not a leaf
    if(child < N)
      swap(data[k], data[child]);
    
    //recursive step. 
    reheap_down(child);
  }

  void reheap_up(size_t k)
  {
    //return if k is zero
    if(k == 0 )
       return;

    //store index of parent of k
    size_t p = parent(k);
  
    //return if count of child is greater
    //than count of parent
    if(data[k]-> count >= data[p]->count)
      return;
    
    //if data is not NULL at 
    //index of parent and k
    //then swap two pointers
    if(data[k] and data[p])
	swap(data[k],data[p]);
    
    //recursive step
    reheap_up(p);
      
  }

  //print the minheap
  void print() {
    _print_help();
  }

  //build the huffman tree
  //removes first two nodes and 
  //makes a new tree with those two as 
  //children. Builds until size is 1.
  void build_huffman()
  {
    if(N == 1) return;
        
    node* first_node = remove();
    node* second_node = remove();
    node* tree = new node( first_node -> count + second_node-> count,
			   first_node,second_node);
    add(tree);
    if(N  > 1){
      build_huffman();  
    }
  }

  //method to print tree in header of outfile
  void  output_tree(std::ostream& out)
  {
    node * tree = data[0];
    _help_output_tree(out,tree);
  }

  //function to create path for every character
  //and store it in codes array
  void path()
  {
    string bits;
    node * tree = data[0];
    _create_path(tree,bits);
  }

private:

  //N is size of array
  //data is the array of node pointers
  size_t N;
  node * data[256];

  //helper function for print
  void _print_help()
  {
    if (N == 0 )
      return;  
    for(size_t k =0; k < N; k++)
      cout << data[k]->data << "  " << data[k]->count << endl;
  }

  //swaps the two node pointers by using
  //temporary pointer
  void  swap(node*& k, node*& p){
    node * temp = k;
    k = p;
    p = temp;
  }
 
  //creates a path for each character to be stored
  //in codes array
  void _create_path(node* tree, string bits){

    //if node is a leaf store the bits for 
    //the character and remove that path from
    // bits.
    if(!tree->right and !tree->left){
      unsigned char index = tree->data;
      codes[index]= bits; 
      bits.erase(bits.length()-1,1);
      return;
    }   

    //if right child exits go right by adding bits and
    //1 to string parameter.
    if(tree -> right)
      _create_path(tree -> right, bits+'1');

    //if left child exists go left by adding bits 
    //and 0 to the string parameter.
    if(tree -> left)
      _create_path(tree -> left, bits+'0');   
  }

  //helper function for printing tree to header of outfile
  void  _help_output_tree(std::ostream& out, node* tree)
  {
    //returns if size is zero
    if(N == 0 ) 
      return;

    //if node is leaf print L followed by character
    if(!tree->left and !tree->right){
      unsigned char x = tree->data;
      out<<'L'<<x;
      return;
    }
    //if not leaf print I and use inorder depth traversal
    //to go from left to right of the tree
    if(tree->left and tree->right){
       out<<'I';  
      _help_output_tree(out,tree->left);
      _help_output_tree(out,tree->right);
    }
  }
};

//convert the string byte to its integer value
//to print the character represented by the value
//to outfile.
int byte_to_number(string byte)
{
  int decimal = 0;
  for(size_t i = 0; i<8; i++){
      if(byte[i] == '1')
	decimal = decimal + pow(2,7-i);
  }
  return decimal;
}

//add zeros to the left over bits
//to convert them to character later
void padding(string & bits)
{
  if(bits.length() == 0)
    return;
  for(size_t i =bits.length(); i < 8; i++){
      bits += '0';
  }
}

//takes input and output stream to write characters to
//output stream. Takes the first 8 bits and writes the
//character represented by byte to outfile.
void generate_bytes(std::istream& in, std::ostream& out)
{ 
  int c;
  string bits;
  char x; string byte;

  //write to outfile till EOF reached.
  while ((c=in.get())!=EOF) {
    bits = bits + codes[c];
  
    //while loop ensures we have written all possible
    //characters before taking the next string from codes[c]
    while(bits.length() >= 8){
      byte = bits.substr(0,8);
	x =  byte_to_number(byte);
	out<<x;
	bits.erase(0,8);
    }
  } 
  //pad the leftover bits to write the final character.
  padding(bits);
  x = byte_to_number(bits);
  out<<x<<endl; 
}

//instruction to write command_line
void usage()
{
  cout << "Usage: ./hdecode filename " << endl;
  exit(1); // 1 means error
}

//print out error message and exit
void error(string msg)
{
  cout<<msg<<endl;
  exit(1);
}

int main(int argc, char **argv)
{
  //error 1: wrong command line 
  if(argc != 2) {
    usage();    
  }
  string infile_name = argv[1];
  ifstream in1(infile_name.c_str());

  //error 2: file could not be opened
  if(!in1.good()) { 
    error(infile_name + "could not be opened!");
  }

  //opens outfile using ofstream
  string outfile_name =  infile_name + ".huf";
  ofstream out1(outfile_name.c_str());
  int c;

  //loop to store frequency of each character in file
  while (in1.good()) {
    c = in1.get();
    if(in1.good())
      counts[c] += 1;
  }
  
  //create minheap object
  minheap heap_array;

  //create a node for each character in file
  //add node to minheap
  for (size_t i = 0;i < 256; i++)
    if (counts[i] > 0) {
      heap_array.add(new node(i, counts[i]));
    }   

  //build huffman after minheap fully created
  heap_array.build_huffman();
  //store paths in codes array from huffman tree
  heap_array.path();

  //calculate total characters in file 
  size_t total_count=0; 
  for (size_t i = 0;i < 256; i++)
    if (counts[i]) {
      total_count = total_count + counts[i];
    }  

  //write header to outfile
  out1<<total_count;  
  heap_array.output_tree(out1);
  
  //generate codes and write associated characters to
  //outfile.
  ifstream in2(infile_name.c_str());
  generate_bytes(in2,out1); 

  //.huf file complete 
  //remove the input file
  remove(infile_name.c_str());   
}
