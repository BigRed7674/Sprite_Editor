//Reviewed By Travis Hobbs using Team Team Code Style Document
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QPoint"
#include "QWidget"
#include <iostream>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&model,
            &DrawingModel::updateVisible,
            this,
            &MainWindow::updateFrameView);

    connect(&model,
            &DrawingModel::updateFrameList,
            this,
            &MainWindow::updateFrameList);

    connect(ui->addFrameBtn,
            &QPushButton::clicked,
            &model,
            &DrawingModel::userPressedAddFrame);

    connect(ui->deleteFrameBtn,
            &QPushButton::clicked,
            &model,
            &DrawingModel::userPressedDeleteFrame);

    connect(ui->eraseBtn,
            &QPushButton::clicked,
            &model,
            &DrawingModel::changedPenToErase);

    connect(ui->mirrorBtn,
            &QPushButton::clicked,
            &model,
            &DrawingModel::changedPenToMirror);

    ui->imageViewer->setPixmap(QPixmap::fromImage(model.getCanvas(800,800)));
    ui->animatedFrame->setPixmap(QPixmap::fromImage(model.getCanvas(scaleX,scaleY)));
    setMouseTracking(true);
    connect(this,
            &MainWindow::mouseLocation,
            &model,
            &DrawingModel::draw);

    connect(&model,
            &DrawingModel::changeFrame,
            this,
            &MainWindow::changeAnimatedFrame);

    connect(&model,
            &DrawingModel::updateSingleFrame,
            this,
            &MainWindow::updateAnimatedFrame);

    connect(this,
            &MainWindow::speedValue,
            &model,
            &DrawingModel::changeTimer);

    connect(ui->rectangleBtn,
            &QPushButton::clicked,
            &model,
            &DrawingModel::changedPenToRect);

    connect(ui->clearCanvasBtn,
            &QPushButton::clicked,
            &model,
            &DrawingModel::clearCanvasPressed);

    connect(ui->saveBtn_2,
            &QPushButton::clicked,
            this,
            &MainWindow::openSaveDialog);

    connect(this,
            &MainWindow::userPressedSave,
            &model,
            &DrawingModel::saveProject);

    connect(ui->loadBtn_2,
            &QPushButton::clicked,
            this,
            &MainWindow::openLoadDialog);

    connect(this,
            &MainWindow::userPressedLoad,
            &model,
            &DrawingModel::loadProject);

    ui->imageViewer->setPixmap(QPixmap::fromImage(model.getCanvas(800,800)));
    setMouseTracking(true);
    connect(ui->frameListView,
            &QListWidget::currentRowChanged,
            &model,
            &DrawingModel::userChangedFrame);

    connect(ui->rectangleBtn,
            &QPushButton::clicked,
            this,
            &MainWindow::rectBtnClicked);
    connect(ui->ColorButton,
            &QPushButton::clicked,
            this,
            &MainWindow::colorBtnClicked);
    connect(ui->Zoom,
            &QPushButton::clicked,
            this,
            &MainWindow::zoomAnimationClicked);
    connect(ui->FullSize,
            &QPushButton::clicked,
            this,
            &MainWindow::fullAnimationClicked);
    connect(ui->animationSpeed,
            &QSlider::valueChanged,
            this,
            &MainWindow::animationSpeedChanged);
}

// Handles press up of mouse to finalize calculation of rectangle dimensions.
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() == Qt::LeftButton) && rectangle) {
        rubberBand->hide();
        int start_x = screen.x() / 12.5;
        int start_y = screen.y() / 12.5;
        int x = (event->pos().x()-320) / 12.5;
        int y = (event->pos().y()-20) / 12.5;
        int width = x - start_x;
        int height = y - start_y;
        emit mouseLocation(start_x, start_y, penCol, width, height);
    }
}

// Handles "drag" events to calculate dimensions of drawn rectangle.
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() &Qt::LeftButton) && pen) {
        int x = event->pos().x();
        int y = event->pos().y();
        calculateCoordinates(x, y);
        emit mouseLocation(x, y, penCol);
    } else {
        if ((event->buttons() &Qt::LeftButton) && rectangle) {
            QPoint box;
            box.setX(event->pos().x()-320);
            box.setY(event->pos().y()-20);
            rubberBand->setGeometry(QRect(screen, box).normalized());
        }
    }
}

