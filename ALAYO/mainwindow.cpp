#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QDesktopWidget"
#include "QTextStream"

//******************************
// ORIGINAL INPUT FILES
//******************************
// original MLB Information const
const string MLB_INFORMATION_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYOAlek/ALAYO---Project-2-Alek/ALAYO/inputMLBInformation.csv";
// original Distances const
const string DISTANCES_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYOAlek/ALAYO---Project-2-Alek/ALAYO/inputDistance.csv";
//original MLB Information expansion const
const string MLB_INFORMATION_EXPANSION_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYOAlek/ALAYO---Project-2-Alek/ALAYO/inputMLBInformationExpansion.csv";
//original Distances expansion const
const string DISTANCES_EXPANSION_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYOAlek/ALAYO---Project-2-Alek/ALAYO/inputDistanceExpansion.csv";

//******************************
// WRITE TO FILES
//******************************
// written to MLB Information const
const string MODIFIED_MLB_INFORMATION_OUTPUT_FILE = "C:/Users/Oscar/Desktop/ALAYOAlek/ALAYO---Project-2-Alek/ALAYO/inputModifiedMLBInformation.csv";
// written to Distances const
const string MODIFIED_DISTANCES_OUTPUT_FILE = "C:/Users/Oscar/Desktop/ALAYOAlek/ALAYO---Project-2-Alek/ALAYO/inputModifiedDistances.csv";
// written to souvenirs const
const string MODIFIED_SOUVENIRS_OUTPUT_FILE = "C:/Users/Oscar/Desktop/ALAYOAlek/ALAYO---Project-2-Alek/ALAYO/inputSouvenirs.csv";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readFromFiles(false);

    //allows user to move around items in the selected locations when taking a trip
    ui->selectedTeamsStackedWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->selectedTeamsStackedWidget->setDragEnabled(true);
    ui->selectedTeamsStackedWidget->setDragDropMode(QAbstractItemView::InternalMove);
    ui->selectedTeamsStackedWidget->viewport()->setAcceptDrops(true);
    ui->selectedTeamsStackedWidget->setDropIndicatorShown(true);

    ui->primaryPageStackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//************************************ PARSING ************************************************

