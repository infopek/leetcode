#include <iostream>
#include <string>
#include <sstream>

#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <stack>
#include <queue>

#include <numeric>
#include <algorithm>

class Node
{
public:
	int val;
	std::vector<Node*> children;

	Node() {}
	Node(int _val)
		: val(_val) {}
	Node(int _val, std::vector<Node*> _children)
		: val(_val), children(_children) {}
};

struct ListNode
{
	int val;
	ListNode* next;

	ListNode()
		: val(0), next(nullptr) {}
	ListNode(int x)
		: val(x), next(nullptr) {}
	ListNode(int x, ListNode* next)
		: val(x), next(next) {}
};

struct TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;

	TreeNode()
		: val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x)
		: val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode* left, TreeNode* right)
		: val(x), left(left), right(right) {}
};

struct TrieNode
{
	TrieNode* next[26];
	int idx = -1;
	std::vector<int> indices;
};

class FenwickTree
{
public:
	FenwickTree(const std::vector<int>& nums) 
	{
		m_Length = nums.size() + 1;

		std::ranges::copy(nums, std::back_inserter(m_Nums));
		m_Tree = std::vector<int>(m_Length);

		for (int i = 1; i < m_Length; i++)
			m_Tree[i] = m_Nums[i];

		for (int child = 1; child < m_Length; child++)
		{
			int parent = child + FindRSB(child);
			if (parent < m_Length)
				m_Tree[parent] += m_Tree[child];
		}
	}

	static int FindRSB(int idx) { return (idx & -idx); }

	void Update(int idx, int val)
	{
		idx++;

		int toAdd = val - m_Nums[idx];
		m_Nums[idx] = val;

		int i = idx;
		while (i < m_Length)
		{
			m_Tree[i] += toAdd;
			i += FindRSB(i);
		}
	}

	int PrefixSum(int idx)
	{
		int result = 0;
		while (idx != 0)
		{
			result += m_Tree[idx];
			idx -= FindRSB(idx);
		}

		return result;
	}
	int SumRange(int left, int right) { return PrefixSum(right + 1) - PrefixSum(left); }

private:
	std::vector<int> m_Nums { 0 };
	std::vector<int> m_Tree;

	size_t m_Length;
};

void DFSCount(TreeNode* node, int& goodNodes, int prevPathMax, int pathMax)
{
	if (!node)
	{
		pathMax = prevPathMax;
		prevPathMax = INT_MIN;
		return;
	}

	if (node->val >= pathMax)
	{
		prevPathMax = pathMax;
		pathMax = node->val;
		goodNodes++;
	}

	std::cout << node->val << ", ";

	DFSCount(node->left, goodNodes, prevPathMax, pathMax);
	DFSCount(node->right, goodNodes, prevPathMax, pathMax);
}

/* 1 SEPT, 2022: COUNT GOOD NODES IN BINARY TREE */
int GoodNodes(TreeNode* root)
{
	int result = 0;
	DFSCount(root, result, INT_MIN, INT_MIN);

	return result;
}

/* Calculates the height of a binary tree. */
int TreeHeight(TreeNode* root)
{
	if (!root)
		return 0;
	else
	{
		int leftHeight = TreeHeight(root->left);
		int rightHeight = TreeHeight(root->right);

		if (leftHeight > rightHeight)
			return leftHeight + 1;
		else
			return rightHeight + 1;
	}
}

/* Helps calculate the average of a level in a tree with root node. */
void AvgCurrLevel(TreeNode* root, int level, double& sum, int& totalNodes)
{
	if (!root)
		return;
	if (level == 0)
	{
		sum += root->val;
		totalNodes++;
	}
	else if (level > 0)
	{
		AvgCurrLevel(root->left, level - 1, sum, totalNodes);
		AvgCurrLevel(root->right, level - 1, sum, totalNodes);
	}
}

/* 2 SEPT, 2022: AVERAGE OF LEVELS IN BINARY TREE */
std::vector<double> AverageOfLevels(TreeNode* root)
{
	int height = TreeHeight(root);
	std::vector<double> averages(height);
	for (int l = 0; l < height; l++)
	{
		double sum = 0;
		int totalNodes = 0;
		AvgCurrLevel(root, l, sum, totalNodes);
		averages[l] = (double)sum / totalNodes;
	}

	return averages;
}

/* Recursively fills out the digits. */
void DigitsRec(std::vector<int>& result, int length, int diff, int num, int digit)
{
	if (length == 0)
	{
		result.push_back(num);
		return;
	}
	for (int d = 0; d <= 9; d++)
	{
		if (std::abs(d - digit) == diff)
			DigitsRec(result, length - 1, diff, 10 * num + d, d);
	}
}

/* 3 SEPT, 2022: NUMBERS WITH SAME CONSECUTIVE DIFFERENCES */
std::vector<int> NumsSameConsecDiff(int length, int diff)
{
	std::vector<int> result;
	for (int d = 1; d <= 9; d++)
		DigitsRec(result, length - 1, diff, d, d);

	return result;
}

void VertTravelDFS(TreeNode* root, std::map<int, std::map<int, std::multiset<int>>>& nodes, int x, int y)
{
	if (root)
	{
		nodes[x][y].insert(root->val);
		VertTravelDFS(root->left, nodes, x - 1, y + 1);
		VertTravelDFS(root->right, nodes, x + 1, y + 1);
	}
}

