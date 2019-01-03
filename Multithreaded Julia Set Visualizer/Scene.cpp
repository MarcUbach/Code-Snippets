#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\System.hpp>
#include "SFML\Graphics.hpp"
#include "SFML\Graphics\Font.hpp"
#include "SFML\Graphics\Text.hpp"
#include <iostream>
#include <complex>
#include <math.h>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <mutex>
#include <list>
#include <thread>
#include <string>
#include "Julia.h"

using std::complex;

//---------------------------------------------------------------------//
//                           INSTRUCTIONS                              //
// Use the arrow keys to move the camera on the X and Y axis           //
// To zoom in press and hold D, and A to zoom out                      // 
// Press SpaceBar to reset the camera to it's original position        //
// Z and C decrease and increase the amount of iterations respectively //
// J and K decrease and increase the amount of threads respectively    //
//---------------------------------------------------------------------//

void InitializeScene(sf::Window* _window);
void ThreadFunction(int, int);
void CalculateJulia(sf::Time dt);
void RenderThread(sf::Font _font, sf::Time dt);
void Data(sf::Font _font, sf::Time dt);
void ShowData(sf::Time dt);

sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
sf::RenderWindow* GraphicWindow = &window;

int screenWidth;
int screenHeight;

Julia julia;

std::mutex mtx;
std::condition_variable cv;

int threadNumber;
std::vector<std::thread> threads;
std::list<bool> thReady;

int main()
{
	sf::Font font;
	if (!font.loadFromFile("calibri.ttf"))
	{
		return -1;
	}

	//Don't get holding keys input
	window.setKeyRepeatEnabled(false);

	sf::Event eventHandler;
	InitializeScene(GraphicWindow);

	sf::Clock clock;
	while (window.isOpen())
	{
		const sf::Time deltaTime = clock.restart();

		while (GraphicWindow->pollEvent(eventHandler))
		{
			if (eventHandler.type == sf::Event::Closed)
			{
				GraphicWindow->close();
			}
		}

		if (eventHandler.type == eventHandler.MouseButtonPressed && eventHandler.mouseButton.button == sf::Mouse::Left)
		{
			ShowData(deltaTime);
		}
		
		CalculateJulia(deltaTime);
		RenderThread(font, deltaTime);

		for (int i = 0; i < threadNumber; i++)
		{
			threads[i].join();
		}
		threads.clear();
	}

	return 0;
}

void InitializeScene(sf::Window* window)
{
	// Activate the window's context
	window->setActive();

	// Initial number of threads
	threadNumber = 8;

	screenWidth = window->getSize().x;
	screenHeight = window->getSize().y;

	julia.InitJuliaParameters(screenWidth, screenHeight);

	// SSETTING OPENGL
	// Projects global coordinates into clip space
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, screenWidth, screenHeight, 0.0f, 1.0f, -1.0f);

	// Places geometry in the global, unprojected space
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ThreadFunction(int start, int end)
{
	julia.UpdateJuliaData(start, end);

	std::unique_lock<std::mutex> lock(mtx);
	thReady.push_back(true);
	cv.notify_all();
}

// Sincronize threads with barrier set by condition variable
void RenderThread(sf::Font _font, sf::Time dt)
{
	std::unique_lock<std::mutex> lock(mtx);

	// Waits until all the the threads are ready before continuing with the function to render the Julia Set
	while (thReady.size() != threadNumber)
	{
		cv.wait(lock);
	}

	// Clear the threads ready for when the function is called again
	thReady.clear();
	// Render the Set
	julia.RenderJulia(0, screenHeight);
	// Display Data values onto the screen here, so that it doesnt override the Julia Set image
	Data(_font, dt);
	GraphicWindow->display();
}

// This function Calculate the Julia Set, and updates it managing the amount of threads being used each time, properly setting up the window
void CalculateJulia(sf::Time dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
		threadNumber++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		threadNumber--;

		if (threadNumber < 1)
		{
			threadNumber = 1;
		}
	}

	julia.UpdateJuliaParameters(dt);

	int perThread = screenHeight / threadNumber;
	int remainder = screenHeight % threadNumber;

	for (int i = 0; i < threadNumber; i++)
	{
		threads.push_back(std::thread(ThreadFunction, i * perThread, (i + 1) * perThread));
	}
}

// This function will display the Julia Set information on the SFML window, being updated every time the function is called
void Data(sf::Font _font, sf::Time dt)
{
	sf::Text numIterations;
	sf::Text zoom;
	sf::Text deltaTime;
	sf::Text threadNum;

	numIterations.setFont(_font);
	numIterations.setString("Iterations: " + std::to_string(julia.getIterations()));
	numIterations.setCharacterSize(24); //pixels
	numIterations.setFillColor(sf::Color::White);

	deltaTime.setFont(_font);
	deltaTime.setString("Time: " + std::to_string(dt.asMilliseconds()));
	deltaTime.setCharacterSize(24); //pixels
	deltaTime.setFillColor(sf::Color::White);
	deltaTime.setPosition(sf::Vector2f(0.0, 25.0));

	zoom.setFont(_font);
	zoom.setString("Zoom: " + std::to_string(round(1 / julia.getZoom())));
	zoom.setCharacterSize(24); //pixels
	zoom.setFillColor(sf::Color::White);
	zoom.setPosition(sf::Vector2f(0.0, 50.0));

	threadNum.setFont(_font);
	threadNum.setString("Threads: " + std::to_string(threadNumber));
	threadNum.setCharacterSize(24); //pixels
	threadNum.setFillColor(sf::Color::White);
	threadNum.setPosition(sf::Vector2f(0.0, 75.0));

	GraphicWindow->draw(numIterations);
	GraphicWindow->draw(deltaTime);
	GraphicWindow->draw(zoom);
	GraphicWindow->draw(threadNum); 
}

// ShowData will display the same information as above, but it will be displayed on the console, 
// once per left mouse click, 
void ShowData(sf::Time dt)
{
	std::cout << "Number of Iterations: " << julia.getIterations() << std::endl;
	std::cout << "Time " << dt.asMilliseconds() << std::endl;
	std::cout << "Zoom " << 1 / julia.getZoom() << std::endl;
	std::cout << "Number of threads " << threadNumber << std::endl;
	std::cout << "--------------------------" << std::endl;
}