// reads from the input files
// if readOriginal = true  - reads from original files (used for reinitializing)
// if readOriginal = false - checks whether files exist, if so reads from those. (used for startup)
void MainWindow::readFromFiles(bool readOriginal)
{
    bool original;
    int min = 1000000000, max = 0;

    if(readOriginal)
    {
        original = true;
    }
    else
    {
        ifstream csvFile1;
        csvFile1.open(MODIFIED_MLB_INFORMATION_OUTPUT_FILE);
        if(csvFile1.is_open())
        {
            original = false;
            csvFile1.close();
        }
        else
        {
            original = true;
        }
    }
    // creates a blank map and sets it to the main window map, so it is all deleted
    Map blankMap;
    thisMap = blankMap;
    entry thisEntry;

    string iStadiumName;
    string iName;
    string iSeatingCapacity;
    string iLocation;
    string iPlayingSurface;
    string iLeague;
    string iDateOpened;
    string iDistanceToCenterField;
    string iBallparkTypology;
    string iRoofType;

    ifstream csvFile1;

    if(original)
    {
        csvFile1.open(MLB_INFORMATION_INPUT_FILE);
    }
    else
    {
        csvFile1.open(MODIFIED_MLB_INFORMATION_OUTPUT_FILE);
    }

    if(csvFile1.is_open())
    {
        int index = 0;

        while(!csvFile1.eof())
        {
            getline(csvFile1, iName, ',');
            if(iName == "")
            {
                break;
            }
            getline(csvFile1, iStadiumName, ',');
            getline(csvFile1, iSeatingCapacity, ',');
            int iSeatingcapacity = stoi(iSeatingCapacity);
            csvFile1.ignore(1);
            getline(csvFile1, iLocation, '"');
            csvFile1.ignore(1);
            getline(csvFile1, iPlayingSurface, ',');
            getline(csvFile1, iLeague, ',');
            getline(csvFile1, iDateOpened, ',');
            int iDateopened = stoi(iDateOpened);
            getline(csvFile1, iDistanceToCenterField, ',');
            int idistanceToCenterField = stoi(iDistanceToCenterField);
            getline(csvFile1, iBallparkTypology, ',');
            getline(csvFile1, iRoofType, '\n');

            MLB thisMLB(index, iStadiumName, iSeatingcapacity,
                        iLocation, iPlayingSurface, iLeague,
                        iDateopened, idistanceToCenterField,
                        iBallparkTypology, iRoofType);

            thisMap.insert(iName, thisMLB);
            thisEntry.key = iName;
            thisEntry.value = thisMLB;
            
            allMLBTeamsAvailable.push_back(thisMLB);

            //this makes it so vector 3 contains only american
            if(thisMLB.getLeague() == "American")
            {
                v1.push_back(thisEntry);
                v2.push_back(thisEntry);
                v3.push_back(thisEntry);
                v5.push_back(thisEntry);
                v7 = v5;
                v8 = v5;
            }
            //this makes it so vector 4 contain only national teams
            else if(thisMLB.getLeague() == "National")
            {
                v1.push_back(thisEntry);
                v2.push_back(thisEntry);
                v4.push_back(thisEntry);
                v5.push_back(thisEntry);
                v7 = v5;
                v8 = v5;
            }
            if(thisMLB.getRoofType() == "Open")
            {
                v6.push_back(thisEntry);
            }
            if(thisMLB.getDistanceToCenterField() <= min){
                min = thisMLB.getDistanceToCenterField();
            }
            if(thisMLB.getDistanceToCenterField() >= max){
                max = thisMLB.getDistanceToCenterField();
            }


            index++;
        }
    }
    else
    {
        QTextStream(stdout) << "file1 opening failed...\n";
    }
    for(int i = 0; i < thisMap.mapSize(); i++){
        thisEntry.key = thisMap.atIndex(i).key;
        thisEntry.value = thisMap.atIndex(i).value;
        if(thisMap.atIndex(i).value.getDistanceToCenterField() == min){
            v9.push_back(thisEntry);
        }
        if(thisMap.atIndex(i).value.getDistanceToCenterField() == max){
            v10.push_back(thisEntry);
        }
    }

    csvFile1.close();

    string iStadium1;
    string iStadium2;
    string iDistance;
    string previous;
    int distance;
    int index1;
    int index2;
    bool found;

    ifstream csvFile2;

    if(original)
    {
        csvFile2.open(DISTANCES_INPUT_FILE);
    }
    else
    {
        csvFile2.open(MODIFIED_DISTANCES_OUTPUT_FILE);
    }

    if(csvFile2.is_open())
    {
        getline(csvFile2, iStadium1, ',');
        while(!csvFile2.eof())
        {
            index1 = 0;
            found = false;
            while(!found)
            {
                if(thisMap.atIndex(index1).value.getStadiumName() == iStadium1)
                {
                    thisMap.atIndex(index1).value.setInitialDistances(thisMap.mapSize());
                    found = true;
                }
                else
                {
                    index1++;
                }
            }

            previous = iStadium1;

            while(iStadium1 == previous)
            {
                getline(csvFile2, iStadium2, ',');
                getline(csvFile2, iDistance, '\n');
                distance = stoi(iDistance);

                index2 = 0;
                found = false;

                while(!found)
                {
                    if(thisMap.atIndex(index2).value.getStadiumName() == iStadium2)
                    {
                        found = true;
                        thisMap.atIndex(index1).value.setDistance(index2, distance);
                    }
                    else
                    {
                        index2++;
                    }
                }
                getline(csvFile2, iStadium1, ',');
            }
        }
    }
    else {
        QTextStream(stdout) << "file2 opening failed...";
    }

    csvFile2.close();

    int index3;
    string souvenirName;
    string souvenirPriceString;
    double souvenirPrice;

    ifstream csvFile3;

    if(original)
    {
        //add default souvenir items to each baseball stadium
        for(int i = 0; i < thisMap.mapSize(); i++)
        {
            thisMap.atIndex(i).value.addSouvenir(souvenir("Baseball cap", 22.99));
            thisMap.atIndex(i).value.addSouvenir(souvenir("Baseball bat", 89.93));
            thisMap.atIndex(i).value.addSouvenir(souvenir("Team pennant", 17.99));
            thisMap.atIndex(i).value.addSouvenir(souvenir("Autographed baseball", 25.99));
            thisMap.atIndex(i).value.addSouvenir(souvenir("Team jersey", 199.99));
        }
    }
    else
    {
        csvFile3.open(MODIFIED_SOUVENIRS_OUTPUT_FILE);

        if(csvFile3.is_open())
        {
            getline(csvFile3, iStadium1, ',');
            while(!csvFile3.eof())
            {
                found = false;
                index3 = 0;

                previous = iStadium1;

                while(!found)
                {
                    if(iStadium1 == thisMap.atIndex(index3).value.getStadiumName())
                    {
                        found = true;
                    }
                    else
                    {
                        index3++;
                    }
                }


                while(iStadium1 == previous)
                {
                    getline(csvFile3, souvenirName, ',');
                    getline(csvFile3, souvenirPriceString, '\n');
                    souvenirPrice = stod(souvenirPriceString);

                    thisMap.atIndex(index3).value.addSouvenir(souvenir(souvenirName, souvenirPrice));

                    getline(csvFile3, iStadium1, ',');
                }
            }
        }
    }
    if(readOriginal)
    {
        writeToFiles();
    }
}