/* 4 SEPT, 2022: VERTICAL ORDER TRAVERSAL OF A BINARY TREE */
std::vector<std::vector<int>> VerticalTraversal(TreeNode* root)
{
	std::map<int, std::map<int, std::multiset<int>>> nodes;
	VertTravelDFS(root, nodes, 0, 0);

	std::vector<std::vector<int>> result;
	for (const auto& map : nodes)
	{
		std::vector<int> currCol;
		for (const auto mSet : map.second)
			currCol.insert(currCol.end(), mSet.second.begin(), mSet.second.end());

		result.push_back(currCol);
	}

	return result;
}

/* 5 SEPT, 2022: N-ARY TREE LEVEL ORDER TRAVERSAL */
std::vector<std::vector<int>> LevelOrder(Node* root)
{
	std::vector<std::vector<int>> result;
	if (!root)
		return result;

	std::queue<Node*> queue;
	queue.push(root);
	while (!queue.empty())
	{
		int size = queue.size();
		std::vector<int> level;
		for (int i = 0; i < size; i++)
		{
			Node* curr = queue.front();
			level.push_back(curr->val);
			queue.pop();

			for (const auto& child : curr->children)
				queue.push(child);
		}

		result.push_back(level);
	}

	return result;
}

/* 6 SEPT, 2022: BINARY TREE PRUNING */
TreeNode* PruneTree(TreeNode* root)
{
	if (!root)
		return nullptr;

	root->left = PruneTree(root->left);
	root->right = PruneTree(root->right);
	if (!root->left && !root->right && root->val == 0)
		return nullptr;

	return root;
}

void PreorderStr(TreeNode* root, std::string& str)
{
	if (!root)
		return;
	else
	{
		str += std::to_string(root->val);
		if (root->left)
		{
			str += '(';
			PreorderStr(root->left, str);
			str += ')';
		}
		else if (root->right)	// no left, but right exists
		{
			str += "()";
		}
		if (root->right)
		{
			str += '(';
			PreorderStr(root->right, str);
			str += ')';
		}
	}
}

/* 7 SEPT, 2022: CONSTRUCT STRING FROM BINARY TREE */
std::string TreeToStr(TreeNode* root)
{
	std::string result;
	PreorderStr(root, result);

	return result;
}

void Inorder(TreeNode* root, std::vector<int>& values)
{
	if (!root)
		return;
	else
	{
		Inorder(root->left, values);
		values.push_back(root->val);
		Inorder(root->right, values);
	}
}

/* 8 SEPT, 2022: BINARY TREE INORDER TRAVERSAL */
std::vector<int> InorderTraversal(TreeNode* root)
{
	std::vector<int> result;
	Inorder(root, result);
	return result;
}

/* 9 SEPT, 2022: THE NUMBER OF WEAK CHARACTERS IN THE GAME */
int NumberOfWeakCharacters(std::vector<std::vector<int>>& properties)
{
	const int size = properties.size();
	auto compare = [](const std::vector<int>& lhs, const std::vector<int>& rhs)
	{
		if (lhs[0] == rhs[0])
			return lhs[1] < rhs[1];
		return lhs[0] > rhs[0];
	};
	std::sort(properties.begin(), properties.end(), compare);


	int weaklings = 0;
	int minDef = INT_MIN;
	for (int i = 0; i < size; i++)
	{
		if (properties[i][1] < minDef)
			weaklings++;
		minDef = std::max(minDef, properties[i][1]);
	}

	return weaklings;
}

int MaxProfitUnlim(const std::vector<int>& prices)
{
	const int size = prices.size();
	int maxProfit = 0;
	for (int i = 1; i < size; i++)
		maxProfit += std::max(prices[i] - prices[i - 1], 0);

	return maxProfit;
}

/* 10 SEPT, 2022: BEST TIME TO BUY AND SELL STOCK IV */
int MaxProfit(const int k, const std::vector<int>& prices)
{
	const int size = prices.size();
	if (k >= size / 2)
		return MaxProfitUnlim(prices);

	std::vector<int> buys(k + 1, INT_MIN);
	std::vector<int> sells(k + 1, 0);
	for (int i = 0; i < size; i++)
	{
		int curr = prices[i];
		for (int j = 1; j <= k; j++)
		{
			buys[j] = std::max(buys[j], sells[j - 1] - curr);
			sells[j] = std::max(sells[j], buys[j] + curr);
		}
	}

	return sells[k];
}

/* 11 SEPT, 2022: MAXIMUM PERFORMANCE OF A TEAM */
int MaxPerformance(const int n, const std::vector<int>& speeds, const std::vector<int>& efficiencies, const int k)
{
	std::vector<std::pair<int, int>> engineers(n);
	for (int i = 0; i < n; i++)
		engineers[i] = std::make_pair(efficiencies[i], speeds[i]);
	std::sort(engineers.rbegin(), engineers.rend());

	std::priority_queue<int, std::vector<int>, std::greater<int>> pQueue;
	long sum = 0;
	long result = 0;
	for (const auto& [eff, spd] : engineers)
	{
		sum += spd;
		pQueue.push(spd);
		if (pQueue.size() > k)
		{
			sum -= pQueue.top();
			pQueue.pop();
		}

		result = std::max(result, sum * eff);
	}

	return result % (int)(1e9 + 7);
}

