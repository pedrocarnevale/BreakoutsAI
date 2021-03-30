#include "Environment.h"

Environment::Environment(sf::RenderWindow* window)
{
    //Construct individuals
    this->individualsAlive.resize(GameConfig::NumGames);
    for(int i = 0; i < GameConfig::NumGames; i++)
    {
        Game newGame(i);
        individualsAlive[i] = newGame;
    }

    //initialize matrix
    BlocksShape.resize(GameConfig::NumBlocksLine);
    BlocksAvailable.resize(GameConfig::NumBlocksLine);
    BlocksBounds.resize(GameConfig::NumBlocksLine);

    for(int i = 0; i < GameConfig::NumBlocksLine; i++)
    {
        BlocksShape[i].resize(GameConfig::NumBlocksColumn);
        BlocksAvailable[i].resize(GameConfig::NumBlocksColumn);
        BlocksBounds[i].resize(GameConfig::NumBlocksColumn);
    }

    //Determine pripreties of the blocks
    for(int i = 0; i < GameConfig::NumBlocksLine; i++)
    {
        for(int j = 0; j < GameConfig::NumBlocksColumn; j++)
        {
            sf::RectangleShape Block;
            Block.setSize(sf::Vector2f{GameConfig::BlockWidth,GameConfig::BlockHeight});
            Block.setFillColor(sf::Color(sf::Color(40*(j%7),127*(j%3),60*((j+2)%5),255)));
            Block.setPosition((GameConfig::BlockWidth + GameConfig::BlockMargin)*i + GameConfig::BlockMargin, GameConfig::BlockOffset+(GameConfig::BlockHeight + GameConfig::BlockMargin)*j);
            BlocksShape[i][j] = Block;
            BlocksAvailable[i][j] = GameConfig::BlockStrength;
            BlocksBounds[i][j] = Block.getGlobalBounds();
        }
    }

    this->window = window;
    this->NumIndividuals = GameConfig::NumGames;

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
    /*std::string textString = "Best player's brain: ";*/
    std::string textString = "Cérebro do melhor indivíduo: ";
    textNeuralNetTitle.setString(textString);
    textNeuralNetTitle.setPosition(GameConfig::WindowWidth * 0.75 - 5 * (int)textString.size(), 0) ;


    //Best score text
    textUpper.setFont(font);
    textUpper.setCharacterSize(25);
    textUpper.setPosition(GameConfig::WindowWidth / 2, 0);

    //Generation text
    textLowerLeft.setFont(font);
    /*textLowerLeft.setCharacterSize(25);*/
    textLowerLeft.setCharacterSize(21);
    textLowerLeft.setPosition(GameConfig::WindowWidth / 2, GameConfig::WindowHeight / 2);

    //Generation text
    textLowerRight.setFont(font);
    /*textLowerRight.setCharacterSize(25);*/
    textLowerRight.setCharacterSize(21);
    textLowerRight.setPosition(GameConfig::WindowWidth * 0.71, GameConfig::WindowHeight / 2);

    int offsetY = (GameConfig::WindowHeight / 2 - (2 * GameConfig::NodeDistance) - 2 * GameConfig::Radius + 40) / 2;
    inputsText.resize(GameConfig::NumInputsNN);
    outputsText.resize(3);

    //Inputs text
    for (int i = 0; i < (int)inputsText.size(); i++)
    {
        inputsText[i].setFont(font);
        inputsText[i].setCharacterSize(15);
        inputsText[i].setPosition(GameConfig::WindowWidth * 0.53, offsetY + GameConfig::NodeDistance * i);
    }

    //Outputs text
    for (int i = 0; i < (int)outputsText.size(); i++)
    {
        outputsText[i].setFont(font);
        outputsText[i].setCharacterSize(15);
        outputsText[i].setPosition(GameConfig::WindowWidth * 0.92, offsetY + GameConfig::NodeDistance * i);
    }

    //Button
    button.setSize(sf::Vector2f(150,50));
    button.setPosition(window->getSize().x * 0.9,window->getSize().y / 2 + 5);
    button.setFillColor(sf::Color(44,128,202));
    button.setOutlineThickness(1);

    //Initialize mean score vector
    meanScoreGeneration.push_back(0);

    gameMode = Mode::TRAINING;

    trainingTime = 0;
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

            //Button click
            if (event.type == sf::Event::MouseButtonPressed && gameMode == Mode::TRAINING)
            {
                bool insideButtonX = event.mouseButton.x >= button.getPosition().x && event.mouseButton.x <= button.getPosition().x + button.getSize().x;
                bool insideButtonY = event.mouseButton.y >= button.getPosition().y && event.mouseButton.y <= button.getPosition().y + button.getSize().y;

                if (event.mouseButton.button == sf::Mouse::Left && insideButtonX && insideButtonY)
                    button.setFillColor(sf::Color::Blue);
            }

            if (event.type == sf::Event::MouseButtonReleased && gameMode == Mode::TRAINING)
            {
                bool insideButtonX = event.mouseButton.x >= button.getPosition().x && event.mouseButton.x <= button.getPosition().x + button.getSize().x;
                bool insideButtonY = event.mouseButton.y >= button.getPosition().y && event.mouseButton.y <= button.getPosition().y + button.getSize().y;

                if (event.mouseButton.button == sf::Mouse::Left && insideButtonX && insideButtonY)
                    changeGameMode();
            }
        }

        window->clear();

        int numAlive = 0;

        for(int i = 0; i < (int)individualsAlive.size(); i++)
        {
            if(individualsAlive[i].getStillAlive() == true)
            {
                numAlive++;

                //Draw games
                drawGame(individualsAlive[i]);

                //Update individual
                individualsAlive[i].update(gameMode, BlocksAvailable);

                //Check if there is a collision
                checkCollisions(i);

                //Check if game was lost
                checkGameOver(i);
            }
        }

        if (numAlive == 0)
        {
            if (gameMode == Mode::TESTING)
                numTestingDeaths++;

            advanceGeneration();
        }


        Game bestPlayer;

        if (gameMode == Mode::TRAINING)
        {
            //Draw graphic
            drawGraphic();

            //Update best player information
            bestPlayer = getBestPlayer();
        }
        else
        {
            //Update best player information
            bestPlayer = individualsAlive[0];

            drawTestingText();
        }

        //Update Text
        updateText(bestPlayer);

        //Draw Network
        drawNeuralNetwork(bestPlayer.getNeuralNetwork());

        //Update time
        updateTime();

        //Draw Menu
        drawMenu();

        //Draw texts
        drawTexts();

        //Draw Blocks
        drawBlocks();

        window->display();
    }
}

