#include "caldifspeedinsphere.h"
#include "ui_caldifspeedinsphere.h"
#include "crawlerserial.h"
#include "crawlerstatusparam.h"

CalDifSpeedInSphere::CalDifSpeedInSphere(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalDifSpeedInSphere),
    m_radiusOfSphere(1500.00),
    m_crawlerWidth(300),
    m_radian_CrawlerCenter(1)
{
    ui->setupUi(this);
    initUI();
}

CalDifSpeedInSphere::~CalDifSpeedInSphere()
{
    delete ui;
}

double CalDifSpeedInSphere::getCrawlerBothSidesRatio_Sin(const RadiusSp &radius, const CrawlerWidth &width, const RadianCenter &radian)
{
    this->setAllParam(radius,width,radian);
    double minRadian = this->CalMinRadiusOfCrawlerWidth();
    double maxRadian = this->CalMaxRadiusOfCrawlerWidth();
    double ratio  = sin(minRadian)/sin(maxRadian);
    return ratio;
}

void CalDifSpeedInSphere::setAllParam(const RadiusSp &radius, const CrawlerWidth &width, const RadianCenter &radian)
{
    this->setRadiusOfSphere(radius);
    this->setCrawlerWidth(width);
    this->setRadianOfCrawlerCenter(radian);
}

void CalDifSpeedInSphere::setRadiusOfSphere(const RadiusSp &radius)
{
    if(radius.m_radius>MINRADIUSOFSPHERE)
        this->m_radiusOfSphere = radius.m_radius;
}

double CalDifSpeedInSphere::getRadiusOfSphere()
{
    return this->m_radiusOfSphere;
}

void CalDifSpeedInSphere::setCrawlerWidth(const CrawlerWidth &width)
{
    if(width.m_width > 0)
        this->m_crawlerWidth = width.m_width;
}

CrawlerWidth CalDifSpeedInSphere::getCrawlerWidth()
{
    return CrawlerWidth(this->m_crawlerWidth);
}

void CalDifSpeedInSphere::setRadianOfCrawlerCenter(const RadianCenter &radian)
{
    if(radian.m_radianCenter>MINRADIAN)
        this->m_radian_CrawlerCenter = radian.m_radianCenter;
}

RadianCenter CalDifSpeedInSphere::getRadianOfCrawlerCenter()
{
    return RadianCenter(this->m_radian_CrawlerCenter);
}
//将中心角度转换为环向周长。
double CalDifSpeedInSphere::radianCenterToPerimeter(double radian)
{
    //球的半径
    double radius =this->getRadiusOfSphere();
    //根据中心角度，求出环向周长的半径。
    double radiusOfPerimeter = radius * sin(radian);
    double perimeter = 2 * D_PI * radiusOfPerimeter;
    return perimeter;
}
//将环向周长转换为角度
double CalDifSpeedInSphere::perimeterToRadianCenter(double perimeter)
{
    //球的半径
    double radius = this->getRadiusOfSphere();
    if(radius <=0) return 0.00;
    //根据环向周长的半径，求出中心角度。
    double radiusOfPerimeter = perimeter / (2*D_PI);
    double radian = asin(radiusOfPerimeter/radius);
    return radian;
}

double CalDifSpeedInSphere::CalMinRadiusOfCrawlerWidth()
{
    return this->CalMinRadiusOfCrawlerWidth(RadiusSp(m_radiusOfSphere),CrawlerWidth(m_crawlerWidth),RadianCenter(m_radian_CrawlerCenter));
}

double CalDifSpeedInSphere::CalMinRadiusOfCrawlerWidth(const RadiusSp &radius, const CrawlerWidth &width, const RadianCenter &radian)
{
    double minRadian = 0.0;
    this->setAllParam(radius,width,radian);
    double radianOfWidth = this->getRadianOfCrawlerWidth(radius,width);
    minRadian = radian.m_radianCenter - radianOfWidth/2; // 车体中心在球体上的夹角弧度值 - 车体宽度弧度值的一半，//得到车体最小的弧度值。
    if(minRadian < MINRADIAN)
        return MINRADIAN;
    else
        return minRadian;
}