void MainWindow::readExpansionFiles()
{
    ifstream csvFile1;
    csvFile1.open(MLB_INFORMATION_EXPANSION_INPUT_FILE);

    bool exists = false;
    int min = 1000000000, max = 0;
    string iStadiumName;
    string iName;
    string iSeatingCapacity;
    string iLocation;
    string iPlayingSurface;
    string iLeague;
    string iDateOpened;
    string iDistanceToCenterField;
    string iBallparkTypology;
    string iRoofType;

    entry thisEntry;

    int index = 0;

    while(!csvFile1.eof())
    {
        getline(csvFile1, iName, ',');

        for(int i = 0; i < thisMap.mapSize(); i++)
        {
            if(iName == thisMap.atIndex(i).key)
            {
                QMessageBox::critical(nullptr, "error", "You can't add a team that already exists!");
                exists = true;
            }
        }

        if(iName == "" || exists)
        {
            break;
        }
        getline(csvFile1, iStadiumName, ',');
        getline(csvFile1, iSeatingCapacity, ',');
        int iSeatingcapacity = stoi(iSeatingCapacity);
        csvFile1.ignore(1);
        getline(csvFile1, iLocation, '"');
        csvFile1.ignore(1);
        getline(csvFile1, iPlayingSurface, ',');
        getline(csvFile1, iLeague, ',');
        getline(csvFile1, iDateOpened, ',');
        int iDateopened = stoi(iDateOpened);
        getline(csvFile1, iDistanceToCenterField, ',');
        int idistanceToCenterField = stoi(iDistanceToCenterField);
        getline(csvFile1, iBallparkTypology, ',');
        getline(csvFile1, iRoofType, '\n');

        MLB thisMLB(index, iStadiumName, iSeatingcapacity,
                    iLocation, iPlayingSurface, iLeague,
                    iDateopened, idistanceToCenterField,
                    iBallparkTypology, iRoofType);

        thisMap.insert(iName, thisMLB);
        thisEntry.key = iName;
        thisEntry.value = thisMLB;

        //this makes it so vector 3 contains only american
        if(thisMLB.getLeague() == "American")
        {
            v1.push_back(thisEntry);
            v2.push_back(thisEntry);
            v3.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }
        //this makes it so vector 4 contain only national teams
        else if(thisMLB.getLeague() == "National")
        {
            v1.push_back(thisEntry);
            v2.push_back(thisEntry);
            v4.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }
        if(thisMLB.getRoofType() == "Open")
        {
            v6.push_back(thisEntry);
        }

        if(thisMLB.getDistanceToCenterField() <= min){
            min = thisMLB.getDistanceToCenterField();
        }
        if(thisMLB.getDistanceToCenterField() >= max){
            max = thisMLB.getDistanceToCenterField();
        }

        index++;

        ui->stadiumListWidget->addItem(QString::fromStdString(thisMap.atIndex(thisMap.mapSize()-1).key));
    }

    writeToFiles();
}

void MainWindow::writeToFiles()
{
    ofstream csvFile1;

    csvFile1.open(MODIFIED_MLB_INFORMATION_OUTPUT_FILE);

    for(int i = 0; i < thisMap.mapSize(); i++)
    {
       csvFile1 << thisMap.atIndex(i).key << ","
                << thisMap.atIndex(i).value.getStadiumName() << ","
                << thisMap.atIndex(i).value.getSeatingCapacity() << ","
                << "\"" << thisMap.atIndex(i).value.getLocation() << "\"" << ","
                << thisMap.atIndex(i).value.getPlayingSurface() << ","
                << thisMap.atIndex(i).value.getLeague() << ","
                << thisMap.atIndex(i).value.getDateOpened() << ","
                << thisMap.atIndex(i).value.getDistanceToCenterField() << ","
                << thisMap.atIndex(i).value.getBallParkTypology() << ","
                << thisMap.atIndex(i).value.getRoofType() << "\n";
    }
    csvFile1.close();

    ofstream csvFile2;

    csvFile2.open(MODIFIED_DISTANCES_OUTPUT_FILE);

    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        for(int k = 0; k < thisMap.atIndex(i).value.getDistanceSize(); k++)
        {
            if(thisMap.atIndex(i).value.getDistance(k) != 0)
            {
                csvFile2 << thisMap.atIndex(i).value.getStadiumName() << ","
                         << thisMap.atIndex(k).value.getStadiumName() << ","
                         << thisMap.atIndex(i).value.getDistance(k) << "\n";
            }
        }
    }
    csvFile2.close();

    ofstream csvFile3;

    csvFile3.open(MODIFIED_SOUVENIRS_OUTPUT_FILE);

    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        for(int k = 0; k < thisMap.atIndex(i).value.getSouvenirListSize(); k++)
        {
            csvFile3 << thisMap.atIndex(i).value.getStadiumName() << ","
                     << thisMap.atIndex(i).value.getSouvenir(k).itemName << ","
                     << thisMap.atIndex(i).value.getSouvenir(k).itemPrice << "\n";
        }
    }
    csvFile3.close();
}
//************************************ LOGIN ************************************************

//This method handles if the check box for show password has been clicked or not
void MainWindow::on_checkBox_showPW_stateChanged(int)
{
    if(ui->checkBox_showPW->checkState() == Qt::CheckState::Checked)
    {
        ui->passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Normal);
    }
    else
    {
        ui->passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    }
}

void MainWindow::on_loginPushButton_clicked()
{
    if(ui->usernameLineEdit->text() == "admin" && ui->passwordLineEdit->text() == "admin")
    {
        string value = "admin";
        int hash_key = rand() % 100 + 1;

        encryptionTable.putQuadraticHash(hash_key, value);

        ui->primaryPageStackedWidget->setCurrentIndex(1);
        ui->adminStackedWidget->setCurrentIndex(0);

        isAdmin = true;
    }
    else if(ui->usernameLineEdit->text() == "user" && ui->passwordLineEdit->text() == "user")
    {
        ui->primaryPageStackedWidget->setCurrentIndex(2);
        ui->userStackedWidget->setCurrentIndex(0);

        isAdmin = false;

        string value = "user";
        int hash_key = rand() % 100 + 1;

        encryptionTable.putQuadraticHash(hash_key, value);
    }
    else
    {
        //error
        QMessageBox::critical(nullptr, "error", "Wrong login information! Please try again.");
    }

    ui->passwordLineEdit->clear();
    ui->usernameLineEdit->clear();
}