void Environment::advanceGeneration()
{
    this->Generation += 1;

    for (int i = 0; i < (int)individualsAlive.size(); i++)
    {
        std::vector<int> memory = individualsAlive[i].getScoreMemory();

        if (memory.size() == GameConfig::NumMaxMemory)
            memory.erase(memory.begin());

        memory.push_back(individualsAlive[i].getScore());
        individualsAlive[i].setScoreMemory(memory);
    }

    if (gameMode == Mode::TRAINING)
    {
        //Evolve
        selection(individualsAlive, Generation);

        this->NumIndividuals = GameConfig::NumGames;

        meanScoreGeneration.push_back(0);
    }

    //restart blocks
    for(int i = 0; i < GameConfig::NumBlocksLine; i++)
    {
        for(int j = 0; j < GameConfig::NumBlocksColumn; j++)
        {
            BlocksShape[i][j].setFillColor(sf::Color(40*(j%7),127*(j%3),60*((j+2)%5),255));
            BlocksAvailable[i][j] = GameConfig::BlockStrength;
        }
    }

    //restart balls and bases
    for (int i = 0; i < (int)individualsAlive.size(); i++)
    {
        individualsAlive[i].setStillAlive(true);
        individualsAlive[i].getBreakoutsBall()->restart();
        individualsAlive[i].getBreakoutsBase()->restart();
        individualsAlive[i].setScore(0);
    }
}
void Environment::checkGameOver(int index)
{
    //Check if ball fell down
    Ball* BreakoutsBall = individualsAlive[index].getBreakoutsBall();
    sf::CircleShape BallShape = BreakoutsBall->getGameBall();

    if(BallShape.getPosition().y + 2*BallShape.getRadius() > window->getSize().y)
    {
        individualsAlive[index].setStillAlive(false);
        NumIndividuals -= 1;
    }
}

