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
    enum QualType      {NOQUAL, AGE, TRACE, CHEM};			//ˮ�䡢�ټ�����ѧ��
    enum QualUnits     {NOUNITS, HRS, PCNT, MGL, UGL};
    enum ReportedItems {NONE, ALL, SOME};

    // ... Options with string values

    enum StringOption {
        HYD_FILE_NAME,         //!< Name of binary file containing hydraulic results		//����ˮ���������Ķ������ļ�
        OUT_FILE_NAME,         //!< Name of binary file containing simulation results		//����ģ��������Ķ������ļ�
        RPT_FILE_NAME,         //!< Name of text file containing output report				//���������ı��ļ�
        MAP_FILE_NAME,         //!< Name of text file containing nodal coordinates			//�ڵ�������ı��ļ�

        HEADLOSS_MODEL,        //!< Name of head loss model used							//ˮͷ��ʧģʽ
        DEMAND_MODEL,          //!< Name of nodal demand model used							//�ڵ�����ģʽ
        LEAKAGE_MODEL,         //!< Name of pipe leakage model used							//�ܵ�й©ģʽ
        HYD_SOLVER,            //!< Name of hydraulic solver method							//ˮ������ģʽ
        STEP_SIZING,           //!< Name of Newton step size method							//ţ�ٲ�����ģʽ
        MATRIX_SOLVER,         //!< Name of sparse matrix eqn. solver						//ϡ�����eqn���
        DEMAND_PATTERN_NAME,   //!< Name of global demand pattern							//ȫ������ģʽ����

        QUAL_MODEL,            //!< Name of water quality model used						//ˮ�ʼ���ģʽ
        QUAL_NAME,             //!< Name of water quality constituent						//ˮ�ʼ������ƣ�"NONE", "AGE", "TRACE", "CHEMICAL"
        QUAL_UNITS_NAME,       //!< Name of water quality units								//ˮ�ʵ�λ
        TRACE_NODE_NAME,       //!< Name of node for source tracing							//ˮ��׷�ٽڵ�

        MAX_STRING_OPTIONS
    };

    // ... Options with integer, categorical or yes/no values	���������ࡢ�Ƿ�ѡ��ֵ

    enum IndexOption {
        UNIT_SYSTEM,           //!< Choice of units system									ϵͳ��λ
        FLOW_UNITS,            //!< Choice of flow rate units								������λ
        PRESSURE_UNITS,        //!< Choice of pressure units								ѹ����λ
        MAX_TRIALS,            //!< Maximum hydraulic trials								���Һѹʵ��
        IF_UNBALANCED,         //!< Stop or continue if network is unbalanced				������ƽ��ʱ��ֹͣ�����
        HYD_FILE_MODE,         //!< Binary hydraulics file mode								������ˮ���ļ�ģʽ
        DEMAND_PATTERN,        //!< Global demand pattern index								ȫ������ģʽ���
        ENERGY_PRICE_PATTERN,  //!< Global energy price pattern index						ȫ����Դ�۸�ģʽ���

        QUAL_TYPE,             //!< Type of water quality analysis							ˮ�ʼ�������
        QUAL_UNITS,            //!< Units of the quality constituent					    ˮ�ʳɷֵ�λ
        TRACE_NODE,            //!< Node index for source tracing							ˮ��׷�ٽڵ���

        REPORT_SUMMARY,        //!< report input/output summary								��������/���ժҪ
        REPORT_ENERGY,         //!< report energy usage										������Դʹ��״��
        REPORT_STATUS,         //!< report system status									����ϵͳ״̬
        REPORT_TRIALS,         //!< report result of each trial								ÿ��ʵ��ı�����
        REPORT_NODES,          //!< report node results										����ڵ���
        REPORT_LINKS,          //!< report link results										������߽��

        MAX_INDEX_OPTIONS
    };

    // ... Options with numerical values

    enum ValueOption {

        // Hydraulic properties	ˮ������
        SPEC_GRAVITY,          //!< Specific Gravity								����
        KIN_VISCOSITY,         //!< Kinematic viscosity (ft2/sec)					�˶�ճ��
        DEMAND_MULTIPLIER,     //!< Global base demand multiplier					ȫ�ֻ����������
        MINIMUM_PRESSURE,      //!< Global minimum pressure to supply demand (ft)	ȫ�ֹ�Ӧ�������Сѹ��
        SERVICE_PRESSURE,      //!< Global pressure to supply full demand (ft)		ȫ�ֹ�Ӧ�������ѹ��
        PRESSURE_EXPONENT,     //!< Global exponent for power function demands		�������������ȫ��ָ��
        EMITTER_EXPONENT,      //!< Global exponent in emitter discharge formula	������������ʽ��ȫ��ָ��
        LEAKAGE_COEFF1,
        LEAKAGE_COEFF2,

        // Hydraulic tolerances	Һѹ����
        RELATIVE_ACCURACY,     //!< sum of all |flow changes| / sum of all |flows|	����������仯ƽ����
        HEAD_TOLERANCE,        //!< Convergence tolerance for head loss balance		ˮͷ��ʧƽ�������ݲ�
        FLOW_TOLERANCE,        //!< Convergence tolerance for flow balance			����ƽ��������ݲ�
        FLOW_CHANGE_LIMIT,     //!< Max. flow change for convergence				�����������仯���ֵ
        TIME_WEIGHT,           //!< Time weighting for variable head tanks			ˮ��ˮͷ�仯��Ȩֵ

        // Water quality options	ˮ��ѡ��
        MOLEC_DIFFUSIVITY,     //!< Chemical's molecular diffusivity (ft2/sec)		��ѧ����ɢϵ��
        QUAL_TOLERANCE,        //!< Tolerance for water quality comparisons			ˮ�ʱȽ��ݲ�
        BULK_ORDER,            //!< Order of all bulk flow reactions in pipes		�ܵ������д�������Ӧ��˳��
        WALL_ORDER,            //!< Order of all pipe wall reactions				���йܱڷ�Ӧ��˳��
        TANK_ORDER,            //!< Order of all bulk water reactions in tanks		ˮ�������д�������Ӧ��˳��
        BULK_COEFF,            //!< Global rate coefficient for bulk reactions		ȫ�ִ�Ӧϵ��
        WALL_COEFF,            //!< Global rate coefficient for wall reactions		ȫ�ֹܱڷ�Ӧϵ��
        LIMITING_CONCEN,       //!< Maximum concentration for growth reactions		������Ӧ�����Ũ��
        ROUGHNESS_FACTOR,      //!< Relates wall reaction coeff. to pipe roughness	��ܱڷ�Ӧϵ���йصĹܵ��ֲڶ�

        // Energy options	��Դѡ��
        ENERGY_PRICE,          //!< Global energy price (per kwh)					ȫ����Դ�۸�
        PEAKING_CHARGE,        //!< Fixed energy charge per peak kw					ÿ��ֵǧ�߹̶���Դ����
        PUMP_EFFICIENCY,       //!< Global pump efficiency (fraction)				ȫ�ֱ�Ч�ʣ��ٷ�����

        MAX_VALUE_OPTIONS
    };

    // ... Time options (in integer seconds) ʱ��ѡ���λ�����룩

    enum TimeOption {
        START_TIME,            //!< Starting time of day							��ʼʱ��
        HYD_STEP,              //!< Hydraulic simulation time step					ˮ��ģ��ʱ����
        QUAL_STEP,             //!< Water quality simulation time step				ˮ�ʼ���ʱ����
        PATTERN_STEP,          //!< Global time interval for time patterns			ȫ��ʱ��ģʽ���
        PATTERN_START,         //!< Time of day at which all time patterns start	����ʱ��ģʽ��ʼʱ��
        REPORT_STEP,           //!< Reporting time step								����ʱ����
        REPORT_START,          //!< Simulation time at which reporting begins		���濪ʼʱ��
        RULE_STEP,             //!< Time step used to evaluate control rules		�������ƹ���ʱ�䲽
        TOTAL_DURATION,        //!< Total simulation duration						��ģ�����ʱ��
        REPORT_STATISTIC,      //!< How results are reported (min, max, range)		��������ʽ����Сֵ�����ֵ����Χֵ��

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
