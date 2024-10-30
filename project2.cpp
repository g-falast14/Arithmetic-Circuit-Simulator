#include <iostream>
#include <string>
using namespace std;

class Chip {
private:
    char chipType; // Type of the chip (A: Addition, S: Subtraction, etc.)
    string id; // Unique identifier for the chip
    Chip* input1; // Pointer to the first input chip
    Chip* input2; // Pointer to the second input chip (can be NULL)
    Chip* output; // Ptr to the output chip (is NULL for output chips)
    double inputValue; //for the input chip
    double result; //for the output chip
    Chip** allChips; // Array of pointers to all chips
    int allChipsSize; // Size of the allChips array
public:
    // Constructor
    Chip(char type, const string& id);
    // Getters and setters
    char getType() const; // Returns the chip type
    int getAllChipsSize() const; // Returns the size of the allChips array
    Chip** getAllChips() const; // Returns the allChips array
    void setInput1(Chip* inputChip); // Sets the first input chip
    void setInput2(Chip* inputChip); // second input chip (can be NULL)
    void setOutput(Chip* outputChip); // Sets the output chip (can be NULL)
    void setResult(double result); // Sets the result
    void setInputValue(double value); // set the input value
    void setAllChips(Chip** allChips); // set the allChips array
    void setAllChipsSize(int size); // set the size of the allChips array
    double getResult() const;
    Chip* getInput1();
    Chip* getInput2();
    string getID() const; // Returns the chip ID
    void compute(); // Performs the operation based on the chip type
    void display() const; // Displays the chip's information
    //****** OTHER METHODS AS NECESSARY ************//
    Chip* findChip(Chip** allChips, int numChips, const string& id); // find chip by ID
    void processCommands(Chip** allChips, int numChips, int numCommands); // method to handle commands
    void Attach(Chip* inputChip); // attach the input chip
    friend ostream& operator<<(ostream& os, const Chip& chip); // overriden << operator
    };

// Constructor
Chip::Chip(char chipType, const string& id){
    this->chipType = chipType;
    this->id = id;
    this->input1 = NULL;
    this->input2 = NULL;
    this->output = NULL;
    this->inputValue = 0;
}

// Overriden << operator
ostream& operator<<(ostream& os, const Chip& chip){
    os << "Chip ID: " << chip.id << endl;
    os << "Chip Type: " << chip.chipType << endl;
    os << "Input 1: " << chip.input1->getID() << endl;
    os << "Input 2: " << chip.input2->getID() << endl;
    return os;
}

// Getters
string Chip::getID() const {return this->id;}
char Chip::getType() const {return this->chipType;}
Chip* Chip::getInput1(){return this->input1;}
Chip* Chip::getInput2(){return this->input2;}
double Chip::getResult() const {return this->result;}
int Chip::getAllChipsSize() const {return this->allChipsSize;}
Chip** Chip::getAllChips() const {return this->allChips;}
// Setters
void Chip::setInput1(Chip* inputChip){this->input1 = inputChip;}
void Chip::setInput2(Chip* inputChip){this->input2 = inputChip;}
void Chip::setOutput(Chip* outputChip){this->output = outputChip;}
void Chip::setInputValue(double value){this->inputValue = value;}
void Chip::setResult(double result){this->result = result;}
void Chip::setAllChipsSize(int size){this->allChipsSize = size;}
void Chip::setAllChips(Chip** allChips){this->allChips = allChips;}


// Find chip by ID
Chip* Chip::findChip(Chip** allChips, int numChips, const string& id){
    for (int i = 0; i < numChips; i++){
        if (allChips[i]->getID() == id){
            return allChips[i];
        }
    }
    return NULL; // If not found
}

// Method for attatching input chips
void Chip::Attach(Chip* inputChip){
    // attach to input 1 if it is NULL; else, attach to input 2
    if (this->input1 == NULL){
        this->input1 = inputChip;
        inputChip->setOutput(this);
    } else {
        this->input2 = inputChip;
        inputChip->setOutput(this);
    }
}