void Environment::checkCollisions(int index)
{
    Ball* BreakoutsBall = individualsAlive[index].getBreakoutsBall();
    sf::FloatRect BallBounds = BreakoutsBall->getGameBall().getGlobalBounds();

    //Check collision with the blocks
    for(int i = 0; i < GameConfig::NumBlocksLine; i++)
    {
        for(int j = 0; j < GameConfig::NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j] > 0 && BallBounds.intersects(BlocksBounds[i][j]) &&
               BreakoutsBall->getGameBall().getPosition().y < ((GameConfig::BlockHeight + GameConfig::BlockMargin) * GameConfig::NumBlocksColumn) + GameConfig::BlockOffset)
            {
                if (gameMode == Mode::TESTING)
                    totalTestingHits++;

                //Increase 5 points if collided with block
                individualsAlive[index].setScore(individualsAlive[index].getScore() + GameConfig::CollidedBlockBonus);
                if (gameMode == Mode::TRAINING)
                    BlocksAvailable[i][j] -= 1;
                else
                    BlocksAvailable[i][j] = 0;
                sf::Color BlockColor = BlocksShape[i][j].getFillColor();

                //Change block opacity
                int newOpacity = (BlocksAvailable[i][j] * 255 / GameConfig::BlockStrength);

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
    {
        std::vector<std::vector<double>> layerWeights = layers[i - 1].getWeights();
        drawLinesShapesNN(layerWeights, layers[i - 1].getNumNeurons(), layers[i].getNumNeurons(), i);
    }
}

void Environment::drawLinesShapesNN(std::vector<std::vector<double>>& layerWeights, int numNeuronsPreviousLayer, int numNeuronsNewLayer, int layerIndex)
{
    int radius = GameConfig::RadiusNN;
    int nodeDistance = GameConfig::NodeDistance;
    int offsetX = GameConfig::OffsetXNN;
    int layerDistance = GameConfig::LayerDistance;

    int offsetYPrevious = (GameConfig::WindowHeight / 2 - (numNeuronsPreviousLayer - 1) * nodeDistance - 2 * radius + 40) / 2;
    int offsetYNew = (GameConfig::WindowHeight / 2 - (numNeuronsNewLayer - 1) * nodeDistance - 2 * radius + 40) / 2;

    for (int i = 0; i < numNeuronsNewLayer; i++)
    {
        for (int j = 0; j < numNeuronsPreviousLayer; j++)
        {
            sf::Color color;

            if (layerWeights[i][j] >= 0)
                color = sf::Color(50, 150, 50);
            else
                color = sf::Color(255, 0, 0);

            sf::Vertex line[] =
            {
               sf::Vertex(
                          sf::Vector2f(GameConfig::WindowWidth / 2 + offsetX + layerDistance * (layerIndex - 1) + radius,
                          offsetYPrevious + nodeDistance * j + radius),
                          color
                          ),
               sf::Vertex(
                          sf::Vector2f(GameConfig::WindowWidth / 2 + offsetX + layerDistance * layerIndex + radius,
                          offsetYNew + nodeDistance * i + radius),
                          color
                          )
            };

            window->draw(line, 2, sf::Lines);
        }
    }
}

void Environment::drawMenu()
{
    //Draw lines
    sf::Vertex line1[] = {
        sf::Vertex(sf::Vector2f(window->getSize().x/2, 0)),
        sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y))
        };
    sf::Vertex line2[] = {
        sf::Vertex(sf::Vector2f(window->getSize().x/2, window->getSize().y/2)),
        sf::Vertex(sf::Vector2f(window->getSize().x, window->getSize().y/2))
        };

    window->draw(line1, 2, sf::Lines);
    window->draw(line2, 2, sf::Lines);

    if (gameMode == Mode::TRAINING)
    {
        //Draw button
        window->draw(button);

        //Draw button text
        sf::Text buttonText;
        buttonText.setFont(font);
        buttonText.setCharacterSize(20);
        /*buttonText.setPosition(window->getSize().x * 0.9 + 42,window->getSize().y / 2 + 5);*/
        buttonText.setPosition(window->getSize().x * 0.9 + 27,window->getSize().y / 2 + 5);
        /*std::string textString = " Finish\ntraining"*/
        std::string textString = " Finalizar\ntreinamento";
        buttonText.setString(textString);
        window->draw(buttonText);
    }
}

void Environment::drawBlocks()
{
    int winGame = 0; //to check if all blocks are already broken

    for(int i = 0; i < GameConfig::NumBlocksLine; i++)
    {
        for(int j = 0; j < GameConfig::NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j] > 0)
            {
                window->draw(BlocksShape[i][j]);
                winGame++;
            }
        }
    }

    if(winGame == 0)
    {
        if (gameMode == Mode::TESTING)
            numTestingWins++;

        advanceGeneration();
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

    if (gameMode == Mode::TRAINING)
    {
        //Draw LowerLeft text
        std::string stringLowerLeft;
        /*stringLowerLeft += " Generation: " + std::to_string(Generation) + '\n';
        stringLowerLeft += " Number of individuals alive: " + std::to_string(NumIndividuals) + '\n';*/

        stringLowerLeft += " Geração: " + std::to_string(Generation) + '\n';
        stringLowerLeft += " Número de indivíduos vivos: " + std::to_string(NumIndividuals) + '\n';

        textLowerLeft.setString(stringLowerLeft);
        window->draw(textLowerLeft);

        //Draw LowerRight text
        std::string stringLowerRight;
        /*stringLowerRight += " All time record: " + std::to_string(record) + '\n';
        stringLowerRight += " Training time: " + updateTime() + '\n';*/

        stringLowerRight += " Recorde de todas gerações: " + std::to_string(record) + '\n';
        stringLowerRight += " Tempo de treinamento: " + updateTime() + '\n';

        textLowerRight.setString(stringLowerRight);
        window->draw(textLowerRight);
    }

    //Draw upper text
    window->draw(textUpper);

}

