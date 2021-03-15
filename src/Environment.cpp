#include "Environment.h"

Environment::Environment(sf::RenderWindow* window)
{
    //Construct individuals
    this->individualsAlive.resize(Config.NumGames);
    for(int i = 0; i < Config.NumGames; i++)
    {
        Game newGame(i);
        individualsAlive[i] = newGame;
    }

    //initialize matrix
    BlocksShape.resize(Config.NumBlocksLine);
    BlocksAvailable.resize(Config.NumBlocksLine);
    BlocksBounds.resize(Config.NumBlocksLine);

    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        BlocksShape[i].resize(Config.NumBlocksColumn);
        BlocksAvailable[i].resize(Config.NumBlocksColumn);
        BlocksBounds[i].resize(Config.NumBlocksColumn);
    }

    //Determine pripreties of the blocks
    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        for(int j = 0; j < Config.NumBlocksColumn; j++)
        {
            sf::RectangleShape Block;
            Block.setSize(sf::Vector2f{Config.BlockWidth,Config.BlockHeight});
            Block.setFillColor(sf::Color(80*(j%4),60*((j+2)%5),127*(j%3),255));
            Block.setPosition((Config.BlockWidth + Config.BlockMargin)*i + Config.BlockMargin, Config.BlockOffset+(Config.BlockHeight + Config.BlockMargin)*j);
            BlocksShape[i][j] = Block;
            BlocksAvailable[i][j] = Config.BlockStrength;
            BlocksBounds[i][j] = Block.getGlobalBounds();
        }
    }

    stillAlive.resize(Config.NumGames);
    for(int i = 0; i < Config.NumGames; i++)
        stillAlive[i] = true;

    this->window = window;
    this->NumIndividuals = Config.NumGames;

    if (!font.loadFromFile("Fonts/ComicNeue-Regular.ttf"))
    {
        std::cout<<"Loading text font error"<<std::endl;
        exit(0);
    }

    this->Generation = 1;
    this->record = 0;

    //NeuralNet title text
    textNeuralNetTitle.setFont(font);
    textNeuralNetTitle.setCharacterSize(30);
    textNeuralNetTitle.setPosition(Config.WindowWidth * 0.69, 0) ;
    textNeuralNetTitle.setString("Best player's brain: ");

    //Best score text
    textUpper.setFont(font);
    textUpper.setCharacterSize(25);
    textUpper.setPosition(Config.WindowWidth / 2, 0);

    //Generation text
    textLowerLeft.setFont(font);
    textLowerLeft.setCharacterSize(25);
    textLowerLeft.setPosition(Config.WindowWidth / 2, Config.WindowHeight / 2);

    //Generation text
    textLowerRight.setFont(font);
    textLowerRight.setCharacterSize(25);
    textLowerRight.setPosition(Config.WindowWidth * 3 / 4, Config.WindowHeight / 2);

    int offsetY = (Config.WindowHeight / 2 - (2 * Config.NodeDistance) - 2 * Config.Radius + 40) / 2;
    inputsText.resize(3);
    outputsText.resize(3);

    //Inputs text
    for (int i = 0; i < (int)inputsText.size(); i++)
    {
        inputsText[i].setFont(font);
        inputsText[i].setCharacterSize(15);
        inputsText[i].setPosition(Config.WindowWidth * 0.53, offsetY + Config.NodeDistance * i);
    }

    //Outputs text
    for (int i = 0; i < (int)outputsText.size(); i++)
    {
        outputsText[i].setFont(font);
        outputsText[i].setCharacterSize(15);
        outputsText[i].setPosition(Config.WindowWidth * 0.92, offsetY + Config.NodeDistance * i);
    }

    //Clock
    sf::Clock environmentClock;
    this->clock = environmentClock;

    //Initialize mean score vector
    meanScoreGeneration.push_back(0);

    //Initialize algorithm
    EvolutionaryAlgorithm algorithm;
}

