#include "fixunicodefilenames.h"
#include "ui_fixunicodefilenames.h"

#include <QFileDialog>
#include <QDragEnterEvent>
#include <QUrl>
#include <QFileSystemModel>
#include <QTextCodec>

#include <QtDebug>

FixUnicodeFilenames::FixUnicodeFilenames(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FixUnicodeFilenames)
{
    ui->setupUi(this);
    fsmodel = new QFileSystemModel(this);
    ui->dirTreeView->setModel(fsmodel);
    connect(ui->dirTreeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(onDirTreeViewSelectionChanged(QItemSelection,QItemSelection)));
    emit on_folderLineEdit_textChanged("");
    emit onDirTreeViewSelectionChanged(QItemSelection(), QItemSelection());
}

FixUnicodeFilenames::~FixUnicodeFilenames()
{
    delete ui;
}

void FixUnicodeFilenames::on_browseButton_clicked()
{
    QString pathToScan = QFileDialog::getExistingDirectory(this, tr("Select folder to scan"));
    if(pathToScan.isEmpty()) return;
    ui->folderLineEdit->setText(pathToScan);
}

void FixUnicodeFilenames::on_folderLineEdit_textChanged(const QString &newText)
{
    if(newText.isEmpty()) {
        ui->dirTreeView->setRootIndex(fsmodel->setRootPath(fsmodel->myComputer().toString()));
    }
    else {
        QDir pathToScan(QDir::cleanPath(newText));
        if(pathToScan.exists()) {
            ui->dirTreeView->setRootIndex(fsmodel->setRootPath(pathToScan.absolutePath()));
        }
    }
    ui->dirTreeView->selectionModel()->clearSelection();
}

void FixUnicodeFilenames::on_checkSelectAll_stateChanged(int arg1)
{
    Qt::CheckState state = static_cast<Qt::CheckState>(arg1);
    switch (state) {
    case Qt::Unchecked:
        ui->dirTreeView->selectionModel()->clearSelection();
        ui->checkSelectAll->setTristate(false);
        break;
    case Qt::Checked:
        ui->dirTreeView->selectAll();
        ui->checkSelectAll->setTristate(false);
        break;
    default:
        break;
    }
}

void FixUnicodeFilenames::onDirTreeViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QItemSelectionModel* selModel = ui->dirTreeView->selectionModel();
    QPushButton* okBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    if(okBtn)
        okBtn->setEnabled(selModel->hasSelection());
    if(!selModel->hasSelection()) {
        ui->checkSelectAll->setCheckState(Qt::Unchecked);
    }
    else {
        QModelIndex idx = fsmodel->index(0,0);
        bool selectedAll = true;
        while(selectedAll && idx.isValid()) {
            selectedAll = selectedAll && selModel->isSelected(idx);
            idx = ui->dirTreeView->indexBelow(idx);
        }
        if(selectedAll) {
            ui->checkSelectAll->setCheckState(Qt::Checked);
        }
        else {
            ui->checkSelectAll->setCheckState(Qt::PartiallyChecked);
        }
    }

}

void FixUnicodeFilenames::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls() && mimeData->urls().size() == 1 && mimeData->urls().at(0).isLocalFile()) {
        event->acceptProposedAction();
    }
    QDialog::dragEnterEvent(event);
}

void FixUnicodeFilenames::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls() && mimeData->urls().size() == 1 && mimeData->urls().at(0).isLocalFile())
    {
        QUrl url = mimeData->urls().at(0);
        ui->folderLineEdit->setText(QDir::toNativeSeparators(url.toLocalFile()));
    }
    QDialog::dropEvent(event);
}

void FixUnicodeFilenames::accept()
{
    QList<QPair<QString, QString> > paths;
    QModelIndexList selModel = ui->dirTreeView->selectionModel()->selectedRows();
    QTextCodec *codec = QTextCodec::codecForName("CP866");
    foreach (QModelIndex idx, selModel) {
        QString path(idx.data(QFileSystemModel::FilePathRole).toString());
        QString newPath = QString::fromUtf8(codec->fromUnicode(path));
        QFile::rename(path, newPath);
    }

}

