#include "mainwindow.h"
#include "./ui_mainwindow.h"

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

bool init=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myOcct = new Opencascade(this);

    ui->gridLayout_opencascade->addWidget(myOcct);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_exec_pressed()
{
    myOcct->Set_orthographic();

    ARC arc = arcpoints().getArc({ui->lineEdit_p1x->text().toDouble(),
                                  ui->lineEdit_p1y->text().toDouble(),
                                  ui->lineEdit_p1z->text().toDouble()},

                                 {ui->lineEdit_p2x->text().toDouble(),
                                  ui->lineEdit_p2y->text().toDouble(),
                                  ui->lineEdit_p2z->text().toDouble()},

                                 {ui->lineEdit_p3x->text().toDouble(),
                                  ui->lineEdit_p3y->text().toDouble(),
                                  ui->lineEdit_p3z->text().toDouble()} , ui->spinBox_division->value()
                                 );

    ui->label_arc_lenght->setText(QString::number(arc.arcLenght));
    ui->label_radius->setText(QString::number(arc.radius));
     ui->label_diameter->setText(QString::number(arc.diameter));
    ui->label_center_x->setText(QString::number(arc.center.x));
    ui->label_center_y->setText(QString::number(arc.center.y));
    ui->label_center_z->setText(QString::number(arc.center.z));

    ui->label_arc_angle_rad->setText(QString::number(arc.arcAngleRad));
    ui->label_arc_angle_deg->setText(QString::number(arc.arcAngleRad*toDegrees));

    if(arc.arcAngleNegative){
        ui->label_signed_angle->setText("arc no signed.");
    } else {
        ui->label_signed_angle->setText("arc signed");
    }

    if(!init){
        //! Draw axis.
        Handle(AIS_Shape) aShape1=draw_primitives().draw_3d_line({0,0,0},{100,0,0});
        aShape1=draw_primitives().colorize(aShape1,Quantity_NOC_RED,0.9);
        myOcct->show_shape(aShape1);

        Handle(AIS_Shape) aShape2=draw_primitives().draw_3d_line({0,0,0},{0,100,0});
        aShape2=draw_primitives().colorize(aShape2,Quantity_NOC_GREEN,0.9);
        myOcct->show_shape(aShape2);

        Handle(AIS_Shape) aShape3=draw_primitives().draw_3d_line({0,0,0},{0,0,100});
        aShape3=draw_primitives().colorize(aShape3,Quantity_NOC_BLUE,0.9);
        myOcct->show_shape(aShape3);
        init=1;
    }

    //! Draw arc startpoint.
    Handle(AIS_Shape) aShapeStartPoint=draw_primitives().draw_3d_point({ui->lineEdit_p1x->text().toDouble(),
                                                                        ui->lineEdit_p1y->text().toDouble(),
                                                                        ui->lineEdit_p1z->text().toDouble()});
    myOcct->show_shape(aShapeStartPoint);

    //! Draw arc waypoint.
    Handle(AIS_Shape) aShapeWayPoint=draw_primitives().draw_3d_point({ui->lineEdit_p2x->text().toDouble(),
                                                                      ui->lineEdit_p2y->text().toDouble(),
                                                                      ui->lineEdit_p2z->text().toDouble()});
    myOcct->show_shape(aShapeWayPoint);

    //! Draw arc endpoint.
    Handle(AIS_Shape) aShapeEndPoint=draw_primitives().draw_3d_point({ui->lineEdit_p3x->text().toDouble(),
                                                                      ui->lineEdit_p3y->text().toDouble(),
                                                                      ui->lineEdit_p3z->text().toDouble()});
    myOcct->show_shape(aShapeEndPoint);

    //! Draw arc center.
    Handle(AIS_Shape) aShapeCenterPoint=draw_primitives().draw_3d_point({arc.center.x,
                                                                         arc.center.y,
                                                                         arc.center.z});
    myOcct->show_shape(aShapeCenterPoint);


    //! Copy result to a occt pointvec.
    std::vector<gp_Pnt> gpVec;
    for(uint i=0; i<arc.pntVec.size(); i++){
        gpVec.push_back({arc.pntVec.at(i).x,
                         arc.pntVec.at(i).y,
                         arc.pntVec.at(i).z});

        //! Draw arc center.
        Handle(AIS_Shape) aShapeCenterPoint=draw_primitives().draw_3d_point({arc.pntVec.at(i).x,
                                                                             arc.pntVec.at(i).y,
                                                                             arc.pntVec.at(i).z});
        myOcct->show_shape(aShapeCenterPoint);
    }

    Handle(AIS_Shape) aShape=draw_primitives().draw_3d_line_wire(gpVec);
    myOcct->show_shape(aShape);

    myOcct->redraw();
}