//************************************ LOGOUT ************************************************
void MainWindow::on_actionLogout_triggered()
{
    ui->primaryPageStackedWidget->setCurrentIndex(0);
    //encryptionTable.clearTable();
}

//************************************ MANAGING STADIUMS (admin) ************************************************

void MainWindow::on_AddTeamButton_clicked()
{
    readExpansionFiles();
}

void MainWindow::on_ReinitializeButton_clicked()
{
    readFromFiles(true);
}

//this updates all the information in the vectors from the map
void MainWindow::updateVectors(){

    entry thisEntry;
    int min = 100000, max = 0;

    v1.clear();
    v2.clear();
    v3.clear();
    v4.clear();
    v5.clear();
    v6.clear();
    v7.clear();
    v8.clear();
    v9.clear();
    v10.clear();

    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        thisEntry.key = thisMap.atIndex(i).key;
        thisEntry.value = thisMap.atIndex(i).value;

        //this makes it so vector 3 contains only american teams
        if(thisMap.atIndex(i).value.getLeague() == "American"){
            v1.push_back(thisEntry);
            v2.push_back(thisEntry);
            v3.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }
        //this makes it so vectors 4 containes national teams
        else if(thisMap.atIndex(i).value.getLeague() == "National"){
            v1.push_back(thisEntry);
            v2.push_back(thisEntry);
            v4.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }
        if(thisMap.atIndex(i).value.getRoofType() == "Open"){
            v6.push_back(thisEntry);
        }
        if(thisMap.atIndex(i).value.getDistanceToCenterField() <= min){
            min = thisMap.atIndex(i).value.getDistanceToCenterField();
        }
        if(thisMap.atIndex(i).value.getDistanceToCenterField() >= max){
            max = thisMap.atIndex(i).value.getDistanceToCenterField();
        }
    }
    for(int i = 0; i < thisMap.mapSize(); i++){
        thisEntry.key = thisMap.atIndex(i).key;
        thisEntry.value = thisMap.atIndex(i).value;
        if(thisMap.atIndex(i).value.getDistanceToCenterField() == max){
            v9.push_back(thisEntry);
        }
        if(thisMap.atIndex(i).value.getDistanceToCenterField() == min){
            v10.push_back(thisEntry);
        }
    }
}

//if the user presses the manage stadiums button, this method changes the page
void MainWindow::on_manageStadiumsButton_clicked()
{
    ui->adminStackedWidget->setCurrentIndex(1);

    ui->groupBox->setTitle("Stadium Attributes");
    ui->capacityLineEdit->clear();
    ui->surfaceLineEdit->clear();
    ui->roofTypeLineEdit->clear();
    ui->typologyLineEdit->clear();
    ui->dateOpenedLineEdit->clear();
    ui->distToCenterLineEdit->clear();
    ui->newLocationLineEdit->clear();
    ui->roofTypeLineEdit->clear();

    //initialize all the data in the list widgets
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        ui->stadiumListWidget->addItem(QString::fromStdString(thisMap.atIndex(i).key));
    }
}

void MainWindow::on_manageStadiumsBackButton_clicked()
{
    writeToFiles();
    //returns to welcome screen
    ui->adminStackedWidget->setCurrentIndex(0);


    ui->stadiumListWidget->blockSignals(true);
    ui->stadiumListWidget->clear();
    ui->stadiumListWidget->blockSignals(false);

    ui->groupBox->setTitle("Stadium Attributes");
    ui->capacityLineEdit->clear();
    ui->surfaceLineEdit->clear();
    ui->roofTypeLineEdit->clear();
    ui->typologyLineEdit->clear();
    ui->dateOpenedLineEdit->clear();
    ui->distToCenterLineEdit->clear();
    ui->newLocationLineEdit->clear();
    ui->roofTypeLineEdit->clear();
}

void MainWindow::on_stadiumListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->stadiumListWidget->openPersistentEditor(item);
}

void MainWindow::on_stadiumListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(ui->stadiumListWidget->isPersistentEditorOpen(previous))
    {
        ui->stadiumListWidget->closePersistentEditor(previous);
        thisMap.atIndex(previous->listWidget()->currentRow()).value.setStadiumName(previous->text().toStdString());
    }

    if(ui->souvenirListWidget->isPersistentEditorOpen(previous))
    {
        thisMap.atIndex(previous->listWidget()->currentRow()).value.getSouvenir(previous->listWidget()->currentRow()).itemName = previous->text().toStdString();
        ui->souvenirListWidget->closePersistentEditor(previous);
    }

    if(ui->souvenirPriceListWidget->isPersistentEditorOpen(previous))
    {
         if(isFloatNumber(previous->text()))
         {
            thisMap.atIndex(previous->listWidget()->currentRow()).value.getSouvenir(previous->listWidget()->row(previous)).setItemPrice(previous->text().toFloat());
         }
         else {
             QMessageBox::warning(nullptr, "Error", "Invalid Price Input! Please Enter a float");
             previous->setText(QString::number(thisMap.atIndex(previous->listWidget()->currentRow()).value.getSouvenir(previous->listWidget()->row(previous)).itemPrice));
         }

        ui->souvenirPriceListWidget->closePersistentEditor(previous);
    }


    ui->souvenirListWidget->blockSignals(true);
    ui->souvenirPriceListWidget->blockSignals(true);
    ui->souvenirListWidget->clear();
    ui->souvenirPriceListWidget->clear();
    ui->souvenirListWidget->blockSignals(false);
    ui->souvenirPriceListWidget->blockSignals(false);


    ui->souvenirPriceListWidget->setCurrentRow(current->listWidget()->currentRow());




    //if the user clicks on an item, the values in the souvenir list widget, along with all other attributes need to be updated
    for(int i = 0; i < thisMap.atIndex(current->listWidget()->currentRow()).value.getSouvenirListSize(); i++)
    {
        ui->souvenirListWidget->addItem(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getSouvenir(i).itemName));
        ui->souvenirPriceListWidget->addItem(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getSouvenir(i).itemPrice));
    }

    ui->groupBox->setTitle(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getStadiumName()));
    ui->capacityLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getSeatingCapacity()));
    ui->surfaceLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getPlayingSurface()));
    ui->roofTypeLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getRoofType()));
    ui->typologyLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getBallParkTypology()));
    ui->dateOpenedLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getDateOpened()));
    ui->distToCenterLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getDistanceToCenterField()));
    //ui->newLocationLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.()));
}