// Process commands
void Chip::processCommands(Chip** allChips, int numChips, int numCommands) {
    
    for (int i = 0; i < numCommands; i++) {
        // Read the command
       char command; cin >> command;
        // Process specific type of command with switch statement
        switch(command) {
            case 'A': {
                string fromChipID, toChipID; cin >> fromChipID >> toChipID;
                // Find the chips
                Chip* fromChip = findChip(allChips, numChips, fromChipID);
                Chip* toChip = findChip(allChips, numChips, toChipID);
                // Attach the chips
                toChip->Attach(fromChip);
                break;
            }
            case 'I': {
                // Input command
                string chipID; cin >> chipID;
                double chipValue; cin >> chipValue;
                // Find the chip
                Chip* chip = findChip(allChips, numChips, chipID);
                // Set the input value
                chip->setInputValue(chipValue);
                break;
            }
            case 'O' : {
                // output commandi
                string chipID; cin >> chipID;
                // Find the chip
                Chip* chip = findChip(allChips, numChips, chipID);
                // Compute the output
                chip->compute();
                cout << "The output value from this circuit is " << chip->getResult() << endl;
                break;
            }
            default:
                break;
        }
    }
}

void testFindChip(Chip** allChips, int numChips){
    cout << "Testing findChip method" << endl;
    cout << "Finding chip with ID 'A1'..." << endl;
    Chip* chip = allChips[0]->findChip(allChips, numChips, "O50");
    if (chip != NULL){
        cout << "Chip found!" << endl;
    } else {
        cout << "Chip not found!" << endl;
    }
}

void testAttach(Chip* toChip, Chip* fromChip1, Chip* fromChip2){
    cout << "Testing attach method" << endl;
    cout << "Attaching fromChip1 to toChip..." << endl;
    toChip->Attach(fromChip1);
    cout << "Attaching fromChip2 to toChip..." << endl;
    toChip->Attach(fromChip2);
    cout << *toChip;
}

void Chip::compute() {
   
    // recursive call to compute the input chips
    // store results in chip's result attribute
    switch(this->getType()) {
        case 'A': // Call compute on both inputs if addition chip
            input1->compute();
            input2->compute();
            this->setResult(input1->getResult() + input2->getResult());
            break;
        case 'S': // Call compute on both inputs if subtraction chip
            input1->compute();
            input2->compute();
            this->setResult(input1->getResult() - input2->getResult());
            break;
        case 'M': // Call compute on both inputs if multiplication chip
            input1->compute();
            input2->compute();
            this->setResult(input1->getResult() * input2->getResult());
            break;
        case 'N': // Call compute on input 1 only if negation chip
            input1->compute();
            this->setResult((-1) * input1->getResult());
            break;
        case 'I': // Set the result to the input value if input chip
            this->result = this->inputValue;
            break;
        case 'D': // Call compute on both inputs if division chip
            input1->compute();
            input2->compute();
            this->setResult(input1->getResult() / input2->getResult());
            break;
        case 'O': // Call compute on input 1 only if output chip
            input1->compute();
            this->result = input1->getResult();
            break;
    }
}

void Chip::display() const{
    // display all input chips first
    for (int i = 0; i < this->getAllChipsSize(); i++) {
        if (this->allChips[i]->getType() == 'I') {
            cout << this->allChips[i]->getID() << ", " << "Output = " << this->allChips[i]->output->getID() << endl;
        }
    }
    // next, print computation chips
    for (int i = 0; i < this->getAllChipsSize(); i++) {
        if (this->allChips[i]->getType() != 'I' && this->allChips[i]->getType() != 'O') {
            cout << this->allChips[i]->getID() << ", ";
            if (this->allChips[i]->input1 != NULL){
                cout << "Input 1 = " << this->allChips[i]->input1->getID() << ", ";
            }
            if (this->allChips[i]->input2 != NULL){
                cout << "Input 2 = " << this->allChips[i]->input2->getID() << ", ";
            } else {
                cout << "Input 2 = None, ";
            }
            if (this->allChips[i]->output != NULL){
                cout << "Output = " << this->allChips[i]->output->getID() << endl;
            }
        }
    }
    // display output chip last
    for (int i = 0; i < this->getAllChipsSize(); i++) {
        if (this->allChips[i]->getType() == 'O') {
            cout << this->allChips[i]->getID() << ", " << "Input 1 = " << this->allChips[i]->input1->getID() << endl;
        }
    }
}

