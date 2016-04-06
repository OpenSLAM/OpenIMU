#include "widget.h"
#include "ui_widget.h"
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include "acquisition/AccelerometerReader.h"
#include <vector>

Widget::Widget(QWidget *parent) : QwtPlot(parent),
     ui(new Ui::Widget)
{
     ui->setupUi(this);
     m_folderPath = "";

}

Widget::~Widget()
{
     delete ui;
}
void Widget::setFolderPath(std::string path)
{
    m_folderPath = path;
}
void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void Widget::setupPlot() {
    this->detachItems(QwtPlotItem::Rtti_PlotItem, true);
    this->setTitle("Données acquises par l'accéléromètre");
    this->setCanvasBackground(QColor(Qt::white));
    this->setAutoReplot(false);
    this->setMinimumWidth(500);

    // legend
    QwtLegend *legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    this->insertLegend(legend, QwtPlot::BottomLegend);

    // axis
    this->setAxisTitle(QwtPlot::xBottom, "Axe temporelle (s)");
    this->setAxisTitle(QwtPlot::yLeft, "Axe x,y,z");

    //data
    // add curves
    QwtPlotCurve *curve1 = new QwtPlotCurve("Axe X");
    QwtPlotCurve *curve2 = new QwtPlotCurve("Axe Y");
    QwtPlotCurve *curve3 = new QwtPlotCurve("Axe Z");

    vector<signed short> x;
    vector<signed short> y;
    vector<signed short> z;
    vector<float> t;

    AccelerometerReader accReader(m_folderPath);

    accReader.LoadSensorData(false);
    vector<SensorDataPerDay> availableData = accReader.GetAccelerometerData();

    for(int i = 0; i< availableData.size();i++){

        for(int j = 0; j< availableData.at(i).getDataPerDay().size() ;j++){

            for(int k = 0; k <availableData.at(i).getDataPerDay().at(j).getAccelerometerDataPerHour().size();k++){
                for(float i=0.0;i<0.98;i+=0.02)
                {
                    t.push_back(i+k);
                }

                vector<signed short> tmpx = availableData.at(i).getDataPerDay().at(j).getAccelerometerDataPerHour().at(k).getXAxisValues();
                vector<signed short> tmpy = availableData.at(i).getDataPerDay().at(j).getAccelerometerDataPerHour().at(k).getYAxisValues();
                vector<signed short> tmpz = availableData.at(i).getDataPerDay().at(j).getAccelerometerDataPerHour().at(k).getZAxisValues();

                x.insert(x.end(),tmpx.begin(),tmpx.end());
                y.insert(y.end(),tmpy.begin(),tmpy.end());
                z.insert(z.end(),tmpz.begin(),tmpz.end());
           }

        }
    }

     QPolygonF points, points1, points2;

     for(unsigned int i = 0; i <x.size(); i++)
     {
       points << QPointF(t.at(i),x.at(i));
       points1 << QPointF(t.at(i),y.at(i));
       points2 << QPointF(t.at(i),z.at(i));
     }


    curve1->setPen(* new QPen(Qt::blue));
    curve1->setSamples(points);
    curve1->attach(this);

    curve2->setPen(* new QPen(Qt::red));
    curve2->setSamples(points1);
    curve2->attach(this);

    curve3->setPen(* new QPen(Qt::green));
    curve3->setSamples(points2);
    curve3->attach(this);

    this->replot();
}