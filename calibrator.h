#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <opencv2/opencv.hpp>

#ifndef SLEEPER_CLASS
#define SLEEPER_CLASS
class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};
#endif // SLEEPER_CLASS

class Calibrator : public QWidget
{
    Q_OBJECT
public:
    explicit Calibrator(QWidget *parent = nullptr);
    ~Calibrator();
    void Set_log(QTextEdit *m_log);

signals:

    void Log_append(QString TextToWrite);

public slots:

    void SetImage(const cv::Mat &input);
    void SetImage(const std::string& filename, int flags);
    void Set_lines(int m_lines, int m_threshold, int m_length);
    bool IsImageEmpty();
    void Find_calibration(double &calibration_value, double &calibration_value_err, bool is_pixel_over_micron);
    bool Calibration_strips(double &calibration_value, double &calibration_value_err, bool is_pixel_over_micron);
    cv::Mat get_component(const cv::Mat &input_mat,const unsigned int &input);

private:
    QTextEdit *log;
    cv::Mat image;
    cv::Mat detected_image;
    int Line_index      = -1;
    int Line_distance   = 0;
    int Line_length     = 0;
    int Line_threshold  = 150;
    bool Use_mask       = false;
};

#endif // CALIBRATOR_H
