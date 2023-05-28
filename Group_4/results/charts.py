import matplotlib.pyplot as plt

# Data for Word Length vs. Num Bits
word_length = [2, 3, 4, 5, 6]
num_bits_word = [7888.84, 13416.4, 26666.7, 45593.8, 66279]

# Data for Alpha vs. Num Bits
alpha = [0.3, 0.4, 0.5, 0.6, 0.7]
num_bits_alpha = [28751.3, 27505.6, 26666.7, 26064.5, 25612.8]

# Data for Min Probability vs. Num Bits
min_prob = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]
num_bits_min_prob = [27296.1, 27540.9, 26736.2, 26666.7, 26632.7, 24126]

# Data for Max Num Cons Fails vs. Num Bits
max_num_cons_fails = [1, 2, 3, 4, 5]
num_bits_cons_fails = [22841.2, 26584.9, 26657.9, 26666.7, 26667.6]

# Data for Literal Cost vs. Num Bits
literal_cost = [5, 6, 7, 8, 9, 10]
num_bits_literal = [24914.7, 25498.7, 26082.7, 26666.7, 27250.7, 27834.7]

# Create a single plot
plt.figure(figsize=(12, 8))

# Word Length vs. Num Bits
plt.subplot(231)
plt.bar(word_length, num_bits_word)
plt.xlabel('Word Length')
plt.ylabel('Num Bits')
plt.title('Fig. 1 - Word Length vs. Num Bits')

# Alpha vs. Num Bits
plt.subplot(232)
plt.plot(alpha, num_bits_alpha)
plt.xlabel('Alpha')
plt.ylabel('Num Bits')
plt.title('Fig. 2 - Alpha vs. Num Bits')

# Min Probability vs. Num Bits
plt.subplot(233)
plt.plot(min_prob, num_bits_min_prob)
plt.xlabel('Min Probability')
plt.ylabel('Num Bits')
plt.title('Fig. 3 - Min Probability vs. Num Bits')

# Max Num Cons Fails vs. Num Bits
plt.subplot(234)
plt.plot(max_num_cons_fails, num_bits_cons_fails)
plt.xlabel('Max Num Cons Fails')
plt.ylabel('Num Bits')
plt.title('Fig. 4 - Max Num Cons Fails vs. Num Bits')

# Literal Cost vs. Num Bits
plt.subplot(235)
plt.plot(literal_cost, num_bits_literal)
plt.xlabel('Literal Cost')
plt.ylabel('Num Bits')
plt.title('Fig. 5 - Literal Cost vs. Num Bits')

# Adjust subplot spacing
plt.tight_layout()

# Show the plot
plt.show()