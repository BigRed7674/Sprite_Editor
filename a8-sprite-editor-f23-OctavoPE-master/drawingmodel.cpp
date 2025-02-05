//Reviewed By Travis Hobbs using Team Team Code Style Document
#include "drawingmodel.h"
#include "drawingcanvas.h"
#include <QColor>
#include <QDebug>
#include <iostream>
#include <QTimer>

using namespace std;
// This is the constructor for the DrawingModel class.
DrawingModel::DrawingModel(QObject *parent)
    : QObject{parent}
{
    animatedFrameCanvas = QImage(64,64, QImage::Format_ARGB32);
    animatedFrameCanvas.fill(QColor(255, 255, 255, 255));
    animationTimer = new QTimer(this);
    connect(animationTimer,
            &QTimer::timeout,
            this,
            &DrawingModel::ChangeFrame);

    animationTimer->start(1000);
    penSelected = 0;
}

// This method will call the getCanvas method in the drawingCanvas class which returns the QImage of the active canvas.
QImage DrawingModel::getCanvas(int width, int height)
{
    return canvas.getCanvas().scaled(width, height, Qt::KeepAspectRatio);
}

// This method controls what frame the animation image will currently be showing
void DrawingModel::ChangeFrame(){
    if (canvas.getFrameList() == 1) {
        emit updateSingleFrame(1);
    }else {
        if (currFrame < canvas.getFrameList()-1){
            currFrame = currFrame + 1;
            emit changeFrame(currFrame);
        } else {
            currFrame = 0;
            emit changeFrame(currFrame);
        }
    }
}

// This method is what is sent to the DrawingCanvas so that it knows what pixel in the screen to turn into a empty space.
void DrawingModel::drawEraser(int x, int y)
{
    canvas.drawPixel(x, y, QColor(255,255,255,255));
    emit updateVisible(1);
}

// This method draws a rectangle with the top left corner being the x,y values of the width and height provided.
void DrawingModel::drawRect(int x, int y, QColor color,int width, int height)
{
    canvas.drawRectangle(x, y, width, height, color);
    emit updateVisible(1);
}

// This method draws the pixel location on the opposite x location of the pixel that was clicked on.
void DrawingModel::drawMirror(int x, int y, QColor color)
{
    int mirrored_x = 64;
    if(x > 31) {
        mirrored_x = 31-(x-31);
    }
    else if (x < 31) {
        mirrored_x = 31+(31-x);
    }
    canvas.drawPixel(mirrored_x,y,color);
    emit updateVisible(1);
}

// This method is called whenever the user clicks on the canvas to draw a pixel on the screen and based on the buttons clicked and active by the user in the ui,
// will either call the DrawPixel method, the Erase method, the drawRect method, or the drawMirror method.
void DrawingModel::draw(int x, int y, QColor penCol, int width, int height)
{
    if(penSelected == 0) {
        drawPixel(x,y, penCol);
    }
    else if (penSelected == 1) {
        drawEraser(x,y);
    }
    else if (penSelected == 2) {
        drawRect(x,  y , penCol, width,  height);
    }
    else if (penSelected == 3) {
        drawPixel(x,y, penCol);
        drawMirror(x,y, penCol);
    }
}

// This method is called whenever the user clicks on the drawing canvas and will draw a pixel at the specifed location with the active color.
void DrawingModel::drawPixel(int x, int y, QColor penCol)
{
    canvas.drawPixel(x, y, penCol);
    emit updateVisible(1);
}

// This method is called whenever the user clicks on the erase button,
// it then tells the draw method that when the user clicks on the screen they are erasing and not drawing.
void DrawingModel::changedPenToErase()
{
    if(penSelected == 1) {
        penSelected = 0;
    }
    else{
        penSelected = 1;
    }
}

// This method is called whenever the user clicks on the rectangle button,
// it then tells the draw method to call the drawRect method when the user draws on the screen.
void DrawingModel::changedPenToRect()
{
    if(penSelected == 2) {
        penSelected = 0;
    }
    else{
        penSelected = 2;
    }
}

// This method is called whenever the user clicks on the Mirror button,
// it then tells the draw method that when the user clicks on the canvas they are using the mirror tool so it calls the drawMirror method.
void DrawingModel::changedPenToMirror()
{
    if(penSelected == 3) {
        penSelected = 0;
    }
    else {
        penSelected = 3;
    }
}

// This method is called whenever the user clicks on the clear canvas button, it then will call a send back a blank canvas, removing any drawing on the active canvas.
void DrawingModel::clearCanvasPressed()
{
    canvas.clearCanvas();
    emit updateVisible(1);
}

