#include "SFML/Graphics.hpp"
#include <cmath>
#include <Windows.h>

void hide_console()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void show_console()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}


int main()
{
	hide_console();
	const auto deg_to_rad = static_cast<float>(0.01745329);
	//const auto g = static_cast<float>(9.80665);
	const float g = 1;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Double Pendulum");
	window.setFramerateLimit(60);
	bool is_first = true;
	const sf::Color white(255, 255, 255), black(0, 0, 0);
	const sf::Color neon_blue(70, 102, 255), rad_red(130, 36, 53);
	const float r1 = 200, r2 = 190;
	const float m1 = 40, m2 = 40, c1 = 20, c2 = 20;
	float theta1 = 90 * deg_to_rad , theta2 = 90 * deg_to_rad;
	const float x0 = 400, y0 = 70;
	float theta1_v = 0, theta2_v = 0, theta1_a = 0, theta2_a = 0;

	sf::Vertex line1[2], line2[2];
	line1[0].color = line1[1].color = black;
	line2[0].color = line2[1].color = black;
	sf::CircleShape p1(c1), p2(c2);
	p1.setFillColor(rad_red);
	p2.setFillColor(rad_red);
	sf::VertexArray m2_trail;
	m2_trail.setPrimitiveType(sf::Lines);

	while (window.isOpen())
	{
		sf::Event event;  // NOLINT
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		const float x1 = x0 + std::sin(theta1) * r1;
		const float y1 = y0 + std::cos(theta1) * r1;
		line1[0].position = sf::Vector2f(x0, y0);
		line1[1].position = sf::Vector2f(x1 + c1, y1 + c1);
		p1.setPosition(x1, y1);

		const float x2 = x1 + std::sin(theta2) * r2;
		const float y2 = y1 + std::cos(theta2) * r2;
		line2[0].position = line1[1].position;
		line2[1].position = sf::Vector2f(x2 + c2, y2 + c2);
		p2.setPosition(x2, y2);

		sf::Vertex v(sf::Vector2f(x2 + c2, y2 + c2));
		v.color = neon_blue;
		m2_trail.append(v);

		if (is_first)
			is_first = false;
		else
			m2_trail.append(v);

		float tmp1 = -g * (2 * m1 + m2) * std::sin(theta1) - m2 * g * std::sin(theta1 - 2 * theta2);
		float tmp2 = 2 * std::sin(theta1 - theta2) * m2 * (theta2_v * theta2_v * r2 + theta1_v * theta1_v * r1 * std::cos(theta1 - theta2));
		const float tmp3 = 2 * m1 + m2 - m2 * std::cos(2 * theta1 - 2 * theta2);
		theta1_a = (tmp1 - tmp2) / (r1 * tmp3);

		tmp1 = g * (m1 + m2) * std::cos(theta1) + theta2_v * theta2_v * r2 * m2 * std::cos(theta1 - theta2);
		tmp2 = 2 * std::sin(theta1 - theta2) * (theta1_v * theta1_v * r1 * (m1 + m2) + tmp1);
		theta2_a = tmp2 / (r2 * tmp3);

		theta1_v += theta1_a;
		theta2_v += theta2_a;

		theta1 += theta1_v;
		theta2 += theta2_v;


		theta1_v *= 0.999;
		theta2_v *= 0.999;

		window.clear(white);
		window.draw(line1, 2, sf::Lines);
		window.draw(line2, 2, sf::Lines);
		window.draw(p1);
		window.draw(p2);
		window.draw(m2_trail);
		window.display();
	}
	m2_trail.clear();

	return 0;
}