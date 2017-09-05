/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include "global.h"
#include "pair.h"
#include "list.h"
#include "wrapper.h"
#include "crypto.h"
#include "databace.h"
#include <sys/time.h>
#include "QMessageBox"
#include <tomcrypt.h>
#include <QTableWidget>
#include "threadfcrypto.h"
#include "threadsetup.h"

#define BASE_LEN 10


static unsigned int  chain1[5][5000]; //the searching result
//static unsigned int  chain2[5][250]; //keyword or id was deleted

static int            fflag = 0;
static char         file[256] = "../original/keyword_id.txt";
static unsigned char    key[16] = "heshuanghong";  // to encrypt plain
static unsigned int id_number = 200000;// how many ids would be generated

static unsigned int total = 51000; // how many pairs would be generated

static unsigned int id_max = 0;//the max id

static pair_handle* ph;
static dsse_handle* dh;
static MainWindow* mainwindow = NULL;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(mainwindow == NULL){
        mainwindow = this;
    }
    QWidget::connect( ui->tabWidget, SIGNAL(tabBarClicked()), this, SLOT( on_tabWidget_tabBarClicked()));
}
MainWindow* MainWindow::getMainWindowHandle()
{
    return mainwindow;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetLineEditText(int index,const QString& str)
{
    switch(index)
    {
        case 0:   ui->lineEdit_knum->setText(str);
                       break;
        case 1:   ui->lineEdit_time->setText(str);
                       break;
        case 2:   ui->lineEdit_time1->setText(str);
                       break;
        case 3:   ui->lineEdit_time2->setText(str);
                       break;
        case 4:   ui->lineEdit_time3->setText(str);
                       break;
        case 5:   ui->lineEdit_key1->setText(str);
                       break;
        case 6:   ui->lineEdit_key2->setText(str);
                       break;
        case 7:   ui->lineEdit_time4->setText(str);
                       break;
        case 8:   ui->lineEdit_fnum->setText(str);
                       break;
        case 9:   ui->time_creat->setText(str);
                       break;
        case 10:   ui->time_setup->setText(str);
                       break;
        default:
                      break;

    }
}

void MainWindow::set_time_stamp( void )
{
    gettimeofday( &stamp, 0 );
}

void  MainWindow::get_time_usage( int index )
{
    struct timeval now;
    gettimeofday( &now, 0 );
    if( now.tv_usec < stamp.tv_usec )
    {
        now.tv_sec  -= 1;
        now.tv_usec += 1000000;
    }
    char timeuse[64];
    sprintf(timeuse,"%lds%ldus",now.tv_sec - stamp.tv_sec,now.tv_usec - stamp.tv_usec);
    MainWindow::SetLineEditText( index,timeuse);
}

void MainWindow::show_tabWidgetInfo(int index)
{
    if( index==1 )
    {
            //keywords
            QString surname[ ] = {
            "ANY","赵","钱","孙","李","周","吴","郑","王","冯","陈",
            "褚","卫","蒋","沈","韩","杨","朱","秦","尤","许",
            "何","吕","施","张","孔","曹","严","华","金","魏",
            "陶","姜","戚","谢","邹","喻","柏","窦","章","云",
            "苏","潘","葛","奚","范","彭","郎","姚","鲁","韦",
            "昌","马","苗","凤","花","方","俞","任","袁","柳",
            "鲍","史","唐","费","廉","岑","薛","雷","贺","倪",
            "汤","滕","殷","罗","毕","郝","邬","安","常","乐",
            "于","时","傅","皮","卞","齐","康","伍","余","元",
            "卜","顾","孟","平","黄","和","穆","萧","尹","END"};
            QString college[ ] = {
            "ANY","北京大学","清华大学","上海交大","复旦大学","武汉大学","浙江大学","中国人大",
            "南京大学","吉林大学","中山大学","北京师大","华中科大","四川大学","中国科大",
            "南开大学","西安交大","厦门大学","哈工大","同济大学","长江师范","天津大学",
            "华东师范","东南大学","中国农大","华南理工","湖南大学","大连理工","北京理工",
            "中国矿大","华中师大","西北大学","兰州大学","华东理工","电子科大","长安大学",
            "南京理工","武汉理工","西南大学","苏州大学","南京师范","云南大学","河南大学",
            "陕西师范","中南财大","上海大学","上海财经","山西大学","福州大学","南昌大学",
            "华南农大","新疆大学","安徽大学","南洋理工","海南大学","东北林大","福建师范",
            "贵州大学","宁夏大学","河北大学","昆明理工","扬州大学","燕山大学","浙江师范",
            "山东师范","天津医大","西南政法","上海理工","山东农大","上海师范","东北财大",
            "湖南农大","天津师范","江西师范","西安理工","西藏大学","福建农大","中北大学",
            "南京林大","长沙理工","青岛大学","成都理工","河北农大","沈阳农大","河南农大",
            "河南理工","兰州理工","河南师范","浙江理工","广西师范","江西财大","安徽医大",
            "杭州师范","华侨大学","安徽农大","四川师范","吉林农大","温州医大","南京邮大",
            "汕头大学","贵州师范","山西师范","长江大学","西安科大","淮北师范","吉首大学",
            "外交学院","集美大学","西安美院","大连大学","东华理工","西安邮大","北华大学",
            "烟台大学","聊城大学","宁夏医大","重庆大学","东北大学","四川农大","东北农大",
            "华中农大","北京交大","南京农大","西南交大","郑州大学","华南师范","江苏大学",
            "江南大学","东华大学","辽宁大学","中央财大","深证大学","广西大学","鲁东大学",
            "安徽师范","湖北大学","延边大学","青海大学","山东科大","河北师范","温州大学",
            "长春理工","宁波大学","河北医大","兰州交大","青岛科大","云南师范","武汉纺织",
            "辽宁师范","沈阳工大","广州大学","END" };
            QString city[ ]= {
            "ANY", "北京","天津","沈阳","长春","哈尔滨","上海","南京","武汉","广州","重庆",
            "成都","西安","石家庄","唐山","太原","包头","大连","鞍山","抚顺","吉林",
            "涪陵","徐州","杭州","福州","南昌","济南","青岛","淄博","南川","郑州",
            "贵阳","昆明","邯郸","保定","张家口","秦皇岛","邢台","承德","沧州","三门峡",
            "大同","武隆","本溪","丹东","锦州","阜新","辽阳","鸡西","鹤岗","大庆",
            "伊春","无锡","常州","苏州","宁波","合肥","淮南","淮北","厦门","枣庄",
            "烟台","潍坊","泰安","临沂","开封","洛阳","顶山","安阳","新乡","焦作",
            "黄石","襄樊","荆州","湘潭","深圳","汕头","湛江","南宁","柳州","西宁",
            "廊坊","衡水","阳泉","黄山","长治","乌海","赤峰","营口","盘锦","朝阳",
            "四平","铁岭","辽源","通化","白山","松原","白城","南通","连云港","淮阴",
            "盐城","扬州","镇江","泰州","温州","嘉兴","湖州","绍兴","台州","芜湖",
            "蚌埠","马鞍山","铜陵","安庆","阜阳","泉州","漳州","南平","龙岩","景德镇",
            "萍乡","九江","新余","东营","济宁","威海","日照","莱芜","德州","鹤壁",
            "许昌","漯河","南阳","商丘","十堰","宜昌","鄂州","荆门","孝感","黄冈",
            "岳阳","常德","益阳","郴州","永州","怀化","韶关","珠海","佛山","江门",
            "潮州","桂林","梧州","贵港","海口","自贡","攀枝花","泸州","德阳","绵阳",
            "六盘水","遵义","曲靖","铜川","宝鸡","咸阳","汉中","白银","天水","银川",
            "黑河","宿迁","金华","衢州","舟山","END"};
            QString  major[ ]= {
            "ANY","新闻学","广告学","传播学","历史学","考古学","应用数学","物理学","应用物理学",
            "化学","天文学","地理信息科学","海洋科学","地质学","地球化学","生物科学",
            "生物技术","生物信息学","心理学","统计学","工程力学","机械工程","工业设计",
            "控制工程","远教工程","测控技术","材料物理","材料化学","教育学",
            "汉语言","英语","冶金工程","高分子材料","复合材料","能源与动力",
            "机械电子工程","电气工程","信息工程","自动化","中医学","口腔医学",
            "电子信息工程","电子科学","通信工程","微电子科学","光电信息科学",
            "药学","法医学","工程管理","会计学","市场营销","计算机科学","软件工程",
            "网络工程","信息安全","物联网工程","数字媒体技术","土木工程","建筑电气",
            "采矿工程","石油工程","给排水工程","水利水电工程","测绘工程","化学工程",
            "制药工程","地质工程","核工程","农业工程","深林工程","林产化工","社会学",
            "软化工程","包装工程","交通运输","交通工程","船海技术","轮机工程","飞行技术",
            "船舶与海洋工程","车辆工程","预防医学","财务管理","国际商务","人力资源",
            "审计学","哲学","逻辑学","宗教学","经济学","经济统计学","END"};
            QString year[ ] = {
                "ANY","1943年","1944年","1945年","1946年","1947年","1948年","1949年",
                "1950年","1951年","1952年", "1953年","1954年","1955年","1956年","1957年","1958年","1959年",
                "1960年","1961年","1962年","1963年","1964年","1965年","1966年","1967年","1968年","1969年",
                "1970年","1971年","1972年","1973年","1974年","1975年","1976年","1977年","1978年","1979年",
                "1980年","1981年","1982年","1983年","1984年","1985年","1986年","1987年","1988年","1989年",
                "1990年","1991年","1992年","1993年","1994年","1995年","1996年","1997年","END"
                };
            int i=0;
            while(surname[i]!="END")
            {
               ui->comboBox_xing->addItem(surname[i],-1);
               i++;
            }
            i=0;
            while(city[i]!="END")
            {
               ui->comboBox_addr->addItem(city[i],-1);
               i++;
            }
            i=0;
            while(major[i]!="END")
            {
               ui->comboBox_major->addItem(major[i],-1);
               i++;
            }
            i=0;
            while( college[i]!="END")
            {
               ui->comboBox_univ->addItem(college[i],-1);
               i++;
            }
            i=0;
            while( year[i]!="END" )
            {
                ui->comboBox_year->addItem(year[i],-1);
                i++;
            }
            ui->comboBox_addr->setCurrentIndex(0);
            ui->comboBox_year->setCurrentIndex(0);
            ui->comboBox_univ->setCurrentIndex(0);
            ui->comboBox_major->setCurrentIndex(0);
            ui->comboBox_xing->setCurrentIndex(0);
            ui->lineEdit_11->clear();
            ui->lineEdit_12->clear();
            ui->lineEdit_21->clear();
            ui->lineEdit_22->clear();
            ui->lineEdit_31->clear();
            ui->lineEdit_32->clear();
            ui->lineEdit_41->clear();
            ui->lineEdit_42->clear();
            ui->lineEdit_51->clear();
            ui->lineEdit_52->clear();
            ui->lineEdit_61->clear();
            ui->lineEdit_62->clear();
            ui->lineEdit_other->clear();
            ui->lineEdit_result->clear();
            ui->lineEdit_time->clear();
            ui->tableWidget->clear();
            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            QStringList header;
            header<<"ID"<<"帐号"<<"密码"<<"邮箱"<<"姓氏"<<"生日"<<"籍贯"<<"大学"<<"专业";
            ui->tableWidget->setHorizontalHeaderLabels(header);
      }
    if(index == 2)
    {
        ui->lineEdit_addzh->clear();
        ui->lineEdit_addmm->clear();
        ui->lineEdit_addyx->clear();
        ui->lineEdit_addxs->clear();
        ui->lineEdit_addsr->clear();
        ui->lineEdit_addjg->clear();
        ui->lineEdit_adddx->clear();
        ui->lineEdit_addzy->clear();
        ui->lineEdit_addid->clear();
        ui->lineEdit_addkeyword->clear();
        ui->lineEdit_delf->clear();
        ui->lineEdit_delkeyword->clear();
        ui->lineEdit_delid->clear();
        ui->lineEdit_time1->clear();
        ui->lineEdit_time2->clear();
        ui->lineEdit_time3->clear();
        ui->lineEdit_time4->clear();
    }
}

void MainWindow::on_pushButton_creat_clicked()
{
    QString fnum;
    if( fflag == 1)
    {
        QMessageBox::information(this, tr("提示"), tr("密文文件系统已存在!"), QMessageBox::Yes);
        FILE *flag_file;
        char flag[10];
        set_time_stamp();
        flag_file = fopen( "../cipher/flag.txt", "r" );
        fgets( flag,9, flag_file );//discard the first line
        fgets( flag, 9, flag_file );
        ui->lineEdit_fnum->setText( flag );
        ui->fprogressBar->setValue(100);
        get_time_usage(9);
     }
    else
    {
            set_time_stamp();
            Threadfcrypto myThread;
            myThread.start();
            QMessageBox::information(this, tr("提示"), tr("请等待(创建完成后点击''Yes'')"), QMessageBox::Yes);
            get_time_usage(9);
    }
    /*
    else
        {
           if( (id_number = encryption( key )) != 0 )
               fnum = QString::number(id_number, 10);
               ui->lineEdit_fnum->setText( fnum );
               id_max = id_number -1;
               setflag((int)id_number,(int)id_max);
               QMessageBox::information(this, tr("提示"), tr("密文文件系统创建成功!"), QMessageBox::Yes);
        }
*/
}
void MainWindow::on_pushButton_acquire_clicked()
{
    char flag[10];
    checkflag(flag,sizeof(flag));
    ui->lineEdit_flag->setText( flag);
    if(strcmp(flag, "Y\n")== 0)
      fflag = 1;
}

void MainWindow::on_pushButton_check_clicked()
{
    ph = pair_init( total, file );
    if( ph == 0 )
    {
        QMessageBox::warning( this, tr("错误"), tr("加载关键字出错!"), QMessageBox::Yes);
       return ;
    }
    QMessageBox::information( this, tr("提示"), tr("关键字检测完成!"), QMessageBox::Yes);
}

void MainWindow::on_pushButton_load_clicked()
{
    QString kinum;
    if( pair_generate(ph) == 0 )
    {
        QMessageBox::warning( this, tr("错误"), tr("关键字id对加载失败!"), QMessageBox::Yes);
        pair_free( ph );
        ph = 0;
        return ;
    }
    kinum = QString::number(ph->pair_amount, 10);
    ui->lineEdit_kinum->setText(kinum);
    QMessageBox::information(this, tr("提示"), tr("关键字id对加载成功!"), QMessageBox::Yes);
}

void MainWindow::on_pushButton_setup_clicked()
{
    set_time_stamp();
    threadsetup myThread;
    myThread.start();
    QMessageBox::information(this, tr("提示"), tr("请等待(启动完成后点击''Yes'')"), QMessageBox::Yes);
    get_time_usage(10);
    /*unsigned int id;
    char *word;
     dh = dsse_init();
     int count = 0;
     if( dh == 0 )
     {
          QMessageBox::warning( this, tr("错误"), tr("初始化dsse handle出错!"), QMessageBox::Yes);
           return ;
     }
     while( pair_get_item(ph, &word, &id) )
     {
         if( dsse_add(dh, id, word) == 0 )
             break;
         count++;
         ui->progressBar->setValue(count*100/ ((int)ph->pair_amount));
     }
     if(count == (int)ph->pair_amount)
        QMessageBox::information(this, tr("提示"), tr("系统启动成功"), QMessageBox::Yes);
        */
}
void MainWindow::setup()
{
    unsigned int id;
    char *word;
    dh = dsse_init();
    int count = 0;
    if( dh == 0 )
    {
        QMessageBox::warning( this, tr("错误"), tr("初始化dsse handle出错!"), QMessageBox::Yes);
        return;
    }

    while( pair_get_item(ph, &word, &id) )
    {
        if( dsse_add(dh, id, word) == 0 )
            break;
        count++;
        ui->progressBar->setValue(count*100/ ((int)ph->pair_amount));
    }
}

void MainWindow::on_pushButton_search_clicked()
{
    int i = 0;
    int j = 0;
    int a[5] = { 0 };
    //int b[5] = { 0 };
    int tok=0;
    unsigned int* result;
    char  token[6][32] = {0};
    unsigned char bkey1[32]={0},bkey2[32]={0};
    unsigned char key1[17]={0},key2[17]={0};
    unsigned long outlen1=17,outlen2=17;

    QString qkey1,qkey2;
    qkey1=ui->lineEdit_key1->text();
    qkey2=ui->lineEdit_key2->text();

    std::string str1 = qkey1.toStdString();
    const char *ch1 = str1.c_str();
    strcpy((char*)bkey1,ch1);
    base64_decode(bkey1, (unsigned long)strlen((char*)bkey1), key1, &outlen1);

    std::string str2 = qkey2.toStdString();
    const char *ch2 = str2.c_str();
    strcpy((char*)bkey2,ch2);
    base64_decode(bkey2, (unsigned long)strlen((char*)bkey2), key2, &outlen2);

    QString addr,year,xing,univ,major;
    unsigned char token1[BASE_LEN],token2[BASE_LEN];
    addr=ui->comboBox_addr->currentText();
    year=ui->comboBox_year->currentText();
    xing=ui->comboBox_xing->currentText();
    univ=ui->comboBox_univ->currentText();
    major=ui->comboBox_major->currentText();
    if(ui->comboBox_addr->currentIndex()>0)
    {
     std::string str = addr.toStdString();
    const char *ch = str.c_str();
    strcpy(token[tok],ch);
    crypto_hmac(key1,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token1, BASE_LEN );
    crypto_hmac(key2,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token2, BASE_LEN );
    unsigned char out1[32]={0},out2[32]={0};
    unsigned long outlen1=32,outlen2=32;
    base64_encode( token1, BASE_LEN, out1, &outlen1);
    base64_encode( token2, BASE_LEN, out2, &outlen2);
    ui->lineEdit_31->setText((char*)out1);
    ui->lineEdit_32->setText((char*)out2);
    tok++;
    }
    if(ui->comboBox_year->currentIndex()>0)
    {
     std::string str = year.toStdString();
    const char *ch = str.c_str();
    strcpy(token[tok],ch);
    crypto_hmac(key1,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token1, BASE_LEN );
    crypto_hmac(key2,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token2, BASE_LEN );
    unsigned char out1[32]={0},out2[32]={0};
    unsigned long outlen1=32,outlen2=32;
    base64_encode( token1, BASE_LEN, out1, &outlen1);
    base64_encode( token2, BASE_LEN, out2, &outlen2);
    ui->lineEdit_21->setText((char*)out1);
    ui->lineEdit_22->setText((char*)out2);
     tok++;
    }
    if(ui->comboBox_xing->currentIndex()>0)
    {
     std::string str = xing.toStdString();
    const char *ch = str.c_str();
    strcpy(token[tok],ch);
    crypto_hmac(key1,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token1, BASE_LEN );
    crypto_hmac(key2,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token2, BASE_LEN );
    unsigned char out1[32]={0},out2[32]={0};
    unsigned long outlen1=32,outlen2=32;
    base64_encode( token1, BASE_LEN, out1, &outlen1);
    base64_encode( token2, BASE_LEN, out2, &outlen2);
    ui->lineEdit_11->setText((char*)out1);
    ui->lineEdit_12->setText((char*)out2);
     tok++;
    }
    if(ui->comboBox_univ->currentIndex()>0)
    {
     std::string str = univ.toStdString();
    const char *ch = str.c_str();
    strcpy(token[tok],ch);
    crypto_hmac(key1,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token1, BASE_LEN );
    crypto_hmac(key2,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token2, BASE_LEN );
    unsigned char out1[32]={0},out2[32]={0};
    unsigned long outlen1=32,outlen2=32;
    base64_encode( token1, BASE_LEN, out1, &outlen1);
    base64_encode( token2, BASE_LEN, out2, &outlen2);
    ui->lineEdit_41->setText((char*)out1);
    ui->lineEdit_42->setText((char*)out2);
     tok++;
    }
    if(ui->comboBox_major->currentIndex()>0)
    {
     std::string str = major.toStdString();
    const char *ch = str.c_str();
    strcpy(token[tok],ch);
    crypto_hmac(key1,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token1, BASE_LEN );
    crypto_hmac(key2,
                        (unsigned char*)token[tok], strlen(token[tok]),
                       token2, BASE_LEN );
    unsigned char out1[32]={0},out2[32]={0};
    unsigned long outlen1=32,outlen2=32;
    base64_encode( token1, BASE_LEN, out1, &outlen1);
    base64_encode( token2, BASE_LEN, out2, &outlen2);
    ui->lineEdit_51->setText((char*)out1);
    ui->lineEdit_52->setText((char*)out2);
     tok++;
    }
    QString qother;
    qother = ui->lineEdit_other->text();
    if(qother.length())
    {
        std::string str = qother.toStdString();
        const char *ch = str.c_str();
        strcpy(token[tok],ch);
        crypto_hmac(key1,
                            (unsigned char*)token[tok], strlen(token[tok]),
                           token1, BASE_LEN );
        crypto_hmac(key2,
                            (unsigned char*)token[tok], strlen(token[tok]),
                           token2, BASE_LEN );
        unsigned char out1[32]={0},out2[32]={0};
        unsigned long outlen1=32,outlen2=32;
        base64_encode( token1, BASE_LEN, out1, &outlen1);
        base64_encode( token2, BASE_LEN, out2, &outlen2);
        ui->lineEdit_61->setText((char*)out1);
        ui->lineEdit_62->setText((char*)out2);
         tok++;
     }
      set_time_stamp(); // TEST
      while(i<tok)
        {
            list_handle* I = list_init( sizeof(unsigned int) );
            list_handle* J = list_init( sizeof(unsigned int) );
            if( I == 0 || J ==0 )
            {
                print_error();
                printf( "init result sets error\n" );
                if( I ) list_free( I );
                if( J ) list_free( J );
                return ;
            }
            dsse_search( dh, token[i], I, J );
            while( list_get_item(I, (void**)(&result)) )
                    {
                    chain1[j][a[j]++] = *result;
                    }
           /* while( list_get_item(J, (void**)(&result)) )
            {
                    chain2[j][b[j]++] = *result;
            }*/
            i++,j++;
            list_free( I );
            list_free( J );
        }

                // caculate the intersection
        unsigned int same[5000];
        int k,c,e;
                for(  k = 0; k < a[0]; k++ )
        {
            same[k] = chain1[0][k];
        }
        int n = a[0];
                for(  k = 0; k < j; k++ )
        {
                    int num = 0;
            for(  c = 0; c < n; c++ )
                        {
                                for(  e = 0; e < a[k]; e++ )
                {
                    if( same[c] == chain1[k][e] )
                                        {
                        same[num++] = same[c];
                        break;
                    }
                }
            }
            n = num;
               }
           QString rnum;
           rnum = QString::number(n, 10);
           ui->lineEdit_result->setText(rnum);
               if( deciphering( key, same, n ) == 0 )
               {
                    QMessageBox::warning(this, tr("错误"),tr("解密文件出错"), QMessageBox::Yes);
                    return ;
               }
               get_time_usage(1); // TEST
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->comboBox_addr->setCurrentIndex(0);
    ui->comboBox_year->setCurrentIndex(0);
    ui->comboBox_univ->setCurrentIndex(0);
    ui->comboBox_major->setCurrentIndex(0);
    ui->comboBox_xing->setCurrentIndex(0);
    ui->lineEdit_11->clear();
    ui->lineEdit_12->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_22->clear();
    ui->lineEdit_31->clear();
    ui->lineEdit_32->clear();
    ui->lineEdit_41->clear();
    ui->lineEdit_42->clear();
    ui->lineEdit_51->clear();
    ui->lineEdit_52->clear();
    ui->lineEdit_61->clear();
    ui->lineEdit_62->clear();
    ui->lineEdit_other->clear();
    ui->lineEdit_result->clear();
    ui->lineEdit_time->clear();
    ui->tableWidget->clear();
    QStringList header;
    header<<"ID"<<"帐号"<<"密码"<<"邮箱"<<"姓氏"<<"生日"<<"籍贯"<<"大学"<<"专业";
    ui->tableWidget->setHorizontalHeaderLabels(header);
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    MainWindow::show_tabWidgetInfo(index);
}

void MainWindow::on_pushButton_clicked()
{
    /*
     if( (id_number = encryption( key )) != 0 )
            QMessageBox::information(this, tr("提示"), tr("密文文件系统还原成功!"), QMessageBox::Yes);
    FILE *flag_file;
    char file[64];
    char flag[10];
    int num;
    flag_file = fopen( "../cipher/flag.txt", "r" );
    fgets( flag,9, flag_file );//discard the first line
    fscanf( flag_file, "%d\n", &num );//discard the second line
    fscanf( flag_file,"%d",&id_max);
    for( ;id_max>id_number-1;id_max--)
    {
        sprintf( file, "../cipher/%d.txt", id_max );
        remove(file);
    }
    setflag((int)id_number,(int)id_max);
   */
    dsse_free( dh );
    pair_free( ph );
    this->close();
}

void MainWindow::on_pushButton_add_clicked()
{
    QString qid,qword;
    unsigned int id;
    char word[33];
    qid=ui->lineEdit_addid->text();
    qword=ui->lineEdit_addkeyword->text();
    id = qid.toInt();
    std::string str = qword.toStdString();
    const char *ch = str.c_str();
    strcpy(word,ch);
     set_time_stamp();
    if( dsse_add(dh, (unsigned)id, word) != 1 )
    {
           QMessageBox::warning(this, tr("错误"),tr("添加KeyWord_ID对失败"), QMessageBox::Yes);
           return ;
     }
    get_time_usage(2);
    QMessageBox::information(this, tr("提示"),tr("添加KeyWord_ID对成功"), QMessageBox::Yes);
}

void MainWindow::on_pushButton_deleteid_clicked()
{
    QString qid;
    qid=ui->lineEdit_delf->text();
    unsigned int id;
    id = qid.toInt();
    char file[64];
    set_time_stamp();
        if( dsse_delete_id(dh, (unsigned int)id) != 0 )
        {
            get_time_usage(3);
            QMessageBox::information(this, tr("提示"),tr("删除ID成功"), QMessageBox::Yes);
        }
        sprintf( file, "../cipher/%d.txt", id );
        if( remove( file ) != 0 )
         {
                 QMessageBox::warning(this, tr("错误"),tr("删除密文文件失败"), QMessageBox::Yes);
                 return ;
         }
         QMessageBox::information(this, tr("提示"),tr("删除密文文件成功"), QMessageBox::Yes);
         QString Qfnum;
         FILE *flag_file;
         char flag[10];
         unsigned int num;
         flag_file = fopen( "../cipher/flag.txt", "r" );
         fgets( flag,9, flag_file );//discard the first line
         fscanf( flag_file, "%d\n", &num );//discard the second line
         fscanf( flag_file,"%d",&id_max);
         num--;
         if(id==id_max)
               id_max--;
         setflag(num,id_max);
         Qfnum = QString::number(num , 10);
         ui->lineEdit_fnum->setText(Qfnum);
}

void MainWindow::on_pushButton_delkeyword_clicked()
{
    QString qid,qword;
     unsigned int id;
    char word[16];
    qid=ui->lineEdit_delid->text();
    qword=ui->lineEdit_delkeyword->text();
    id = qid.toInt();
    std::string str = qword.toStdString();
    const char *ch = str.c_str();
    strcpy(word,ch);
    set_time_stamp();
    if( dsse_delete_mix(dh, (unsigned int)id, word) == 0 )
    {
        QMessageBox::warning(this, tr("错误"),tr("删除KeyWord_ID对失败"), QMessageBox::Yes);
        return ;
    }
    get_time_usage(4);
    QMessageBox::information(this, tr("提示"),tr("删除KeyWord_ID对成功"), QMessageBox::Yes);
}
void MainWindow::ShowfprogressBar(int n,int m)
{
    ui->fprogressBar->setValue(n*100/m);
}

void MainWindow::ShowResult(int row, int id,char* result )
{
    QString qid;
    qid = QString::number(id, 10);

    char List[8][32]={0};
    sscanf( result, "%s\t%*s\t%s\t%*s\t%s\t%*s\t%s\t%*s\t%s\t%*s\t%s\t%*s\t%s\t%*s\t%s\n",
                            List[0], List[1], List[2], List[3], List[4], List[5],List[6], List[7] );
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,80);
    ui->tableWidget->setColumnWidth(3,80);
    ui->tableWidget->setColumnWidth(4,80);
    ui->tableWidget->setColumnWidth(5,80);
    ui->tableWidget->setColumnWidth(6,80);
    ui->tableWidget->setColumnWidth(7,80);
    ui->tableWidget->setColumnWidth(8,80);
     ui->tableWidget->setItem(row,0,new QTableWidgetItem(qid));
     ui->tableWidget->setItem(row,1,new QTableWidgetItem(List[0]));
     ui->tableWidget->setItem(row,2,new QTableWidgetItem(List[1]));
     ui->tableWidget->setItem(row,3,new QTableWidgetItem(List[2]));
     ui->tableWidget->setItem(row,4,new QTableWidgetItem(List[3]));
     ui->tableWidget->setItem(row,5,new QTableWidgetItem(List[4]));
     ui->tableWidget->setItem(row,6,new QTableWidgetItem(List[5]));
     ui->tableWidget->setItem(row,7,new QTableWidgetItem(List[6]));
     ui->tableWidget->setItem(row,8,new QTableWidgetItem(List[7]));
}

