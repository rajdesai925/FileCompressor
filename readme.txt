For building, our fileCompressor first reads through the file and arranges all tokens into a linked list. Each node contains the token and its frequency. This linked list is then converted into a minheap using the frequencies and the heap is then used to build the Huffman codebook.

For compressing, we first iterate through the Huffman codebook and store the tokens with their corresponding Huffman codes in a linked list. Then, we parse through the target file, matching tokens with their corresponding code from the linked list and printing that code to the output file.

For decompressing, we also iterate through the Huffman codebook and store the tokens with their corresponding Huffman codes into a linked list. Then, we parse through the target file and decompress the code using the linked list as a translation key.

Each of these functions had a recursive part which could be activated to recursively apply the operation to all valid targets in all subdirectories.

Overall, out program has a run time of O(nlogn). Our program utilizes both a linked list and tree to store tokens and their frequencies/codes. Accessing a linked list takes O(n) and accessing a binary tree takes O(nlogn). Since both operations are needed, the overall run times comes out to O(nlogn).