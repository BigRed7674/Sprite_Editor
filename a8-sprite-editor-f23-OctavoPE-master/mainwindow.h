//Reviewed by Jeanmarco Allain following the coding style document
/**
 * Jeanmarco Allain, Jason Davies, Travis Hobbs, Abdul Mansoor
 * CS 3505 - A8: Sprite Editor Implementation
 *
 * Header class for main window. Serves as view presented to the user.
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawingmodel.h"
#include "QMouseEvent"
#include "QRubberBand"
#include "QPushButton"
#include <QFileDialog>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor for MainWindow object
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // Instance variable of the DrawingModel class
    DrawingModel model;
    // Method calculates coordinates that will then be passed to corresponding drawing instrument.
    void calculateCoordinates(int &x, int &y);
public slots:
    // Updates visual list of frames in the view
    void updateFrameView();
    // Keeps track of how many frames to display
    void updateFrameList(int totalFrames);
    // Creates save file dialog for the user to specify .ssp save location
    void openSaveDialog();
    // Creates load file dialog for the user to specify .ssp load location
    void openLoadDialog();
    // With the passed in value, increases/decreases animation playback speed.
    void animationSpeedChanged(int value);
    // Zooms in on animation thumbnail
    void zoomAnimationClicked();
    // Zooms out on animation thumbnail
    void fullAnimationClicked();
    // Updates the list of total frames to animate
    void updateAnimatedFrame();
    // Handles loading of the next subsequent frame
    void changeAnimatedFrame();

private:
    Ui::MainWindow *ui;
    // Handles redirection and calculation of mouse presses on canvas.
    void mousePressEvent(QMouseEvent*);

    QString desktopPath;
    QString fileName;
    QString fileLoad;
    QColor penCol = QColor(0, 0, 0, 255);
    // Handles "drag" events to calculate dimensions of drawn rectangle.
    void mouseMoveEvent(QMouseEvent*);
    // Handles press up of mouse to finalize calculation of rectangle dimensions.
    void mouseReleaseEvent(QMouseEvent*);
    bool rectangle = false;
    bool pen = true;
    QPoint origin;
    QPoint screen;
    QRubberBand* rubberBand = nullptr;
    int origin_x;
    int origin_y;
    int scaleX = 200;
    int scaleY = 200;

signals:
    // signal that passes along a QPoint as the location of the mouse.
    void mousePos(QPoint);
    // emits location, color, width and height of the corresponding rectangle to draw
    void mouseLocation(int x, int y, QColor penCol, int width = 0, int height = 0);
    // passes save location filepath to begin writing the JSON object.
    void userPressedSave(QString fileName);
    // passes load location filepath to begin reading the JSON object.
    void userPressedLoad(QString fileName);
    // passes playback speed of animation
    void speedValue(int);

private slots:
    // rectangle draw tool button has been pressed, performs switching to it.
    void rectBtnClicked();
    // color changer button has been pressed, performs retrieving new color.
    void colorBtnClicked();

};
#endif // MAINWINDOW_H
