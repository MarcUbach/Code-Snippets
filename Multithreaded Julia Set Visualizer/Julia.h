#ifndef JULIA_H
#define JULIA_H

#include <complex>
#include <vector>
#include "SFML\OpenGL.hpp"
#include "SFML\Window\Keyboard.hpp"
#include "SFML\Window.hpp"

class Julia
{
public:
	void InitJuliaParameters(int width, int height);
	void UpdateJuliaParameters(sf::Time dt);
	void UpdateJuliaData(int start, int end);
	void RenderJulia(int start, int end);
	
	double getOffsetX();
	double getOffsetY();
	double getZoom();
	int getIterations();

private:
	std::vector<std::vector<double>> juliaData;

	int _height, _width;
	int iterations;

	double zoom, offsetX, offsetY;
	double increment, cR, cI;
};

#endif 
