#include "EvolutionaryAlgorithm.h"

void mutation(Game* individual)
{
    NeuralNetwork* net = individual->getNeuralNetwork();

    float mutate = getRandomFloat(0, 1);
    if (mutate < GameConfig::ProbabilityMutation)
    {
        int numLayers = net->getLayers().size();
        int chosenLayer = rand() % (numLayers - 1);

        Layer mutatedLayer = net->getLayerByIndex(chosenLayer);

        std::vector<std::vector<double>> newWeights = mutatedLayer.getWeights();
        int numLines = newWeights.size();
        int numColumns = newWeights[0].size();

        int mutationLine = rand() % numLines;
        int mutationcolumn = rand() % numColumns;

        newWeights[mutationLine][mutationcolumn] = static_cast <double>(rand()) /( static_cast <double>(RAND_MAX/2)) - 1;

        mutatedLayer.setWeights(newWeights);

        net->setLayer(mutatedLayer, chosenLayer);
    }
}

void selection(std::vector<Game>& v, int generation)
{
    int populationSize = v.size();

    //merge sort
    mergeSortIndividuals(v, 0, populationSize - 1);

    int numSurvived = static_cast<int>(std::ceil(GameConfig::NumGames * GameConfig::FractionSelection));
    int numCrossOver = static_cast<int>(std::ceil(GameConfig::NumGames * GameConfig::FractionCrossOver));
    /*
    //If all blocks were broken, there is the possibility that more than numSurvived players had survived
    while(numSurvived < populationSize)
    {
        if (v[numSurvived].getStillAlive())
            numSurvived++;
        else
            break;
    }
    */
    for(int i = numSurvived; i < GameConfig::NumGames; i++)
    {
        //create childs
        Game childGame((generation - 1) * GameConfig::NumGames + i);

        float crossing = getRandomFloat(0, 1);

        //crossOver
        if (numCrossOver > 1 && crossing < GameConfig::ProbabilityCrossOver)
        {
            int index1 = rand() % numCrossOver;
            int index2;
            do
            {
                index2 = rand() % numCrossOver;
            }
            while (index2 == index1);

           crossOver(childGame, *v[index1].getNeuralNetwork(), *v[index2].getNeuralNetwork());
        }

        mutation(&childGame);

        v[i] = childGame;
    }

}
void crossOver(Game& individual, NeuralNetwork& net1, NeuralNetwork& net2)
{
    std::vector<double> inputs;
    NeuralNetwork* individualNet = individual.getNeuralNetwork();

    int numLayers = net1.getLayers().size();
    for(int i = 0; i < numLayers - 1; i++) //it's necessary to discard the output layer
    {
        Layer newLayer = net1.getLayerByIndex(i);

        //generate child's weights
        std::vector<std::vector<double>> layer1Weights = net1.getLayerByIndex(i).getWeights();
        std::vector<std::vector<double>> layer2Weights = net2.getLayerByIndex(i).getWeights();

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
        individualNet->setLayer(newLayer, i);
    }
}

void mergeIndividuals(std::vector<Game>& v, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Game L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = v[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = v[(mid + 1) + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].getScore() > R[j].getScore())
        {
            v[k] = L[i];
            i++;
        }
        else
        {
            v[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        v[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        v[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortIndividuals(std::vector<Game>& v, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    //divide
    mergeSortIndividuals(v, left, mid);
    mergeSortIndividuals(v, mid + 1, right);

    //conquer
    mergeIndividuals(v, left, mid, right);
}
