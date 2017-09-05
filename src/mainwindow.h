/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct timeval stamp;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetLineEditText(int index,const QString& str);
    void ShowfprogressBar(int n,int m);
    void ShowResult( int row, int id, char* result );
    static MainWindow* getMainWindowHandle();
    void  set_time_stamp();
    void  get_time_usage( int index );
    void  setup();
private slots:
    void on_pushButton_creat_clicked();
    void show_tabWidgetInfo(int index);
    void on_pushButton_acquire_clicked();
    void on_pushButton_check_clicked();
    void on_pushButton_load_clicked();

    void on_pushButton_setup_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_clear_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_deleteid_clicked();

    void on_pushButton_delkeyword_clicked();

    void on_pushButton_addfile_clicked();

    void on_renew_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
