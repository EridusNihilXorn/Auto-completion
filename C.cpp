#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
//Input: text consisting of rows. Every row ends word or sentence and adds '\n'
//Output: amount of symbols required to input this text with usage of auto-completion

using namespace std;

const int ALPHABET_SIZE = 26;
const string punctuation = ".,?!'- ";

//Check if char is punctuational
bool isPunc(char ch)
{
	for (char buf : punctuation)
		if (ch == buf) return true;
	return false;
}

struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];
	bool isEndOfWord;
};

// Returns new trie node with children == NULL
// i.e. constructor-like function
struct TrieNode *createNode(void)
{
	struct TrieNode *pNode = new TrieNode;

	pNode->isEndOfWord = false;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = NULL;

	return pNode;
}

// Insert word in a trie
// If the word is prefix, mark leaf node
void insert(struct TrieNode *root, string word)
{
	struct TrieNode *pCrawl = root;

	for (int i = 0; i < word.length(); i++)
	{
		int index = word[i] - 'a';
		if (!pCrawl->children[index])
			pCrawl->children[index] = createNode();

		pCrawl = pCrawl->children[index];
	}
	pCrawl->isEndOfWord = true;
}

// Returns true if word is in trie
bool search(struct TrieNode *root, string word)
{
	struct TrieNode *pCrawl = root;

	for (int i = 0; i < word.length(); i++)
	{
		int index = word[i] - 'a';
		if (!pCrawl->children[index])
			return false;

		pCrawl = pCrawl->children[index];
	}
	bool exist = (pCrawl != NULL);
	if (exist == false) return exist;
	else return (pCrawl != NULL && pCrawl->isEndOfWord);
}

int findEconomy(struct TrieNode *root, string word)
{
	int point = 0;
	int branchNum = 0;
	TrieNode *current = root;
	//We won't get profit from word with length < 3
	if (word.length() < 3) return 0;
	for (int i = 0; i < word.length(); i++)
	{
		int index = word[i] - 'a';
		for (auto child : current->children)
		{
			//Trying to find last branched node leading to end of word
			//Also takes care about prefix-word leading to end of word
			if (child != NULL || current->isEndOfWord) branchNum++;
			if (branchNum > 1)
			{
				//Point == last branched node index from the begining
				point = i;
				break;
			}
		}
		current = current->children[index];
		branchNum = 0;
	}
	//Check if end symbol of a word has children
	//We can't use shorter words with children
	//Due to limitation of only one prediction
	for (auto child : current->children)
	{
		if (child != NULL) branchNum++;
		if (branchNum >= 1) return 0;
	}
	//Returns economy of symbols
	int economy = word.length() - point - 2;
	if (economy < 0) economy = 0;
	return economy;
}

int main()
{
	string buffer;
	vector<string> text;
	while (getline(cin, buffer))
	{
		//if (buffer == "0") break;
		text.push_back(buffer);
	}

	int sum = 0;
	string word;
	struct TrieNode *root = createNode();
	for (string str : text)
	{
		for (int i = 0; i < str.length(); i++)
		{
			sum++;
			//If it's end of a word
			if (isPunc(str[i]) || str[i + 1] == '\0' || str[i + 1] == '\n')
			{
				if (str[i+1] == '\0' && !isPunc(str[i]))	word.push_back(str[i]);
				cout << word << "\n";
				//Check every substr from full word to 1 char
				for (int i = word.length(); i > 0; i--)
				{
					string buffer = word.substr(0, i);
					if (search(root, buffer))
					{
						//If possible shortening was found, calculate economy
						//And go to next word
						cout << findEconomy(root, buffer) << "\n";
						sum -= findEconomy(root, buffer);
						break;
					}
				}
				insert(root, word);
				word.clear();
			}
			else
				word.push_back(str[i]);
		}
		sum++;
	}
	cout << sum;
	return 0;
}
