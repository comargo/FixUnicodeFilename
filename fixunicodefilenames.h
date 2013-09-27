#ifndef FIXUNICODEFILENAMES_H
#define FIXUNICODEFILENAMES_H

#include <QDialog>
class QFileSystemModel;
class QItemSelection;

namespace Ui {
class FixUnicodeFilenames;
}

class FixUnicodeFilenames : public QDialog
{
    Q_OBJECT

public:
    explicit FixUnicodeFilenames(QWidget *parent = 0);
    ~FixUnicodeFilenames();

private slots:
    void on_browseButton_clicked();

    void on_folderLineEdit_textChanged(const QString &newText);

    void on_checkSelectAll_stateChanged(int arg1);

    void onDirTreeViewSelectionChanged(const QItemSelection &selected,const QItemSelection &deselected);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void accept();

private:
    Ui::FixUnicodeFilenames *ui;
    QFileSystemModel *fsmodel;
};

#endif // FIXUNICODEFILENAMES_H