/* 12 SEPT, 2022: BAG OF TOKENS */
int BagOfTokensScore(std::vector<int>& tokens, int power)
{
	const int size = tokens.size();
	std::sort(tokens.begin(), tokens.end());

	int result = 0;
	int score = 0;
	int left = 0;
	int right = size - 1;
	while (left <= right)
	{
		if (power >= tokens[left])
		{
			power -= tokens[left];
			result = std::max(result, ++score);
			left++;
		}
		else if (score > 0)
		{
			power += tokens[right];
			score--;
			right--;
		}
		else
			break;
	}

	return result;
}

/* 13 SEPT, 2022: UTF-8 VALIDATION */
bool ValidUtf8(const std::vector<int>& data)
{
	const int bytes = data.size();

	// Special case, only 1 byte
	if (bytes == 1)
		return ((data[0] & 0b10000000) != 0b10000000);
	else if (bytes > 4)
		return false;

	int remaining = 0;
	for (int i = 0; i < bytes; i++)
	{
		int curr = data[i];
		if (remaining == 0)
		{
			if ((curr >> 3) == 0b11110)
				remaining = 3;
			else if ((curr >> 4) == 0b1110)
				remaining = 2;
			else if ((curr >> 5) == 0b110)
				remaining = 1;
		}
		else
		{
			// Continuation bytes, 2 msb must be 10
			if ((curr >> 6) != 0b10)
				return false;

			remaining--;
		}
	}

	return remaining == 0;
}

void PalindromeSearchDFS(TreeNode* node, int& result, int oddFreq)
{
	if (!node)
		return;

	oddFreq ^= (1 << node->val);
	if (!node->left && !node->right)
		if ((oddFreq & (oddFreq - 1)) == 0)
			result++;

	PalindromeSearchDFS(node->left, result, oddFreq);
	PalindromeSearchDFS(node->right, result, oddFreq);
}

/* 14 SEPT, 2022: PSEUDO-PALINDROMIC PATHS IN A BINARY TREE */
int PseudoPalindromicPaths(TreeNode* root)
{
	int result = 0;
	PalindromeSearchDFS(root, result, 0);
	return result;
}

/* 15 SEPT, 2022: FIND ORIGINAL ARRAY FROM DOUBLED ARRAY */
std::vector<int> FindOriginalArray(std::vector<int>& changed)
{
	const int size = changed.size();
	if (size % 2 != 0)
		return {};

	std::unordered_map<int, int> amountMap;
	for (int i = 0; i < size; i++)
		amountMap[changed[i]]++;

	std::sort(changed.begin(), changed.end());
	std::vector<int> original;
	for (int i = 0; i < size; i++)
	{
		int num = changed[i];
		if (amountMap[num] > 0)
		{
			// Isn't already a double of something else
			int numDouble = 2 * num;
			if (amountMap.find(numDouble) != amountMap.end() && amountMap[numDouble] > 0)
			{
				// Has a double somewhere later in the array
				original.push_back(num);
				amountMap[numDouble]--;
			}
			else
			{
				// Doesn't have a double
				return {};
			}

			amountMap[num]--;
		}
	}

	for (auto it = amountMap.begin(); it != amountMap.end(); it++)
		if (it->second > 0)
			return {};

	return original;
}

int ScoreMemo(std::vector<std::vector<int>>& dp, std::vector<int>& nums, std::vector<int>& multipliers, int mSize,
	int left, int right, int depth)
{
	if (depth == mSize)
		return 0;
	if (dp[depth][left] > INT_MIN)
		return dp[depth][left];

	int leftBranch = nums[left] * multipliers[depth] + ScoreMemo(dp, nums, multipliers, mSize, left + 1, right, depth + 1);
	int rightBranch = nums[right] * multipliers[depth] + ScoreMemo(dp, nums, multipliers, mSize, left, right - 1, depth + 1);
	dp[depth][left] = std::max(leftBranch, rightBranch);

	return dp[depth][left];
}

/* 16 SEPT, 2022: MAXIMUM SCORE FROM PERFORMING MULTIPLICATION OPERATIONS */
int MaximumScore(std::vector<int>& nums, std::vector<int>& multipliers)
{
	const int nSize = nums.size();
	const int mSize = multipliers.size();
	std::vector<std::vector<int>> dp(mSize + 1, std::vector<int>(mSize + 1, INT_MIN));

	return ScoreMemo(dp, nums, multipliers, mSize, 0, nSize - 1, 0);
}

/* 17 SEPT, 2022: PALINDROME PAIRS */
class PaliPairs
{
public:
	std::vector<std::vector<int>> PalindromePairs(const std::vector<std::string>& words)
	{
		const int numWords = words.size();
		for (int i = 0; i < numWords; i++)
			Add(words[i], i);

		std::vector<std::vector<int>> indices;
		for (int i = 0; i < numWords; i++)
		{
			const std::string str = words[i];
			const int length = str.length();

			TrieNode* node = &m_Root;
			for (int j = 0; j < length; j++)
			{
				if (!node)
					break;
				else
				{
					if (node->idx > -1 && node->idx != i && IsPalindrome(str, j, length - 1))
						indices.push_back({ i, node->idx });

					node = node->next[str[j] - 'a'];
				}
			}
			if (!node)
				continue;

			for (int j = 0; j < node->indices.size(); j++)
				if (i != j)
					indices.push_back({ i, j });
		}

		return indices;
	}

private:
	TrieNode m_Root;

