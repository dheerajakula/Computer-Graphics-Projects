#pragma once

#include <glm/glm.hpp>
#include "object.h"
#include "shader.h"

class table
{

public:
	std::vector<object> obj_array;
	table()
	{
		object o1 = object("cuboid.obj");
		obj_array.push_back(o1);

		object o2 = object("cuboid.obj");
		o2.translate(glm::vec3(5.0f,0.0f,0.0f));
		obj_array.push_back(o2);

		object o3 = object("cuboid.obj");
		o3.translate(glm::vec3(5.0f, 0.0f, -5.0f));
		obj_array.push_back(o3);

		object o4 = object("cuboid.obj");
		o4.translate(glm::vec3(0.0f, 0.0f, -5.0f));
		obj_array.push_back(o4);
		
		std::vector<float> v = {0.0f,2.0f,0.0f,0.0f,2.0f,-5.0f,5.0f,2.0f,0.0f,0.0f,2.0f,-5.0f,5.0f,2.0f,-5.0f,5.0f,2.0f,0.0f};
		object o5 = object(v);
		obj_array.push_back(o5);

	}
	
	void draw(Shader ourShader)
	{
		for (int i = 0; i < obj_array.size(); i++)
		{
			obj_array[i].draw(ourShader);
		}
	}

	void translate(glm::vec3 translation)
	{
		for (int i = 0; i < obj_array.size(); i++)
		{
			obj_array[i].translate(translation);
		}

	}
};