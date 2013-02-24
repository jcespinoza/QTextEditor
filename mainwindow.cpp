#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFontDialog>
#include <QFileDialog>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent, const QString &fileName) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("QTextEditor [*]");
    //this->setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(documentModified()));
    QSettings font("Progra III Inc.", "QTextEditor");
    QFont fontV = font.value("viewFont", QApplication::font()).value<QFont>();
    ui->textEdit->setFont(fontV);

    initComps();
    makeConnections();
    loadFile(fileName);
}

void MainWindow::makeConnections(){
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
    connect(ui->actionCopy, SIGNAL(triggered()), ui->textEdit, SLOT(copy()) );
    connect(ui->actionCut, SIGNAL(triggered()), ui->textEdit, SLOT(cut()) );
    connect(ui->actionPaste, SIGNAL(triggered()), ui->textEdit, SLOT(paste()) );
    connect(ui->actionUndo, SIGNAL(triggered()), ui->textEdit, SLOT(undo()) );
    connect(ui->actionRedo, SIGNAL(triggered()), ui->textEdit, SLOT(redo()) );
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)) );
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)) );
    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)) );
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)) );
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()) );
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()) );
}

void MainWindow::initComps(){
    ui->actionCopy->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionCut->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    MainWindow *nWindow = new MainWindow(0);
    nWindow->show();
}

void MainWindow::on_actionExit_triggered()
{
    qApp->closeAllWindows();
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::documentModified(){
    setWindowModified(true);
}

void MainWindow::closeEvent(QCloseEvent *e){
    if( ! this->isWindowModified())
        e->accept();
    else{
        int ans = QMessageBox::warning(this, "Documento modificado", "El documento ha sido modificado, desea guardar los cambios?\n Perdera todos los cambios no guardados.",
                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
        switch(ans){
        case QMessageBox::Yes:
            if(saveFile())
                e->accept();
            else
                e->ignore();
            break;
        case QMessageBox::No:
            e->accept();
            break;
        case QMessageBox::Cancel:
            e->ignore();
            break;
        }
    }
}

void MainWindow::on_actionSelectFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, ui->textEdit->font(), this);
    if(ok){
        ui->textEdit->setFont(font);
        QSettings fontS("Progra III Inc.", "QTextEditor");
        fontS.setValue("viewFont", font);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Acerca de QTextEditor", "QTextEditor es una aplicacion para editar archivos de texto equipado con funcionalidades superiores que lo diferencian del resto.\n\nDesarrollado por Michello Corporation.");
}

void MainWindow::loadFile(const QString &fileName){
    if(fileName.isEmpty()){
        setFileName(QString());
        return;
    }
    QFile file(fileName);
    if( !file.open(QFile::ReadOnly | QIODevice::Text) ){
        QMessageBox::warning(this, "Error", "El archivo no se pudo abrir");
        return;
    }
    QTextStream text(&file);
    ui->textEdit->setText(text.readAll());
    file.close();
    setFileName(fileName);
    setWindowModified(false);
}

void MainWindow::setFileName(const QString &fileName){
    m_fileName = fileName;
    setWindowTitle( QString("%1[*] - %2")
                    .arg(m_fileName.isNull()?"untitled":QFileInfo(m_fileName).fileName())
                    .arg(QApplication::applicationName()) );
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Abrir documento",
                                QDir::currentPath(), "Archivos de Texto (*.txt)");
    if(filename.isNull())
        return;
    if( m_fileName.isNull() && !isWindowModified())
        loadFile(filename);
    else{
        MainWindow *s = new MainWindow(0, filename);
        s->show();
    }
}

bool MainWindow::saveFileAs(){
    QString filename = QFileDialog::getSaveFileName(this, "Guardar documento",
                                    m_fileName.isNull()?QDir::currentPath():m_fileName, "Archivos de Texto (*.txt)");
    if(filename.isNull())
        return false;
    setFileName(filename);
    return saveFile();
}

bool MainWindow::saveFile(){
    if (m_fileName.isNull())
        return saveFileAs();
    QFile file(m_fileName);
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Error", "Se produjo un error al intentar guardar el archivo");
        setFileName(QString());
        return false;
    }
    QTextStream tFile(&file);
    tFile << ui->textEdit->toPlainText();
    file.close();
    setWindowModified(false);
    return true;
}
