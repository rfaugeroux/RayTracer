#ifndef WINDOW_H
#define WINDOW_H

#include "GLViewer.h"
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QSlider>
#include <QLCDNumber>
#include <QSpinBox>
#include <QImage>
#include <QLabel>

#include <vector>
#include <string>

#include "QTUtils.h"


class Window : public QMainWindow {
    Q_OBJECT
public:
    Window();
    virtual ~Window();

    static void showStatusMessage (const QString & msg);  

public slots :
    void renderRayImage ();
    void setBGColor ();
    void showRayImage ();
    void exportGLImage ();
    void exportRayImage ();
    void about ();
    void setBRDF(int MODE);
    void setAAMode(int _MODE);
    void setShadowMode(int state);
    void setLightSampling(int density);
    void recordVideo();
    
signals :
    void lightSamplingChanged(QString text);

private :
    void initControlWidget ();
        
    QActionGroup * actionGroup;
    QGroupBox * controlWidget;
    QString currentDirectory;

    int BRDF_MODE;
    int AA_MODE;
    bool WITH_SHADOWS;
    int LIGHT_SAMPLING;

    GLViewer * viewer;
};

#endif // WINDOW_H


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
