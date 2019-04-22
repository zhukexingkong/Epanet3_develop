#include "reportTools.h"
#include "Elements\link.h"
#include "Core\options.h"

extern "C" {

reportTools::reportTools()
{
	tmpStr = "";
	project = new Project();
	network = new Network();
}


reportTools::~reportTools()
{
}

void reportTools::getNetworkQuality(const char* inpFile, const char* rptFile, const char* outFile, const char* optionsStr){
	// ... declare a Project variable and an error indicator
	int err = 0;
	double lcf = network->ucf(Units::LENGTH);
	double pcf = network->ucf(Units::PRESSURE);
	double qcf = network->ucf(Units::FLOW);
	double ccf = network->ucf(Units::CONCEN);

	for (;;)
	{
		// ... open the command line files and load network data
		//if ((err = project->openReport(rptFile))) break;
		if ((err = project->load(inpFile))) break;
		//if ((err = project->openOutput(""))) break;
		project->writeSummary();

        ////多个水厂配置
        //vector<string> reservoirLine = split(optionsStr, ';');
        //vector<string> setting;
        //int line = reservoirLine.size();
        ////二维数组分配内存
        //string** qualitieSetting = new string*[line];
        //for (int i = 0; i < line; i++){
        //    setting = split(reservoirLine[i], ',');
        //    qualitieSetting[i] = new string[3];
        //    //赋值
        //    for (int j = 0; j < 3; j++){
        //        qualitieSetting[i][j] = setting[j];
        //    }
        //}

        ////每个水厂分别水质计算
        //for (int i = 0; i < line; i++){
        //    //初始化计算引擎时设置水源点
        //    Node* testNode = setOptions(project, qualitieSetting[i]);
        //    //clear 计算引擎
        //    project->clearSolver();
        //    // ... initialize the solver
        //    if ((err = project->initSolver(false))) break;

        //    // ... step through each time period
        //    int t = 0;
        //    int tstep = 0;
        //    //初始时间0也要输出Quality
        //   // reportStream << "reservoir: " << qualitieSetting[i][0] << endl;
        //    double qualityTmp = testNode->quality*ccf;
        //    /*string time = Utilities::getTime(t + tstep);
        //    reportStream << time << " " << qualityTmp << endl;*/
        //    string time = Utilities::getTime(t + tstep);
        //    printf("reservoir: %s\n", qualitieSetting[i][0].c_str());
        //    printf("%s:v=%lf ", time.c_str(), qualityTmp);

        //    do
        //    {
        //        //reportStream << "\nSolving network at "<< Utilities::getTime(t + tstep) << " hrs ...        \n";
        //        // ... run solver to compute hydraulics
        //        err = project->runSolver(&t);

        //        // ... advance solver to next period in time while solving for water quality
        //        if (!err) err = project->advanceSolver(&tstep);

        //        //输入各时间Quality
        //        qualityTmp = testNode->quality*ccf;
        //        //time = Utilities::getTime(t + tstep);
        //        //reportStream << time << " " << qualityTmp << endl;
        //        time = Utilities::getTime(t + tstep);
        //        printf("%s:v=%f ", time.c_str(), qualityTmp);
        //        ////node Quality 输出
        //        //reportStream << "[NODE]    [QUALITY mg/L]" << endl;
        //        //writeNodeResults();
        //        ////link Quality 输出
        //        //reportStream << "[LINK]    [Flow]" << endl;
        //        //writeLinkResults();
        //        //writeStreamMsg();
        //    } while (tstep > 0 && !err);
        //    //writeStreamMsg();
        //    system("PAUSE ");
        //}

        
        //clear 计算引擎
        project->clearSolver();
        // ... initialize the solver
        if ((err = project->initSolver(false))) break;

        // ... step through each time period
        int t = 0;
        int tstep = 0;
        network = project->getNetwork();
        //初始时间0也要输出Quality
        printf("reservoir: 900247\n");
        Node* testNode = network->node("687198");
        double qualityTmp = testNode->quality*ccf;
        //string time = Utilities::getTime(t + tstep);
        //reportStream << time << " " << qualityTmp << endl;
        string time = Utilities::getTime(t + tstep);
        printf("%s:v=%lf ", time.c_str(), qualityTmp);

        do
        {
            //reportStream << "\nSolving network at "<< Utilities::getTime(t + tstep) << " hrs ...        \n";
            // ... run solver to compute hydraulics
            err = project->runSolver(&t);

            // ... advance solver to next period in time while solving for water quality
            if (!err) err = project->advanceSolver(&tstep);

            //输入各时间Quality
            testNode = network->node("687198");
            qualityTmp = testNode->quality*ccf;
            //time = Utilities::getTime(t + tstep);
            //reportStream << time << " " << qualityTmp << endl;
            time = Utilities::getTime(t + tstep);
            printf("%s:v=%f ", time.c_str(), qualityTmp);
            ////node Quality 输出
            //reportStream << "[NODE]    [QUALITY mg/L]" << endl;
            //writeNodeResults();
            ////link Quality 输出
            //reportStream << "[LINK]    [Flow]" << endl;
            //writeLinkResults();
            //writeStreamMsg();
        } while (tstep > 0 && !err);
        //writeStreamMsg();
		system("PAUSE ");
		break;
	}
}

void reportTools::writeNodeResults()
{
	float         nodeResults[NumNodeVars];
	// ... units conversion factors
	double lcf = network->ucf(Units::LENGTH);
	double pcf = network->ucf(Units::PRESSURE);
	double qcf = network->ucf(Units::FLOW);
	double ccf = network->ucf(Units::CONCEN);
	double outflow;
	double quality;
	// ... results for each node
	for (Node* node : network->nodes)
	{
		nodeResults[0] = (float)(node->head*lcf);								//Total Head
		nodeResults[1] = (float)((node->head - node->elev)*pcf);				//Pressure
		nodeResults[2] = (float)(node->actualDemand*qcf);						//Actual Demand
		nodeResults[3] = (float)((node->fullDemand - node->actualDemand)*qcf);	//Full Demand

		// ... total external outflow (reverse sign for tanks & reservoirs)
		outflow = node->outflow;
		if (node->type() != Node::JUNCTION) outflow = -outflow;
		nodeResults[4] = (float)(outflow*qcf);									//Out Flow

		// ... use source-ammended quality for WQ source nodes
		if (node->qualSource) quality = node->qualSource->quality;
		else                    quality = node->quality;
		nodeResults[5] = (float)(quality*ccf);									//Quality
		reportStream << node->name << "         " << nodeResults[5] << endl;
	}
}

void reportTools::writeLinkResults(){
	float         linkResults[NumLinkVars];
	// ... units conversion factors
	double lcf = network->ucf(Units::LENGTH);
	double qcf = network->ucf(Units::FLOW);
	double hloss;

	// ... results for each link
	for (Link* link : network->links)
	{
		linkResults[1] = (float)(link->leakage*qcf);                 //leakage
		linkResults[2] = (float)(link->getVelocity()*lcf);           //velocity
		hloss = link->getUnitHeadLoss();
		if (link->type() != Link::PIPE) hloss *= lcf;
		linkResults[3] = (float)(hloss);                             //head loss
		linkResults[4] = (float)link->status;                        //status
		linkResults[5] = (float)link->getSetting(network);           //setting
		linkResults[6] = (float)(link->quality*FT3perL);             //quality
        reportStream << link->name << "         " << linkResults[6] << endl;
		
	}
}

void reportTools::clearStream(){
	reportStream.clear();
	tmpStr.clear();
}

void reportTools::writeStreamMsg(){
	tmpStr = reportStream.str();
	project->writeMsg(tmpStr);
	clearStream();
	project->writeMsgLog();
}


vector<string> reportTools::split(string str, char del) //忽略“空位”
{
    stringstream ss(str);
    string tok;
    vector<string> ret;
    while (getline(ss, tok, del))
    {
        if (tok > "")
            ret.push_back(tok);
    }
    return ret;
}

Node*  reportTools::setOptions(Project* project, string* settingStr){
    //水质计算设置
    project->runQuality = true;
    network = project->getNetwork();
    Options* options = &(network->options);
    options->setOption(Options::StringOption::QUAL_MODEL, "CHEMICAL");                  //水质计算模式
    options->setOption(Options::IndexOption::QUAL_TYPE, Options::QualType::CHEM);       //水质计算类型
    //以下默认配置已设置
    //options->setOption(Options::StringOption::QUAL_NAME, "CHEMICAL");                 //水质计算模式名称
    //options->setOption(Options::StringOption::QUAL_UNITS_NAME, "MG/L");               //水质单位名称
    //options->setOption(Options::IndexOption::QUAL_UNITS, Options::QualUnits::MGL);    //水质单位
    //options->setOption(Options::ValueOption::MOLEC_DIFFUSIVITY, 1);                   //化学物扩散系数
    //options->setOption(Options::ValueOption::QUAL_TOLERANCE, 0.01);                   //水质比较容差
    //options->setOption(Options::TimeOption::QUAL_STEP, 300);                          //水质计算时间间隔

    //水源点，计算为英制单位，100为mg/L，要乘以28.317

    //reservoir name
    Node* qualityNode = network->node(settingStr[0]);
    //initial quality
    qualityNode->initQual = atof(settingStr[1].c_str())*LperFT3;
    //test node name
    Node* testNode = network->node(settingStr[2]);
    return testNode;
}

}