void MainWindow::on_souvenirListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->souvenirListWidget->openPersistentEditor(item);
}

void MainWindow::on_souvenirListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(ui->souvenirListWidget->isPersistentEditorOpen(previous))
    {
        thisMap.atIndex(previous->listWidget()->currentRow()).value.getSouvenir(previous->listWidget()->currentRow()).itemName = previous->text().toStdString();
        ui->souvenirListWidget->closePersistentEditor(previous);
    }
    ui->souvenirPriceListWidget->setCurrentRow(current->listWidget()->currentRow());
}

void MainWindow::on_souvenirPriceListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->souvenirPriceListWidget->openPersistentEditor(item);
}

void MainWindow::on_souvenirPriceListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(ui->souvenirPriceListWidget->isPersistentEditorOpen(previous))
    {
         if(isFloatNumber(previous->text()))
         {
            thisMap.atIndex(previous->listWidget()->currentRow()).value.getSouvenir(previous->listWidget()->row(previous)).setItemPrice(previous->text().toFloat());
         }
         else {
             QMessageBox::warning(nullptr, "Error", "Invalid Price Input! Please Enter a float");
             previous->setText(QString::number(thisMap.atIndex(previous->listWidget()->currentRow()).value.getSouvenir(previous->listWidget()->row(previous)).itemPrice));
         }

        ui->souvenirPriceListWidget->closePersistentEditor(previous);
    }
    ui->souvenirPriceListWidget->setCurrentRow(current->listWidget()->currentRow());
}

bool MainWindow::isFloatNumber(const QString& Qstring)
{
    string stdString = Qstring.toStdString();

    string::const_iterator it = stdString.begin();
    bool decimalPoint = false;
    unsigned int minSize = 0;
    if(stdString.size()>0 && (stdString[0] == '-' || stdString[0] == '+')){
      it++;
      minSize++;
    }
    while(it != stdString.end()){
      if(*it == '.'){
        if(!decimalPoint) decimalPoint = true;
        else break;
      }else if(!std::isdigit(*it) && ((*it!='f') || it+1 != stdString.end() || !decimalPoint)){
        break;
      }
      ++it;
    }
    return stdString.size() > minSize && it == stdString.end();
}

bool MainWindow::isInteger(const QString &mystring)
{
   string s = mystring.toStdString();
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

void MainWindow::on_capacityLineEdit_textEdited(const QString &arg1)
{
    if(isInteger(arg1))
    {
        thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setSeatingCapacity(arg1.toInt());
    }
    else {
        QMessageBox::warning(nullptr, "Error", "Invalid Input! Please Enter an int");
    }
}

void MainWindow::on_surfaceLineEdit_textEdited(const QString &arg1)
{
    thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setPlayingSurface(arg1.toStdString());
}


void MainWindow::on_roofTypeLineEdit_textEdited(const QString &arg1)
{
     thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setRoofType(arg1.toStdString());
}

void MainWindow::on_typologyLineEdit_textEdited(const QString &arg1)
{
    thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setTypology(arg1.toStdString());
}


void MainWindow::on_dateOpenedLineEdit_textEdited(const QString &arg1)
{
    if(isInteger(arg1))
    {
        thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setDateOpened(arg1.toInt());
    }
    else {
        QMessageBox::warning(nullptr, "Error", "Invalid Input! Please Enter an int");
    }

}

void MainWindow::on_distToCenterLineEdit_textEdited(const QString &arg1)
{
    if(isInteger(arg1))
    {
        thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setDistanceToCenterField(arg1.toInt());
    }
    else {
        QMessageBox::warning(nullptr, "Error", "Invalid Input! Please Enter an int");
    }
}

void MainWindow::on_newLocationLineEdit_textEdited(const QString &arg1)
{
    thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setDateOpened(arg1.toInt());
}

//************************************ VIEWING STADIUMS (user) ************************************************
//if the user presses the manage stadiums button, this method changes the page
void MainWindow::on_viewStadiumsButton_clicked()
{
    ui->userStackedWidget->setCurrentIndex(3);

    ui->capacityLineEdit_2->setReadOnly(true);
    ui->surfaceLineEdit_2->setReadOnly(true);
    ui->roofTypeLineEdit_2->setReadOnly(true);
    ui->typologyLineEdit_2->setReadOnly(true);
    ui->dateOpenedLineEdit_2->setReadOnly(true);
    ui->distToCenterLineEdit_2->setReadOnly(true);

    //initialize all the data in the list widgets
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
//        ui->stadiumListWidget_2->addItem(QString::fromStdString(thisMap.atIndex(i).value.getStadiumName()));
        ui->stadiumListWidget_2->addItem(QString::fromStdString(thisMap.atIndex(i).key));

    }
}