	bool IsPalindrome(const std::string& string, int left, int right)
	{
		while (left < right && string[left] == string[right])
		{
			left++;
			right--;
		}

		return left >= right;;
	}

	void Add(const std::string& str, const int i)
	{
		const int length = str.length();
		TrieNode* node = &m_Root;
		for (int r = length - 1; r >= 0; r--)
		{
			if (IsPalindrome(str, 0, r))
				node->indices.push_back(i);

			int ch = str[r] - 'a';
			if (!node->next[ch])
				node->next[ch] = new TrieNode();

			node = node->next[ch];
		}
		node->idx = i;
		node->indices.push_back(i);
	}
};

/* 18 SEPT, 2022: TRAPPING RAIN WATER */
int Trap(const std::vector<int>& heights)
{
	const int size = heights.size();

	int trapped = 0;
	std::stack<int> indices;	// decreasing monotonic stack
	for (int i = 0; i < size; i++)
	{
		while (!indices.empty() && heights[i] > heights[indices.top()])
		{
			int currTop = indices.top();
			indices.pop();
			if (!indices.empty())
				trapped += (i - indices.top() - 1) * (std::min(heights[indices.top()], heights[i]) - heights[currTop]);

		}

		indices.push(i);
	}

	return trapped;
}

/* 19 SEPT, 2022: FIND DUPLICATE FILE IN SYSTEM */
std::vector<std::vector<std::string>> FindDuplicate(const std::vector<std::string>& paths)
{
	std::unordered_map<std::string, std::vector<std::string>> allFiles;
	std::vector<std::vector<std::string>> duplicates;

	for (const auto& path : paths)
	{
		std::stringstream ss(path);
		std::string str, dir, filename, content;
		ss >> dir;
		while (ss >> str)
		{
			auto it = str.find('(');
			filename = str.substr(0, it);
			content = str.substr(it);
			allFiles[content].push_back(dir + '/' + filename);
		}
	}

	for (const auto& [content, files] : allFiles)
	{
		const int numFiles = files.size();
		if (numFiles > 1)
			duplicates.push_back(files);
	}

	return duplicates;
}

/* 20 SEPT, 2022: MAXIMUM LENGTH OF REPEATED SUBARRAY */
int FindLength(const std::vector<int>& nums1, const std::vector<int>& nums2)
{
	const int size1 = nums1.size();
	const int size2 = nums2.size();
	std::vector<std::vector<int>> dp(size1 + 1, std::vector<int>(size2 + 1, 0));

	int maxLength = 0;
	for (int i = 1; i <= size1; i++)
	{
		for (int j = 1; j <= size2; j++)
		{
			if (nums1[i - 1] == nums2[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1] + 1;
				maxLength = std::max(maxLength, dp[i][j]);
			}
			else
				dp[i][j] = 0;
		}
	}

	return maxLength;
}

/* 21 SEPT, 2022: SUM OF EVEN NUMBERS AFTER QUERIES */
std::vector<int> SumEvenAfterQueries(std::vector<int>& nums, std::vector<std::vector<int>>& queries)
{
	const int size = nums.size();
	const int numQueries = queries.size();
	std::vector<int> result(numQueries);
	int rIdx = 0;

	// 0th even sum
	int currSum = 0;
	for (const int num : nums)
	{
		if (num % 2 == 0)
			currSum += num;
	}

	// Queries
	for (int i = 0; i < numQueries; ++i)
	{
		int val = queries[i][0];
		int idx = queries[i][1];

		currSum -= (nums[idx] % 2 == 0) ? nums[idx] : 0;
		currSum += ((nums[idx] + val) % 2 == 0) ? (nums[idx] + val) : 0;

		result[rIdx] = currSum;
		rIdx++;

		nums[idx] += val;
	}

	return result;
}

/* 22 SEPT, 2022: REVERSE WORDS IN A STRING III */
std::string ReverseWords(std::string& str)
{
	const int length = str.length();
	for (int i = 0; i < length; i++)
	{
		if (str[i] != ' ')
		{
			int j = i;
			while (j < length && str[j] != ' ')
				j++;

			std::reverse(str.begin() + i, str.begin() + j);
			i = j - 1;
		}
	}

	return str;
}

/* 23 SEPT, 2022: CONCATENATION OF CONSECUTIVE BINARY NUMBERS */
int ConcatenatedBinary(const int num)
{
	const int M = 1e9 + 7;

	long result = 0;
	for (int i = 1; i <= num; i++)
	{
		// Count length of current binary
		int length = 0;
		for (int j = i; j != 0; j >>= 1)
			length++;

		result = ((result << length) % M + i) % M;
	}

	return result;
}

void PreorderPathSum(std::vector<std::vector<int>>& paths, std::vector<int>& path, TreeNode* node, const int target)
{
	if (!node)
		return;

	path.push_back(node->val);
	if (!node->left && !node->right && (target - node->val) == 0)	// leaf and path sum is equal to target
		paths.push_back(path);

	PreorderPathSum(paths, path, node->left, target - node->val);
	PreorderPathSum(paths, path, node->right, target - node->val);
	path.pop_back();
}

/* 24 SEPT, 2022: PATH SUM II */
std::vector<std::vector<int>> PathSum2(TreeNode* root, const int targetSum)
{
	std::vector<std::vector<int>> paths;
	std::vector<int> path;
	PreorderPathSum(paths, path, root, targetSum);

	return paths;
}

