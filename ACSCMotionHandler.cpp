#include "ACSCMotionHandler.h"
#include <QtMessageHandler>

//******************************************
ACSCMotionHandler::ACSCMotionHandler() :
    gantryConnected(false),
    xAxisEnabled(false),
    yAxisEnabled(false),
    zAxisEnabled(false),
    z_2_AxisEnabled(false),
    uAxisEnabled(false)
    {
    gantry = NULL;

    //    xAxis = (AXISMASK)AXISMASK_02;
    //    xIndex = (AXISINDEX)AXISINDEX_02;

    //    yAxis = (AXISMASK)AXISMASK_00; // AXISMASK_01 is YY, the y-axis slave
    //    yIndex = AXISINDEX_00;

    //    zAxis = (AXISMASK)AXISMASK_03;
    //    zIndex = AXISINDEX_03;

    //    uAxis = (AXISMASK)AXISMASK_04;
    //    uIndex = AXISINDEX_04;

    //    allAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00 | AXISMASK_03 | AXISMASK_04);
    //    xyAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00);
}

//******************************************
ACSCMotionHandler::~ACSCMotionHandler() {
    if (gantry != NULL) {
        if(acsc_DisableAll(gantry,ACSC_SYNCHRONOUS) == 0 )
            qWarning("Error stopping all axes: %d ",acsc_GetLastError());
        else
            qWarning("stopped all axes");
        //connection functions: see: SPiiPlus C Library Reference Programmer Guide.pdf
        if(!acsc_CloseComm(gantry))
            qWarning("Closing gantry connection error: %d ",acsc_GetLastError());
        else
            qWarning("Connection closed");
        ///////////////////////////////////////////////
    }
}

//******************************************
// connect to the gantry

