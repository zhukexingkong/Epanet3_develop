/* EPANET 3
 *
 * Copyright (c) 2016 Open Water Analytics
 * Distributed under the MIT License (see the LICENSE file for details).
 *
 */

//! \file options.h
//! \brief Describes the Options class.

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "Output/reportfields.h"
#include <string>
#include <sstream>

class Network;

//! \class Options
//! \brief User-supplied options for analyzing a pipe network.

class Options
{
  public:

    // ... Enumerated values for categorical options

    enum UnitSystem    {US, SI};
    enum FlowUnits     {CFS, GPM, MGD, IMGD, AFD, LPS, LPM, MLD, CMH, CMD};
    enum PressureUnits {PSI, METERS, KPA};
    enum FileMode      {SCRATCH, USE, SAVE};
    enum IfUnbalanced  {STOP, CONTINUE};
    enum QualType      {NOQUAL, AGE, TRACE, CHEM};			//水龄、踪迹、化学物
    enum QualUnits     {NOUNITS, HRS, PCNT, MGL, UGL};
    enum ReportedItems {NONE, ALL, SOME};

    // ... Options with string values

    enum StringOption {
        HYD_FILE_NAME,         //!< Name of binary file containing hydraulic results		//保存水力计算结果的二进制文件
        OUT_FILE_NAME,         //!< Name of binary file containing simulation results		//保存模拟计算结果的二进制文件
        RPT_FILE_NAME,         //!< Name of text file containing output report				//输出报告的文本文件
        MAP_FILE_NAME,         //!< Name of text file containing nodal coordinates			//节点坐标的文本文件

        HEADLOSS_MODEL,        //!< Name of head loss model used							//水头损失模式
        DEMAND_MODEL,          //!< Name of nodal demand model used							//节点需求模式
        LEAKAGE_MODEL,         //!< Name of pipe leakage model used							//管道泄漏模式
        HYD_SOLVER,            //!< Name of hydraulic solver method							//水力计算模式
        STEP_SIZING,           //!< Name of Newton step size method							//牛顿步长法模式
        MATRIX_SOLVER,         //!< Name of sparse matrix eqn. solver						//稀疏矩阵eqn求解
        DEMAND_PATTERN_NAME,   //!< Name of global demand pattern							//全局需求模式名称

        QUAL_MODEL,            //!< Name of water quality model used						//水质计算模式
        QUAL_NAME,             //!< Name of water quality constituent						//水质计算名称："NONE", "AGE", "TRACE", "CHEMICAL"
        QUAL_UNITS_NAME,       //!< Name of water quality units								//水质单位
        TRACE_NODE_NAME,       //!< Name of node for source tracing							//水质追踪节点

        MAX_STRING_OPTIONS
    };

    // ... Options with integer, categorical or yes/no values	整数、分类、是否选项值

    enum IndexOption {
        UNIT_SYSTEM,           //!< Choice of units system									系统单位
        FLOW_UNITS,            //!< Choice of flow rate units								流量单位
        PRESSURE_UNITS,        //!< Choice of pressure units								压力单位
        MAX_TRIALS,            //!< Maximum hydraulic trials								最大液压实验
        IF_UNBALANCED,         //!< Stop or continue if network is unbalanced				管网不平衡时，停止或继续
        HYD_FILE_MODE,         //!< Binary hydraulics file mode								二进制水力文件模式
        DEMAND_PATTERN,        //!< Global demand pattern index								全局需求模式编号
        ENERGY_PRICE_PATTERN,  //!< Global energy price pattern index						全局能源价格模式编号

        QUAL_TYPE,             //!< Type of water quality analysis							水质计算类型
        QUAL_UNITS,            //!< Units of the quality constituent					    水质成分单位
        TRACE_NODE,            //!< Node index for source tracing							水质追踪节点编号

        REPORT_SUMMARY,        //!< report input/output summary								报告输入/输出摘要
        REPORT_ENERGY,         //!< report energy usage										报告能源使用状况
        REPORT_STATUS,         //!< report system status									报告系统状态
        REPORT_TRIALS,         //!< report result of each trial								每个实验的报告结果
        REPORT_NODES,          //!< report node results										报告节点结果
        REPORT_LINKS,          //!< report link results										报告管线结果

        MAX_INDEX_OPTIONS
    };

    // ... Options with numerical values

    enum ValueOption {

        // Hydraulic properties	水力特性
        SPEC_GRAVITY,          //!< Specific Gravity								比重
        KIN_VISCOSITY,         //!< Kinematic viscosity (ft2/sec)					运动粘度
        DEMAND_MULTIPLIER,     //!< Global base demand multiplier					全局基本需求乘数
        MINIMUM_PRESSURE,      //!< Global minimum pressure to supply demand (ft)	全局供应需求的最小压力
        SERVICE_PRESSURE,      //!< Global pressure to supply full demand (ft)		全局供应满需求的压力
        PRESSURE_EXPONENT,     //!< Global exponent for power function demands		动力功能需求的全局指数
        EMITTER_EXPONENT,      //!< Global exponent in emitter discharge formula	发射器流量公式的全局指数
        LEAKAGE_COEFF1,
        LEAKAGE_COEFF2,

