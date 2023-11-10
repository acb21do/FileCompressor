#include "Huffman.h"

void Huffman::CreateMinHeap()
{
    for (int i = 0; i < 128; i++)
    {
        CharArray[i] = new Node();
    }

    InFile.open(InFileName);
    if (!InFile.is_open())
    {
        std::cout << "Unable to open the file: " << InFileName << std::endl;
        return;
    }

    // Increases the frequency of chars in the text
    char c;
    std::string line;
    while (InFile.get(c)) {
        CharArray[c]->Frequency++;
    }
    // Add the nodes to the minHeap
    for (int i = 0; i < 128; i++)
    {
        if (CharArray[i]->Frequency > 0)
        {
            CharArray[i]->Character = i;
            minHeap.push(CharArray[i]);
        }
    }

    InFile.close();
}

void Huffman::CreateTree()
{
    std::priority_queue<Node*, std::vector<Node*>, Compare> tempHeap(minHeap);
    // Connects the two lowest nodes.
    while (tempHeap.size() != 1)
    {
        RootNode = new Node();

        RootNode->Left = tempHeap.top();
        tempHeap.pop();
        RootNode->Right = tempHeap.top();
        tempHeap.pop();

        RootNode->Frequency = RootNode->Left->Frequency + RootNode->Right->Frequency;
        tempHeap.push(RootNode);
    }

}

void Huffman::CreateCodes(Node* CurrNode, std::string CurrStr)
{
    if (CurrNode->Left == nullptr && CurrNode->Right == nullptr)
    {
        CurrNode->Code = CurrStr;
        return;
    }
    CreateCodes(CurrNode->Left, CurrStr + "0");
    CreateCodes(CurrNode->Right, CurrStr + "1");
}



void Huffman::EncodeText()
{
    std::string res;
    // Adds the number of nodes
    res += (char)minHeap.size();
    std::string BinaryString;

    /* Add each Node's character, code length and code. */

    std::priority_queue<Node*, std::vector<Node*>, Compare> tempHeap(minHeap);
    while (!tempHeap.empty())
    {
        // Add the character.
        Node* currNode = tempHeap.top();
        res += currNode->Character;


        // Add the code length.
        char codeLength = static_cast<char>('0' + currNode->Code.length());
        res += codeLength;

        /* Add the code. */
        // Convert the code into 8 bit chars
        BinaryString = currNode->Code;
        while (BinaryString.length() >= 8)
        {
            int bin = std::stoi(BinaryString.substr(0, 8), 0, 2);
            char binRes = static_cast<char>(bin);

            res += binRes;
            BinaryString.erase(0, 8);
        }
        // Add zeros to the remaining string to make it 8 bits.
        if (BinaryString.length() > 0)
        {
            int zeroCount = 8 - (int)BinaryString.length();
            BinaryString.append(zeroCount, '0');
            int bin = std::stoi(BinaryString, 0, 2);
            char binRes = static_cast<char>(bin);
            res += binRes;
        }
        tempHeap.pop();
    }

    /* Encode each character and store them as 8 bit chars */
    char c;
    BinaryString = "";
    InFile.open(InFileName);
    if (!InFile.is_open())
    {
        std::cout << "Unable to open the file: " << InFileName << std::endl;
        return;
    }
    // Convert each char in the text file to its Huffman equivalent
    while (InFile.get(c))
    {
        BinaryString += CharArray[c]->Code;
        while (BinaryString.length() >= 8)
        {
            std::string subString = BinaryString.substr(0, 8);
            int bin = std::stoi(subString, 0, 2);
            char binRes = static_cast<char>(bin);
            res += binRes;
            BinaryString.erase(0, 8);
        }
    }
    // Add zeros to the remaining string to make it 8 bits.
    int zeroCount = 8 - (int)BinaryString.length();
    /*if (BinaryString.length() > 0)
    {

    }*/
    BinaryString.append(zeroCount, '0');
    int bin = std::stoi(BinaryString, 0, 2);
    char binRes = static_cast<char>(bin);
    res += binRes;
    // Add the 0 count to the end of the string.
    res += (char)zeroCount;


    InFile.close();
    OutFile.open(OutFileName, std::ios::out | std::ios::binary);
    OutFile.write(res.c_str(), res.size());

    OutFile.close();
}