void MainWindow::on_pushButton_addfile_clicked()
{
    QString qzh,qmm,qyx,qxs,qsr,qdx,qzy,qjg;
    char zh[32],mm[32],yx[32],xs[32],sr[32],jg[32],dx[32],zy[32];
    qzh=ui->lineEdit_addzh->text();
    std::string str1 = qzh.toStdString();
    const char *ch1 = str1.c_str();
    strcpy(zh,ch1);
    if(qzh==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("帐号不能为空！"), QMessageBox::Yes);
        return ;
    }
    qmm=ui->lineEdit_addmm->text();
    std::string str2 = qmm.toStdString();
    const char *ch2 = str2.c_str();
    strcpy(mm,ch2);
    if(qmm==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("密码不能为空！"), QMessageBox::Yes);
        return ;
    }
    qyx=ui->lineEdit_addyx->text();
    std::string str3 = qyx.toStdString();
    const char *ch3 = str3.c_str();
    strcpy(yx,ch3);
    if(qyx==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("邮箱不能为空！"), QMessageBox::Yes);
        return ;
    }
    qxs=ui->lineEdit_addxs->text();
    std::string str4 = qxs.toStdString();
    const char *ch4 = str4.c_str();
    strcpy(xs,ch4);
    if(qxs==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("姓氏不能为空！"), QMessageBox::Yes);
        return ;
    }
    qjg=ui->lineEdit_addjg->text();
    std::string str5 = qjg.toStdString();
    const char *ch5 = str5.c_str();
    strcpy(jg,ch5);
    if(qjg==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("籍贯不能为空！"), QMessageBox::Yes);
        return ;
    }
    qsr=ui->lineEdit_addsr->text();
    std::string str6 = qsr.toStdString();
    const char *ch6 = str6.c_str();
    strcpy(sr,ch6);
    if(qsr==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("生日不能为空！"), QMessageBox::Yes);
        return ;
    }
    qdx=ui->lineEdit_adddx->text();
    std::string str7 = qdx.toStdString();
    const char *ch7 = str7.c_str();
    strcpy(dx,ch7);
    if(qdx==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("大学不能为空！"), QMessageBox::Yes);
        return ;
    }
    qzy=ui->lineEdit_addzy->text();
    std::string str8 = qzy.toStdString();
    const char *ch8 = str8.c_str();
    strcpy(zy,ch8);
    if(qzy==0)
    {
        QMessageBox::warning(this, tr("警告"),tr("专业不能为空！"), QMessageBox::Yes);
        return ;
    }
    char plain[512];
    sprintf(plain,"%s # %s # %s # %s # %s # %s # %s # %s\n",
            zh, mm, yx, xs, sr, jg,dx, zy );
    FILE *flag_file;
    char flag[10];
    int num;
    flag_file = fopen( "../cipher/flag.txt", "r" );
    fgets( flag,9, flag_file );//discard the first line
    fscanf( flag_file, "%d\n", &num );//discard the second line
    fscanf( flag_file,"%d",&id_max);
    id_max++;
    num++;
    setflag(num,(int)id_max);
    QString qfnum;
    qfnum = QString::number(num , 10);
    ui->lineEdit_fnum->setText(qfnum);
    char cname[50];
    sprintf( cname, "../cipher/%d.txt", id_max);
    set_time_stamp();
    if( crypto_aes_cryp( key, id_max, strlen(plain), (unsigned char*)plain, (unsigned char*)cname ) == 0 )
    {
          print_error();
           printf( "encryption newfile failed!\n");
           return ;
    }
    if( dsse_add(dh, id_max, xs) != 1||dsse_add(dh, id_max, sr) != 1||dsse_add(dh, id_max, jg) != 1
            ||dsse_add(dh, id_max, dx) != 1 ||dsse_add(dh, id_max, zy) != 1 )
    {
           QMessageBox::warning(this, tr("添加文件错误"),tr("添加KeyWord_ID对失败"), QMessageBox::Yes);
           return ;
     }
     get_time_usage(7); // TEST
     QMessageBox::information(this, tr("提示"),tr("添加文件成功"), QMessageBox::Yes);
}

void MainWindow::on_renew_clicked()
{
    FILE *file;
    system("rm -rf ../cipher");
    system("mkdir  ../cipher");
    file = fopen("../cipher/flag.txt","w");
    fwrite("N",1,1,file);
    fclose(file);
    QMessageBox::information(this, tr("提示"), tr("系统还原成功!"), QMessageBox::Yes);
}
