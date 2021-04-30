# Breakout AI:
The main goal of this project is to create an agent that can play the Atari game Breakout. 

 The algorithm used to create the AI that plays the game is an Evolutionary Algorithm, based on the Darwin's evolution theory, using a Neural Network to output the movement of the agent. 
 
 The project uses the C++ library SFML to generate graphics 

**How the algorithm works?**

 1. A population is made of individuals. Each individual (that is a base + a ball in the breakout game) has a "brain", that is represented by a neural network. Based on the output of the neural network, the base moves to some direction (left, right or stationary).
  
 2. Firstly, the population is generated randomly, so each neural network has random weights and biases. Each time a ball hits a block, the individual's score is increased. When a ball is dropped, the individual "dies".
 
 3. When all individuals dies, the generation ends, and those that had best scores, pass to the next generation. Those that had bad scores are replaced by new individuals, which "brain" is made by the crossing over of 2 of the best neural networks. By doing that, the characteristics of the neural networks that reached high scores are passed to the next generation.

4. Through generations, the score of the individuals increases. When the population is enough evolved, the evolution can be finished by pressing the button "finish training". By doing that, the best individual plays breakout game, and the AI is created.

The project has the following classes:

 - **Ball**: Saves information about the ball state, such as the angle, the direction and the speed. Methods are used to update the ball and check collisions.
 
 - **Base**: Class used to store the base position and direction and to update the base.
 
 - **Environment**: Main class of the project, because has the game loop and is in charge of displaying on the screen the graphics and menu. This class saves information about the blocks, that are shared beetween all individuals, and statistics (number of individuals alive, training time, etc). 

 - **Game**: Joins the ball and base information, and represents an individual in the environment. Saves the individual's score, and has a Neural Network whose outputs determine the movement of the base. 

 - **Layer**: Class that saves the weights and biases values to the next layer, the activation function and the output of each node. A vector of layers is part of the NeuralNetwork class.
  
 - **NeuralNetwork**: Receives information about the individual and does a feed foward operation to determine the movement of the base. Also, this class has a method that returns the information about the neural network that will be printed on the screen.
 
 There are other files in the project:
 - **Evolutionary Algorithm**: Includes all functions that are related with the evolutionary algorithm.
 -- Mutation: Randomly picks a weight of the Neural Network and changes its value.
 -- CrossOver: Generates a new individual, mixing the weights and biases of two neural networks.
 -- Selection: Selects the best individuals to pass for the next generation (elitism), and creates all the other individuals, using crossing over.
 -- Merge sort functions: Used to sort individuals and determine which ones will pass to the next generation.
 
 - **utils**: Includes some tools, functions and parameters that will be used to run the project. 
 --There are two enum classes, one that points the game mode (training or testing), and the other that indicates a direction (left, right or stationary).
-- Namespace GameConfig has all parameters of the project, such as neural networks parameters, graphics information etc.
-- Also includes math functions, that scales the data that inputs the neural network, and some activation functions. 

![ReadMeImg](https://user-images.githubusercontent.com/42678222/116715813-6e4e6580-a9ad-11eb-998e-e341e3884ab3.png)