void MainWindow::on_viewStadiumsBackButton_clicked()
{
    //returns to welcome screen
    ui->userStackedWidget->setCurrentIndex(0);

    ui->stadiumListWidget_2->blockSignals(true);
    ui->stadiumListWidget_2->clear();
    ui->stadiumListWidget_2->blockSignals(false);

    ui->groupBox_2->setTitle("Stadium Attributes");
    ui->capacityLineEdit_2->clear();
    ui->surfaceLineEdit_2->clear();
    ui->roofTypeLineEdit_2->clear();
    ui->typologyLineEdit_2->clear();
    ui->dateOpenedLineEdit_2->clear();
    ui->distToCenterLineEdit_2->clear();
}

void MainWindow::on_stadiumListWidget_2_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->souvenirListWidget_2->clear();
    ui->souvenirPriceListWidget_2->clear();
    //if the user clicks on an item, the values in the souvenir list widget, along with all other attributes need to be updated
    for(int i = 0; i < thisMap.atIndex(current->listWidget()->currentRow()).value.getSouvenirListSize(); i++)
    {
        ui->souvenirListWidget_2->addItem(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getSouvenir(i).itemName));
        ui->souvenirPriceListWidget_2->addItem(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getSouvenir(i).itemPrice));
    }

    ui->groupBox_2->setTitle(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getStadiumName()));
    ui->capacityLineEdit_2->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getSeatingCapacity()));
    ui->surfaceLineEdit_2->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getPlayingSurface()));
    ui->roofTypeLineEdit_2->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getRoofType()));
    ui->typologyLineEdit_2->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getBallParkTypology()));
    ui->dateOpenedLineEdit_2->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getDateOpened()));
    ui->distToCenterLineEdit_2->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getDistanceToCenterField()));
    //ui->newLocationLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.()));
}

//************************************ INFORMATION ************************************************

//admin has requested to view stadiums
void MainWindow::on_informationButton_admin_clicked()
{
    //function "reaches" into the user side of the program to re-use the user methods for viewing stadums
    //changes page
    ui->primaryPageStackedWidget->setCurrentIndex(2);

    on_informationButton_user_clicked();
}


