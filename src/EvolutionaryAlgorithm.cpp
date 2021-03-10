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
        int numCollumns = newWeights[0].size();

        int mutationLine = rand() % numLines;
        int mutationCollumn = rand() % numCollumns;

        newWeights[mutationLine][mutationCollumn] = static_cast <double>(rand()) /( static_cast <double>(RAND_MAX/2)) - 1;

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

}