void Environment::drawTestingText()
{
    //Draw LowerLeft text
    std::string stringLowerLeft;
    std::string stringLowerRight;

    /*stringLowerLeft += " Number of victories: " + std::to_string(numTestingWins) + '\n';
    stringLowerLeft += " Number of defeats: " + std::to_string(numTestingDeaths) + '\n';

    stringLowerLeft += " Test time: " + updateTime() + '\n';*/

    stringLowerLeft += " Número de vitórias: " + std::to_string(numTestingWins) + '\n';
    stringLowerLeft += " Número de derrotas: " + std::to_string(numTestingDeaths) + '\n';

    stringLowerLeft += " Tempo de treinamento: " + updateTime() + '\n';

    int numRemainingBlocks = 0;
    for (int i = 0; i < GameConfig::NumBlocksLine; i++)
    {
        for (int j = 0; j < GameConfig::NumBlocksColumn; j++)
        {
            if(BlocksAvailable[i][j] > 0)
            {
                numRemainingBlocks++;
            }
        }
    }

    /*stringLowerRight += " Number of remaining blocks: " + std::to_string(numRemainingBlocks) + '\n';*/
    stringLowerRight += " Número de blocos restantes: " + std::to_string(numRemainingBlocks) + '\n';

    float time = static_cast<float>(clock.getElapsedTime().asSeconds());
    time -= trainingTime;

    /*stringLowerRight += " Blocks hit per second: " + std::to_string(static_cast<float>(totalTestingHits) / time).substr(0,4) + '\n';

    stringLowerRight += " Average victory time: " + std::to_string(numTestingWins / time).substr(0,4) + '\n';*/

    stringLowerRight += " Blocos acertados por segundo: " + std::to_string(static_cast<float>(totalTestingHits) / time).substr(0,4) + '\n';

    stringLowerRight += " Tempo de vitória médio: " + std::to_string(numTestingWins / time).substr(0,4) + '\n';

    textLowerLeft.setString(stringLowerLeft);
    textLowerRight.setString(stringLowerRight);
    window->draw(textLowerLeft);
    window->draw(textLowerRight);

    //draw testing menu base
    sf::RectangleShape winnerBase;
    sf::RectangleShape baseShape = individualsAlive[0].getBreakoutsBase()->getBaseShape();

    winnerBase.setFillColor(baseShape.getFillColor());
    winnerBase.setSize(sf::Vector2f(baseShape.getSize().x * 3,baseShape.getSize().y * 3));
    winnerBase.setPosition(window->getSize().x * 0.76 - (baseShape.getSize().x * 3 / 2), window->getSize().y * 0.85);
    window->draw(winnerBase);

    //draw testing menu title
    sf::Text winner;
    winner.setFont(font);
    winner.setCharacterSize(60);
    /*std::string winnerID = "Player " + std::to_string(individualsAlive[0].getId()) + ":";*/
    std::string winnerID = "Indivíduo " + std::to_string(individualsAlive[0].getId()) + ":";
    winner.setPosition(window->getSize().x * 0.75 - 10 * (int)winnerID.size(), window->getSize().y * 0.65) ;
    winner.setString(winnerID);
    window->draw(winner);

}

