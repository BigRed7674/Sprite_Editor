//Reviewed By Abdul Mansoor using Team Team Code Style Document
/**
 * Abdul Mansoor, Jason Davies, Jeanmarco Allain, Travis Hobbs
 * 3505 - A8: Sprite Editor Implementation
 *
 * This is the header class for the implementation of the DrawingCanvas.
*/
#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QList>
#include <QPainter>

class DrawingCanvas : public QObject
{
    Q_OBJECT
    
public:
    // This is the constructor for creating a new canvas.
    explicit DrawingCanvas(QObject *parent = nullptr);
    // This method colors a pixel based on a given color, x position on the canvas, and y position on the canvas.
    void drawPixel(int x, int y, QColor color);
    // This method returns the current Active canvas as a QImage.
    QImage getCanvas();
    // This method is used whenever the user wants to add a new frame to draw on. It also increments the number of frames.
    void addFrame();
    // This method is used to delete a frame if we have more than one.
    void deleteFrame();
    // This method returns the size of frames created.
    int getFrameList();
    // This method is used to change into a selected frame.
    void changeFrame(int chosenFrame);
    // This method allows the user to draw the outline of a rectangle.
    void drawRectangle(int x, int y, int width, int height, QColor color);
    // This method allows us to access a specific frame based on its index.
    QImage getFrameAt(int index);
    // This method allows us to delete all changes made to the canvas and reset it completely to a white color.
    void clearCanvas();
    QPainter painter;

private:
    QImage canvas;
    QColor color;
    QList<QImage> frameList;
    int canvasSize;
    int activeCanvas;
};

#endif // DRAWINGCANVAS_H
