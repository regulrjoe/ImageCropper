#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QMessageBox>
#include <qstringlistmodel.h>
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnInput_clicked()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (dialog.exec())
        input_file_names = dialog.selectedFiles();

    QStringListModel *model = new QStringListModel(input_file_names, NULL);
    ui->listInputFileNames->setModel(model);
}

void MainWindow::on_btnOutput_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);

    output_dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   "~/",
                                                   QFileDialog::ShowDirsOnly);

    std::cout << output_dir.toUtf8().constData() << std::endl;
    ui->txtOutput->setText(output_dir.toUtf8().constData());
}

void MainWindow::on_btnProcess_clicked()
{
    QMessageBox msg_box;
    cv::Mat image;
    cv::Mat cropped_image;
    cv::Mat split_image;
    cv::Rect ROI;
    std::string output_filename;
    QFileInfo f_inf;
    int i, j;
    double pct;

    if (input_file_names.empty() || output_dir == NULL) {
        msg_box.setText("Please ensure you have selected an image and output directory");
        msg_box.exec();
    } else {
        pct = ui->spinPercentage->text().toDouble() / 200;
        for (i = 0; i < input_file_names.length(); i++) {
            image = cv::imread(input_file_names[i].toUtf8().constData());
            ROI = cv::Rect(cv::Point(image.cols * pct,
                                     image.rows * pct),
                           cv::Point(image.cols - (image.cols * pct),
                                     image.rows - (image.rows * pct)));
            cropped_image = image(ROI);

            f_inf = QFileInfo(input_file_names[i].toUtf8().constData());

            if (ui->checkSplitV->isChecked()) {
                for (j = 1; j <= 2; j++) {
                    output_filename = "";
                    output_filename.append(output_dir.toUtf8().constData());
                    output_filename.append("/");
                    output_filename.append(QString(f_inf.fileName()).toUtf8().constData());
                    output_filename.erase(output_filename.end()-4, output_filename.end());
                    output_filename.append("_0");
                    output_filename.append(std::to_string(j));
                    output_filename.append(".jpg");
                    std::cout << "output filename: " << output_filename << std::endl;

                    ROI = cv::Rect(cv::Point(0 + ((cropped_image.cols/2) * (j-1)),
                                             0),
                                   cv::Size(cropped_image.cols/2,
                                            cropped_image.rows));

                    split_image = cropped_image(ROI);
                    cv::imwrite(output_filename, split_image);
                }
            } else {
                output_filename = "";
                output_filename.append(output_dir.toUtf8().constData());
                output_filename.append("/");
                output_filename.append(QString(f_inf.fileName()).toUtf8().constData());
                std::cout << "output filename: " << output_filename << std::endl;

                cv::imwrite(output_filename, cropped_image);
            }
        }

        msg_box.setText("Cropping finished.");
        msg_box.exec();
    }
}