void Huffman::DecodeText()
{
    InFile.open(InFileName, std::ios::in | std::ios::binary);
    if (!InFile.is_open())
    {
        std::cout << "Unable to open the file: " << InFileName << std::endl;
        return;
    }
    // Get the number of nodes
    char size;
    InFile.read(&size, 1);
    /* Get each node character, code length and code */
    for (int i = 0; i < size; i++)
    {
        char Character;
        char CodeLength;
        int CodeByteSize;
        // Get character
        InFile.read(&Character, 1);
        // Get code length
        InFile.read(&CodeLength, 1);
        CodeLength = static_cast<int>(CodeLength - '0');
        /* Get code */
        CodeByteSize = (CodeLength + 7) / 8;
        char* CodeChar = new char[CodeByteSize];
        std::string CodeStr;
        InFile.read(CodeChar, CodeByteSize);

        for (int j = 0; j < CodeByteSize; j++)
        {
            std::string binaryString = std::bitset<8>(CodeChar[j]).to_string();
            CodeStr += binaryString;
        }
        int diff = CodeStr.length() - (int)CodeLength;
        CodeStr = CodeStr.substr(0, CodeStr.length() - diff);

        /* Create a new node with these variables*/

        // Build the tree's path to the node
        GetTree(Character, CodeStr);

        delete[] CodeChar;
    }

    /* Get the text*/

    // Get the current reading position
    std::streampos originalPos = InFile.tellg();
    // Get the zero padding at the end of the text
    InFile.seekg(-1, std::ios::end);
    char zeroCount;
    InFile.read(&zeroCount, 1);
    // Return back to current reading position
    InFile.seekg(originalPos);
    // Get each char and convert it to a binary string
    char c;
    while (InFile.get(c))
    {
        //InFile.read(&c, 1);

        std::string binaryString = std::bitset<8>(c).to_string();
        FileContents += binaryString;
    }
    // Take off the zero padding and zero count at the end of the string
    int diff = FileContents.length() - (int)zeroCount - 8;
    int fileSize = FileContents.length();
    FileContents = FileContents.substr(0, diff);

    InFile.close();
}


void Huffman::GetTree(char Character, std::string& code)
{
    Node* currentNode = RootNode;
    for (int i = 0; i < code.length(); i++)
    {
        if (code[i] == '0') {
            if (currentNode->Left == NULL) {
                currentNode->Left = new Node();
            }
            currentNode = currentNode->Left;
        }
        else if (code[i] == '1') {
            if (currentNode->Right == NULL) {
                currentNode->Right = new Node();
            }
            currentNode = currentNode->Right;
        }
    }
    currentNode->Character = Character;
}


void Huffman::Compress()
{
    CreateMinHeap();
    CreateTree();
    CreateCodes(RootNode, "");
    EncodeText();
}

void Huffman::GenerateText()
{
    OutFile.open(OutFileName, std::ios::out);
    std::string res;
    Node* currNode = RootNode;
    for (char c : FileContents)
    {

        if (c == '0')
        {
            if (currNode->Left->Character != NULL)
            {
                res += currNode->Left->Character;
                currNode = RootNode;
            }
            else
                currNode = currNode->Left;
        }
        else
        {
            if (currNode->Right->Character != NULL)
            {
                res += currNode->Right->Character;
                currNode = RootNode;
            }
            else
                currNode = currNode->Right;
        }
    }

    OutFile.write(res.c_str(), res.size());
    OutFile.close();
}

void Huffman::Decompress()
{
    RootNode = new Node;
    DecodeText();
    GenerateText();
}