void Environment::update()
{
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        window->clear();

        int numAlive = 0;

        for(int i = 0; i < Config.NumGames; i++)
        {
            if(stillAlive[i] == true)
            {
                numAlive++;

                //Draw games
                drawGame(individualsAlive[i]);

                //Update individual
                individualsAlive[i].update();

                //Check if there is a collision
                checkCollisions(i);

                //Check if game was lost
                checkGameOver(i);
            }
        }

        if (numAlive == 0)
            advanceGeneration();

        //Update time
        updateTime();

        //Update best player information
        Game bestPlayer = getBestPlayer();

        //Update Text
        updateText(bestPlayer);

        //Draw Menu
        drawMenu();

        //Draw texts
        drawTexts();

        //Draw Blocks
        drawBlocks();

        //Draw graphic
        drawGraphic();

        //Draw Network
        drawNeuralNetwork(bestPlayer.getNeuralNetwork());

        window->display();
    }
}

void Environment::advanceGeneration()
{
    this->Generation += 1;

    //Evolve
    algorithm.selection(individualsAlive, Generation);

    double sumGeneration = 0;

    this->NumIndividuals = Config.NumGames;

    //restart balls and bases
    for (int i = 0; i < (int)stillAlive.size(); i++)
    {
        sumGeneration += individualsAlive[i].getScore();
        stillAlive[i] = true;
        individualsAlive[i].getBreakoutsBall()->restart();
        individualsAlive[i].getBreakoutsBase()->restart();
        individualsAlive[i].setScore(0);
    }

    meanScoreGeneration.push_back(sumGeneration / static_cast<double>(stillAlive.size()));

    //restart blocks
    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        for(int j = 0; j < Config.NumBlocksColumn; j++)
        {
            BlocksShape[i][j].setFillColor(sf::Color(80*(j%4),60*((j+2)%5),127*(j%3),255));
            BlocksAvailable[i][j] = Config.BlockStrength;
        }
    }
}
void Environment::checkGameOver(int index)
{
    //Check if ball fell down
    Ball* BreakoutsBall = individualsAlive[index].getBreakoutsBall();
    sf::CircleShape BallShape = BreakoutsBall->getGameBall();

    if(BallShape.getPosition().y + 2*BallShape.getRadius() > window->getSize().y)
    {
        stillAlive[index] = false;
        NumIndividuals -= 1;
    }
}

void Environment::checkCollisions(int index)
{
    Ball* BreakoutsBall = individualsAlive[index].getBreakoutsBall();
    sf::FloatRect BallBounds = BreakoutsBall->getGameBall().getGlobalBounds();

    //Check collision with the blocks
    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        for(int j = 0; j < Config.NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j] > 0 && BallBounds.intersects(BlocksBounds[i][j]) &&
               BreakoutsBall->getGameBall().getPosition().y < ((Config.BlockHeight + Config.BlockMargin) * Config.NumBlocksColumn) + Config.BlockOffset)
            {
                //Increase 1 point if collided with block
                individualsAlive[index].setScore(individualsAlive[index].getScore() + 1);

                BlocksAvailable[i][j] -= 1;
                sf::Color BlockColor = BlocksShape[i][j].getFillColor();

                //Change block opacity
                int newOpacity = (BlocksAvailable[i][j] * 255 / Config.BlockStrength);

                if (BlocksAvailable[i][j] != 0)
                    BlocksShape[i][j].setFillColor(sf::Color(BlockColor.r, BlockColor.g, BlockColor.b, newOpacity));

                //Upper collision
                float upperDistance = std::abs(BlocksBounds[i][j].top - (BallBounds.top + BallBounds.height));

                //Bottom collision
                float bottomDistance = std::abs((BlocksBounds[i][j].top + BlocksBounds[i][j].height) - BallBounds.top);

                //Right collision
                float rightDistance = std::abs((BlocksBounds[i][j].left + BlocksBounds[i][j].width) - BallBounds.left);

                //Left collision
                float leftDistance = std::abs(BlocksBounds[i][j].left - (BallBounds.left + BallBounds.width));

                //Determine which distance is the smallest to check the type of collision
                float minDistance = std::min(std::min(upperDistance, bottomDistance), std::min(rightDistance, leftDistance));

                //Vertical collision
                if(minDistance == upperDistance || minDistance == bottomDistance)
                {
                    float x = BreakoutsBall->getVel().x;
                    float y = (-1)*BreakoutsBall->getVel().y;
                    BreakoutsBall->setVel(sf::Vector2f{x, y});
                }

                //Horizontal collision
                else if(minDistance == leftDistance || minDistance == rightDistance)
                {
                    float x = (-1)*BreakoutsBall->getVel().x;
                    float y =  BreakoutsBall->getVel().y;
                    BreakoutsBall->setVel(sf::Vector2f{x, y});

                }
            }
        }
    }
}

