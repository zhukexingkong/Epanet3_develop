/* EPANET 3
 *
 * Copyright (c) 2016 Open Water Analytics
 * Distributed under the MIT License (see the LICENSE file for details).
 *
 */

//! \file main.cpp
//! \brief The main function used to run EPANET from the command line.

#include "epanet3.h"
#include "reportTools.h"

#include <iostream>

int main(int argc, char* argv[])
{
    //... check number of command line arguments
    if (argc < 3)
    {
        std::cout << "\nCorrect syntax is: epanet3 inpFile rptFile (outFile)\n";
        return 0;
    }

    //... retrieve file names from command line
    const char* f1 = argv[1];           //inp
    const char* f2 = argv[2];           //rpt
    const char* f3 = "";                //out
    const char* f4 = "";
    if (argc > 3) f3 = argv[3];
    //reservoir, initial quality, test node
    //875916,100,766852£»
    if (argc > 4) f4 = argv[4];

    // ... run a full EPANET analysis
    //EN_runEpanet(f1, f2, f3);
	//reportTools reportTool;
    //reportTool.getNetworkQuality(f1, f2, f3, f4);
    //system("PAUSE");
    return 0;
}
