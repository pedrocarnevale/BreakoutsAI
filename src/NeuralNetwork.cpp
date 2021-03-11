#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int numInputs, std::vector<double> inputs, sf::RenderWindow* window):numInputs(numInputs),window(window)
{
    this->inputs.resize(numInputs);

    //initialize inputs
    for(int i = 0; i < numInputs; i++)
        this->inputs[i] = inputs[i];

    std::string activationFunction = "tanh";

    Layer newLayer(numInputs, activationFunction);
    newLayer.insertInputs(this->inputs);
    this->layers.push_back(newLayer);

    //shapes information
    layerDistance = 220;
    nodeDistance = 80;
    radius = 25;
    offsetX = 200;

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

    int windowWidth = window->getSize().x;
    int windowHeight = window->getSize().y;
    int offsetY = (windowHeight / 2 - (numNeuronsNewLayer - 1) * nodeDistance - 2 * radius + 40) / 2;

    for (int i = 0; i < numNeuronsNewLayer; i++)
    {
        sf::CircleShape circle;
        circle.setRadius(radius);
        circle.setPosition(windowWidth / 2 + offsetX + layerDistance * layerIndex, offsetY + nodeDistance * i);
        circle.setFillColor(sf::Color::Red);
        nodesShapeLayer.push_back(circle);
    }

    nodesShape.push_back(nodesShapeLayer);
}

void NeuralNetwork::drawLinesShapes(int numNeuronsPreviousLayer, int numNeuronsNewLayer, int layerIndex)
{
    int windowWidth = window->getSize().x;
    int windowHeight = window->getSize().y;

    int offsetYPrevious = (windowHeight / 2 - (numNeuronsPreviousLayer - 1) * nodeDistance - 2 * radius + 40) / 2;
    int offsetYNew = (windowHeight / 2 - (numNeuronsNewLayer - 1) * nodeDistance - 2 * radius + 40) / 2;

    for (int i = 0; i < numNeuronsPreviousLayer; i++)
    {
        for (int j = 0; j < numNeuronsNewLayer; j++)
        {
            sf::Vertex line[] =
            {
               sf::Vertex(sf::Vector2f(windowWidth / 2 + offsetX + layerDistance * (layerIndex - 1) + radius, offsetYPrevious + nodeDistance * i + radius)),
                sf::Vertex(sf::Vector2f(windowWidth / 2 + offsetX + layerDistance * layerIndex + radius, offsetYNew + nodeDistance * j + radius))
            };

            //std::cout<<line[0].position.x<<" "<<line[0].position.y<<" "<<line[1].position.x<<" "<<line[1].position.y<<std::endl;
            window->draw(line, 2, sf::Lines);
        }
    }
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

void NeuralNetwork::draw()
{
    for (int i = 0; i < (int)nodesShape.size(); i++)
    {
        for (int j = 0; j < (int)nodesShape[i].size(); j++)
        {
            float transparency = 0;
            //last layer -> get output of softmax
            if (i == (int)nodesShape.size() - 1)
                 transparency = layers[i].getOutputs()[j];
            //middle layers -> get input
            else
                 transparency = layers[i].getInputs()[j];

            if (transparency < 0)
                nodesShape[i][j].setFillColor(sf::Color(255, 0, 0, static_cast<int>(255 * (-1) * transparency)));

            else
                nodesShape[i][j].setFillColor(sf::Color(0, 255, 0, static_cast<int>(255 * transparency)));

            window->draw(nodesShape[i][j]);
        }
    }

    for (int i = 1; i < (int)layers.size(); i++)
        drawLinesShapes(layers[i - 1].getNumNeurons(), layers[i].getNumNeurons(), i);
}