/* 25 SEPT, 2022: DESIGN CIRCULAR QUEUE */
class CircularQueue
{
public:
	CircularQueue(const int k)
	{
		m_Capacity = k;

		m_Front = -1;
		m_Rear = -1;

		m_Buffer = new int[k];
	}

	~CircularQueue()
	{
		delete[] m_Buffer;
	}

	bool EnQueue(const int value)
	{
		if (!IsFull())
		{
			// First element
			if (m_Front == -1)
				m_Front = 0;

			m_Rear = (m_Rear + 1) % m_Capacity;
			m_Buffer[m_Rear] = value;
			return true;
		}
		return false;
	}

	bool DeQueue()
	{
		if (!IsEmpty())
		{
			// Last element
			if (m_Front == m_Rear)
			{
				m_Front = -1;
				m_Rear = -1;
			}
			else
				m_Front = (m_Front + 1) % m_Capacity;

			return true;
		}
		return false;
	}

	int Front() const
	{
		if (!IsEmpty())
			return m_Buffer[m_Front];
		return -1;
	}

	int Rear() const
	{
		if (!IsEmpty())
			return m_Buffer[m_Rear];
		return -1;
	}

	bool IsEmpty() const
	{
		return m_Front == -1;
	}

	bool IsFull() const
	{
		if (m_Front == 0 && m_Rear == m_Capacity - 1 || m_Front == m_Rear + 1)
			return true;
		return false;
	}

private:
	size_t m_Capacity;

	int m_Front;
	int m_Rear;

	int* m_Buffer;
};

int FindEq(int uf[], const int x)
{
	if (uf[x] == x)
		return x;

	uf[x] = FindEq(uf, uf[x]);
	return uf[x];
}

/* 26 SEPT, 2022: SATISFIABILITY OF EQUALITY EQUATIONS */
bool EquationsPossible(const std::vector<std::string>& equations)
{
	constexpr static int numLetters = 26;
	int uf[numLetters];

	for (int i = 0; i < numLetters; i++)
		uf[i] = i;

	for (const auto& eq : equations)
		if (eq[1] == '=')
			uf[FindEq(uf, eq[0] - 'a')] = FindEq(uf, eq[3] - 'a');

	for (const auto& eq : equations)
		if (eq[1] == '!' && FindEq(uf, eq[0] - 'a') == FindEq(uf, eq[3] - 'a'))
			return false;

	return true;
}

/* 27 SEPT, 2022: PUSH DOMINOES */
std::string PushDominoes(std::string& dominoes)
{
	const int size = dominoes.size();
	int left = 0;
	for (int right = 0; right < size; right++)
	{
		if (dominoes[right] == '.')
			continue;
		else if ((dominoes[left] == dominoes[right]) || (dominoes[left] == '.' && dominoes[right] == 'L'))
			for (int i = left; i < right; i++)
				dominoes[i] = dominoes[right];
		else if (dominoes[left] == 'L' && dominoes[right] == 'R')
		{
			// Nothing happens
		}
		else if (dominoes[left] == 'R' && dominoes[right] == 'L')
		{
			// Meet in middle
			int mid = (right - left - 1) / 2;
			for (int i = 1; i <= mid; i++)
			{
				dominoes[right - i] = 'L';
				dominoes[left + i] = 'R';
			}
		}

		left = right;
	}

	if (dominoes[left] == 'R')
		for (int i = left; i < size; i++)
			dominoes[i] = 'R';

	return dominoes;
}

/* 28 SEPT, 2022: REMOVE NTH NODE FROM END OF LIST */
ListNode* RemoveNthFromEnd(ListNode* head, const int n)
{
	ListNode* slow = head;
	ListNode* fast = head;

	int center = 1;
	while (fast && fast->next)
	{
		fast = fast->next->next;
		slow = slow->next;

		center++;
	}

	// Handle special cases
	int size;
	if (!head->next)				// one node in list				
		return nullptr;
	else if (slow && !slow->next)	// two nodes in list
	{
		if (n == 1)	// remove last nodes
		{
			head->next = nullptr;
		}
		else if (n == 2)	// remove head
		{
			ListNode* temp = head;
			head = head->next;
			delete temp;
		}
		return head;
	}
	else
		size = fast ? (2 * center - 1) : (2 * center - 2);

	ListNode* prev = nullptr;
	ListNode* curr = head;
	int index = 1;
	int target = size - n + 1;
	while (curr && index < target)
	{
		prev = curr;
		curr = curr->next;
		index++;
	}

	if (n == size)		// remove head        
		head = head->next;
	else if (n == 1)	// remove last node      
		prev->next = nullptr;
	else        		// remove from somewhere in the middle
	{
		prev->next = curr->next;
		curr = nullptr;
	}

	return head;
}

/* 29 SEPT, 2022: FIND K CLOSEST ELEMENTS */
std::vector<int> FindClosestElements(const std::vector<int>& arr, int k, int x)
{
	auto closer = [x](const int lhs, const int rhs)
	{
		if (std::abs(lhs - x) == std::abs(rhs - x))
			return lhs > rhs;
		else
			return std::abs(lhs - x) > std::abs(rhs - x);
	};
	std::priority_queue<int, std::vector<int>, decltype(closer)> pq(closer);
	std::vector<int> result(k);

	for (int num : arr)
		pq.push(num);

	int i = 0;
	while (!pq.empty() && k > 0)
	{
		result[i++] = pq.top();

		pq.pop();
		k--;
	}

	std::sort(result.begin(), result.end());
	return result;
}

