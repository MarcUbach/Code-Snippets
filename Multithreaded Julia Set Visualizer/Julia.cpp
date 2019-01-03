#include "Julia.h"

//---------------------------------------------------------------------//
//                           INSTRUCTIONS                              //
// Use the arrow keys to move the camera on the X and Y axis           //
// To zoom in press and hold D, and A to zoom out                      // 
// Press SpaceBar to reset the camera to it's original position        //
// Z and C decrease and increase the amount of iterations respectively //
// J and K decrease and increase the amount of threads respectively    //
//---------------------------------------------------------------------//

// Set the initial parameters for the first Julia Render
void Julia::InitJuliaParameters(int width, int height)
{
	juliaData = std::vector<std::vector<double>>(width);
	for (unsigned int i = 0; i < juliaData.size(); i++)
	{
		juliaData[i] = std::vector<double>(height);
	}

	_width = width;
	_height = height;

	zoom = 0.75;
	offsetX = 0.0;
	offsetY = -0.75;
	iterations = 60;
}

// Manage button presses
void Julia::UpdateJuliaParameters(sf::Time dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		offsetY += zoom * dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		offsetY -= zoom * dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		offsetX -= zoom * 0.5 * dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		offsetX += zoom * 0.5 * dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		zoom *= 1.0 - dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		zoom *= 1.0 + dt.asSeconds();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		iterations++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
	{
		if (iterations > 0)
		{
			iterations--;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		zoom = 1.0;
		offsetX = 0.0;
		offsetY = -0.85;
	}
}

void Julia::UpdateJuliaData(int start, int end)
{
	double aspectRatio = (double)_width / (double)(_height / 2.0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		increment += 0.001;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		increment -= 0.001;
	}

	for (int i = 0; i < _width; i++)
	{
		for (int j = start; j < end; j++)
		{
			double newWidth = (((double)i / (double)_width) - 0.5) * 2.0;
			double newHeight = (((double)j / (double)(_height / 2.0)) - 0.5) * 2.0;

			newWidth *= zoom;
			newHeight *= zoom;

			newWidth += offsetX;
			newHeight += offsetY;

			cR = sin(-0.79 + increment);
			cI = sin(0.15 + increment);

			std::complex<double> z(newWidth * aspectRatio, newHeight);
			// Substitute c below with this line to see the mandelbrot set:
			// std::complex<double> c(newWidth * aspectRatio, newHeight);
			std::complex<double> c(cR, cI);

			juliaData[i][j] = -1;

			for (int k = 0; k < iterations; k++)
			{
				z = z*z + c;

				if (z.real() * z.real() + z.imag() * z.imag() > 4.0)
				{
					juliaData[i][j] = k;
					break;
				}
			}
		}
	}
}

void Julia::RenderJulia(int start, int end)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glBegin(GL_POINTS);

	for (int i = 0; i < _width; i++)
	{
		for (int j = start; j < end; j++)
		{
			if (juliaData[i][j] != -1)
			{
				glColor3d((-cos(juliaData[i][j] * 0.12) + 1.0) / 2.0, (-cos(juliaData[i][j] * 0.08) + 1.0) / 2.0, (-cos(juliaData[i][j] * 0.03) + 1.0) / 2.0);
				glVertex2i(i, j);
			}
			else
			{
				glColor3f(0.0, 0.0, 0.0);
				glVertex2i(i, j);
			}
		}
	}

	glEnd();
}

double Julia::getOffsetX()
{
	return offsetX;
}

double Julia::getOffsetY()
{
	return offsetY;
}

double Julia::getZoom()
{
	return zoom;
}

int Julia::getIterations()
{
	return iterations;
}