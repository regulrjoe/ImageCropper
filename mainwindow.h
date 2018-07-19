#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnInput_clicked();
    void on_btnOutput_clicked();
    void on_btnProcess_clicked();

private:
    Ui::MainWindow *ui;
    QStringList input_file_names; // List of file names of images to process
    QString output_dir; // output directory where images are saved
};

#endif // MAINWINDOW_H
