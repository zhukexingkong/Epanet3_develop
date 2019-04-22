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

class assesspressure
{
public:
    assesspressure();
    ~assesspressure();
};