int main() {

    int numChips;
    Chip** allChips; // Array of pointers to all chips
    int allChipsIdx = 0; // Index of the next chip to be created
    int numCommands;

    cin >> numChips;

    cout << "Computation Starts" << endl;

    // Dynamically create an array of pointers to all chips
    allChips = new Chip*[numChips];

    for (int i = 0; i < numChips; i++) {
        // read the chip ID based on the first letter to determine its type
        string chipID; cin >> chipID;
        char chipType = chipID[0];
        // create the chip object and initialize it appropriately
        allChips[allChipsIdx++] = new Chip(chipType, chipID);
    }

    // store all chips array
    allChips[0]->setAllChips(allChips);

    // set size of allChips array
    allChips[0]->setAllChipsSize(numChips);

    // read the number of commands
    cin >> numCommands;
    
    // Process the commands by 
    allChips[0]->processCommands(allChips, numChips, numCommands);

    cout << "***** Showing the connections that were established" << endl;

    // display the connections that were established
    allChips[0]->display();

    int** test [2][2];

    return 0;
}

/*

Prompt: Help me debug this switch statement that is supposed to execute different commands based on the type of chip
Rationale: I chose this prompt because I was having issues with my switch statement that I was using to process the commands given to us in the code input. I was
trying to keep my work organized and keep a lot of the logic out of main so that the program would be a lot easier to read and a lot more modular.

Prompt: Genereate a test case for my findChip method
Rationale: On this project, I was working on breaking down my code into smaller parts and testing each part individually within smaller test cases. The findChip method
ensured that I could find a chip by its ID without having to have a large chunk of the code nested within my other methods. It helped to keep a lot of my code very organized.

Prompt: Generate a test case for my attach method
Rationale: Similar to the above prompt, I wanted to focus on making sure that my code was very modular and would work with various different types of input and methods.
The attach method was very important to connecting the pieces of my circuit, so I wanted to make sure that everything was working properly and had Copilot genereate
a tester method for verifying that my attach method worked properly.

Prompt: Why is my display method displaying the outputs in the incorrect order?
Rationale: I was having issues with my display method because it was not displaying my output chip in the correct order. The output chip was supposed to be last, before the inputs
and the computation chips. I asked Copilot to see if it could identify a reason why this might be happening.

Prompt: Help me generate a recursive display method
Rationale: I thought that a solution to the display method might be to make it recurisve instead and have it call display on inputs. I quickly found, though, that
while it did genereate a recursive display method, it was not the solution to my problem. It instead caused outputs to display in the incorrect order, although it did work as intended technically.

Prompt: Generate all getters and setters for my variables
Rationale: I had added a few different class attributes to my program. I wanted to make sure that I was correctly encapsulating them, so I had Copilot generate getters and setters
that I needed to make sure that I followed through with this process properly.

Prompt: Help me implement my overriden '<<' operator
Rationale: I was mostly familiar with the process of overloading operators, but I had an issue with my syntax. Copilot helped me figure out that I was slightly off, and
helped me to correctly implement this feature to aid with my display method.

Prompt: Why is the compiler telling me that 'cout is ambiguous'?
Rationale: This is an issue that I run into a lot when I am working with C++. Usually I find that it is because I am missing a bracket or a semicolon somewhere, or that
I forgot to put 'using namespace std;' at the top of my program. Copilot helped me identify the location that was causing the issue.

Prompt: In my compute method I highlighted, why can I not call compute on the input chips?
Rationale: Originally, when I started building compute, I was not correctly calling compute on my input chips, but instead on a getter method. I was unfamiliar with the syntax,
and had to have Copilot help me realize my mistake. Here, it did not necessarily help me implement anything, but was useful in identifiying an area where I had an issue.

Prompt: How can I set the outputs of my function to a specific chip?
Rationale: This was an issue that I mainly encountered when trying to display the connections in my circuit that were established. Copilot was able to recommend adding a small line
of code into my 'Attach' method that helped me to resolve this issue.

----------------------------------------------------------------------------------------------------------

Incremental Development:
What I have found with these projects is that when I can make my code a lot more modular and focus on breaking down larger methods into helper methods, it makes it 
a lot easier to both stay organized and outline what exactly it is that I am trying to accomplish with my code. The way I use Copilot is mainly to recommend to me how
I can break down a lot of my larger problems into smaller problems and keep a lot of clutter out of my main method. For example, in this project I created helper methods for
attaching chips (instead of keeping it in a messy for-loop in main). Calling 'attach' on two chips also really helps with the readability of your code so that somebody
can understand what it is that you are doing with two Chip objects. I also created a helper method called 'processCommands' that handles any of the commands that are given
in the input instructions for the project. I used Copilot to help me design the outline for a lot of my project, and it was also responsible for helping me create test
methods and test cases to test my work. I find that the best way to use LLMs for projects is to have it break down small pieces for you. If you give it the entire thing all
at once (not that I did this), it cannot comprehend the small details within your work that are necessary.

----------------------------------------------------------------------------------------------------------

Debugging and Testing Plan:

Specific Tests: For my project 2, I chose to implement smaller helper methods this time and break down a lot of the code into smaller problems to ensure that I would be
able to both debug my code more effectively and properly test everything. Within my code, there are three helper methods that represent each of the smaller portions of the project
that lead up to compute being called: Attach, processCommands, and findChip. findChip is more of a helper method than the other two, which I broke down into smaller pieces so I could
ensure that they were working properly. On top of creating these smaller methods to make my code more modular, I created several test cases and test methods for each of
my smaller broken down methods to make sure that each individual piece was working properly and as intended before I put them all together. This method of building my code
ensured that everything ran smoothly and that I wasn't just blindly guessing when I was trying to debug the larger compute method. I created custom test cases to ensure
that all connections and chips were being properly established before I ran compute. I also implemented an overriden '<<' operator to help with debugging. Not included in this file but in another
C++ file in this folder are methods that I had Copilot help me write to properly test my compute method with various types of inputs to ensure that it could handle different types of operations.

Issues and Resolutions:
Most of the issues that I encoutered revolves around the compute method. By making my code modular and implementing a couple of smaller methods to take care of smaller tasks,
I was able to ensure that everything in my code worked properly before I implemented compute. I determined that compute on its own was small enough that it could be 
completed without any extra helper methods, but I did have to make sure that everything leading up to it, including reading in and setting chip values and setting connections/outputs
was working properly. Specifically within my display method, I wanted to make sure I was avoiding any nested for loops so that everything would run in O(n) time. I believe that I
accomplished that within my project, as everything either runs in linear or O(1)/constant time. One of the main issues that I ran into was making sure that if I was calling
a class method, I was calling it on an object of the Chip class because otherwise it would throw me an error saying that the method was not a member of the class. I had very few issues handling any zero values
because of the tests that I implemented to make sure that my program could handle any weird edge cases.

Verification:
The main way that I verified that my classes/methods work correctly is by breaking down my code into smaller problems to solve. My goal was to make different methods for pieces of my project
so that it was not too small. I wanted to make sure that I could test each piece of my project individually before combinbing many of them together to make one larger program.
For each of the smaller methods that I broke my code down into, I created test methods and custom test cases to ensure that the operations they were performing matched my expected
output. For example, on my Attatch method I created custom input parameters, then ran the testAttach method which printed the connections of the circuits to ensure that everything
was working correctly before I moved on to compute. I created a test method for my findChip helper method to ensure that it was returning the proper chip to help with my processCommands method.
I put a lot of work in this project into making sure each individual component of my program worked before combining them all. This strategy and process made me really successful
and minimized the amount of debugging I had to do, especially when it came to compute because I was confident that everything prior was working properly.

*/
