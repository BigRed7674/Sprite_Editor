//Reviewed By Travis Hobbs using Team Team Code Style Document
#include "drawingcanvas.h"
#include <iostream>
#include "QMouseEvent"
#include "QPainter"
#include <iostream>

using namespace std;
DrawingCanvas::DrawingCanvas(QObject *parent)
    : QObject{parent}
{
    canvasSize = 64;
    activeCanvas = 0;
    color = QColor(255,0,0,255);
    canvas = QImage(canvasSize,canvasSize, QImage::Format_ARGB32);

    canvas.fill(QColor(255,255,255,255));
    frameList.append(canvas);
}
// This method colors a pixel based on a given color, x position on the canvas, and y position on the canvas.
void DrawingCanvas::drawPixel(int x, int y, QColor color)
{
   frameList[activeCanvas].setPixelColor(x,y,color);
}

// This method returns the current Active canvas as a QImage.
QImage DrawingCanvas::getCanvas()
{
   return frameList[activeCanvas];
}

// This method is used whenever the user wants to add a new frame to draw on. It also increments the number of frames.
void DrawingCanvas::addFrame()
{
    QImage newImg(canvasSize,canvasSize,QImage::Format_ARGB32);
    newImg.fill(QColor(255,255,255,255));
    frameList.append(newImg);
    activeCanvas++;
}

// This method is used to delete a frame if we have more than one.
void DrawingCanvas::deleteFrame()
{
    if (getFrameList() > 1) {
        if(activeCanvas != 0) {
           activeCanvas--;
        }
        frameList.pop_back();
    }
}

// This method returns the size of frames created.
int DrawingCanvas::getFrameList()
{
    return frameList.size();
}

// This method allows us to access a specific frame based on its index.
// passes in index of the frame list to return the QImage at that point.
QImage DrawingCanvas::getFrameAt(int index)
{
    return frameList[index];
}

// This method is used to change into a selected frame.
// passes in int of image to make the active canvas
void DrawingCanvas::changeFrame(int chosenFrame)
{
    activeCanvas = chosenFrame;
}

// This method allows the user to draw the outline of a rectangle.
// passes in coordinates, width, height, and color to create the rectangle
void DrawingCanvas::drawRectangle(int x, int y, int width, int height, QColor color)
{
    QPainter painter;
    painter.begin(&frameList[activeCanvas]);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawRect(x, y, width, height);
}

// This method allows us to delete all changes made to the canvas and reset it completely to a white color.
void DrawingCanvas::clearCanvas()
{
    frameList[activeCanvas].fill(Qt::white);
}
