#include "EvolutionaryAlgorithm.h"

EvolutionaryAlgorithm::EvolutionaryAlgorithm(GameConfig config, sf::RenderWindow* window):config(config), window(window){}

void EvolutionaryAlgorithm::mutation(NeuralNetwork& net)
{
    float mutate = getRandomFloat(0, 1);
    if (mutate < config.ProbabilityMutation)
    {
        int numLayers = net.getLayers().size();
        int chosenLayer = rand() % (numLayers - 1);

        Layer mutatedLayer = net.getLayerByIndex(chosenLayer);

        std::vector<std::vector<double>> newWeights = mutatedLayer.getWeights();
        int numLines = newWeights.size();
        int numColumns = newWeights[0].size();

        int mutationLine = rand() % numLines;
        int mutationcolumn = rand() % numColumns;

        newWeights[mutationLine][mutationcolumn] = static_cast <double>(rand()) /( static_cast <double>(RAND_MAX/2)) - 1;

        mutatedLayer.setWeights(newWeights);

        net.setLayer(mutatedLayer, chosenLayer);
    }
}

void EvolutionaryAlgorithm::selection(Game* v, int generation)
{
    //bubble sort
    for (int i = 0; i < config.NumGames - 1; i++)
    {
        for (int j = 0; j < config.NumGames - i - 1; j++)
        {
            int score1 = v[j].getScore();
            int score2 = v[j+1].getScore();

            if (score1 > score2)
            {
                int temp = score1;
                score1 = score2;
                score2 = temp;
            }
        }
    }

    int numSurvived = static_cast<int>(std::ceil(config.NumGames * config.FractionSelection));

    for(int i = numSurvived; i < config.NumGames; i++)
    {
        //create childs
        v[i].becomeNewGame(config, (generation - 1) * config.NumGames + i, window);

        float crossing = getRandomFloat(0, 1);

        //crossOver
        if (numSurvived > 1 && crossing < config.ProbabilityCrossOver)
        {
            int index1 = rand() % numSurvived;
            int index2;
            do
            {
                index2 = rand() % numSurvived;
            }
            while (index2 == index1);

            v[i].setNeuralNetwork(crossOver(*v[index1].getNeuralNetwork(), *v[index2].getNeuralNetwork()));
        }
    }

}
NeuralNetwork EvolutionaryAlgorithm::crossOver(NeuralNetwork& net1, NeuralNetwork& net2)
{
    std::vector<double> inputs;
    NeuralNetwork newNet(config.NumInputsNN, inputs, window);

    int numLayers = net1.getLayers().size();
    for(int i = 0; i < numLayers - 1; i++) //it's necessary to discard the output layer
    {
        Layer newLayer = net1.getLayerByIndex(i); //VERIFY IF ITS COPYING CORRECTLY

        //generate child's weights
        std::vector<std::vector<double>> layer1Weights = newLayer.getWeights();
        std::vector<std::vector<double>> layer2Weights = newLayer.getWeights();

        if (layer1Weights.size() != layer2Weights.size() || layer1Weights[0].size() != layer2Weights[0].size())
        {
            std::cout<<"Cross over error";
            exit(0);
        }

        int numLines = layer1Weights.size();
        int numColumns = layer1Weights[0].size();

        std::vector<std::vector<double>> newWeights = newLayer.getWeights();

        for (int i = 0; i < numLines; i++)
        {
            for (int j = 0; j < numColumns; j++)
            {
                int choice = rand() % 2;

                if (choice) //if weight comes from net2, else stays with weight of net1
                    newWeights[i][j] = layer2Weights[i][j];
            }
        }

        newLayer.setWeights(newWeights);

        //generate child's biases
        std::vector<double> layer1Biases = net1.getLayerByIndex(i).getBiases();
        std::vector<double> layer2Biases = net2.getLayerByIndex(i).getBiases();

        if (layer1Biases.size() != layer2Biases.size())
        {
            std::cout<<"Cross over error";
            exit(0);
        }

        int numBiases = layer1Biases.size();

        std::vector<double> newBiases = newLayer.getBiases();

        for (int i = 0; i < numBiases; i++)
        {
                int choice = rand() % 2;

                if (choice) //if weight comes from net1
                    newBiases[i] = layer2Biases[i];
        }

        newLayer.setBiases(newBiases);

        //store new layer
        newNet.setLayer(newLayer, i);
    }

    return newNet;
}