//regular user has requested to view stadiums
void MainWindow::on_informationButton_user_clicked()
{

    int totalSeatingCapacity = 0;

    updateVectors();

    QStringList tableHeader1;
    tableHeader1 << "Team Name" << "Stadium Name";
    QStringList tableHeader2;
    tableHeader2 << "Team Name" << "Stadium Name";
    QStringList tableHeader3;
    tableHeader3 << "Team Name" << "Stadium Name";
    QStringList tableHeader4;
    tableHeader4 << "Team Name" << "Stadium Name";
    QStringList tableHeader5;
    tableHeader5 << "Team Name" << "Stadium Name" << "Park Typology";
    QStringList tableHeader6;
    tableHeader6 << "Team Name" << "Stadium Name" << "Roof Type";
    QStringList tableHeader7;
    tableHeader7 << "Team Name" << "Stadium Name" << "Date Opened";
    QStringList tableHeader8;
    tableHeader8 << "Team Name" << "Stadium Name" << "Stadium Seating Capacity";
    QStringList tableHeader9;
    tableHeader9 << "Team Name" << "Stadium Name" << "Distance To Center";
    QStringList tableHeader10;
    tableHeader10 << "Team Name" << "Stadium Name" << "Distance To Center";

    int min;
    //sorting for major teams via team name
    for(int i = 0; i < v1.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v1.size(); j++){
            if(v1[j].key < v1[min].key){
                min = j;
            }
        }
        swap(v1[min], v1[i]);
    }
    //sorting for major teams via stadium name
    for(int i = 0; i < v2.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v2.size(); j++){
            if(v2[j].value.getStadiumName() < v2[min].value.getStadiumName()){
                min = j;
            }
        }
        swap(v2[min], v2[i]);
    }
    //sorting for american teams via team name
    for(int i = 0; i < v3.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v3.size(); j++){
            if(v3[j].key < v3[min].key){
                min = j;
            }
        }
        swap(v3[min], v3[i]);
    }
    //sorting for national teams via stadium name
    for(int i = 0; i < v4.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v4.size(); j++){
            if(v4[j].value.getStadiumName() < v4[min].value.getStadiumName()){
                min = j;
            }
        }
        swap(v4[min], v4[i]);
    }
    //sorting by park typology
    for(int i = 0; i < v5.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v5.size(); j++){
            if(v5[j].value.getBallParkTypology() < v5[min].value.getBallParkTypology()){
                min = j;
            }
        }
        swap(v5[min], v5[i]);
    }
    //sorting open roof types by team name
    for(int i = 0; i < v6.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v6.size(); j++){
            if(v6[j].key < v6[min].key){
                min = j;
            }
        }
        swap(v6[min], v6[i]);
    }
    //sorting by chronological order
    for(int i = 0; i < v7.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v7.size(); j++){
            if(v7[j].value.getDateOpened() < v7[min].value.getDateOpened()){
                min = j;
            }
        }
        swap(v7[min], v7[i]);
    }
    //sorting by seating capacity
    for(int i = 0; i < v8.size()-1; i++){

        min = i;
        for(int j = i + 1; j < v8.size(); j++){
            if(v8[j].value.getSeatingCapacity() < v8[min].value.getSeatingCapacity()){
                min = j;
            }
        }
        swap(v8[min], v8[i]);
    }
    for(int i = 0; i < v8.size(); i++){
        totalSeatingCapacity = totalSeatingCapacity + v8[i].value.getSeatingCapacity();
    }

    //empties all the table widgets so there is no double stacking of information
    ui->teamTableWidget_1->clear();
    ui->teamTableWidget_2->clear();
    ui->teamTableWidget_3->clear();
    ui->teamTableWidget_4->clear();
    ui->teamTableWidget_5->clear();
    ui->teamTableWidget_6->clear();
    ui->teamTableWidget_7->clear();
    ui->teamTableWidget_8->clear();
    ui->teamTableWidget_9->clear();
    ui->teamTableWidget_10->clear();

    //initializes all the tables with rows and header names
    ui->teamTableWidget_1->setRowCount(v1.size());
    ui->teamTableWidget_1->setColumnCount(2);
    ui->teamTableWidget_1->setHorizontalHeaderLabels(tableHeader1);
    ui->label_19->setText(QString::number(v1.size()));
    ui->teamTableWidget_2->setRowCount(v2.size());
    ui->teamTableWidget_2->setColumnCount(2);
    ui->teamTableWidget_2->setHorizontalHeaderLabels(tableHeader2);
    ui->label_20->setText(QString::number(v2.size()));
    ui->teamTableWidget_3->setRowCount(v3.size());
    ui->teamTableWidget_3->setColumnCount(2);
    ui->teamTableWidget_3->setHorizontalHeaderLabels(tableHeader3);
    ui->label_21->setText(QString::number(v3.size()));
    ui->teamTableWidget_4->setRowCount(v4.size());
    ui->teamTableWidget_4->setColumnCount(2);
    ui->teamTableWidget_4->setHorizontalHeaderLabels(tableHeader4);
    ui->label_22->setText(QString::number(v4.size()));
    ui->teamTableWidget_5->setRowCount(v5.size());
    ui->teamTableWidget_5->setColumnCount(3);
    ui->teamTableWidget_5->setHorizontalHeaderLabels(tableHeader5);
    ui->label_23->setText(QString::number(v5.size()));
    ui->teamTableWidget_6->setRowCount(v6.size());
    ui->teamTableWidget_6->setColumnCount(3);
    ui->teamTableWidget_6->setHorizontalHeaderLabels(tableHeader6);
    ui->label_24->setText(QString::number(v6.size()));
    ui->teamTableWidget_7->setRowCount(v7.size());
    ui->teamTableWidget_7->setColumnCount(3);
    ui->teamTableWidget_7->setHorizontalHeaderLabels(tableHeader7);
    ui->label_25->setText(QString::number(v7.size()));
    ui->teamTableWidget_8->setRowCount(v8.size());
    ui->teamTableWidget_8->setColumnCount(3);
    ui->teamTableWidget_8->setHorizontalHeaderLabels(tableHeader8);
    ui->label_26->setText(QString::number(v8.size()));
    ui->label_32->setText(QString::number(totalSeatingCapacity));
    ui->teamTableWidget_9->setRowCount(v9.size());
    ui->teamTableWidget_9->setColumnCount(3);
    ui->teamTableWidget_9->setHorizontalHeaderLabels(tableHeader9);
    ui->label_27->setText(QString::number(v9.size()));
    ui->teamTableWidget_10->setRowCount(v10.size());
    ui->teamTableWidget_10->setColumnCount(3);
    ui->teamTableWidget_10->setHorizontalHeaderLabels(tableHeader10);
    ui->label_28->setText(QString::number(v10.size()));

    //for major teams
    for(int i = 0; i < v1.size(); i++){
        ui->teamTableWidget_1->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v1[i].key)));
        ui->teamTableWidget_1->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v1[i].value.getStadiumName())));
        ui->teamTableWidget_2->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v2[i].key)));
        ui->teamTableWidget_2->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v2[i].value.getStadiumName())));
    }
    //for american teams
    for(int i = 0; i < v3.size(); i++){
        ui->teamTableWidget_3->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v3[i].key)));
        ui->teamTableWidget_3->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v3[i].value.getStadiumName())));
    }
    //for national teams
    for(int i = 0; i < v4.size(); i++){
        ui->teamTableWidget_4->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v4[i].key)));
        ui->teamTableWidget_4->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v4[i].value.getStadiumName())));
    }
    //for open roof
    for(int i = 0; i < v6.size(); i++){
        ui->teamTableWidget_6->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v6[i].key)));
        ui->teamTableWidget_6->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v6[i].value.getStadiumName())));
        ui->teamTableWidget_6->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(v6[i].value.getRoofType())));
    }
    //for everything else
    for(int i = 0; i < v5.size(); i++){
        ui->teamTableWidget_5->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v5[i].key)));
        ui->teamTableWidget_5->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v5[i].value.getStadiumName())));
        ui->teamTableWidget_5->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(v5[i].value.getBallParkTypology())));
        \
        ui->teamTableWidget_7->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v7[i].key)));
        ui->teamTableWidget_7->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v7[i].value.getStadiumName())));
        ui->teamTableWidget_7->setItem(i, 2, new QTableWidgetItem(QString::number(v7[i].value.getDateOpened())));

        ui->teamTableWidget_8->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v8[i].key)));
        ui->teamTableWidget_8->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v8[i].value.getStadiumName())));
        ui->teamTableWidget_8->setItem(i, 2, new QTableWidgetItem(QString::number(v8[i].value.getSeatingCapacity())));
    }
    //for greatest distance to center field
    for(int i = 0; i < v9.size(); i++){
        ui->teamTableWidget_9->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v9[i].key)));
        ui->teamTableWidget_9->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v9[i].value.getStadiumName())));
        ui->teamTableWidget_9->setItem(i, 2, new QTableWidgetItem(QString::number(v9[i].value.getDistanceToCenterField())));
    }
    //for least distance to center field
    for(int i = 0; i < v10.size(); i++){
        ui->teamTableWidget_10->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(v10[i].key)));
        ui->teamTableWidget_10->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(v10[i].value.getStadiumName())));
        ui->teamTableWidget_10->setItem(i, 2, new QTableWidgetItem(QString::number(v10[i].value.getDistanceToCenterField())));
    }


    ui->userStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_informationBackButton_clicked()
{
    //since this method is shared between admins and regular users, we perform a check
    if(isAdmin)
    {
        ui->primaryPageStackedWidget->setCurrentIndex(1);
        ui->adminStackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->userStackedWidget->setCurrentIndex(0);
    }
}

