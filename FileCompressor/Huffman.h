#pragma once

#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <bitset>

struct Node
{
	unsigned int Frequency = 0;
	char Character = NULL;
	std::string Code;
	Node* Left = nullptr, * Right = nullptr;


};

struct Compare
{
	bool operator() (Node* Left, Node* Right)
	{
		return Left->Frequency > Right->Frequency;
	}
};


class Huffman
{
private:
	std::fstream InFile, OutFile;
	std::string InFileName, OutFileName;
	std::priority_queue<Node*, std::vector<Node*>, Compare> minHeap;
	Node* CharArray[128];
	std::string FileContents;
	Node* RootNode = nullptr;
public:
	Huffman(const std::string& InputFile, const std::string& OutputFile)
		: InFileName(InputFile), OutFileName(OutputFile) {}

private:
	// Creates a min heap of nodes based on their frequency
	void CreateMinHeap();
	// Attatches nodes to create a tree.
	void CreateTree();
	// Runs through the tree and assigns code to each of the leaves
	void CreateCodes(Node* CurrNode, std::string CurrStr);
	// Applies Huffman codes to the input file.
	void EncodeText();
	// Gets information from an encoded file
	void DecodeText();
	// Uses the Huffman code to create a tree.
	void GetTree(char Character, std::string& code);
	// Decodes the input file
	void GenerateText();
public:
	void Compress();
	void Decompress();

};