/* 30 SEPT, 2022: THE SKYLINE PROBLEM */
std::vector<std::vector<int>> GetSkyline(const std::vector<std::vector<int>>& buildings)
{
	std::vector<std::vector<int>> result;
	std::priority_queue<std::pair<int, int>> pq;

	int i = 1;
	int curr = buildings[0][2];
	int start = buildings[0][0];
	int end = buildings[0][1];
	while (i < buildings.size()) 
	{
		if (end < buildings[i][0]) 
		{
			result.push_back({ start, curr });
			while (!pq.empty() && pq.top().second <= end) 
				pq.pop();
			if (!pq.empty()) 
			{
				start = end;
				curr = pq.top().first;
				end = pq.top().second;
				pq.pop();
			}
			else 
			{
				result.push_back({ end,0 });
				start = buildings[i][0];
				end = buildings[i][1];
				curr = buildings[i][2];
				i++;
			}
		}
		else 
		{
			if (buildings[i][2] == curr) 
				end = std::max(end, buildings[i][1]);
			else if (buildings[i][2] > curr) 
			{
				if (start != buildings[i][0])
					result.push_back({ start, curr });
				pq.push({ curr,end });
				start = buildings[i][0];
				curr = buildings[i][2];
				end = buildings[i][1];
			}
			else 
				pq.push({ buildings[i][2],buildings[i][1] });

			i++;
		}
	}
	while (!pq.empty()) 
	{
		while (!pq.empty() && pq.top().second <= end) 
			pq.pop();
		if (!pq.empty()) 
		{
			result.push_back({ start, curr });
			start = end;
			curr = pq.top().first;
			end = pq.top().second;
			pq.pop();
		}
	}
	result.push_back({ start, curr });
	result.push_back({ end, 0 });

	return result;
}

/* 1 OCT, 2022: Decode Ways */
int NumDecodings(const std::string& str)
{
	int length = str.length();
	return length;
}

/* 2 OCT, 2022: NUMBER OF DICE ROLLS WITH TARGET SUM */
int NumRollsToTarget(int dice, int faces, int target) 
{
	const int mod = 1e9 + 7;
	std::vector<int> dp1(target + 1);
	std::vector<int> dp2(target + 1);
	dp1[0] = 1;

	for (int i = 1; i <= dice; i++)
	{
		int prev = dp1[0];
		for (int j = 1; j <= target; j++)
		{
			dp2[j] = prev;
			prev = (prev + dp1[j]) % mod;
			if (j >= faces)
				prev = (prev - dp1[j - faces]) % mod;
		}

		std::swap(dp1, dp2);
		dp2[0] = 0;
	}

	return dp1[target];
}

/* 3 OCT, 2022: MINIMUM TIME TO MAKE ROPE COLORFUL */
int MinCost(std::string& colors, const std::vector<int>& costs)
{
	const int numColors = colors.size();
	int minTime = 0;
	bool colorful = false;

	// Brute-force
	while (!colorful)
	{
		colorful = true;

		// Check two consecutive colors, delete the one with less cost
		for (int i = 0; i < numColors - 1; i++)
		{
			// Skip deleted colors
			int j = i + 1;
			while (j < numColors && colors[j] == '0')
				j++;

			if (j < numColors)
			{
				if (colors[i] == colors[j])
				{
					colorful = false;
					if (costs[i] <= costs[j])
					{
						colors[i] = '0';
						minTime += costs[i];
					}
					else
					{
						colors[j] = '0';
						minTime += costs[j];
					}
				}
			}

			i = j - 1;
		}
	}

	return minTime;
}

void HasPathSumDFS(TreeNode* node, int target, bool& result)
{
	if (!node)
		return;

	if (!node->left && !node->right && (target - node->val) == 0)	// leaf
		result = true;
	
	HasPathSumDFS(node->left, target - node->val, result);
	HasPathSumDFS(node->right, target - node->val, result);
}

/* 4 OCT, 2022: PATH SUM */
bool HasPathSum(TreeNode* root, int targetSum)
{
	bool result = false;
	HasPathSumDFS(root, targetSum, result);
	return result;
}

/* 5 OCT, 2022: ADD ONE ROW TO TREE */
TreeNode* AddOneRow(TreeNode* root, int val, int depth)
{
	if (depth == 1)
		return new TreeNode(val, root, nullptr);
	else if (depth == 2)
	{
		root->left = new TreeNode(val, root->left, nullptr);
		root->right = new TreeNode(val, nullptr, root->right);
	}
	else
	{
		if (root->left)
			AddOneRow(root->left, val, depth - 1);
		if (root->right)
			AddOneRow(root->right, val, depth - 1);
	}

	return root;
}

/* 6 OCT, 2022: TIME BASED KEY-VALUE STORE */
class TimeMap
{
public:
	TimeMap()
	{

	}

	void Set(const std::string& key, const std::string& value, const int timestamp)
	{
		m_Map[key].insert({ timestamp, value });
	}

