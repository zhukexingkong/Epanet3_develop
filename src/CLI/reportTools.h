#pragma once
#include "epanet3.h"
#include "project.h"
#include "datamanager.h"
#include "constants.h"
#include "error.h"
#include "node.h"
#include "link.h"
#include "qualsource.h"
#include "utilities.h"

#include <iostream>
#include <iomanip>
#include <time.h>

using namespace Epanet;
using namespace std;

class Network;
class reportTools
{
public:
	ostringstream  reportStream;
	string tmpStr;
	Project* project;
	Network* network;

	reportTools();
	~reportTools();

    void getNetworkQuality(const char* inpFile, const char* rptFile, const char* outFile, const char* optionsStr);

	void reportTools::writeNodeResults();

	void writeLinkResults();

	void clearStream();

	void writeStreamMsg();

    vector<string> split(string str, char del);

    void getSetQualities(string** qualities, int* reservoirNum, const char* optionsStr);

    Node*  setOptions(Project* project, string* settingStr);
};

