#pragma once

#include "Layer.h"
#include "utils.h"

#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/window.hpp>

class NeuralNetwork
{
public:
    NeuralNetwork(int numInputs, std::vector<double> inputs);
    NeuralNetwork();
    void addLayer(Layer* newLayer);
    void includeNodesShapes(int numNeuronsNewLayer, int layerIndex);
    void FeedFoward();

    Layer getLayerByIndex(int index);
    void setLayer(Layer newLayer, int index);
    void setInputs(std::vector<double> newInputs);
    std::vector<double> getInputs();
    std::vector<Layer> getLayers();
    std::vector<double> getOutputs();
    std::vector<std::vector<sf::CircleShape>> getNodesShape();
private:
    int numInputs;
    std::vector<double> inputs;
    std::vector<Layer> layers;
    std::vector<double> outputs;
    std::vector<std::vector<sf::CircleShape>> nodesShape;
};