	std::string Get(const std::string& key, const int timestamp)
	{
		auto it = m_Map[key].upper_bound(timestamp);
		return (it == m_Map[key].begin()) ? "" : std::prev(it)->second;
	}

private:
	std::unordered_map<std::string, std::map<int, std::string>> m_Map;
};

/* 7 OCT, 2022: MY CALENDAR III */
class Calendar3
{
public:
	Calendar3()
	{
		m_Root = new Node();
	}

	int Book(const int start, const int end)
	{
		m_MaxValue = std::max(m_MaxValue, UpdateVal(m_Root, 0, LIMIT, start, end - 1));
		return m_MaxValue;
	}

private:
	struct Node
	{
		int val;
		int lazy;
		Node* left;
		Node* right;

		Node()
			: val(0), lazy(0), left(nullptr), right(nullptr) {}
	};

	const int LIMIT = 1e9 + 1;

	Node* m_Root;
	int m_MaxValue = 0;

	int UpdateVal(Node* node, int minRange, int maxRange, int low, int high)
	{
		if (!node->left)
			node->left = new Node();
		if (!node->right)
			node->right = new Node();

		if (node->lazy != 0)
		{
			node->val += node->lazy;
			if (low != high)
			{
				node->left->lazy += node->lazy;
				node->right->lazy += node->lazy;
			}
			node->lazy = 0;
		}

		if (maxRange < low || minRange > high)
			return 0;
		
		if (minRange >= low && maxRange <= high)
		{
			node->val++;
			if (low != high)
			{
				node->left->lazy++;
				node->right->lazy++;
			}

			return node->val;
		}

		int mid = minRange + (maxRange - minRange) / 2;

		UpdateVal(node->left, minRange, mid, low, high);
		UpdateVal(node->right, mid + 1, maxRange, low, high);

		node->val = std::max(node->left->val, node->right->val);
		return node->val;
	}
};

/* 8 OCT, 2022: 3SUM CLOSEST */
int ThreeSumClosest(std::vector<int>& nums, const int target)
{
	const int size = nums.size();
	int best = nums[0] + nums[1] + nums[2];
	if (size == 3)
		return best;

	std::sort(nums.begin(), nums.end());
	for (int i = 0; i < size - 2; i++)
	{
		int left = i + 1;
		int right = size - 1;
		while (left < right)
		{
			int sum = nums[i] + nums[left] + nums[right];
			if (abs(target - sum) < abs(target - best))
				best = sum;

			if (sum == target)
				return sum;
			else if (sum < target)
				left++;
			else
				right--;
		}
	}

	return best;
}

void FindTargetDFS(TreeNode* node, std::unordered_map<int, int>& map, const int k, bool& found)
{
	if (!node)
		return;

	int target = k - node->val;
	if (map.find(target) != map.end())
	{
		found = true;
		return;
	}
	else
		map[node->val] = node->val;

	FindTargetDFS(node->left, map, k, found);
	FindTargetDFS(node->right, map, k, found);
}

/* 9 OCT, 2022: TWO SUM IV - INPUT IS A BST */
bool FindTarget(TreeNode* root, const int k)
{
	bool result = false;
	std::unordered_map<int, int> map;
	FindTargetDFS(root, map, k, result);
	return result;
}

/* 10 OCT, 2022: BREAK A PALINDROME */
std::string BreakPalindrome(std::string& palindrome)
{
	int length = palindrome.length();
	if (length < 2)
		return "";

	for (int i = 0; i < length / 2; i++)
	{
		if (palindrome[i] > 'a')
		{
			palindrome[i] = 'a';
			return palindrome;
		}
	}

	palindrome[length - 1] = 'b';
	return palindrome;
}

/* 11 OCT, 2022: INCREASING TRIPLET SUBSEQUENCE */
bool IncreasingTriplet(const std::vector<int>& nums)
{
	int first = INT_MAX;
	int second = INT_MAX;
	for (const auto n : nums)
	{
		if (n <= first)
			first = n;
		else if (n <= second)
			second = n;
		else
			return true;
	}

	return false;
}

/* 12 OCT, 2022: LARGEST PERIMETER TRIANGLE */
int LargestPerimeter(std::vector<int>& nums)
{
	std::sort(nums.rbegin(), nums.rend());
	for (int i = 0; i < nums.size() - 2; i++)
		if (nums[i] < nums[i + 1] + nums[i + 2])
			return nums[i] + nums[i + 1] + nums[i + 2];

	return 0;
}

/* 13 OCT, 2022: DELETE NODE IN A LINKED LIST */
void DeleteNode(ListNode* node)
{
	ListNode* prev = node;
	ListNode* curr = node;
	while (curr->next)
	{
		curr->val = curr->next->val;
		
		prev = curr;
		curr = curr->next;
	}
	
	prev->next = nullptr;
}

/* 14 OCT, 2022: DELETE THE MIDDLE NODE OF A LINKED LIST */
ListNode* DeleteMiddle(ListNode* head)
{
	if (head && !head->next)
		return nullptr;

	ListNode* prev = nullptr;
	ListNode* slow = head;
	ListNode* fast = head;
	while (fast && fast->next)
	{
		prev = slow;
		slow = slow->next;
		fast = fast->next->next;
	}
	prev->next = slow->next;

	return head;
}

int CalcLength(const int length)
{
	if (length <= 1)		return length;
	else if (length < 10)	return 2;
	else if (length < 100)	return 3;
	else					return 4;
}