void Environment::drawGame(Game& individual)
{
    window->draw(individual.getBreakoutsBall()->getGameBall());
    window->draw(individual.getBreakoutsBase()->getBaseShape());
}

void Environment::drawNeuralNetwork(NeuralNetwork* net)
{

    std::vector<std::vector<sf::CircleShape>> nodesShape = net->getNodesShape();
    std::vector<Layer> layers = net->getLayers();

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
        drawLinesShapesNN(layers[i - 1].getNumNeurons(), layers[i].getNumNeurons(), i);
}

void Environment::drawLinesShapesNN(int numNeuronsPreviousLayer, int numNeuronsNewLayer, int layerIndex)
{
    int radius = Config.RadiusNN;
    int nodeDistance = Config.NodeDistance;
    int offsetX = Config.OffsetXNN;
    int layerDistance = Config.LayerDistance;

    int offsetYPrevious = (Config.WindowHeight / 2 - (numNeuronsPreviousLayer - 1) * nodeDistance - 2 * radius + 40) / 2;
    int offsetYNew = (Config.WindowHeight / 2 - (numNeuronsNewLayer - 1) * nodeDistance - 2 * radius + 40) / 2;

    for (int i = 0; i < numNeuronsPreviousLayer; i++)
    {
        for (int j = 0; j < numNeuronsNewLayer; j++)
        {
            sf::Vertex line[] =
            {
               sf::Vertex(sf::Vector2f(Config.WindowWidth / 2 + offsetX + layerDistance * (layerIndex - 1) + radius, offsetYPrevious + nodeDistance * i + radius)),
                sf::Vertex(sf::Vector2f(Config.WindowWidth / 2 + offsetX + layerDistance * layerIndex + radius, offsetYNew + nodeDistance * j + radius))
            };

            window->draw(line, 2, sf::Lines);
        }
    }
}

void Environment::drawMenu()
{
    //Draw lines
    sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, 0)), sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y))};
    sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y/2)), sf::Vertex(sf::Vector2f(window->getSize().x, window->getSize().y/2))};

    window->draw(line1, 2, sf::Lines);
    window->draw(line2, 2, sf::Lines);

}

void Environment::drawBlocks()
{
    for(int i = 0; i < Config.NumBlocksLine; i++)
    {
        for(int j = 0; j < Config.NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j] > 0)
                window->draw(BlocksShape[i][j]);
        }
    }
}

void Environment::drawTexts()
{
    //Draw neural network title
    window->draw(textNeuralNetTitle);

    //Draw neural network inputs
    for(int i = 0; i < (int)inputsText.size(); i++)
        window->draw(inputsText[i]);

    //Draw neural network outputs
    for(int i = 0; i < (int)outputsText.size(); i++)
        window->draw(outputsText[i]);

    //Draw LowerLeft text
    std::string stringLowerLeft;
    stringLowerLeft += " Generation: " + std::to_string(Generation) + '\n';
    stringLowerLeft += " Number of individuals alive: " + std::to_string(NumIndividuals) + '\n';

    textLowerLeft.setString(stringLowerLeft);
    window->draw(textLowerLeft);

    //Draw LowerRight text
    std::string stringLowerRight;
    stringLowerRight += " All time record: " + std::to_string(record) + '\n';
    stringLowerRight += " Training time: " + updateTime() + '\n';

    textLowerRight.setString(stringLowerRight);
    window->draw(textLowerRight);

    //Draw upper text
    window->draw(textUpper);

}

