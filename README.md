# FireWall-Projects
Illumio Challenge 

# ILUMIO CODING CHALLENGE:

Name: Anh Nguyen
Firewall Packet Classification Project

# Testing:

I was too concentrated on the thinking and coding process and then figured out I don't have time left for testing.

# Implementation Details:

When I consider this problem, I noticed that (direction , protocol) only take up to 4 combinations. Therefore, I decided to use 4 similar data structure to maintain different combinations of the two. I also convert IP address string into 32 bit integer to simplify the problem. Moving forward, I figured out a very interesting geometric representation of the problem. If we select a range of port and a range of IP, we can easily see that they together form a rectangular shape. The packet coming in therefore can be regarded as a query point in 2D. We can therefore formalize the problem as follow: Given n rectangles in 2D and a query point q, return true if we can find at least one rectangle that contains q. I then noticed that this problem is a generalization of the famous 1D stabbing query problem that can be solve efficiently using Segment Tree. After some thinking, I decided to use a Segment Tree data structure for this problem as well, the main Segment Tree will maintain horizontal segments of each rectangle. Moreover, in each subtree of the main Segment Tree, I also stored a pointer to another Segment Tree that maintain vertical segments of each rectangle. Note that we only need to store all vertical segments in the x-range that the current node maintains.The space complexity turns out to be O(nlogn^2) with n is the number of network rules. In my implementation, due to time constraint, I don't use coordinate compression to compress the IP range from 32 bit integer down to [1 , n]. The space complexity mentioned above can be easily achieved with coordinate compression. Query time for a package coming in turns out to be O(logn^2)

# More Optimization:

I would want to use somewhat easier approach. I have thought of some data structure like Bloom Filter, Radix Trie but those data structure have following issues:
Bloom Filter suffers from false positive issue and we need the bit array size to be quite big to handle FP.
Radix Trie suffers from very high space complexity also.
As I mentioned, if given more time, I will develop a coordinate compression methods to compress the IP range down to [1 , n]

# Teams:

I'm specifically interested in the Platform and Policy Team