// This method is called when the user clicks the save button which will convert all of the canvas's into a Json array,
// which is then saved into a ssp file of the name that the user provides
void DrawingModel::saveProject(QString fileName)
{
    if (!fileName.isEmpty()) {
        QFile *saveFile = new QFile(fileName);
        saveFile->open(QIODevice::WriteOnly);
        QJsonObject obj;
        for(int i = 0; i<canvas.getFrameList();i++) {
            QJsonObject thisFrame;
            QJsonArray pixels;
            QImage thisImg = canvas.getFrameAt(i);
            for(int x=0; x<64; x++) {
                for(int y=0; y<64; y++) {
                    QJsonObject thisPixel;
                    thisPixel["x"] = x;
                    thisPixel["y"] = y;
                    QJsonObject rgba;
                    QColor thisColor = thisImg.pixelColor(x,y);
                    rgba["r"] = thisColor.red();
                    rgba["g"] = thisColor.green();
                    rgba["b"] = thisColor.blue();
                    rgba["a"] = thisColor.alpha();
                    thisPixel["rgba"] = rgba;
                    pixels.append(thisPixel);
                }
            }
            thisFrame["pixels"] = pixels;
            QString thisFrameNum = "Frame " + QString::number(i);
            obj[thisFrameNum] = thisFrame;
        }
        QJsonDocument json(obj);
        QString jsonStr = json.toJson(QJsonDocument::Indented);
        QTextStream saveStream(saveFile);
        saveStream << jsonStr;
        saveFile->close();
        saveFile->deleteLater();
    }
}

// This method is called when the user clicks the load button which will use Json to load the file given by the filename that the user selects.
void DrawingModel::loadProject(QString fileLoad)
{
    QFile file(fileLoad);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    clearProject();
    QByteArray jsonData = file.readAll();
    QJsonDocument json = QJsonDocument::fromJson(jsonData);
    file.close();
    if (!json.isNull() && json.isObject()) {
        QJsonObject obj = json.object();
        QStringList totalFrameKeys = obj.keys();
        int totalFrames = totalFrameKeys.count()-1;
        for(const QString& frame : obj.keys()) {
            QJsonObject thisFrame = obj.value(frame).toObject();
            QJsonArray pixels = thisFrame.value("pixels").toArray();
            for(const QJsonValue& thisPixel : pixels) {
                QJsonObject pixelCoords = thisPixel.toObject();
                int x = pixelCoords.value("x").toInt();
                int y = pixelCoords.value("y").toInt();
                QJsonObject pixelObj = pixelCoords.value("rgba").toObject();
                int r = pixelObj.value("r").toInt();
                int g = pixelObj.value("g").toInt();
                int b = pixelObj.value("b").toInt();
                int a = pixelObj.value("a").toInt();
                canvas.drawPixel(x,y,QColor(r,g,b,a));
            }
            if(totalFrames > 0) {
                userPressedAddFrame();
                totalFrames--;
            }
        }
        emit updateVisible(1);
        emit updateFrameList(canvas.getFrameList());
    }
}

// This method is called by the loadProject method, this method will delete all the frames except the base and turn the base frame into a blank canvas.
void DrawingModel::clearProject()
{
    for(int i=0; i<canvas.getFrameList()-1;i++) {
        userPressedDeleteFrame();
    }
    for(int x=0; x<64; x++) {
        for(int y=0; y<64; y++) {
            canvas.drawPixel(x,y,QColor(255,255,255,255));
        }
    }
}

// This method is called whenever the user clicks the Add Frame button on the ui, which tells the canvas to add another one to the list.
void DrawingModel::userPressedAddFrame()
{
    canvas.addFrame();
    emit updateVisible(1);
    emit updateFrameList(canvas.getFrameList());
}

// This method is called whenever the user clicks on the Delete Frame button on the ui, it then tells the canvas to delete the frame that the user is currently on.
void DrawingModel::userPressedDeleteFrame()
{
    canvas.deleteFrame();
    emit updateVisible(1);
    emit updateFrameList(canvas.getFrameList());
}

// This method is called whenever the user clicks on a frame in the ui, it then switches the active frame from the current one to the one that was clicked on.
void DrawingModel::userChangedFrame(int currentRow)
{
    if(currentRow != -1) {
        canvas.changeFrame(currentRow);
        emit updateVisible(1);
    }
}

// This method is called to return the next frame to be shown in the Animation image.
QImage DrawingModel::changeAnimatedFrame(int width, int height)
{
    return canvas.getFrameAt(currFrame).scaled(width, height, Qt::KeepAspectRatio);
}

// This method is called whenever the user moves the slidebar on the ui, this will change the time taken to switch the active canvas on the animation image.
void DrawingModel::changeTimer(int time)
{
    if(!(canvas.getFrameList() == 1)) {
        if(!(animationTimer->isActive())) {
            animationTimer->start((10000/time));
        }
        else {
            if(time == 0) {
                animationTimer->stop();
                emit changeFrame(0);
            } else {
                animationTimer->setInterval(10000/time);
            }
        }
    }
}