void Environment::drawGraphic()
{
    sf::Text scoreText;
    sf::Text generationText;
    sf::Text graphicTitleText;

    //update generation's mean score
    double sumGeneration = 0;

    for (int i = 0; i < (int)stillAlive.size(); i++)
        sumGeneration += individualsAlive[i].getScore();

    meanScoreGeneration[Generation - 1] = sumGeneration / static_cast<double>(stillAlive.size());

    //discover max element of all means
    double maxMean = 0;
    for (int i = 0; i < (int)meanScoreGeneration.size(); i++)
        maxMean = maxMean < meanScoreGeneration[i] ? meanScoreGeneration[i] : maxMean;

    //draw graphic
    int sizeVector = (int)meanScoreGeneration.size();
    int maxBarWidth = (Config.WindowWidth / 2 - 100);

    for (int i = 0; i < sizeVector; i++)
    {
        sf::RectangleShape bar;
        float barWidth = std::min(static_cast<float>(maxBarWidth) / static_cast<float>(sizeVector), float(100));
        float barHeight = 250 * meanScoreGeneration[i] / maxMean;
        float barX = Config.WindowWidth * 3 / 4 + ((i + 1) - static_cast<double>(Generation) / 2) * barWidth;
        float barY = Config.WindowHeight - 50;

        bar.setPosition(barX, barY);
        bar.setSize(sf::Vector2f((-1) * barWidth, (-1) * barHeight));

        window->draw(bar);

        //draw bars separation
        sf::Vertex line[] = {sf::Vertex(sf::Vector2f(barX, barY - barHeight) , sf::Color::Black), sf::Vertex(sf::Vector2f(barX, barY), sf::Color::Black)};
        window->draw(line, 2, sf::Lines);

        //draw generation text
        generationText.setFont(font);
        generationText.setCharacterSize(20);
        generationText.setPosition(barX - barWidth / 2, barY + 15);
        generationText.setString(std::to_string(i + 1));

        window->draw(generationText);

        //draw score text
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setPosition(barX - barWidth / 2 - 15, barY - barHeight - 30) ;
        scoreText.setString(std::to_string(meanScoreGeneration[i]).substr(0,4));

        window->draw(scoreText);
    }

    graphicTitleText.setFont(font);
    graphicTitleText.setCharacterSize(35);
    graphicTitleText.setPosition(Config.WindowWidth * 0.63, Config.WindowHeight * 0.57) ;
    graphicTitleText.setString("Mean generation's score");

    window->draw(graphicTitleText);
}

std::string Environment::updateTime()
{
    int time = static_cast<int>(clock.getElapsedTime().asSeconds());
    std::string timePassed = "";

    //hours
    timePassed += std::to_string(time / 3600) + "h ";

    //minutes
    timePassed += std::to_string(time / 60) + "m ";

    //seconds
    timePassed += std::to_string(time % 60) + "s";

    return timePassed;
}

void Environment::updateText(Game& bestPlayer)
{
    int maxScore = bestPlayer.getScore();
    int id = bestPlayer.getId();

    //Best player text data
    std::string stringtextUpper = " Best player: " + std::to_string(id) + '\n';
    stringtextUpper += " Best player's score: " + std::to_string(maxScore) + '\n';
    textUpper.setString(stringtextUpper);

    //Best player input text
    sf::CircleShape ball = bestPlayer.getBreakoutsBall()->getGameBall();
    sf::RectangleShape base = bestPlayer.getBreakoutsBase()->getBaseShape();
    std::vector<double> inputLayer = bestPlayer.getNeuralNetwork()->getLayers()[0].getInputs();

    inputsText[0].setString("Ball on X axis: " + std::to_string(static_cast<int>(ball.getPosition().x + ball.getRadius())));
    inputsText[1].setString("Ball on Y axis: " + std::to_string(static_cast<int>(ball.getPosition().y + ball.getRadius())));
    inputsText[2].setString("Base on X axis: " + std::to_string(static_cast<int>(base.getPosition().x + base.getSize().x)));

    //Best player output text
    std::vector<Layer> layers = bestPlayer.getNeuralNetwork()->getLayers();
    std::vector<double> outputLayer = layers[layers.size() - 1].getOutputs();

    outputsText[0].setString("Left: " + std::to_string(ceilf(outputLayer[0] * 100) / 100).substr(0,4));
    outputsText[1].setString("Stationary: " + std::to_string(ceilf(outputLayer[1] * 100) / 100).substr(0,4));
    outputsText[2].setString("Right: " + std::to_string(ceilf(outputLayer[2] * 100) / 100).substr(0,4));
}

Game Environment::getBestPlayer()
{
    int maxScore = 0;
    int indexBestGame = 0;

    for(int i = 0; i < Config.NumGames; i++)
    {
        if (maxScore < individualsAlive[i].getScore())
        {
            indexBestGame = i;
            maxScore = individualsAlive[i].getScore();
        }
    }

    //update record
    if (maxScore > record)
        record = maxScore;

    return individualsAlive[indexBestGame];
}

std::vector<Game> Environment::getIndividualsAlive()
{
    return this->individualsAlive;
}

std::vector<bool> Environment::getStillAlive()
{
    return this->stillAlive;
}
