#ifndef CALDIFSPEEDINSPHERE_H
#define CALDIFSPEEDINSPHERE_H

#include <QWidget>
#include "globaldefine.h"
#define D_PI                    3.141592654
#define MINRADIUSOFSPHERE       1000.0
#define MAXRADIUSOFSPHERE       10000.0
#define MINRADIAN               0.0
#define MAXRADIAN               2*D_PI
#define MINANGLE                0.0
#define MAXANGLE                360.0

struct RadiusSp
{
    explicit RadiusSp(double radius) :m_radius(radius){}
    double m_radius;
};
struct CrawlerWidth
{
    CrawlerWidth(double width):m_width(width) {}
    double m_width;
};
struct RadianCenter
{
    RadianCenter(double radian):m_radianCenter(radian) {}
    double m_radianCenter;
};
struct PerimeterOfCenter
{
    PerimeterOfCenter(double perimeter): m_perimeter(perimeter){}
    double m_perimeter;
};

namespace Ui {
class CalDifSpeedInSphere;
}

class CalDifSpeedInSphere : public QWidget
{
    Q_OBJECT

public:
    explicit CalDifSpeedInSphere(QWidget *parent = 0);
    ~CalDifSpeedInSphere();

    double getCrawlerBothSidesRatio_Sin(const RadiusSp & radius = RadiusSp(1500),const CrawlerWidth& width = CrawlerWidth(300),const RadianCenter& radian = RadianCenter(1));

private slots:
    void on_CenterAngle_editingFinished();

    void on_crawlerWidth_editingFinished();

    void on_radiusOfSphere_editingFinished();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    void initUI();
    void setAllParam(const RadiusSp & radius = RadiusSp(1500),const CrawlerWidth& width = CrawlerWidth(300),const RadianCenter& radian = RadianCenter(1));

    void setRadiusOfSphere(const RadiusSp & radius);
    double getRadiusOfSphere();

    void setCrawlerWidth(const CrawlerWidth& width);
    CrawlerWidth getCrawlerWidth();

    void setRadianOfCrawlerCenter(const RadianCenter& radian);
    RadianCenter getRadianOfCrawlerCenter();

    double radianCenterToPerimeter(double radian);
    double perimeterToRadianCenter(double perimeter);
    //double getRatioOfRing(const RadiusSp & radius,const CrawlerWidth& width,const RadianCenter& radian);
    double CalMinRadiusOfCrawlerWidth();
    double CalMinRadiusOfCrawlerWidth(const RadiusSp &radius, const CrawlerWidth &width, const RadianCenter &radian);
    double CalMaxRadiusOfCrawlerWidth();
    double CalMaxRadiusOfCrawlerWidth(const RadiusSp &radius, const CrawlerWidth &width, const RadianCenter &radian);

    double getRadianOfCrawlerWidth(const RadiusSp& radius,const CrawlerWidth& width);
    double AngleToRadian(const double angle);
    double RadianToAngle(const double radian);

    void updatePWMValueByRatio(double Ratio);
private slots:
    void slUpdatePWMValueByConfig(EnumPWMType PWMType,quint16 value);
    void on_changedPWMValueBtn_clicked();

    void on_clearChangedBtn_clicked();

    void on_perimeterSpinBox_editingFinished();

private:
    Ui::CalDifSpeedInSphere *ui;
    double m_radiusOfSphere,m_crawlerWidth,m_radian_CrawlerCenter;
    quint16 m_PWMLeftByConfig,m_PWMRightByConfig;
    quint16 m_changedPWMLeft,m_changedPWMRight;

};

#endif // CALDIFSPEEDINSPHERE_H