//------------------------------------------
bool ACSCMotionHandler::connectGantry(bool flag)
{
    if (flag) {
        qInfo("Closing pending connections.");
        ACSC_CONNECTION_DESC connection_descriprtions[100];
        int NumberOfConnections = 0;
        if(acsc_GetConnectionsList(connection_descriprtions,100,&NumberOfConnections) == 0 )
            qInfo("Error getting number of connections: %d ",acsc_GetLastError());
        for(int i =0; i<NumberOfConnections;i++){
            QString str(connection_descriprtions[i].Application);
            if(acsc_TerminateConnection (& (connection_descriprtions[i])))
                qInfo("Error closing application %s connection: %d ",str.toLocal8Bit().constData(),acsc_GetLastError());
            else
                qInfo("Application %s connection closed.",str.toLocal8Bit().constData());
        }
        //end of closing pending connections
        qInfo("connecting gantry...");
        ///////////////////////////////////////////////
        //connection functions: see: SPiiPlus C Library Reference Programmer Guide.pdf
        int Port = 701;
        gantry = acsc_OpenCommEthernetTCP("10.0.0.100",Port); //see manual for reference <----
        ///////////////////////////////////////////////
        if(gantry == ACSC_INVALID){
            qWarning("Error init Gantry: %d ",acsc_GetLastError());
            qWarning("could not connect gantry");
            gantryConnected=false;
            return false;
        }else{
            qInfo("gantry connected");
            gantryConnected=true;
            return true;
        }
    } else {
        qInfo("disconnecting gantry...");
        if(!acsc_CloseComm(gantry)){
            qWarning("Closing gantry connection error: %d ",acsc_GetLastError());
            qWarning("could not disconnect gantry");
            return false;
        }else{
            qWarning("Connection closed");
            gantryConnected=false;
            return true;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::disconnectGantry()
{
    return connectGantry(false);
}

//------------------------------------------
bool ACSCMotionHandler::stop(){
    qInfo("stopping...");
    int Axes[6] = {X_axis,Y_axis,Z_axis,
                         Z_2_axis,U_axis,-1};
    if (acsc_BreakM(gantry,Axes,ACSC_SYNCHRONOUS) != 0) { //stop here
        qInfo("stop");
        return true;
    } else {
        qWarning("could not stop");
        qInfo("Error : %d ",acsc_GetLastError());
        return false;
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::acknowledgeMotionFaultGantry()
{
    qInfo("resetting errors...");
    if (true) { //acknowledge and clear axes faults
        qInfo("errors reset");
        return true;
    } else {
        qWarning("could not reset errors");
        return false;
    }
    return true;}


//------------------------------------------
bool ACSCMotionHandler::enableAxes(bool flag)
{
    if (flag) {
        qInfo("enabling axes...");
        int Axes[6] = {X_axis,Y_axis,Z_axis,
                             Z_2_axis,U_axis,-1};
        if (acsc_EnableM(gantry,Axes,ACSC_SYNCHRONOUS)) { //enable all axes here
            qInfo("axes enabled");
            return true;
        } else {
            qInfo("Error initiating axes: %d ",acsc_GetLastError());
            qWarning("could not enable axes");
            return false;
        }
    } else {
        qInfo("disabling axes...");
        if (acsc_DisableAll(gantry,ACSC_SYNCHRONOUS)) { //disable all axes here
            qInfo("axes disabled");
            return true;
        } else {
            qWarning("Error stopping all axes: %d ",acsc_GetLastError());
            qWarning("could not disable axes");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableXAxis(bool flag)
{
    if (flag) {
        qInfo("enabling x axis...");
        if (acsc_Enable(gantry,X_axis,ACSC_SYNCHRONOUS)) { //enable x axis here
            qInfo("x axis enabled");
            xAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis x: %d ",acsc_GetLastError());
            qInfo("could not enable x axis");
            return false;
        }
    } else {
        qInfo("disabling x axis...");
        if (acsc_Disable(gantry,X_axis,ACSC_SYNCHRONOUS)) {
            qInfo("x axis disabled");
            xAxisEnabled=false;
            return true;
        } else {
            qInfo("Error disabling axis X: %d ",acsc_GetLastError());
            qInfo("could not disable x axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableYAxis(bool flag)
{
    if (flag) {
        qInfo("enabling y axis...");
        if (acsc_Enable(gantry,Y_axis,ACSC_SYNCHRONOUS)) { //enable y axis here
            qInfo("y axis enabled");
            yAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis 1: %d ",acsc_GetLastError());
            qInfo("could not enable y axis");
            return false;
        }
    } else {
        qInfo("disabling y axis...");
        if (acsc_Disable(gantry,Y_axis,ACSC_SYNCHRONOUS)) {
            qInfo("Y axis disabled");
            yAxisEnabled=false;
            return true;
        } else {
            qInfo("Error disabling axis Y: %d ",acsc_GetLastError());
            qInfo("could not disable Y axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableZAxis(bool flag)
{
    if (flag) {
        qInfo("enabling z axis...");
        if (acsc_Enable(gantry,Z_axis,ACSC_SYNCHRONOUS)) { //enable z axis here
            qInfo("z axis enabled");
            zAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis z: %d ",acsc_GetLastError());
            qInfo("could not enable z axis");
            return false;
        }
    } else {
        qInfo("disabling z axis...");
        if (acsc_Disable(gantry,Z_axis,ACSC_SYNCHRONOUS)) {
            qInfo("Z axis disabled");
            zAxisEnabled=false;
            return true;
        } else {
            qInfo("Error disabling axis Z: %d ",acsc_GetLastError());
            qInfo("could not disable Z axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableZ_2_Axis(bool flag)
{
    if (flag) {
        qInfo("enabling z 2 axis...");
        if (acsc_Enable(gantry,Z_2_axis,ACSC_SYNCHRONOUS)) { //enable z axis here
            qInfo("z 2 axis enabled");
            z_2_AxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis z 2: %d ",acsc_GetLastError());
            qInfo("could not enable z 2 axis");
            return false;
        }
    } else {
        qInfo("disabling z 2 axis...");
        if (acsc_Disable(gantry,Z_2_axis,ACSC_SYNCHRONOUS)) {
            qInfo("Z2 axis disabled");
            z_2_AxisEnabled=false;
            return true;
        } else {
            qInfo("Error disabling axis Z: %d ",acsc_GetLastError());
            qInfo("could not disable Z axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::enableUAxis(bool flag)
{
    if (flag) {
        qInfo("enabling u axis...");
        if (acsc_Enable(gantry,U_axis,ACSC_SYNCHRONOUS)) { //enable u axis here
            qInfo("u axis enabled");
            uAxisEnabled=true;
            return true;
        } else {
            qInfo("Error initiating axis 3: %d ",acsc_GetLastError());
            qInfo("could not enable u axis");
            return false;
        }
    } else {
        qInfo("disabling u axis...");
        if (acsc_Disable(gantry,U_axis,ACSC_SYNCHRONOUS)) {
            qInfo("U axis disabled");
            uAxisEnabled=false;
            return true;
        } else {
            qInfo("Error disabling axis U: %d ",acsc_GetLastError());
            qInfo("could not disable U axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::disableAxes()
{
    return enableAxes(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableXAxis()
{
    return enableXAxis(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableYAxis()
{
    return enableYAxis(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableZAxis()
{
    return enableZAxis(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableZ_2_Axis()
{
    return enableZ_2_Axis(false);
}

//------------------------------------------
bool ACSCMotionHandler::disableUAxis()
{
    return enableUAxis(false);
}

//******************************************
// Set Limits
bool ACSCMotionHandler::SetLimitsController()
{
//setting limits in the controller, the machine will stop if it gets outside of these boundries
//See Commmand and Variable Reference Guide for more info
    std::string temp = "SLLIMIT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&x_min,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position X axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&y_min,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Y axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&z_1_min,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z1 axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&z_2_min,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z2 axis: %d ",acsc_GetLastError());

    std::string temp2 = "SRLIMIT";
    char * tab2 = new char [temp2.length()+1];
    strcpy (tab2, temp2.c_str());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&x_max,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position X axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&y_max,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Y axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&z_1_max,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z1 axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&z_2_max,ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z2 axis: %d ",acsc_GetLastError());
    return true;
}

// Set Limits
bool ACSCMotionHandler::SetLimitsController(std::vector <double> & limits)
{
//setting limits in the controller, the machine will stop if it gets outside of these boundries
//See Commmand and Variable Reference Guide for more info
    if(limits.size() != 8){
        qWarning("Error set limit position. wrong limits size");
        return false;
    }
    std::string temp = "SLLIMIT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    if(limits.at(0) > limits.at(4))
        limits.at(0) = limits.at(4) -1.;
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&limits.at(0),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position X axis: %d ",acsc_GetLastError());
    if(limits.at(1) > limits.at(5))
        limits.at(1) = limits.at(5) -1.;
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&limits.at(1),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Y axis: %d ",acsc_GetLastError());
    if(limits.at(2) > limits.at(6))
        limits.at(2) = limits.at(6) -1.;
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&limits.at(2),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z1 axis: %d ",acsc_GetLastError());
    if(limits.at(3) > limits.at(7))
        limits.at(3) = limits.at(7) -1.;
    if(acsc_WriteReal(gantry,ACSC_NONE,tab,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&limits.at(3),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z2 axis: %d ",acsc_GetLastError());

    std::string temp2 = "SRLIMIT";
    char * tab2 = new char [temp2.length()+1];
    strcpy (tab2, temp2.c_str());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&limits.at(4),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position X axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&limits.at(5),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Y axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&limits.at(6),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z1 axis: %d ",acsc_GetLastError());
    if(acsc_WriteReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&limits.at(7),ACSC_SYNCHRONOUS)==0)
        qWarning("Error set limit position Z2 axis: %d ",acsc_GetLastError());
    return true;
}


bool ACSCMotionHandler::GetLimitsController(std::vector <double> & limits){
    std::string temp = "SLLIMIT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    std::vector <double> output;
    output.clear();
    double output_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position X axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position Y axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position Z1 axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position Z2 axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);

    std::string temp2 = "SRLIMIT";
    char * tab2 = new char [temp2.length()+1];
    strcpy (tab2, temp2.c_str());
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position X axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position Y axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position Z1 axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&output_temp,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get position Z2 axis: %d ",acsc_GetLastError());
    output.push_back(output_temp);
    limits.clear();
    limits = output;
    return true;
};

int ACSCMotionHandler::GetfaultSateXAxis(){
    std::string temp = "FAULT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    int fault_state = 1;
    if(acsc_ReadInteger(gantry,ACSC_NONE,tab,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&fault_state,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get fault X axis: %d ",acsc_GetLastError());
    return fault_state;
};

int ACSCMotionHandler::GetfaultSateYAxis(){
    std::string temp = "FAULT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    int fault_state = 1;
    if(acsc_ReadInteger(gantry,ACSC_NONE,tab,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&fault_state,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get fault Y axis: %d ",acsc_GetLastError());
    return fault_state;
};

int ACSCMotionHandler::GetfaultSateZ1Axis(){
    std::string temp = "FAULT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    int fault_state = 1;
    if(acsc_ReadInteger(gantry,ACSC_NONE,tab,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&fault_state,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get fault Z1 axis: %d ",acsc_GetLastError());
    return fault_state;
};

int ACSCMotionHandler::GetfaultSateZ2Axis(){
    std::string temp = "FAULT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    int fault_state = 1;
    if(acsc_ReadInteger(gantry,ACSC_NONE,tab,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&fault_state,ACSC_SYNCHRONOUS)==0)
        qWarning("Error get fault Z2 axis: %d ",acsc_GetLastError());
    return fault_state;
};

//******************************************
// home axes

//------------------------------------------
bool ACSCMotionHandler::home() {

    qInfo("Running homing buffer...");
    acsc_RunBuffer(gantry,1,NULL,ACSC_SYNCHRONOUS);
    //    qInfo("homing axes...");
    //    homeX();
    //    homeY();
    //    homeZ();
    //    homeZ_2();
    //    homeU();
    qInfo("Done...");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeX() {
    qInfo("homing x axis...");
    moveXTo(Home_coord[0],default_speed);
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeY() {
    qInfo("homing y axis...");
    moveYTo(Home_coord[1],default_speed);
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeZ() {
    qInfo("homing z axis...");
    moveZTo(Home_coord[2],default_speed);
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeZ_2() {
    qInfo("homing z 2 axis...");
    moveZTo(Home_coord[4],default_speed);
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::homeU() {
    qInfo("homing u axis...");
    moveUTo(Home_coord[3],default_angular_speed);
    return true;
}

//******************************************
// absolute motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool ACSCMotionHandler::moveTo(double x, double y, double z, double speed)
{
    x = x+0.01;
    y = y+0.01;
    z = z+0.01;
    speed = speed + 0.01;
    qInfo("Deprecated in Valencia, use the function with array input");
    return true;
}
//------------------------------------------
bool ACSCMotionHandler::moveTo(double positions[4], double speed)
{
    qInfo("moving to (%.3f mm, %.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed...",
          positions[0],positions[1],positions[2],positions[3], speed);
    moveXTo(positions[0],speed);
    moveYTo(positions[1],speed);
    moveZTo(positions[2],speed);
    moveZ_2_To(positions[3],speed);
    //qInfo("...");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveXTo(double x, double speed) {
    qInfo("moving x axis to %.3f mm at %.3f mm/s speed", x, speed);
    //may change to acsc_ExtToPoint sec 4.15 C library manual
    //validate target position
    if(!validate_target_pos_x(x))
        return false;
    if(acsc_SetVelocity(gantry,X_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed X axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,X_axis,x,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,X_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion X axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved x axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion X axis: %d ",acsc_GetLastError());
        qWarning("could not move x axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveYTo(double y, double speed) {
    //validate target position
    if(!validate_target_pos_y(y))
        return false;
    qInfo("moving y axis to %.3f mm at %.3f mm/s speed", y, speed);
    if(acsc_SetVelocity(gantry,Y_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Y axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,Y_axis,y,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Y_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Y axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Y axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Y axis: %d ",acsc_GetLastError());
        qWarning("could not move Y axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveZTo(double z, double speed) {
    //validate target position
    if(!validate_target_pos_z_1(z))
        return false;
    qInfo("moving z axis to %.3f mm at %.3f mm/s speed", z, speed);
    if(acsc_SetVelocity(gantry,Z_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Z axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,Z_axis,z,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Z_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Z axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Z axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Z axis: %d ",acsc_GetLastError());
        qWarning("could not move Z axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveZ_2_To(double z, double speed) {
    //validate target position
    if(!validate_target_pos_z_2(z))
        return false;
    qInfo("moving z 2 axis to %.3f mm at %.3f mm/s speed", z, speed);
    if(acsc_SetVelocity(gantry,Z_2_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Z axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,Z_2_axis,z,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Z_2_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Z 2 axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Z 2 axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Z 2 axis: %d ",acsc_GetLastError());
        qWarning("could not move Z 2 axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveUTo(double u, double speed) {
    qInfo("moving U axis to %.3f <set angle units> at %.3f <set angle units>/s speed", u, speed);
    if(acsc_SetVelocity(gantry,U_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed U axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,U_axis,u,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,U_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion U axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved U axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion U axis: %d ",acsc_GetLastError());
        qWarning("could not move U axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//******************************************
// step  motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool ACSCMotionHandler::moveBy(double x, double y, double z,double speed)
{
    x = x+0.01;
    y = y+0.01;
    z = z+0.01;
    speed = speed + 0.01;
    qInfo("Deprecated in Valencia, use the function with array input");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveBy(double positions[4], double speed)
{
    qInfo("moving to (%.3f mm, %.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed...",
          positions[0],positions[1],positions[2],positions[3],speed);
    moveXTo(positions[0],speed);
    moveYTo(positions[1],speed);
    moveZTo(positions[2],speed);
    moveZ_2_To(positions[3],speed);
    //qInfo("...");
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveXBy(double x, double speed) {
    std::vector <double> pos_t = whereAmI(0);
    //validate target position
    if(!validate_target_pos_x(x+pos_t[0]))
        return false;
    qInfo("moving x axis to %.3f mm at %.3f mm/s speed", x, speed);
    if(acsc_SetVelocity(gantry,X_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed X axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,ACSC_AMF_RELATIVE,X_axis,x,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,X_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion X axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved x axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion X axis: %d ",acsc_GetLastError());
        qWarning("could not move x axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveYBy(double y, double speed) {
    std::vector <double> pos_t = whereAmI(0);
    //validate target position
    if(!validate_target_pos_y(y+pos_t[1]))
        return false;
    qInfo("moving y axis to %.3f mm at %.3f mm/s speed", y, speed);
    if(acsc_SetVelocity(gantry,Y_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Y axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,ACSC_AMF_RELATIVE,Y_axis,y,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Y_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Y axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Y axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Y axis: %d ",acsc_GetLastError());
        qWarning("could not move Y axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveZBy(double z, double speed) {
    std::vector <double> pos_t = whereAmI(0);
    //validate target position
    if(!validate_target_pos_z_1(z+pos_t[2]))
        return false;
    qInfo("moving z axis to %.3f mm at %.3f mm/s speed", z, speed);
    if(acsc_SetVelocity(gantry,Z_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Z axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,ACSC_AMF_RELATIVE,Z_axis,z,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Z_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Z axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Z axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Z axis: %d ",acsc_GetLastError());
        qWarning("could not move Z axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveZ_2_By(double z, double speed) {
    std::vector <double> pos_t = whereAmI(0);
    //validate target position
    if(!validate_target_pos_z_2(z+pos_t[4]))
        return false;
    qInfo("moving z 2 axis to %.3f mm at %.3f mm/s speed", z, speed);
    if(acsc_SetVelocity(gantry,Z_2_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed Z 2 axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,ACSC_AMF_RELATIVE,Z_2_axis,z,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,Z_2_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion Z 2 axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved Z 2 axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion Z 2 axis: %d ",acsc_GetLastError());
        qWarning("could not move Z 2 axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//------------------------------------------
bool ACSCMotionHandler::moveUBy(double u, double speed) {
    qInfo("moving U axis to %.3f <set angle units> at %.3f <set angle units>/s speed", u, speed);
    if(acsc_SetVelocity(gantry,U_axis,speed,ACSC_SYNCHRONOUS) == 0)
        qWarning("Error gantry, setting speed U axis: %d ",acsc_GetLastError());
    if (acsc_ToPoint(gantry,0,U_axis,u,ACSC_SYNCHRONOUS) != 0) { //move to destination here
        if(acsc_WaitMotionEnd(gantry,U_axis,TimeOut) == 0)
            qWarning("Error gantry, waiting motion U axis to end: %d ",acsc_GetLastError());//wait
        qInfo("moved U axis to destination");
        emit updatePositions_s();
        return true;
    } else {
        qWarning("Error gantry, motion U axis: %d ",acsc_GetLastError());
        qWarning("could not move U axis to destination");
        emit updatePositions_s();
        return false;
    }
    emit updatePositions_s();
    return true;
}

//******************************************
// free run
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool ACSCMotionHandler::runX(double direction, double speed)
{
    double sign = direction<0?-1.:1.;
    speed = fabs(speed);
    qInfo("free running %s X axis at %.1f mm/s", direction<0?"-":"+", speed);
    if(!acsc_Jog(gantry,ACSC_AMF_VELOCITY,X_axis,sign*speed,ACSC_SYNCHRONOUS)){
        qInfo("running X axis");
        return true;
    }else{
        qWarning("Error gantry, jog motion X axis: %d ",acsc_GetLastError());
        qWarning("Could not free run along X axis");
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::endRunX()
{
    if(!acsc_Halt(gantry,X_axis,ACSC_SYNCHRONOUS)){
        qInfo("STOP running X axis");
        return true;
    }else{
        qWarning("Error gantry,stop jog motion X axis: %d ",acsc_GetLastError());
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::runY(double direction, double speed)
{
    double sign = direction<0?-1.:1.;
    speed = fabs(speed);
    qInfo("free running %s Y axis at %.1f mm/s", direction<0?"-":"+", speed);
    if(!acsc_Jog(gantry,ACSC_AMF_VELOCITY,Y_axis,sign*speed,ACSC_SYNCHRONOUS)){
        qInfo("running Y axis");
        return true;
    }else{
        qWarning("Error gantry, jog motion Y axis: %d ",acsc_GetLastError());
        qWarning("Could not free run along Y axis");
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::endRunY()
{
    if(!acsc_Halt(gantry,Y_axis,ACSC_SYNCHRONOUS)){
        qInfo("STOP running Y axis");
        return true;
    }else{
        qWarning("Error gantry,stop jog motion Y axis: %d ",acsc_GetLastError());
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::runZ(double direction, double speed)
{
    double sign = direction<0?-1.:1.;
    speed = fabs(speed);
    qInfo("free running %s Z axis at %.1f mm/s", direction<0?"-":"+", speed);
    if(!acsc_Jog(gantry,ACSC_AMF_VELOCITY,Z_axis,sign*speed,ACSC_SYNCHRONOUS)){
        qInfo("running Z axis");
        return true;
    }else{
        qWarning("Error gantry, jog motion Z axis: %d ",acsc_GetLastError());
        qWarning("Could not free run along Z axis");
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::endRunZ()
{
    if(!acsc_Halt(gantry,Z_axis,ACSC_SYNCHRONOUS)){
        qInfo("STOP running Z axis");
        return true;
    }else{
        qWarning("Error gantry,stop jog motion Z axis: %d ",acsc_GetLastError());
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::runZ_2(double direction, double speed)
{
    double sign = direction<0?-1.:1.;
    speed = fabs(speed);
    qInfo("free running %s Z 2 axis at %.1f mm/s", direction<0?"-":"+", speed);
    if(!acsc_Jog(gantry,ACSC_AMF_VELOCITY,Z_2_axis,sign*speed,ACSC_SYNCHRONOUS)){
        qInfo("running Z 2 axis");
        return true;
    }else{
        qWarning("Error gantry, jog motion Z 2 axis: %d ",acsc_GetLastError());
        qWarning("Could not free run along Z 2 axis");
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::endRunZ_2()
{
    if(!acsc_Halt(gantry,Z_2_axis,ACSC_SYNCHRONOUS)){
        qInfo("STOP running Z 2 axis");
        return true;
    }else{
        qWarning("Error gantry,stop jog motion Z 2 axis: %d ",acsc_GetLastError());
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::runU(double direction, double speed)
{
    double sign = direction<0?-1.:1.;
    speed = fabs(speed);
    qInfo("free running %s U axis at %.1f rad/s", direction<0?"-":"+", speed);
    if(!acsc_Jog(gantry,ACSC_AMF_VELOCITY,U_axis,sign*speed,ACSC_SYNCHRONOUS)){
        qInfo("running U axis");
        return true;
    }else{
        qWarning("Error gantry, jog motion U axis: %d ",acsc_GetLastError());
        qWarning("Could not free run along U axis");
        return false;
    }
}

//------------------------------------------
bool ACSCMotionHandler::endRunU()
{
    if(!acsc_Halt(gantry,U_axis,ACSC_SYNCHRONOUS)){
        qInfo("STOP running U axis");
        return true;
    }else{
        qWarning("Error gantry,stop jog motion U axis: %d ",acsc_GetLastError());
        return false;
    }
}

//******************************************
//gantry current position
std::vector<double> ACSCMotionHandler::whereAmI(int ific_value) {
    std::vector<double> position = {-99990.0,-99990.0,-99990.0,-99990.0,-99990.0};
    double position_tmp = -99999.9;
    if(!gantryConnected)
        return position;
    //try difference with acsc_GetTargetPosition sec 4.16 C library manual
    //try also APOS variable with acsc_ReadReal function (4.4.3)
    //try difference with acsc_GetRPosition sec 4.12 C library manual
    //try difference with acsc_GetFPosition sec 4.12 C library manual
    if(ific_value == 0){
        if(acsc_GetFPosition(gantry,X_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
            qWarning("Error get position X axis: %d ",acsc_GetLastError());
        position[0] = position_tmp;

        if(acsc_GetFPosition(gantry,Y_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
            qWarning("Error get position Y axis: %d ",acsc_GetLastError());
        position[1] = position_tmp;

        if(acsc_GetFPosition(gantry,Z_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
            qWarning("Error get position Z axis: %d ",acsc_GetLastError());
        position[2] = position_tmp;

        if(acsc_GetFPosition(gantry,U_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
            qWarning("Error get position U axis: %d ",acsc_GetLastError());
        position[3] = position_tmp;

        if(acsc_GetFPosition(gantry,Z_2_axis,&position_tmp,ACSC_SYNCHRONOUS) == 0)
            qWarning("Error get position Z 2 axis: %d ",acsc_GetLastError());
        position[4] = position_tmp;
    } else if(ific_value == 1){
        std::string temp = "APOS";
        char * tab = new char [temp.length()+1];
        strcpy (tab, temp.c_str());
        if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&position_tmp,ACSC_SYNCHRONOUS)==0)
            qWarning("Error get position X axis: %d ",acsc_GetLastError());
        position[0] = position_tmp;

        if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&position_tmp,ACSC_SYNCHRONOUS)==0)
            qWarning("Error get position Y axis: %d ",acsc_GetLastError());
        position[1] = position_tmp;

        if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&position_tmp,ACSC_SYNCHRONOUS)==0)
            qWarning("Error get position Z axis: %d ",acsc_GetLastError());
        position[2] = position_tmp;

        if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_6,ACSC_AXIS_6,ACSC_NONE,ACSC_NONE,&position_tmp,ACSC_SYNCHRONOUS)==0)
            qWarning("Error get position U axis: %d ",acsc_GetLastError());
        position[3] = position_tmp;

        if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&position_tmp,ACSC_SYNCHRONOUS)==0)
            qWarning("Error get position Z 2 axis: %d ",acsc_GetLastError());
        position[4] = position_tmp;
    }
    return position;
}

//******************************************
//Get Axis states
//------------------------------------------
bool ACSCMotionHandler::getXAxisState(){
    int State;
    if(!gantryConnected)
        return false;
    if(!acsc_GetMotorState(gantry,X_axis,&State,ACSC_SYNCHRONOUS)){
        qWarning("Error get X axis state: %d ",acsc_GetLastError());
    }else{
        xAxisEnabled = static_cast<bool>(State & ACSC_MST_ENABLE);
    }
    return xAxisEnabled;
}

//------------------------------------------
bool ACSCMotionHandler::getYAxisState(){
    int State;
    if(!gantryConnected)
        return false;
    if(!acsc_GetMotorState(gantry,Y_axis,&State,ACSC_SYNCHRONOUS)){
        qWarning("Error get Y axis state: %d ",acsc_GetLastError());
    }else{
        yAxisEnabled = static_cast<bool>(State & ACSC_MST_ENABLE);
    }
    return yAxisEnabled;
}

//------------------------------------------
bool ACSCMotionHandler::getZAxisState(){
    int State;
    if(!gantryConnected)
        return false;
    if(!acsc_GetMotorState(gantry,Z_axis,&State,ACSC_SYNCHRONOUS)){
        qWarning("Error get Z axis state: %d ",acsc_GetLastError());
    }else{
        zAxisEnabled = static_cast<bool>(State & ACSC_MST_ENABLE);
    }
    return zAxisEnabled;
}

//------------------------------------------
bool ACSCMotionHandler::getZ_2_AxisState(){
    int State;
    if(!gantryConnected)
        return false;
    if(!acsc_GetMotorState(gantry,Z_2_axis,&State,ACSC_SYNCHRONOUS)){
        qWarning("Error get Z2 axis state: %d ",acsc_GetLastError());
    }else{
        z_2_AxisEnabled = static_cast<bool>(State & ACSC_MST_ENABLE);
    }
    return z_2_AxisEnabled;
}

//------------------------------------------
bool ACSCMotionHandler::getUAxisState(){
    int State;
    if(!gantryConnected)
        return false;
    if(!acsc_GetMotorState(gantry,U_axis,&State,ACSC_SYNCHRONOUS)){
        qWarning("Error get U axis state: %d ",acsc_GetLastError());
    }else{
        uAxisEnabled = static_cast<bool>(State & ACSC_MST_ENABLE);
    }
    return uAxisEnabled;
}

//******************************************
// Safety limits for movement
bool ACSCMotionHandler::validate_target_pos_x(double val){
    std::string temp = "SLLIMIT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    double min_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&min_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position X axis: %d ",acsc_GetLastError());
        return false;
    }
    std::string temp2 = "SRLIMIT";
    char * tab2 = new char [temp2.length()+1];
    strcpy (tab2, temp2.c_str());
    double max_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_0,ACSC_AXIS_0,ACSC_NONE,ACSC_NONE,&max_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position X axis: %d ",acsc_GetLastError());
        return false;
    }
    if(val < min_lim_temp || val > max_lim_temp){
        qWarning("ERROR!! Target X position is NOT valid, aborting motion.");
        return false;
    } else {
        return true;
    }
}

bool ACSCMotionHandler::validate_target_pos_y(double val){
    std::string temp = "SLLIMIT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    double min_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&min_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position Y axis: %d ",acsc_GetLastError());
        return false;
    }
    std::string temp2 = "SRLIMIT";
    char * tab2 = new char [temp2.length()+1];
    strcpy (tab2, temp2.c_str());
    double max_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_1,ACSC_AXIS_1,ACSC_NONE,ACSC_NONE,&max_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position Y axis: %d ",acsc_GetLastError());
        return false;
    }
    if(val < min_lim_temp || val > max_lim_temp){
        qWarning("ERROR!! Target Y position is NOT valid, aborting motion.");
        return false;
    } else {
        //add control on step
        return true;
    }
}

bool ACSCMotionHandler::validate_target_pos_z_1(double val){
    std::string temp = "SLLIMIT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    double min_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&min_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position Z1 axis: %d ",acsc_GetLastError());
        return false;
    }
    std::string temp2 = "SRLIMIT";
    char * tab2 = new char [temp2.length()+1];
    strcpy (tab2, temp2.c_str());
    double max_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_5,ACSC_AXIS_5,ACSC_NONE,ACSC_NONE,&max_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position Z1 axis: %d ",acsc_GetLastError());
        return false;
    }
    if(val < min_lim_temp || val > max_lim_temp){
        qWarning("ERROR!! Target Z1 position is NOT valid, aborting motion.");
        return false;
    } else {
        //add control on step
        return true;
    }
}

bool ACSCMotionHandler::validate_target_pos_z_2(double val){
    std::string temp = "SLLIMIT";
    char * tab = new char [temp.length()+1];
    strcpy (tab, temp.c_str());
    double min_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&min_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position Z2 axis: %d ",acsc_GetLastError());
        return false;
    }
    std::string temp2 = "SRLIMIT";
    char * tab2 = new char [temp2.length()+1];
    strcpy (tab2, temp2.c_str());
    double max_lim_temp = 0.0;
    if(acsc_ReadReal(gantry,ACSC_NONE,tab2,ACSC_AXIS_4,ACSC_AXIS_4,ACSC_NONE,ACSC_NONE,&max_lim_temp,ACSC_SYNCHRONOUS)==0){
        qWarning("Error get position Z2 axis: %d ",acsc_GetLastError());
        return false;
    }
    if(val < min_lim_temp || val > max_lim_temp){
        qWarning("ERROR!! Target Z2 position is NOT valid, aborting motion.");
        return false;
    } else {
        //add control on step
        return true;
    }
}
