#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

struct Intersection {
	float x;
	float y;
	float T;
	float angle;
};

void setColor(sf::VertexArray& shape, int size, sf::Color color) {
	for (int i = 0; i < size; i++) {
		shape[i].color = color;
	}
}

bool getIntersection(sf::Vertex ray[2], sf::Vertex segment[2], struct Intersection& intersection) {
	
	
	//Parametric Equation L = a + T1 * v -- (point + t1*direction)
	//Direction t(b - a);
	//Parametric Equation of the Ray
	float r_pX = ray[0].position.x;
	float r_pY = ray[0].position.y;
	float r_dX = ray[1].position.x - ray[0].position.x;
	float r_dY = ray[1].position.y - ray[0].position.y;
	
	//Parametric equation of the Segment
	float seg_pX = segment[0].position.x;
	float seg_pY = segment[0].position.y;
	float seg_dX = segment[1].position.x - segment[0].position.x;
	float seg_dY = segment[1].position.y - segment[0].position.y;

	/*Check if they are parallel (There is no intersection if so)
	x = mag * cos(alpha);
	y = mag * sin (alpha);
	
	x / mag = cos(alpha);
	y / mag = sin(alpha);
	*/
	float r_mag = std::sqrt((r_dX * r_dX) + (r_dY * r_dY));
	float seg_mag = std::sqrt((seg_dX * seg_dX) + (seg_dY * seg_dY));
	if ((r_dX / r_mag) == (seg_dX / seg_mag) && (r_dY / r_mag) == (seg_dY / seg_mag)) {
		return false;
	}

	/*Solve T1 && T2
		L = a + (T1 * v) => L = a + (T1 * (a-b))
		(x,y) = (r_pX,r_pY) + (T1 * (r_pX - seg_pX, r_pY - seg_pY))
		(x,y) = (r_pX, r_pY) + (T1 * (r_dX, r_dT)) && (x,y) = (seg_pX, seg_pY) + (T2 * (seg_dX, seg_dY))
		Image here
	*/
	float T2 = ((r_dX * (seg_pY - r_pY)) - (r_dY * (seg_pX - r_pX))) / ((seg_dX * r_dY) - (seg_dY * r_dX));
	float T1 = (seg_pX + (T2 * seg_dX) - r_pX) / r_dX;

	if (T1<0) return false;
	if (T2<0 || T2>1) return false;

	intersection.x = (r_pX + r_dX * T1);
	intersection.y = (r_pY + r_dY * T1);
	intersection.T = T1;
	return true;
}

