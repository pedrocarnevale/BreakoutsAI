#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int numInputs, std::vector<double> inputs):numInputs(numInputs)
{
    this->inputs.resize(numInputs);

    //initialize inputs
    for(int i = 0; i < numInputs; i++)
        this->inputs[i] = inputs[i];

    std::string activationFunction = "tanh";

    Layer newLayer(numInputs, activationFunction);
    newLayer.insertInputs(this->inputs);
    this->layers.push_back(newLayer);

    int numNeuronsNewLayer = newLayer.getNumNeurons();
    includeNodesShapes(numNeuronsNewLayer, 0);
}

NeuralNetwork::NeuralNetwork()
{

}

void NeuralNetwork::addLayer(Layer* newLayer)
{
    layers[layers.size() - 1].linkLayer(newLayer);
    layers.push_back(*newLayer);
    int numNeuronsNewLayer = newLayer->getNumNeurons();

    //Update nodes shapes
    includeNodesShapes(numNeuronsNewLayer, (int)layers.size() - 1);
}

void NeuralNetwork::includeNodesShapes(int numNeuronsNewLayer, int layerIndex)
{
    std::vector<sf::CircleShape> nodesShapeLayer;

    int offsetY = (GameConfig::WindowHeight / 2 - (numNeuronsNewLayer - 1) * GameConfig::NodeDistance - 2 * GameConfig::RadiusNN + 40) / 2;

    for (int i = 0; i < numNeuronsNewLayer; i++)
    {
        sf::CircleShape circle;
        circle.setRadius(GameConfig::RadiusNN);
        circle.setPosition(GameConfig::WindowWidth / 2 + GameConfig::OffsetXNN + GameConfig::LayerDistance * layerIndex, offsetY + GameConfig::NodeDistance * i);
        circle.setFillColor(sf::Color::Red);
        nodesShapeLayer.push_back(circle);
    }

    nodesShape.push_back(nodesShapeLayer);
}

void NeuralNetwork::FeedFoward()
{
    int numLayers = (int)layers.size();
    for(int i = 0; i < numLayers - 1; i++)
    {
        std::vector<double> outputPreviewLayer = layers[i].getOutputs();
        layers[i+1].insertInputs(outputPreviewLayer);
    }
    this->outputs = layers[numLayers - 1].getOutputs();
}

Layer NeuralNetwork::getLayerByIndex(int index)
{
    if (index >= (int)layers.size())
    {
        std::cout<<"index error"<<std::endl;
        exit(0);
    }
    return layers[index];
}

std::vector<double> NeuralNetwork::getInputs()
{
    return this->inputs;
}

void NeuralNetwork::setLayer(Layer newLayer, int index)
{
    this->layers[index] = newLayer;
}

void NeuralNetwork::setInputs(std::vector<double> newInputs)
{
    this->inputs = newInputs;
    layers[0].insertInputs(this->inputs);
}

std::vector<Layer> NeuralNetwork::getLayers()
{
    return this->layers;
}

std::vector<double> NeuralNetwork::getOutputs()
{
    return this->outputs;
}

std::vector<std::vector<sf::CircleShape>> NeuralNetwork::getNodesShape()
{
    return this->nodesShape;
}