        // Hydraulic tolerances	液压公差
        RELATIVE_ACCURACY,     //!< sum of all |flow changes| / sum of all |flows|	总流量和其变化平均数
        HEAD_TOLERANCE,        //!< Convergence tolerance for head loss balance		水头损失平衡收敛容差
        FLOW_TOLERANCE,        //!< Convergence tolerance for flow balance			流量平衡的收敛容差
        FLOW_CHANGE_LIMIT,     //!< Max. flow change for convergence				收敛的流量变化最大值
        TIME_WEIGHT,           //!< Time weighting for variable head tanks			水塔水头变化加权值

        // Water quality options	水质选项
        MOLEC_DIFFUSIVITY,     //!< Chemical's molecular diffusivity (ft2/sec)		化学物扩散系数
        QUAL_TOLERANCE,        //!< Tolerance for water quality comparisons			水质比较容差
        BULK_ORDER,            //!< Order of all bulk flow reactions in pipes		管道中所有大流动反应的顺序
        WALL_ORDER,            //!< Order of all pipe wall reactions				所有管壁反应的顺序
        TANK_ORDER,            //!< Order of all bulk water reactions in tanks		水塔中所有大流动反应的顺序
        BULK_COEFF,            //!< Global rate coefficient for bulk reactions		全局大反应系数
        WALL_COEFF,            //!< Global rate coefficient for wall reactions		全局管壁反应系数
        LIMITING_CONCEN,       //!< Maximum concentration for growth reactions		增长反应得最大浓度
        ROUGHNESS_FACTOR,      //!< Relates wall reaction coeff. to pipe roughness	与管壁反应系数有关的管道粗糙度

        // Energy options	能源选择
        ENERGY_PRICE,          //!< Global energy price (per kwh)					全局能源价格
        PEAKING_CHARGE,        //!< Fixed energy charge per peak kw					每峰值千瓦固定能源花费
        PUMP_EFFICIENCY,       //!< Global pump efficiency (fraction)				全局泵效率（百分数）

        MAX_VALUE_OPTIONS
    };

    // ... Time options (in integer seconds) 时间选项（单位整数秒）

    enum TimeOption {
        START_TIME,            //!< Starting time of day							开始时间
        HYD_STEP,              //!< Hydraulic simulation time step					水力模拟时间间隔
        QUAL_STEP,             //!< Water quality simulation time step				水质计算时间间隔
        PATTERN_STEP,          //!< Global time interval for time patterns			全局时间模式间隔
        PATTERN_START,         //!< Time of day at which all time patterns start	所有时间模式开始时间
        REPORT_STEP,           //!< Reporting time step								报告时间间隔
        REPORT_START,          //!< Simulation time at which reporting begins		报告开始时间
        RULE_STEP,             //!< Time step used to evaluate control rules		评估控制规则时间步
        TOTAL_DURATION,        //!< Total simulation duration						总模拟持续时间
        REPORT_STATISTIC,      //!< How results are reported (min, max, range)		报告结果方式（最小值、最大值、范围值）

        MAX_TIME_OPTIONS
    };

    //... Constructor / Destructor

    Options();
    ~Options() {}

    // ... Methods that return an option's value

    int         flowUnits();
    int         pressureUnits();
    std::string stringOption(StringOption option);
    int         indexOption(IndexOption option);
    double      valueOption(ValueOption option);
    int         timeOption(TimeOption option);

    // ... Methods that set an option's value

    void   setDefaults();
    void   adjustOptions();
    int    setOption(StringOption option, const std::string& value);
    int    setOption(IndexOption option, const std::string& value, Network* nw);
    void   setOption(IndexOption option, int value);
    void   setOption(ValueOption option, double value);
    void   setOption(TimeOption option, int value);
    void   setReportFieldOption(int fieldType,
                                int fieldIndex,
                                int enabled,
                                int precision,
                                double lowerLimit,
                                double upperLimit);

    // ... Methods that write a collection of options to a string

    std::string hydOptionsToStr();
    std::string qualOptionsToStr();
    std::string demandOptionsToStr();
    std::string timeOptionsToStr();
    std::string reactOptionsToStr();
    std::string energyOptionsToStr(Network* network);
    std::string reportOptionsToStr();

  private:

    std::string  stringOptions[MAX_STRING_OPTIONS];
    int          indexOptions[MAX_INDEX_OPTIONS];
    double       valueOptions[MAX_VALUE_OPTIONS];
    int          timeOptions[MAX_TIME_OPTIONS];
    ReportFields reportFields;
};

//-----------------------------------------------------------------------------
//    Inline Functions
//-----------------------------------------------------------------------------

inline int Options::flowUnits()
       { return indexOptions[FLOW_UNITS]; }

inline int Options::pressureUnits()
       { return indexOptions[PRESSURE_UNITS]; }

inline std::string Options::stringOption(StringOption option)
       { return stringOptions[option]; }

inline int Options::indexOption(IndexOption option)
       { return indexOptions[option]; }

inline double Options::valueOption(ValueOption option)
       { return valueOptions[option]; }

inline int Options::timeOption(TimeOption option)
       { return timeOptions[option]; }

#endif
