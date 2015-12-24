/////////////////////////////////////////
// Huffman 
// Muhammad Najib
// hdecode.cc
// CS 111 Data Structures
// May 11 2015
//
// The program takes an input file and produces
// an output file by decoding characters taken  
// from the input file. The program takes the length
// of original file , reads tree from header and 
// then decodes character by character. The
// characters from file are converted to bits
// using their byte value and then converted to
// data by following the path given by bits.

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <ctype.h>
#include <cstdlib>

using namespace std;

//node contains the char data,
//its frequency in file as count
//and two pointers to left and right
//children.
struct node {
  char data;
  size_t count;
  node *left, *right;
  node(char data, size_t count) :
    data(data), count(count),
    left(NULL), right(NULL) {}
  node(node* left, node* right) :
    left(left), right(right){}
};

//method to read tree from file
//if character is L it creates leaf
//using next character otherwise builds
//an internal node if character is 'I'
node* read_tree(ifstream & in)
{
  unsigned char type = in.get();
  if(type == 'L'){
    unsigned char x = in.get(); 
    return new node(x,0);
  }  
  else if (type == 'I'){
    node* left = read_tree(in);
    node* right = read_tree(in);
    return new node(left, right);
  }
  return NULL;
}

//method converts a byte into 8 bits
//byte is read from file and converted to bits
//that reveal path to a leaf storing data
void byte_to_bits(int byte, string & bits)
{
  for(int i = 7; i >= 0; i--){
    byte < pow (2,i)? bits = bits + '0':bits = bits+'1';
    byte >= pow(2,i)? byte = byte - pow(2,i):byte = byte;
  }  
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

int main(int argc, char** argv)
{
  //error 1: wrong command line 
  if(argc != 2){
    usage();    
  }
  //input file opened using ifstream
  string infile_name = argv[1];
  ifstream in1(infile_name.c_str());
  
  //error 2: file could not be opened
  if(!in1.good()) { 
    error(infile_name + "could not be opened!");
  }

  //creates outfilename from input file
  //to print decoded characters to output
  //file stream
  string outfile_name = argv[1];
  size_t length = infile_name.length();
  outfile_name = outfile_name.erase(length-4,4);
  ofstream out1(outfile_name.c_str());

  //total characters of original file that was encoded
  int total_count;
  in1>>total_count;

  //read tree from header of encoded file
  node* sub_tree = read_tree(in1);

  //count for total chars output to outfile
  //tree is root of tree that was read from header
  //bits to store bits after conversion from bytes taken from
  //encoded file
  int count = 0; node* tree = sub_tree; string bits = "";
 
  //loop to output decoded characters 
  //till total count is reached
  while(count<total_count) {
    //if node is a leaf output data,
    //increment count and go back to root.
    //else move to right or left child as given by
    //first element of bits.
    if(!sub_tree->left or !sub_tree->right) { 
      out1<<sub_tree->data;
      count++;
      sub_tree = tree;
    }
    else{
      if(bits == ""){
        unsigned char x = in1.get();
	byte_to_bits(x,bits);
      }
      //if first element is 0 go left, otherwise
      //go right.
      if(bits[0] == '0')
	sub_tree = sub_tree->left;
      else
	sub_tree = sub_tree->right;
       
      //remove first element from bits
      bits = bits.substr(1);
    }
  }
  //remove the input file
  remove(infile_name.c_str());
}