double CalDifSpeedInSphere::CalMaxRadiusOfCrawlerWidth()
{
    return this->CalMaxRadiusOfCrawlerWidth(RadiusSp(m_radiusOfSphere),CrawlerWidth(m_crawlerWidth),RadianCenter(m_radian_CrawlerCenter));
}

double CalDifSpeedInSphere::CalMaxRadiusOfCrawlerWidth(const RadiusSp &radius, const CrawlerWidth &width, const RadianCenter &radian)
{
    double maxRadian = 0.0;
    this->setAllParam(radius,width,radian);
    double radianOfWidth = this->getRadianOfCrawlerWidth(radius,width);
    maxRadian = radian.m_radianCenter + radianOfWidth/2; // 车体中心在球体上的夹角弧度值 + 车体宽度弧度值的一半，//得到车体最大的弧度值。
    if(maxRadian > MAXRADIAN)
        return MAXRADIAN;
    else
        return maxRadian;
}

//double CalDifSpeedInSphere::getRatioOfRing(const RadiusSp &radius, const CrawlerWidth &width, const RadianCenter &radian)
//{
//    double radianOfObjArc = this->getRadianOfArcLength(radius,width);
//    double topPointRadian  = radian - radianOfObjArc/2;
//    double tailPointRadian = radian + radianOfObjArc/2;
//    //the topPoint radius
//    double topRadius = radius * sin(topPointRadian);
//    double tailRadius = radius * sin(tailPointRadian);
//    double ratioOfRing = topRadius/tailRadius;
//    return ratioOfRing;
//}

double CalDifSpeedInSphere::getRadianOfCrawlerWidth(const RadiusSp &radius, const CrawlerWidth &width)
{
    if(radius.m_radius>0)
        return width.m_width/radius.m_radius;
    else
        return 0.00;
}
//转换
//角度与弧度转换
//#define D_PI  3.141592654
//#define ANGTORAD(a)  ((a) * D_PI / 180.0)
//#define RADTOANG(a)  ((a) * 180.0 / D_PI)

double CalDifSpeedInSphere::AngleToRadian(const double angle)
{
    if((angle >= MINANGLE) && (angle <= MAXANGLE))
        return (angle * D_PI/180.0);
    else
        return 0.0;
}

double CalDifSpeedInSphere::RadianToAngle(const double radian)
{
    if((radian >=MINRADIAN) && (radian <=MAXRADIAN))
        return (radian *180.0/D_PI);
    else
        return 0.0;
}

void CalDifSpeedInSphere::updatePWMValueByRatio(double Ratio)
{
    //
    quint16  m_leftValue,m_rightValue;
    m_leftValue = m_PWMLeftByConfig;
    m_rightValue = m_leftValue / Ratio;
    //    MinPWMValue            = 300,
    //MaxPWMValue            = 2100,
    if(m_rightValue >= MaxPWMValue)
    {
        m_rightValue = MaxPWMValue;
        m_leftValue = m_rightValue * Ratio;
    }

    if(m_rightValue <= MinPWMValue)
    {
        m_rightValue = MinPWMValue;
        m_leftValue = m_rightValue *Ratio;
    }
    //
    ui->showChangedLeftPWM->setText(QString::number(m_leftValue));
    ui->showChangedRightPWM->setText(QString::number(m_rightValue));

    m_changedPWMLeft = m_leftValue;
    m_changedPWMRight = m_rightValue;
}

void CalDifSpeedInSphere::slUpdatePWMValueByConfig(EnumPWMType PWMType, quint16 value)
{
    switch (PWMType) {
    case PWMLeftByConfig:
        ui->showCurLeftPWM->setText(QString::number(value));
        m_PWMLeftByConfig = value;
        break;
    case PWMRightByConfig:
        ui->showCurRightPWM->setText(QString::number(value));
        m_PWMRightByConfig =value;
        break;
    default:
        break;
    }
}

