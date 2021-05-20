#ifndef GUI_H
#define GUI_H
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


class Gui{
public:
	Gui(int w, int h);
	void addButton(int x, int y, int r, int id,int col);
	int buttonClick(int x, int y);

	vector<int> buttons;
	vector<int> buttonCenters;

	int width;
	int height;
private:

};

#endif