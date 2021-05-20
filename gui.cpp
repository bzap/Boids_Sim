#include "gui.h"
#include<cmath>

Gui::Gui(int w, int h)
{
	width = w;
	height = h;
}

void Gui::addButton(int x, int y, int r, int id, int col)
{
	buttonCenters.push_back(x);
	buttonCenters.push_back(y);
	buttonCenters.push_back(r);
	buttonCenters.push_back(id);

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			if(( pow((i-x),2) + pow((j-y),2)) < pow(r,2))
			{
				buttons.push_back(i);
				buttons.push_back(j);
			}
		}
	}

}

int Gui::buttonClick(int x, int y)
{
	for(int i = 0; i < buttonCenters.size(); i += 4)
	{
		if(pow(x-buttonCenters[i],2)+pow(y-buttonCenters[i+1],2) < pow(buttonCenters[i+2],2))
		{
			return buttonCenters[i+3];
		}
	}

	return 0;
}