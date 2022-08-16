<h1>C-Project-2022b, by Moshe Friedland</h1>
<h2>What it's all about?</h2>
<p>This project, is the final product of the Systems Programming Laboratory course in the Open University.
The task, was to create an assembler, that is supposed to do the 'assembling' phase of some assembly-code files
provided by the user. The task features an 'imaginary' computer, as detailed in the course booklet.
It was written in C language, by Moshe Friedland, for semester 2022b, and was compiled and tested on an Ubuntu system</p>

<h2>Project Structure</h2>
<p>The project is divided into 4 main parts:</p>
<p>1. Helpers directory - contains utills, that were useful in various places in the project and helped
with implementation</p>
<p>2. Types directory - contains all structs that were declared and used in the project, 
with their own helpful utils </p>
<p>3. Processing directory - contains all the implementations and logics behind the 3 main processing phases of this 
assembler 

The project also contains the main.c file, which is the main program uniting all mentioned above, some other 
compiling-necessary files, and an examples directory containing valid/invalid input files and their output 
</p>

<h2>How to use?</h2>
<p>First of all, make sure project is compiled. To do so, on an Ubuntu 16+ machine, 
run 'make' in the root directory. Then, a new 'compiler' file should be created.
Then, just run ./compiler, and as arguments to it just provide as many files paths as you want, without their extension,
separated by spaces.
For example:

./compiler examples/valid/valid_1 examples/valid/valid_2 examples/valid/valid_3</p>

After the run, the terminal should print some output. If there are any errors in any of the provided files,
it will print proper errors. If not, it will print that the file was compiled successfully.
Pay attention that there might be output files, in the same dir of the given files paths, depending on the compiling success.