// Handles redirection and calculation of mouse presses on canvas.
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && pen) {
        int x = event->pos().x();
        int y = event->pos().y();
        calculateCoordinates(x, y);
        emit mouseLocation(x, y, penCol);
    } else {
        if ((event->button() == Qt::LeftButton) && rectangle) {
            origin = event->pos();
            int x = origin.x() - 320;
            int y = origin.y() - 20;
            screen.setX(x);
            screen.setY(y);
            if (!rubberBand) {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, ui->imageViewer);
            }
            rubberBand->setGeometry(QRect(screen, QSize(0,0)));
            rubberBand->show();
        }
    }
}

// Method calculates coordinates that will then be passed to corresponding drawing instrument.
void MainWindow::calculateCoordinates(int &x, int &y)
{
    x = x-320;
    y = y - 20;
    if((x >= 0) && x <= 800) {
        if((y >= 0) && y <= 800) {
            x=x/12.5;
            y = y / 12.5;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Updates visual list of frames in the view
void MainWindow::updateFrameView()
{
    ui->imageViewer->setPixmap(QPixmap::fromImage(model.getCanvas(800,800)));
}

// Keeps track of how many frames to display
void MainWindow::updateFrameList(int totalFrames)
{
    int total = totalFrames;
    ui->frameListView->clear();
    ui->frameListView->addItem(QString("Base Frame"));
    total--;
    for(int i=0; i<total; i++) {
        ui->frameListView->addItem(QString("Frame " + QString::number(i+1)));
    }
}

// color changer button has been pressed, performs retrieving new color.
void MainWindow::colorBtnClicked()
{
    QColor possibleColor = QColorDialog::getColor(Qt::white,this, "choose Color");
    if(possibleColor.isValid()) {
        penCol = possibleColor;
        QString qss = QString("QPushButton {background-color: %1};").arg(penCol.name(QColor::HexArgb));
        ui->ColorButton->setStyleSheet(qss);
    }
}

// Creates save file dialog for the user to specify .ssp save location
void MainWindow::openSaveDialog()
{
    if(fileName.isEmpty()) {
        desktopPath = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save Sprite Project"), desktopPath, QObject::tr("Sprite Project (*.ssp);;"));
    }
    emit userPressedSave(fileName);
}

// Creates load file dialog for the user to specify .ssp load location
void MainWindow::openLoadDialog()
{
    desktopPath = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    fileLoad = QFileDialog::getOpenFileName(this, QObject::tr("Load Sprite Project"), desktopPath, QObject::tr("Sprite Project (*.ssp);;"));
    if(!fileLoad.isEmpty()) {
        emit userPressedLoad(fileLoad);
    }
}

// Handles loading of the next subsequent frame
inline void MainWindow::changeAnimatedFrame()
{
    ui->animatedFrame->setPixmap(QPixmap::fromImage(model.changeAnimatedFrame(scaleX,scaleY)));
}

// Updates the list of total frames to animate
void MainWindow::updateAnimatedFrame()
{
    ui->animatedFrame->setPixmap(QPixmap::fromImage(model.getCanvas(scaleX,scaleY)));

}

// With the passed in value, increases/decreases animation playback speed.
void MainWindow::animationSpeedChanged(int value)
{
    emit speedValue(value);
}

// Zooms in on animation thumbnail
void MainWindow::zoomAnimationClicked()
{
    scaleX = 200;
    scaleY = 200;
}

// Zooms out on animation thumbnail
void MainWindow::fullAnimationClicked()
{
    scaleX = 50;
    scaleY = 50;
    ui->animatedFrame->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

// rectangle draw tool button has been pressed, performs switching to it.
void MainWindow::rectBtnClicked()
{
    if(rectangle){
        rectangle = false;
        pen = true;
    }
    else{
        rectangle = true;
        pen = false;
    }
}