//************************************ TAKING TRIPS ************************************************
void MainWindow::on_takeTripButton_admin_clicked()
{
    //function "reaches" into the user side of the program to re-use the user methods for taking a trip
    //changes page
    ui->primaryPageStackedWidget->setCurrentIndex(2);
    ui->userStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_takeTripButton_user_clicked()
{
    ui->userStackedWidget->setCurrentIndex(1);
    ui->takeTripStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_takeTripPageBackButton_clicked()
{
    if(isAdmin)
    {
        //return to admin
        ui->primaryPageStackedWidget->setCurrentIndex(1);
        ui->adminStackedWidget->setCurrentIndex(0);
    }

    ui->userStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_visitMultipleButton_clicked()
{
    ui->availibleTeamsStackedWidget->clear();
    ui->selectedTeamsStackedWidget->clear();

    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        //fill up the availible teams stacked widget
        ui->availibleTeamsStackedWidget->addItem(QString::fromStdString(thisMap.atIndex(i).key));
        ui->availibleTeamsStackedWidget->item(i)->setCheckState(Qt::Unchecked);
        ui->availibleTeamsStackedWidget->item(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }

    ui->takeTripStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_visitSingleButton_clicked()
{
    ui->takeTripStackedWidget->setCurrentIndex(2);

    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        //fill up the availible teams stacked widget
        ui->availibleTeamsStackedWidget->addItem(QString::fromStdString(thisMap.atIndex(i).key));
        ui->availibleTeamsStackedWidget->item(i)->setCheckState(Qt::Unchecked);
        ui->availibleTeamsStackedWidget->item(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
}

void MainWindow::on_multipleSelectionPageBackButton_clicked()
{
    ui->takeTripStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_availibleTeamsStackedWidget_itemChanged(QListWidgetItem *item)
{
    //when user checks a team, it moves it into the next list widget
    if(item->checkState() == 2)
    {
        //item is checked
        ui->selectedTeamsStackedWidget->addItem(item->text());
    }
    else if(item->checkState() == 0)
    {
        //perform sequential search for the item

        bool found = false;
        unsigned int k = 0;
        while(!found && k < ui->selectedTeamsStackedWidget->count())
        {
            if(item->text() == ui->selectedTeamsStackedWidget->item(k)->text())
            {
                found = true;
            }
            else {
                ++k;
            }
        }

        ui->selectedTeamsStackedWidget->takeItem(k);
    }
}

void MainWindow::on_singleSelectionPageBackButton_clicked()
{
     ui->takeTripStackedWidget->setCurrentIndex(0);
}

vector<vector<int>> MainWindow::createAdjacencyMatrix()
{
    vector<vector<int>> matrix;

    //reads the selected teams matrix and creates the matrix from distances
    for(int i = 0; i < ui->selectedTeamsStackedWidget->count(); i++)
    {
//        matrix.push_back();
//        matrix.push_back(thisMap);
    }

}



void MainWindow::on_optimizeButton_clicked()
{
    //run optimization algorithm, return
    //steps to the shortest path algo

    //1: create the distance matrix explicity, iterate across the whole map, add distance vectors as columns to the matrix
    //   after adding all columns, track which stadiums are not being visited, and delete their indexes from the matrix
    //   The resulting matrix will be an NxN symmetric matrix, where n is the number of stadiums to be visted
    //   This same process will be used for the bfs, dfs

    //2: Use distance matrix to "visit" (actually visiting comes later) each stadium, recursively "visiting" the closest one,
    //   creating an ordered vector of stadiums

    //3: write the vectors contents to the selected teams stacked widget

}
