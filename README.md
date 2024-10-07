# Wordle Solver
This repository contains a C program that solves the popular word game Wordle. The solver works by computing the word with the highest expected entropy after each guess and refines the possible list of words based on user feedback (Green, Yellow, or Gray), similar to how the actual game is played.

## Optimal Guess using Entropy 
The program solves Wordle by selecting the word with the highest expected entropy to minimize the remaining possible word set after each guess. Here's a breakdown of how it approaches the problem:

### Entropy Calculation: 
Entropy is a measure of uncertainty or information. In this context, it helps estimate how informative a guess will be in reducing the set of possible solutions.

### Simulating Feedback: 
For each word in the remaining possible word list, the program simulates how the feedback (Green, Yellow, Gray) would look if that word was the correct solution. It calculates how often each feedback pattern would occur if a particular word was guessed.

### Expected Entropy: The program calculates the expected entropy for each word by:
1. Counting how many times each feedback pattern would occur for a guess.
2. Using these counts to compute the probability of each pattern and, from that, the entropy.

### Choosing the Best Guess: 
The word with the highest expected entropy is the one that is expected to provide the most information, as it leads to the largest reduction in the number of possible words. By choosing words this way, the program maximizes its chances of quickly narrowing down the word set and solving the puzzle efficiently.

This approach ensures that each guess is as informative as possible, minimizing the number of remaining possibilities after every round.