void Environment::drawGraphic()
{
    sf::Text scoreText;
    sf::Text generationText;
    sf::Text graphicTitleText;

    //update generation's mean score
    double sumGeneration = 0;

    for (int i = 0; i < (int)individualsAlive.size(); i++)
        sumGeneration += individualsAlive[i].getScore();

    meanScoreGeneration[Generation - 1] = sumGeneration / static_cast<double>(individualsAlive.size());

    //discover max element of all means
    double maxMean = 0;
    for (int i = 0; i < (int)meanScoreGeneration.size(); i++)
        maxMean = maxMean < meanScoreGeneration[i] ? meanScoreGeneration[i] : maxMean;

    //draw graphic
    int sizeVector = (int)meanScoreGeneration.size();
    int maxBarWidth = (GameConfig::WindowWidth / 2 - 100);

    //Update text size
    int characterSize = 20;
    int j = sizeVector;

    while (j > 10)
    {
        characterSize *= 0.8;
        j -= 10;
    }

    for (int i = 0; i < sizeVector; i++)
    {
        sf::RectangleShape bar;
        float barWidth = std::min(static_cast<float>(maxBarWidth) / static_cast<float>(sizeVector), float(100));
        float barHeight = 250 * meanScoreGeneration[i] / maxMean;
        float barX = GameConfig::WindowWidth * 3 / 4 + (((i) + 1) - static_cast<double>(sizeVector) / 2) * barWidth;
        float barY = GameConfig::WindowHeight - 50;

        bar.setPosition(barX, barY);
        bar.setSize(sf::Vector2f((-1) * barWidth, (-1) * barHeight));

        window->draw(bar);

        //draw bars separation
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(barX, barY - barHeight) , sf::Color::Black),
            sf::Vertex(sf::Vector2f(barX, barY), sf::Color::Black)
            };
        window->draw(line, 2, sf::Lines);


        if (sizeVector < 50)
        {
            //draw generation text
            generationText.setFont(font);
            generationText.setCharacterSize(characterSize);
            generationText.setPosition(barX - barWidth / 2, barY + 15);
            generationText.setString(std::to_string(i + 1));

            window->draw(generationText);

            //draw score text
            scoreText.setFont(font);
            scoreText.setCharacterSize(characterSize);
            scoreText.setPosition(barX - barWidth / 2 - characterSize, barY - barHeight - 30) ;
            scoreText.setString(std::to_string(meanScoreGeneration[i]).substr(0,4));

            window->draw(scoreText);
        }
    }

    graphicTitleText.setFont(font);
    graphicTitleText.setCharacterSize(35);
    /*std::string textString = "Mean generation's score";*/
    std::string textString = "Pountuação média das gerações";
    graphicTitleText.setPosition(GameConfig::WindowWidth * 0.75 - 8 * textString.size(), GameConfig::WindowHeight * 0.57) ;
    graphicTitleText.setString(textString);

    window->draw(graphicTitleText);
}

std::string Environment::updateTime()
{
    int time = static_cast<int>(clock.getElapsedTime().asSeconds());

    if (gameMode == Mode::TESTING)
        time -= (int)trainingTime;

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
    /*std::string stringtextUpper = " Best player: " + std::to_string(id) + '\n';
    stringtextUpper += " Best player's score: " + std::to_string(maxScore) + '\n';*/

    std::string stringtextUpper = " Melhor indivíduo: " + std::to_string(id) + '\n';
    stringtextUpper += " Pontuação: " + std::to_string(maxScore) + '\n';

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

void Environment::changeGameMode()
{
    advanceGeneration();

    button.setFillColor(sf::Color(44,128,202));
    gameMode = Mode::TESTING;
    Game bestAveragePlayer = getBestAveragePlayer();
    individualsAlive.resize(1);
    individualsAlive[0] = bestAveragePlayer;

    this->numTestingDeaths = 0;
    this->numTestingWins = 0;

    float time = clock.getElapsedTime().asSeconds();
    trainingTime = time;

    textLowerRight.setPosition(GameConfig::WindowWidth * 0.75, GameConfig::WindowHeight / 2);
    textLowerRight.setCharacterSize(25);
    textLowerLeft.setCharacterSize(25);

    std::vector<int> scores = bestAveragePlayer.getScoreMemory();

    for(int x: scores)
        std::cout<<x<<" ";

    std::cout<<std::endl;

    std::cout<<bestAveragePlayer.getAverageScore()<<std::endl;
}

Game Environment::getBestPlayer()
{
    float maxScore = 0;
    int indexBestGame = 0;

    for(int i = 0; i < (int)individualsAlive.size(); i++)
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

Game Environment::getBestAveragePlayer()
{
    float maxAverageScore = 0;
    int indexBestGame = 0;

    for(int i = 0; i < (int)individualsAlive.size(); i++)
    {
        if (maxAverageScore < individualsAlive[i].getAverageScore())
        {
            indexBestGame = i;
            maxAverageScore = individualsAlive[i].getAverageScore();
        }
    }
    return individualsAlive[indexBestGame];
}

std::vector<Game> Environment::getIndividualsAlive()
{
    return this->individualsAlive;
}
