# TAI_P2

In order to compile the programs, we have to go to src folder and execute the three commands below:
(note that the programs are already compiled in bin folder)

### Command to compile the program lang.cpp

g++ -o lang lang.cpp

### Command to compile the program findlang.cpp

g++ -o findlang findlang.cpp

### Command to compile the program locatelang.cpp

g++ -o locatelang locatelang.cpp

Then, to run the programs we have to go to bin folder and execute the three commands below:

### Command to run the program lang

/lang filename_ri filename_t word_length alpha min_prob max_num_cons_fails literal_cost
 
Example: ./lang portuguese.txt por.txt 3 0.5 0.4 3 8.0

(note that filename_ri and filename_t should be inside bin folder or you should replace them for the path where the files are stored)

 
### Command to run the program findlang

./findlang ri_directory filename_t word_length alpha min_prob max_num_cons_fails literal_cost

Example: ./findlang /home/renatito/Desktop/TAI/praticas/TAI_P2/Group_4/models ara.txt 3 0.5 0.4 3 8.0

(note that filename_t should be inside bin folder or you should replace it for the path where the file is stored)


### Command to run the program locatelang

./findlang ri_directory filename_t word_length alpha min_prob max_num_cons_fails literal_cost

Example: ./locatelang /home/renatito/Desktop/TAI/praticas/TAI_P2/Group_4/models example.txt 3 0.5 0.4 3 8.0

(note that filename_t should be inside bin folder or you should replace it for the path where the file is stored)