void CalDifSpeedInSphere::on_CenterAngle_editingFinished()
{
    double radian = this->AngleToRadian(ui->CenterAngle->value());
    this->setRadianOfCrawlerCenter(radian);
    //求出环向周长
    double perimeter = radianCenterToPerimeter(radian);
    ui->perimeterSpinBox->setValue(perimeter);
}

void CalDifSpeedInSphere::on_crawlerWidth_editingFinished()
{
    this->setCrawlerWidth(ui->crawlerWidth->value());
}

void CalDifSpeedInSphere::on_radiusOfSphere_editingFinished()
{
    this->setRadiusOfSphere(RadiusSp(ui->radiusOfSphere->value()));
}

void CalDifSpeedInSphere::on_pushButton_clicked()
{
    //
    double radius = ui->radiusOfSphere->value();
    double width = ui->crawlerWidth->value();
    double radian = this->AngleToRadian(ui->CenterAngle->value());
    double ratioValue = this->getCrawlerBothSidesRatio_Sin(RadiusSp(radius),CrawlerWidth(width),RadianCenter(radian));
    ui->showRatioLabel->setText(QString::number(ratioValue,'f',2));
    this->updatePWMValueByRatio(ratioValue);
}

void CalDifSpeedInSphere::on_pushButton_2_clicked()
{
    double radius = ui->radiusOfSphere->value();
    double width = ui->crawlerWidth->value();
    double radian = this->AngleToRadian(ui->CenterAngle->value());
    double ratioValue = this->getCrawlerBothSidesRatio_Sin(RadiusSp(radius),CrawlerWidth(width),RadianCenter(radian));
    ui->showRatioLabel->setText(QString::number(1/ratioValue,'f',2));
    this->updatePWMValueByRatio(1/ratioValue);
}

void CalDifSpeedInSphere::initUI()
{
    CrawlerStatusParam *crawlerStatus = CrawlerStatusParam::getInstance();
    connect(crawlerStatus,SIGNAL(sgUpdatePWMValueByConfig(EnumPWMType,quint16)),this,SLOT(slUpdatePWMValueByConfig(EnumPWMType,quint16)));

    m_PWMLeftByConfig =crawlerStatus->getCurPWMValueByConfig(PWMLeftByConfig);
    m_PWMRightByConfig = crawlerStatus->getCurPWMValueByConfig(PWMRightByConfig);
    m_changedPWMLeft = m_PWMLeftByConfig;
    m_changedPWMRight = m_PWMRightByConfig;

    ui->showCurLeftPWM->setText(QString::number(m_PWMLeftByConfig));
    ui->showCurRightPWM->setText(QString::number(m_PWMRightByConfig));
    ui->showChangedLeftPWM->setText(QString::number(m_PWMLeftByConfig));
    ui->showChangedRightPWM->setText(QString::number(m_PWMLeftByConfig));
}

void CalDifSpeedInSphere::on_changedPWMValueBtn_clicked()
{
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->setPWMValue(PWMLeftByConfig,m_changedPWMLeft);
    crawlerSerial->setPWMValue(PWMRightByConfig,m_changedPWMRight);
}

void CalDifSpeedInSphere::on_clearChangedBtn_clicked()
{
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->setPWMValue(PWMLeftByConfig,m_PWMLeftByConfig);
    crawlerSerial->setPWMValue(PWMRightByConfig,m_PWMLeftByConfig);
    ui->showChangedLeftPWM->setText(QString::number(m_PWMLeftByConfig));
    ui->showChangedRightPWM->setText(QString::number(m_PWMLeftByConfig));
    m_changedPWMLeft = m_PWMLeftByConfig;
    m_changedPWMRight = m_PWMLeftByConfig;
}

void CalDifSpeedInSphere::on_perimeterSpinBox_editingFinished()
{
    double perimeter = ui->perimeterSpinBox->value();
    double radian = this->perimeterToRadianCenter(perimeter);
    double angle = this->RadianToAngle(radian);
    ui->CenterAngle->setValue(angle);
}