std::vector<sf::Vector2f> getUniquePoints(std::vector<sf::VertexArray> shapes) {
	
	std::vector<sf::Vector2f> points;
	for each (sf::VertexArray x in shapes)
	{
		int nVertices = x.getVertexCount();
		for (int i = 0; i < nVertices; i++) {
			sf::Vector2f pointA = sf::Vector2f(x[i].position.x, x[i].position.y);
			points.push_back(pointA);
		}
	}

	std::vector<sf::Vector2f> uniquePoints;
	std::vector<sf::Vector2f> existingPoints;
	for each (sf::Vector2f x in points) {
		if (!(std::find(existingPoints.begin(), existingPoints.end(), x) != existingPoints.end())) {
			uniquePoints.push_back(x);
			existingPoints.push_back(x);
		}
	}

	return uniquePoints;
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Cone Light System!", sf::Style::Default, settings);
	
	std::vector<sf::VertexArray> shapes;

	//Background
	sf::RectangleShape whiteBackground(sf::Vector2f(1280, 720));
	whiteBackground.setFillColor(sf::Color::White);
	
	//Border
	sf::VertexArray border(sf::LinesStrip, 5);
	border[0].position = sf::Vector2f(0, 0);
	border[1].position = sf::Vector2f(1280, 0);
	border[2].position = sf::Vector2f(1280, 720);
	border[3].position = sf::Vector2f(0, 720);
	border[4].position = sf::Vector2f(0, 0);
	setColor(border, 5, sf::Color::Black);

	//Shape 1
	sf::VertexArray shape1(sf::LinesStrip, 5);
	shape1[0].position = sf::Vector2f(80, 190);
	shape1[1].position = sf::Vector2f(410, 70);
	shape1[2].position = sf::Vector2f(480,250);
	shape1[3].position = sf::Vector2f(150,380);
	shape1[4].position = sf::Vector2f(80, 190);
	setColor(shape1, 5, sf::Color::Black);
	
	//Shape 2
	sf::VertexArray shape2(sf::LinesStrip, 5);
	shape2[0].position = sf::Vector2f(940, 300);
	shape2[1].position = sf::Vector2f(1112, 399);
	shape2[2].position = sf::Vector2f(976, 636);
	shape2[3].position = sf::Vector2f(810, 538);
	shape2[4].position = sf::Vector2f(940, 300);
	setColor(shape2, 5, sf::Color::Black);

	//Shape 3
	sf::VertexArray shape3(sf::LinesStrip, 4);
	shape3[0].position = sf::Vector2f(445, 431);
	shape3[1].position = sf::Vector2f(481, 636);
	shape3[2].position = sf::Vector2f(284, 564);
	shape3[3].position = sf::Vector2f(445, 431);
	setColor(shape3, 4, sf::Color::Black);

	//Shape 4
	sf::VertexArray shape4(sf::LinesStrip, 4);
	shape4[0].position = sf::Vector2f(717, 69);
	shape4[1].position = sf::Vector2f(909, 151);
	shape4[2].position = sf::Vector2f(735, 275);
	shape4[3].position = sf::Vector2f(717, 69);
	setColor(shape4, 4, sf::Color::Black);

	//Shape 4
	sf::VertexArray shape5(sf::LinesStrip, 5);
	shape5[0].position = sf::Vector2f(522, 569);
	shape5[1].position = sf::Vector2f(700, 469);
	shape5[2].position = sf::Vector2f(763, 570);
	shape5[3].position = sf::Vector2f(574, 669);
	shape5[4].position = sf::Vector2f(522, 569);
	setColor(shape5, 5, sf::Color::Black);

	shapes.push_back(border);
	shapes.push_back(shape1);
	shapes.push_back(shape2);
	shapes.push_back(shape3);
	shapes.push_back(shape4);
	shapes.push_back(shape5);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Update

		//Update Mouse Position 
		sf::Vector2f mouseCoordinates(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

		// Get unique points
		std::vector<sf::Vector2f> uniquePoints = getUniquePoints(shapes);

		std::vector<float> uniqueAngles;
		for (std::vector<sf::Vector2f>::iterator it = uniquePoints.begin(); it != uniquePoints.end(); it++){
			sf::Vector2f uniquePoint = *it;
			float angle = atan2(uniquePoint.y - mouseCoordinates.y, uniquePoint.x - mouseCoordinates.x);
			uniqueAngles.push_back(angle - 0.00001);
			uniqueAngles.push_back(angle);
			uniqueAngles.push_back(angle + 0.00001);
		}


		std::vector<struct Intersection> intersections;
		for (float i = 0; i < uniqueAngles.size(); i++){
			float angle = uniqueAngles[i];

			float dx = cos(angle);
			float dy = sin(angle);

			sf::Vertex ray[2] = {
				sf::Vertex(sf::Vector2f(mouseCoordinates)),
				sf::Vertex(sf::Vector2f(mouseCoordinates.x + (dx*1000), mouseCoordinates.y + (dy*1000)))
			};

			struct Intersection closestIntersection;
			closestIntersection.T = 2;

			for (std::vector<sf::VertexArray>::iterator it = shapes.begin(); it != shapes.end(); it++) {

				struct Intersection intersection;
				for (int i = 0; i < (it->getVertexCount() - 1); i++){

					sf::Vertex segment[] = {
						sf::Vertex((*it)[i].position),
						sf::Vertex((*it)[i + 1].position)
					};
					bool intersects = getIntersection(ray, segment, intersection);
					if (!intersects) continue;
					if (intersects) {
						if (intersection.T < closestIntersection.T) {
							closestIntersection = intersection;
						}
					}
				}
				
			}

			closestIntersection.angle = angle;

			intersections.push_back(closestIntersection);
		}

		std::sort(intersections.begin(), intersections.end(), 
			[](const struct Intersection& a, const struct Intersection& b) -> bool
			{
				return a.angle < b.angle;
			}
		);

		std::vector <sf::VertexArray> lines;
		for each (struct Intersection x in intersections) {
			sf::VertexArray line(sf::LinesStrip, 2);
			line[0].position = sf::Vector2f(mouseCoordinates);
			line[1].position = sf::Vector2f(x.x, x.y);
			setColor(line, 2, sf::Color(16, 113, 165));
			lines.push_back(line);
		}
		

		std::vector<sf::VertexArray> lights;
		for (int i = 0; i < lines.size() - 1; i++) {
			sf::VertexArray light(sf::Quads, 4);
			
			sf::VertexArray line1 = lines.at(i);
			sf::VertexArray line2 = lines.at(i + 1);

			if (i == lines.size() - 2) {
				line1 = lines.at(i + 1);
				line2 = lines.at(0);
			}

			light[0].position = sf::Vector2f(line1[0].position.x, line1[0].position.y);
			light[1].position = sf::Vector2f(line1[1].position.x, line1[1].position.y);
			light[2].position = sf::Vector2f(line2[0].position.x, line2[0].position.y);
			light[3].position = sf::Vector2f(line2[1].position.x, line2[1].position.y);
			setColor(light, 4, sf::Color(46, 160,221));

			lights.push_back(light);
		}
		

		//End Update

		//Draw
		window.clear();
		window.draw(whiteBackground);
		
		for (std::vector<sf::VertexArray>::iterator it = shapes.begin(); it != shapes.end(); it++) {
			window.draw(*it);
		}
		for (std::vector<sf::VertexArray>::iterator it = lights.begin(); it != lights.end(); it++) {
			window.draw(*it);
		}
		//Debug LINES
		/*
		for (std::vector<sf::VertexArray>::iterator it = lines.begin(); it != lines.end(); it++) {
			window.draw(*it);
		}
		*/

		window.display();
		//End Draw
	}

	return 0;
}