/* 15 OCT, 2022: STRING COMPRESSION II */
int GetLengthOfOptimalCompression(const std::string& str, const int k) 
{
	const int length = str.length();
	std::vector<std::vector<long>> dp(length + 1, std::vector<long>(k + 1, INT_MAX));
	dp[0][0] = 0;
	for (int i = 1; i <= length; i++)
	{
		for (int j = 0; j <= k; j++)
		{
			if (j > 0)
				dp[i][j] = dp[i - 1][j - 1];

			int removed = 0;
			int count = 0;
			for (int p = i; p > 0; p--)
			{
				if (str[p - 1] == str[i - 1])
					count++;
				else if (++removed > j)
					break;
				dp[i][j] = std::min(dp[i][j], dp[p - 1][j - removed] + CalcLength(count));
			}
		}
	}

	return dp[length][k];
}

/* 16 OCT, 2022: MINIMUM DIFFICULTY OF A JOB SCHEDULE */
int MinDifficulty(const std::vector<int>& difficulty, const int days)
{
	const int size = difficulty.size();
	if (days > size)
		return -1;

	std::vector<long> dp(size + 1, INT_MAX);
	dp[size] = 0;
	for (int d = 1; d <= days; d++)
	{
		for (int i = 0; i <= size - d; i++)
		{
			int currMax = 0;
			dp[i] = INT_MAX;
			for (int j = i; j <= size - d; j++)
			{
				currMax = std::max(currMax, difficulty[j]);
				dp[i] = std::min(dp[i], dp[j + 1] + currMax);
			}
		}
	}

	return dp[0];
}

/* 17 OCT, 2022: CHECK IF THE SENTENCE IS PANGRAM */
bool CheckIfPangram(const std::string& sentence)
{
	static constexpr int s_NumLetters = 26;
	const int length = sentence.length();
	if (length < s_NumLetters)
		return false;

	std::vector<int> letters(s_NumLetters, 0);
	int sum = 0;
	for (const char l : sentence)
	{
		if (letters[l - 'a'] == 0)
		{
			letters[l - 'a']++;
			sum++;
		}
	}

	return sum == s_NumLetters;
}

/* 18 OCT, 2022: COUNT AND SAY */
std::string CountAndSay(int n)
{
	std::string result = "1";
	int iter = 1;
	while (iter < n)
	{
		std::string tempResult = "";
		const int length = result.length();
		int idx = 0;
		while (idx < length)
		{
			int digitCount = 1;
			while (idx < length - 1 && result[idx] == result[idx + 1])
			{
				idx++;
				digitCount++;
			}

			tempResult += std::to_string(digitCount);
			tempResult += result[idx];
			idx++;
		}

		result = tempResult;
		iter++;
	}

	return result;
}

/* 19 OCT, 2022: TOP K FREQUENT WORDS */
std::vector<std::string> TopKFrequent(std::vector<std::string>& words, int k)
{
    // Store word frequency in a hash map
    const int size = words.size();
    std::unordered_map<std::string, int> wordFreq;
    for (const auto& word : words)
        wordFreq[word]++;

    // Initialize priority queue
    using FreqWordPair = std::pair<int, std::string>;
    auto compare = [](const FreqWordPair& p1, const FreqWordPair& p2)
    {
        if (p1.first == p2.first)
            return p1.second < p2.second;
        return p1.first > p2.first;
    };
    std::priority_queue<FreqWordPair, std::vector<FreqWordPair>, decltype(compare)> pQueue(compare);

    // Fill priority queue with k most common words
    int qSize = 0;
    for (const auto& [word, count] : wordFreq)
    {
        pQueue.push({ count, word });
        qSize++;
        if (qSize > k)
        {
            pQueue.pop();
            qSize--;
        }
    }

    // Fill result
    std::vector<std::string> result(k);
    k--;
    while (!pQueue.empty())
    {
        result[k] = pQueue.top().second;
        k--;
        pQueue.pop();
    }

    return result;
}

/* 20 OCT, 2022: INTEGER TO ROMAN */
std::string IntToRoman(int num)
{
	// Greedy algorithm
	std::string symbols[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
	int values[] = { 1000, 900, 500, 400,  100,  90,   50,  40,   10,  9,    5,   4,    1 };

	std::string result;
	int index = 0;
	while (num != 0)
	{
		while (values[index] <= num)
		{
			num -= values[index];
			result += symbols[index];
		}
		index++;
	}

	return result;
}

/* 21 OCT, 2022: CONTAINS DUPLICATE II */
bool ContainsNearbyDuplicate(const std::vector<int>& nums, const int k)
{
	std::unordered_map<int, int> hashMap;
	for (int i = 0; i < nums.size(); i++)
	{
		if (hashMap.find(nums[i]) != hashMap.end())
			if (abs(i - hashMap[nums[i]]) <= k)
				return true;

		hashMap[nums[i]] = i;
	}

	return false;
}

/* 22 OCT, 2022:  */

/* 23 OCT, 2022:  */

/* 24 OCT, 2022:  */

/* 25 OCT, 2022:  */

/* 26 OCT, 2022:  */

/* 27 OCT, 2022:  */

/* 28 OCT, 2022:  */

int main()
{
	const std::vector<int> nums = { 1, 2, 3, 1 };
	const int k = 3;
	std::cout << ContainsNearbyDuplicate(nums, k);
}
