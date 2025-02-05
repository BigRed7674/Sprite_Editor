//Reviewed by Jason Davies following the Code Style Document
/**
 * Abdul Mansoor, Jason Davies, Jeanmarco Allain, Travis Hobbs
 * 3505 - A8: Sprite Editor Implementation
 *
 * This is the header class for the implementation of the DrawingModel.
*/
#ifndef DRAWINGMODEL_H
#define DRAWINGMODEL_H

#include <QObject>
#include <QImage>
#include "drawingcanvas.h"
#include <QListWidget>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class DrawingModel : public QObject
{
    Q_OBJECT

public:
    //This is the constructor for the DrawingModel class.
    explicit DrawingModel(QObject *parent = nullptr);
    //This is an instance variable of the DrawingCanvas class.
    DrawingCanvas canvas;
    //This method will call the getCanvas method in the drawingCanvas class which returns the QImage of the active canvas.
    QImage getCanvas(int width, int height);
    //This method is called to return the next frame to be shown in the Animation image.
    QImage changeAnimatedFrame(int weight, int height);
    //This method controls what frame the animation image will currently be showing
    void ChangeFrame();

private:
    //This method draws the pixel location on the opposite x location of the pixel that was clicked on.
    void drawMirror(int x, int y, QColor color);
    //This method draws a rectangle with the top left corner being the x,y values of the width and height provided.
    void drawRect(int x, int y,QColor color, int width, int height);
    //This method is what is sent to the DrawingCanvas so that it knows what pixel in the screen to turn into a empty space.
    void drawEraser(int x, int y);
    //This method is called by the loadProject method, this method will delete all the frames except the base and turn the base frame into a blank canvas.
    void clearProject();
    QImage animatedFrameCanvas;
    QTimer* animationTimer;
    int currFrame;
    int penSelected;

public slots:
    //This method is called whenever the user clicks the Add Frame button on the ui, which tells the canvas to add another one to the list.
    void userPressedAddFrame();
    //This method is called whenever the user clicks on the Delete Frame button on the ui, it then tells the canvas to delete the frame that the user is currently on.
    void userPressedDeleteFrame();
    //This method is called whenever the user clicks on a frame in the ui, it then switches the active frame from the current one to the one that was clicked on.
    void userChangedFrame(int currentRow);
    //This method is called whenever the user clicks on the drawing canvas and will draw a pixel at the specifed location with the active color.
    void drawPixel(int x, int y, QColor penCol);
    //This method is called whenever the user moves the slidebar on the ui, this will change the time taken to switch the active canvas on the animation image.
    void changeTimer(int time);
    //This method is called when the user clicks the load button which will use Json to load the file given by the filename that the user selects.
    void loadProject(QString fileLoad);
    //This method is called when the user clicks the save button which will convert all of the canvas's into a Json array,
    //which is then saved into a ssp file of the name that the user provides
    void saveProject(QString fileName);
    //This method is called whenever the user clicks on the canvas to draw a pixel on the screen and based on the buttons clicked and active by the user in the ui,
    //will either call the DrawPixel method, the Erase method, the drawRect method, or the drawMirror method.
    void draw(int x, int y, QColor penCol, int width = 0, int height = 0);
    //This method is called whenever the user clicks on the erase button,
    //it then tells the draw method that when the user clicks on the screen they are erasing and not drawing.
    void changedPenToErase();
    //This method is called whenever the user clicks on the rectangle button,
    //it then tells the draw method to call the drawRect method when the user draws on the screen.
    void changedPenToRect();
    //This method is called whenever the user clicks on the Mirror button,
    //it then tells the draw method that when the user clicks on the canvas they are using the mirror tool so it calls the drawMirror method.
    void changedPenToMirror();
    //This method is called whenever the user clicks on the clear canvas button, it then will call a send back a blank canvas, removing any drawing on the active canvas.
    void clearCanvasPressed();

signals:
    //This signal is emitted to update the canvas on the ui.
    void updateVisible(int);
    //This signal is emitted to update the ui to the number of canvas's in the drawing.
    void updateFrameList(int);
    //This signal is emitted to update the ui of the animated screen to show the next canvas in the animation.
    void changeFrame(int);
    //This signal is emitted to update the animation canvas when there is only one frame created, afterwards it is updated by changeFrame.
    void updateSingleFrame(int);
};

#endif // DRAWINGMODEL_H
