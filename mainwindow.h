#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0, const QString &fileName=QString());
    ~MainWindow();
    
private slots:
    void on_actionNew_triggered();
    void on_actionExit_triggered();
    void on_actionClose_triggered();
    void documentModified();
    void on_actionSelectFont_triggered();
    void on_actionAbout_triggered();
    void on_actionOpen_triggered();
    bool saveFile();
    bool saveFileAs();

private:
    Ui::MainWindow *ui;
    void makeConnections();
    void initComps();
    QString m_fileName;
    void loadFile(const QString &);
    void setFileName(const QString &fileName);
    QString getFileName(){return m_fileName;}

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
