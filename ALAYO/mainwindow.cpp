#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph.h"
#include "QMessageBox"
#include "QDesktopWidget"
#include "QTextStream"

//******************************
// ORIGINAL INPUT FILES
//******************************
// original MLB Information const
const string MLB_INFORMATION_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYO/ALAYO/inputMLBInformation.csv";
// original Distances const
const string DISTANCES_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYO/ALAYO/inputDistance.csv";
//original MLB Information expansion const
const string MLB_INFORMATION_EXPANSION_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYO/ALAYO/inputMLBInformationExpansion.csv";
//original Distances expansion const
const string DISTANCES_EXPANSION_INPUT_FILE = "C:/Users/Oscar/Desktop/ALAYO/ALAYO/inputDistanceExpansion.csv";

//******************************
// WRITE TO FILES
//******************************
// written to MLB Information const
const string MODIFIED_MLB_INFORMATION_OUTPUT_FILE = "C:/Users/Oscar/Desktop/ALAYO/ALAYO/inputModifiedMLBInformation.csv";
// written to Distances const
const string MODIFIED_DISTANCES_OUTPUT_FILE = "C:/Users/Oscar/Desktop/ALAYO/ALAYO/inputModifiedDistances.csv";
// written to souvenirs const
const string MODIFIED_SOUVENIRS_OUTPUT_FILE = "C:/Users/Oscar/Desktop/ALAYO/ALAYO/inputSouvenirs.csv";

//******************************
// IMAGES
//******************************
// unadded stadium image
const char ORIGINAL_STADIUMS_IMAGE[] = {"C:/Users/Oscar/Desktop/ALAYO/ALAYO/BaseballStadiumGraph.jpg"};
// modified stadiums image
const char MODIFIED_STADIUMS_IMAGE[] = {"C:/Users/Oscar/Desktop/ALAYO/ALAYO/BaseballStadiumGraphLV.jpg"};
// Alayo logo image
const char ALAYO_LOGO[] = {"C:/Users/Oscar/Desktop/ALAYO/ALAYO/ALAYOLogo.png"};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readFromFiles(false);

    //allows user to move around items in the selected locations when taking a trip
    ui->selectedStadiumsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->selectedStadiumsListWidget->setDragEnabled(true);
    ui->selectedStadiumsListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    ui->selectedStadiumsListWidget->viewport()->setAcceptDrops(true);
    ui->selectedStadiumsListWidget->setDropIndicatorShown(true);

    ui->primaryPageStackedWidget->setCurrentIndex(0);

    currentLocationIndex = -1;

    //totalDistanceTraveled = 0;
    subTotal = 0;
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
        isAddedStadium = false;
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

            MLB thisMLB(iStadiumName, iSeatingcapacity,
                        iLocation, iPlayingSurface, iLeague,
                        iDateopened, idistanceToCenterField,
                        iBallparkTypology, iRoofType);

            thisMap.insert(iName, thisMLB);
            thisEntry.key = iName;
            thisEntry.value = thisMLB;
            
            //this makes it so vector 3 contains only american
            if(thisMLB.getLeague() == "American")
            {
                v3.push_back(thisEntry);
                v5.push_back(thisEntry);
                v7 = v5;
                v8 = v5;
            }
            //this makes it so vector 4 contain only national teams
            else if(thisMLB.getLeague() == "National")
            {
                v4.push_back(thisEntry);
                v5.push_back(thisEntry);
                v7 = v5;
                v8 = v5;
            }
            v1.push_back(thisEntry);
            v2.push_back(thisEntry);

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
        }
    }
    else
    {
        QTextStream(stdout) << "***CHANGE FILE DIRECTORY IN \"mainwindow.cpp\"***\n";
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
    else
    {
        QTextStream(stdout) << "***CHANGE FILE DIRECTORY IN \"mainwindow.cpp\"***\n";;
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
        QMessageBox::information(nullptr, "Success!", "All MLB teams have been reinitialized!");
        writeToFiles();
    }
    else
    {
        if(thisMap.mapSize() == 31)
        {
            isAddedStadium = true;
        }
        else
        {
            isAddedStadium = false;
        }
    }
}

void MainWindow::readExpansionFiles()
{
    ifstream csvFile1;
    csvFile1.open(MLB_INFORMATION_EXPANSION_INPUT_FILE);

    int newTeamCount = 0;
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

    while(!csvFile1.eof())
    {
        getline(csvFile1, iName, ',');

        for(int i = 0; i < thisMap.mapSize(); i++)
        {
            if(iName == thisMap.atIndex(i).key)
            {
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

        MLB thisMLB(iStadiumName, iSeatingcapacity,
                    iLocation, iPlayingSurface, iLeague,
                    iDateopened, idistanceToCenterField,
                    iBallparkTypology, iRoofType);

        thisMap.insert(iName, thisMLB);
        thisEntry.key = iName;
        thisEntry.value = thisMLB;

        //this makes it so vector 3 contains only american
        if(thisMLB.getLeague() == "American")
        {
            v3.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }
        //this makes it so vector 4 contain only national teams
        else if(thisMLB.getLeague() == "National")
        {
            v4.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }

        v1.push_back(thisEntry);
        v2.push_back(thisEntry);

        if(thisMLB.getRoofType() == "Open")
        {
            v6.push_back(thisEntry);
        }

        if(thisMLB.getDistanceToCenterField() <= min)
        {
            min = thisMLB.getDistanceToCenterField();
        }
        if(thisMLB.getDistanceToCenterField() >= max)
        {
            max = thisMLB.getDistanceToCenterField();
        }

        thisMap.atIndex(thisMap.mapSize()-1).value.addSouvenir(souvenir("Baseball cap", 22.99));
        thisMap.atIndex(thisMap.mapSize()-1).value.addSouvenir(souvenir("Baseball bat", 89.93));
        thisMap.atIndex(thisMap.mapSize()-1).value.addSouvenir(souvenir("Team pennant", 17.99));
        thisMap.atIndex(thisMap.mapSize()-1).value.addSouvenir(souvenir("Autographed baseball", 25.99));
        thisMap.atIndex(thisMap.mapSize()-1).value.addSouvenir(souvenir("Team jersey", 199.99));

        newTeamCount++;
    }
    // reads in distances from input file
    if(!exists)
    {
        // runs the amount of times new times were added to the map
        for(int i = 0; i < newTeamCount; i++)
        {
            // runs through all the old teams and adds 0 to the end of distance vector
            for(int k = 0; k < thisMap.mapSize()-newTeamCount; k++)
            {
                thisMap.atIndex(k).value.pushBackDistance();
            }

            thisMap.atIndex(thisMap.mapSize()-i-1).value.setInitialDistances(thisMap.mapSize());
        }

        ifstream csvFile2;
        csvFile2.open(DISTANCES_EXPANSION_INPUT_FILE);

        string iStadium1;
        string iStadium2;
        string iDistance;
        int distance;
        string previous;
        bool found;
        int index1;
        int index2;

        getline(csvFile2, iStadium1, ',');
        while(!csvFile2.eof())
        {
            previous = iStadium1;
            found = false;
            index1 = 0;

            // searches for the index of the first stadium
            while(!found)
            {
                if(thisMap.atIndex(index1).value.getStadiumName() == iStadium1)
                {
                    found = true;
                }
                else
                {
                    index1++;
                }
            }

            while(iStadium1 == previous)
            {
                getline(csvFile2, iStadium2, ',');
                getline(csvFile2, iDistance, '\n');
                distance = stoi(iDistance);

                found = false;
                index2 = 0;

                // searches for the index of the second stadium
                while(!found)
                {
                    if(thisMap.atIndex(index2).value.getStadiumName() == iStadium2)
                    {
                        found = true;
                    }
                    else
                    {
                        index2++;
                    }
                }

                thisMap.atIndex(index1).value.setDistance(index2, distance);
                thisMap.atIndex(index2).value.setDistance(index1, distance);

                getline(csvFile2, iStadium1, ',');
            }
        }

        csvFile2.close();

        QMessageBox::information(nullptr, "Success!", "Additional team(s) have been added!");
        isAddedStadium = true;
        writeToFiles();
    }
    else
    {
        QMessageBox::critical(nullptr, "Error!", "You can't add a team that already exists!");
    }
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

        QPixmap pix(ALAYO_LOGO);
        ui->AlayoLogoLabel->setPixmap(pix);
        ui->AlayoLogoLabel->setScaledContents(true);

        isAdmin = true;
    }
    else if(ui->usernameLineEdit->text() == "user" && ui->passwordLineEdit->text() == "user")
    {
        ui->primaryPageStackedWidget->setCurrentIndex(2);
        ui->userStackedWidget->setCurrentIndex(0);

        QPixmap pix(ALAYO_LOGO);
        ui->AlayoLogoLabel_2->setPixmap(pix);
        ui->AlayoLogoLabel_2->setScaledContents(true);

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
}

//************************************ MANAGING STADIUMS (admin) ************************************************

void MainWindow::resetManageStadiumsInformation()
{
    ui->stadiumListWidget->blockSignals(true);
    ui->stadiumListWidget->clear();
    ui->stadiumListWidget->blockSignals(false);

    ui->groupBox->setTitle("Stadium Attributes");

    ui->capacityLineEdit->clear();
    ui->surfaceLineEdit->clear();
    ui->roofTypeLineEdit->clear();
    ui->typologyLineEdit->clear();
    ui->dateOpenedLineEdit->clear();
    ui->leagueLineEdit->clear();
    ui->distToCenterLineEdit->clear();
    ui->locationLineEdit->clear();
    ui->stadiumLineEdit->clear();
    ui->roofTypeLineEdit->clear();
    ui->souvenirAddItemNameTextEdit->clear();
    ui->souvenirAddItemPriceTextEdit->clear();
    ui->changeSouvenirNameLineEdit->clear();
    ui->changeSouvenirPriceLineEdit->clear();
    ui->souvenirChangingName->setText("");

    ui->changeSouvenirPushButton->setEnabled(false);
    ui->souvenirAddAllTeamsCheckbox->setCheckable(false);
    ui->deleteSouvenirPushButton->setEnabled(false);
    ui->addSouvenirPushButton->setEnabled(false);
    ui->changeSouvenirNameLineEdit->setReadOnly(true);
    ui->changeSouvenirPriceLineEdit->setReadOnly(true);
    ui->capacityLineEdit->setReadOnly(true);
    ui->surfaceLineEdit->setReadOnly(true);
    ui->roofTypeLineEdit->setReadOnly(true);
    ui->typologyLineEdit->setReadOnly(true);
    ui->dateOpenedLineEdit->setReadOnly(true);
    ui->leagueLineEdit->setReadOnly(true);
    ui->distToCenterLineEdit->setReadOnly(true);
    ui->locationLineEdit->setReadOnly(true);
    ui->stadiumLineEdit->setReadOnly(true);
    ui->roofTypeLineEdit->setReadOnly(true);
    ui->souvenirAddItemNameTextEdit->setReadOnly(true);
    ui->souvenirAddItemPriceTextEdit->setReadOnly(true);
    ui->souvenirAddAllTeamsCheckbox->setChecked(false);
    ui->souvenirChangeAllTeamsCheckbox->setCheckable(false);
    ui->souvenirDeleteAllTeamsCheckbox->setCheckable(false);
    ui->souvenirChangeAllTeamsCheckbox->setChecked(false);
    ui->souvenirDeleteAllTeamsCheckbox->setChecked(false);

    ui->souvenirListWidget->blockSignals(true);
    ui->souvenirListWidget->clear();
    ui->souvenirListWidget->blockSignals(false);
    ui->souvenirPriceListWidget->blockSignals(true);
    ui->souvenirPriceListWidget->clear();
    ui->souvenirPriceListWidget->blockSignals(false);


    //initialize all the data in the list widgets
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        ui->stadiumListWidget->addItem(QString::fromStdString(thisMap.atIndex(i).key));
    }
}

void MainWindow::on_AddTeamButton_clicked()
{
    readExpansionFiles();
    resetManageStadiumsInformation();
}

void MainWindow::on_ReinitializeButton_clicked()
{
    readFromFiles(true);
    resetManageStadiumsInformation();
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
            v3.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }
        //this makes it so vectors 4 containes national teams
        else if(thisMap.atIndex(i).value.getLeague() == "National"){
            v4.push_back(thisEntry);
            v5.push_back(thisEntry);
            v7 = v5;
            v8 = v5;
        }

        v1.push_back(thisEntry);
        v2.push_back(thisEntry);

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
    resetManageStadiumsInformation();

    ui->capacityLineEdit->setValidator(new QIntValidator(this));
    ui->dateOpenedLineEdit->setValidator(new QIntValidator(this));
    ui->distToCenterLineEdit->setValidator(new QIntValidator(this));
    ui->souvenirAddItemPriceTextEdit->setValidator(new QDoubleValidator(this));
    ui->changeSouvenirPriceLineEdit->setValidator(new QDoubleValidator(this));
}

void MainWindow::on_manageStadiumsBackButton_clicked()
{
    writeToFiles();
    //returns to welcome screen
    ui->adminStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_stadiumListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
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

    ui->capacityLineEdit->setReadOnly(false);
    ui->surfaceLineEdit->setReadOnly(false);
    ui->roofTypeLineEdit->setReadOnly(false);
    ui->typologyLineEdit->setReadOnly(false);
    ui->dateOpenedLineEdit->setReadOnly(false);
    ui->leagueLineEdit->setReadOnly(false);
    ui->distToCenterLineEdit->setReadOnly(false);
    ui->locationLineEdit->setReadOnly(false);
    ui->stadiumLineEdit->setReadOnly(false);
    ui->roofTypeLineEdit->setReadOnly(false);
    ui->souvenirAddItemNameTextEdit->setReadOnly(false);
    ui->souvenirAddItemPriceTextEdit->setReadOnly(false);
    ui->souvenirAddAllTeamsCheckbox->setCheckable(true);
    ui->addSouvenirPushButton->setEnabled(true);
    ui->locationLineEdit->clear();
    ui->stadiumLineEdit->clear();
    ui->changeSouvenirNameLineEdit->clear();
    ui->changeSouvenirPriceLineEdit->clear();
    ui->souvenirAddItemNameTextEdit->clear();
    ui->souvenirAddItemPriceTextEdit->clear();
    ui->changeSouvenirNameLineEdit->setReadOnly(true);
    ui->changeSouvenirPriceLineEdit->setReadOnly(true);
    ui->changeSouvenirPushButton->setEnabled(false);
    ui->deleteSouvenirPushButton->setEnabled(false);
    ui->souvenirAddAllTeamsCheckbox->setChecked(false);
    ui->souvenirChangeAllTeamsCheckbox->setCheckable(false);
    ui->souvenirDeleteAllTeamsCheckbox->setCheckable(false);
    ui->souvenirChangeAllTeamsCheckbox->setChecked(false);
    ui->souvenirDeleteAllTeamsCheckbox->setChecked(false);
    ui->groupBox->setTitle(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getStadiumName()));
    ui->capacityLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getSeatingCapacity()));
    ui->surfaceLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getPlayingSurface()));
    ui->roofTypeLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getRoofType()));
    ui->typologyLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getBallParkTypology()));
    ui->dateOpenedLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getDateOpened()));
    ui->leagueLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getLeague()));
    ui->distToCenterLineEdit->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getDistanceToCenterField()));
    ui->stadiumLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getStadiumName()));
    ui->locationLineEdit->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getLocation()));
}

void MainWindow::on_souvenirListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->changeSouvenirPushButton->setEnabled(true);
    ui->changeSouvenirNameLineEdit->setReadOnly(false);
    ui->changeSouvenirPriceLineEdit->setReadOnly(false);
    ui->deleteSouvenirPushButton->setEnabled(true);
    ui->souvenirChangeAllTeamsCheckbox->setCheckable(true);
    ui->souvenirDeleteAllTeamsCheckbox->setCheckable(true);
    ui->souvenirChangeAllTeamsCheckbox->setChecked(false);
    ui->souvenirDeleteAllTeamsCheckbox->setChecked(false);
    ui->souvenirChangingName->setText(QString::fromStdString(thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.getSouvenir(ui->souvenirListWidget->currentRow()).itemName));
}

void MainWindow::on_capacityLineEdit_textEdited(const QString &arg1)
{    
    if(arg1.toStdString() == "-")
    {
        QMessageBox::critical(nullptr, "Error!", "Capacity cannot be negative!");
        ui->capacityLineEdit->clear();
    }
    else
    {
        thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setSeatingCapacity(arg1.toInt());
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

void MainWindow::on_locationLineEdit_textEdited(const QString &arg1)
{
    thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setLocation(arg1.toStdString());
}

void MainWindow::on_dateOpenedLineEdit_textEdited(const QString &arg1)
{
    if(arg1.toStdString() == "-")
    {
        QMessageBox::critical(nullptr, "Error!", "Date Opened cannot be negative!");
        ui->dateOpenedLineEdit->clear();
    }
    else
    {
        thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setDateOpened(arg1.toInt());
    }
}

void MainWindow::on_leagueLineEdit_textEdited(const QString &arg1)
{
    thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setLeague(arg1.toStdString());
}

void MainWindow::on_distToCenterLineEdit_textEdited(const QString &arg1)
{
    if(arg1.toStdString() == "-")
    {
        QMessageBox::critical(nullptr, "Error!", "Distance to center field cannot be negative!");
        ui->distToCenterLineEdit->clear();
    }
    else
    {
        thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setDistanceToCenterField(arg1.toInt());
    }
}

void MainWindow::on_stadiumLineEdit_textEdited(const QString &arg1)
{
    thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.setStadiumName(arg1.toStdString());
    ui->groupBox->setTitle(QString::fromStdString(thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.getStadiumName()));
}

void MainWindow::on_souvenirAddItemPriceTextEdit_textEdited(const QString &arg1)
{
    if(arg1.toStdString() == "-")
    {
        QMessageBox::critical(nullptr, "Error!", "Price cannot be negative!");
        ui->souvenirAddItemPriceTextEdit->clear();
    }
}

void MainWindow::on_addSouvenirPushButton_clicked()
{
    bool isExist = false;
    bool isExisted = false;
    bool isValid = true;

    if(ui->souvenirAddItemNameTextEdit->text() == "" || ui->souvenirAddItemPriceTextEdit->text() == "")
    {
        QMessageBox::critical(nullptr, "Error!", "Please fill out both Name and Price for new souvenirs!");
        isValid = false;
    }
    else if(ui->souvenirAddItemPriceTextEdit->text().toDouble() == 0)
    {
        QMessageBox::critical(nullptr, "Error!", "Price cannot be free!");
        isValid = false;
    }

    if(isValid)
    {
        if(ui->souvenirAddAllTeamsCheckbox->isChecked())
        {
            for(int i = 0; i < thisMap.mapSize(); i++)
            {
                isExist = false;
                for(int k = 0; k < thisMap.atIndex(i).value.getSouvenirListSize(); k++)
                {
                    if(ui->souvenirAddItemNameTextEdit->text() == QString::fromStdString(thisMap.atIndex(i).value.getSouvenir(k).itemName))
                    {
                        isExisted = true;
                        isExist = true;
                        break;
                    }
                }

                if(!isExist)
                {
                    thisMap.atIndex(i).value.addSouvenir(souvenir(ui->souvenirAddItemNameTextEdit->text().toStdString(),
                                                                  ui->souvenirAddItemPriceTextEdit->text().toDouble()));
                }
            }
        }
        else
        {
            for(int k = 0; k < thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.getSouvenirListSize(); k++)
            {
                if(ui->souvenirAddItemNameTextEdit->text() == QString::fromStdString(thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.getSouvenir(k).itemName))
                {
                    QMessageBox::critical(nullptr, "Error!", "This team already exists!");
                    isExist = true;
                    break;
                }
            }

            if(!isExist)
            {
                thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.addSouvenir(souvenir(ui->souvenirAddItemNameTextEdit->text().toStdString(),
                                                                                                ui->souvenirAddItemPriceTextEdit->text().toDouble()));
            }

        }
        if(isExisted)
        {
            QMessageBox::information(nullptr, "Notice!", "One or more teams already had this souvenir. All others have been added!");
        }
        resetManageStadiumsInformation();
    }
}

void MainWindow::on_deleteSouvenirPushButton_clicked()
{
    if(ui->souvenirDeleteAllTeamsCheckbox->isChecked())
    {
        string deletedSouvenirName;
        deletedSouvenirName = thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.getSouvenir(ui->souvenirListWidget->currentRow()).itemName;
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Souvenir", "Are you sure you would like to delete " + QString::fromStdString(deletedSouvenirName) + " from all teams?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            for(int i = 0; i < thisMap.mapSize(); i++)
            {
                for(int k = 0; k < thisMap.atIndex(i).value.getSouvenirListSize(); k++)
                {
                    if(thisMap.atIndex(i).value.getSouvenir(k).itemName == deletedSouvenirName)
                    {
                        thisMap.atIndex(i).value.deleteSouvenir(k);
                        break;
                    }
                }
            }
            resetManageStadiumsInformation();
        }
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Souvenir", "Are you sure you would like to delete this souvenir?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.deleteSouvenir(ui->souvenirListWidget->currentRow());
            resetManageStadiumsInformation();
        }
    }
}

void MainWindow::on_changeSouvenirPriceLineEdit_textEdited(const QString &arg1)
{
    if(arg1.toStdString() == "-")
    {
        QMessageBox::critical(nullptr, "Error!", "New price cannot be negative!");
        ui->changeSouvenirPriceLineEdit->clear();
    }
}

void MainWindow::on_changeSouvenirPushButton_clicked()
{
    bool isValid = true;

    if(ui->changeSouvenirNameLineEdit->text() == "" || ui->changeSouvenirPriceLineEdit->text() == "")
    {
        QMessageBox::critical(nullptr, "Error!", "Please fill out both name and Price for editing souvenirs!");
        isValid = false;
    }
    else if(ui->changeSouvenirPriceLineEdit->text().toDouble() == 0)
    {
        QMessageBox::critical(nullptr, "Error!", "Price cannot be free!");
        isValid = false;
    }


    if(isValid)
    {
        if(ui->souvenirChangeAllTeamsCheckbox->isChecked())
        {
            string check;
            check = thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.getSouvenir(ui->souvenirListWidget->currentRow()).itemName;
            for(int i = 0; i < thisMap.mapSize(); i++)
            {
                for(int k = 0; k < thisMap.atIndex(i).value.getSouvenirListSize(); k++)
                {
                    if(thisMap.atIndex(i).value.getSouvenir(k).itemName == check)
                    {
                        thisMap.atIndex(i).value.changeSouvenir(ui->changeSouvenirNameLineEdit->text().toStdString(),
                                                                ui->changeSouvenirPriceLineEdit->text().toDouble(),
                                                                k);
                        break;
                    }
                }
            }
            resetManageStadiumsInformation();
        }
        else
        {
            thisMap.atIndex(ui->stadiumListWidget->currentRow()).value.changeSouvenir(ui->changeSouvenirNameLineEdit->text().toStdString(),
                                                                                      ui->changeSouvenirPriceLineEdit->text().toDouble(),
                                                                                      ui->souvenirListWidget->currentRow());
        }
        resetManageStadiumsInformation();
    }
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
    ui->leagueLineEdit_2->setReadOnly(true);
    ui->locationLineEdit_2->setReadOnly(true);
    ui->distToCenterLineEdit_2->setReadOnly(true);

    ui->stadiumListWidget_2->blockSignals(true);
    ui->stadiumListWidget_2->clear();
    ui->stadiumListWidget_2->blockSignals(false);

    ui->groupBox->setTitle("Stadium Attributes");
    ui->capacityLineEdit_2->clear();
    ui->surfaceLineEdit_2->clear();
    ui->roofTypeLineEdit_2->clear();
    ui->typologyLineEdit_2->clear();
    ui->dateOpenedLineEdit_2->clear();
    ui->leagueLineEdit_2->clear();
    ui->locationLineEdit_2->clear();
    ui->distToCenterLineEdit_2->clear();
    ui->roofTypeLineEdit_2->clear();

    ui->souvenirListWidget_2->clear();
    ui->souvenirPriceListWidget_2->clear();

    //initialize all the data in the list widgets
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        ui->stadiumListWidget_2->addItem(QString::fromStdString(thisMap.atIndex(i).key));
    }
}

void MainWindow::on_viewStadiumsBackButton_clicked()
{
    //returns to welcome screen
    ui->userStackedWidget->setCurrentIndex(0);
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
    ui->leagueLineEdit_2->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getLeague()));
    ui->locationLineEdit_2->setText(QString::fromStdString(thisMap.atIndex(current->listWidget()->currentRow()).value.getLocation()));
    ui->distToCenterLineEdit_2->setText(QString::number(thisMap.atIndex(current->listWidget()->currentRow()).value.getDistanceToCenterField()));
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
    if(!isAddedStadium)
    {
        QPixmap pix(ORIGINAL_STADIUMS_IMAGE);
        ui->MLBImageLabel->setPixmap(pix);
        ui->MLBImageLabel->setScaledContents(true);
    }
    else
    {
        QPixmap pix(MODIFIED_STADIUMS_IMAGE);
        ui->MLBImageLabel->setPixmap(pix);
        ui->MLBImageLabel->setScaledContents(true);
    }
    //function "reaches" into the user side of the program to re-use the user methods for taking a trip
    //changes page
    ui->primaryPageStackedWidget->setCurrentIndex(2);
    ui->userStackedWidget->setCurrentIndex(1);
    ui->takeTripStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_takeTripButton_user_clicked()
{
    if(!isAddedStadium)
    {
        QPixmap pix(ORIGINAL_STADIUMS_IMAGE);
        ui->MLBImageLabel->setPixmap(pix);
        ui->MLBImageLabel->setScaledContents(true);
    }
    else
    {
        QPixmap pix(MODIFIED_STADIUMS_IMAGE);
        ui->MLBImageLabel->setPixmap(pix);
        ui->MLBImageLabel->setScaledContents(true);
    }

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
    ui->selectAllTeamsCheckBox->setChecked(false);
    ui->availibleStadiumsListWidget->clear();
    ui->selectedStadiumsListWidget->clear();

    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        //fill up the availible teams stacked widget
        ui->availibleStadiumsListWidget->addItem(QString::fromStdString(thisMap.atIndex(i).value.getStadiumName() + ": " + thisMap.atIndex(i).key));
        ui->availibleStadiumsListWidget->item(i)->setCheckState(Qt::Unchecked);
        ui->availibleStadiumsListWidget->item(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }

    ui->takeTripStackedWidget->setCurrentIndex(1);

    ui->optimizeButton->setEnabled(false);
    ui->goButton->setEnabled(false);
}

void MainWindow::on_selectAllTeamsCheckBox_stateChanged(int arg1)
{
    if(arg1)
    {
        for(int i = 0; i < thisMap.mapSize(); i++)
        {
            ui->availibleStadiumsListWidget->item(i)->setCheckState(Qt::Checked);
        }
    }
    else
    {
        for(int i = 0; i < thisMap.mapSize(); i++)
        {
            ui->availibleStadiumsListWidget->item(i)->setCheckState(Qt::Unchecked);
        }
    }
}

void MainWindow::on_multipleSelectionPageBackButton_clicked()
{
    ui->takeTripStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_availibleStadiumsListWidget_itemChanged(QListWidgetItem *item)
{
    QStringList stadiumTeam;
    //when user checks a team, it moves it into the next list widget
    if(item->checkState() == 2)
    {
        //item is checked

        stadiumTeam = item->text().split(": ");
        ui->selectedStadiumsListWidget->addItem(stadiumTeam[0]);
    }
    else if(item->checkState() == 0)
    {
        //perform sequential search for the item

        bool found = false;
        int k = 0;
        while(!found && k < ui->selectedStadiumsListWidget->count())
        {
            stadiumTeam = item->text().split(": ");
            if(stadiumTeam[0] == ui->selectedStadiumsListWidget->item(k)->text())
            {
                found = true;
            }
            else {
                ++k;
            }
        }

        ui->selectedStadiumsListWidget->takeItem(k);
    }

    //new stuff
    ui->selectedStadiumsListWidget->setDragEnabled(true);

    if(ui->selectedStadiumsListWidget->count() > 1)
    {
        ui->optimizeButton->setEnabled(true);
        ui->goButton->setEnabled(false);
    }
    else {
        ui->optimizeButton->setEnabled(false);
    }
}

vector<vector<int>> MainWindow::createAdjacencyMatrix()
{
    vector<vector<int>> matrix;

    //reads the selected teams matrix and creates the matrix from distances
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
       // matrix.push_back(thisMap.atIndex(i).value.getDistanceVector());
    }

    return matrix;
}

void MainWindow::createGraph()
{
    graph.clear();
    ifstream file;
    string startingLocation;
    string endingLocation;
    string distanceBetweenString;
    int distanceBetween;

    file.open(MODIFIED_DISTANCES_OUTPUT_FILE);
    if(file.is_open())
    {
       getline(file, startingLocation, ',');

       while(!file.eof())
       {
           getline(file, endingLocation, ',');
           getline(file, distanceBetweenString, '\n');

           distanceBetween = stoi(distanceBetweenString);

           graph.insert(startingLocation, endingLocation, distanceBetween);

           getline(file, startingLocation, ',');
       }
    }


}

void MainWindow::on_optimizeButton_clicked()
{
    //CHANGES I MADE
    //i made it so that when you check the box, it just adds the stadium name to the selectedstadiumslistwidget
    //that is found in availibleStadiumsListWidget_itemChanged

    //i also modified visitmultiplebutton clicked, added one line

    if(ui->selectedStadiumsListWidget->count() > 1)
    {
        orderedStadiumList.clear();
        orderedWeightsList.clear();

        vector<string> stadiumsToVisit;

        //obtain starting location
        string startingStadium;

        startingStadium = ui->selectedStadiumsListWidget->item(0)->text().toStdString();

        for(int i = 0; i < ui->selectedStadiumsListWidget->count(); i++)
        {
            stadiumsToVisit.push_back(ui->selectedStadiumsListWidget->item(i)->text().toStdString());
        }


        createGraph();

        vector<string> vertexList;

        vector<int> weights(graph.getGraphSize());
        vector<int> nextLocation(graph.getGraphSize());

        vector<vector<string>> paths;
        vector<int> pathWeights;

        vector<vector<string>> properPaths;
        vector<int> properPathWeights;

        stadiumsToVisit.erase(stadiumsToVisit.begin());

        int smallestWeight = 10000000000;
        int smallestWeightIndex = 0;

        int count = 0;
        while(stadiumsToVisit.size() > 0)
        {
            vertexList.clear();

            //might not want to do this
            paths.clear();
            pathWeights.clear();

            weights.clear();
            nextLocation.clear();
            for(int m = 0; m < graph.getGraphSize(); m++)
            {
                weights.push_back(0);
                nextLocation.push_back(0);
            }

            properPaths.clear();
            properPathWeights.clear();

            //set starting stadium
            if(count > 0)
            {
                //begins on second loop
                startingStadium = orderedStadiumList.back();
            }

            graph.dijkstraAlgorithm(startingStadium, vertexList, weights, nextLocation);

            for(unsigned int index = 1; index < graph.getGraphSize(); index++)
            {

                //if the next location is not in the list widget, we do not care about the path
                vector<string> outputVec = graph.determineTripVector(startingStadium, vertexList[index], nextLocation);

                paths.push_back(outputVec);

                pathWeights.push_back(weights[graph.getVertex(vertexList[index])]);
            }


            //loop through paths and remove the paths that visit locations not in the list widget
            for(int i = 0; i < paths.size(); i++)
            {
                //search to see if the final location of path is in the teamstovisit vector
                //sequential search

                bool found = false;
                int k = 0;
                while(k < stadiumsToVisit.size() && !found)
                {
                    if(paths[i][paths[i].size() -1] == stadiumsToVisit[k])
                    {
                        //the final location exists in the teams to visit
                        //do nothing to the paths vector
                        found = true;

                        properPaths.push_back(paths[i]);
                        properPathWeights.push_back(pathWeights[i]);
                    }
                    else {
                        ++k;
                    }
                }
            }

            smallestWeight = 10000000000;
            smallestWeightIndex = 0;

            if(properPaths.size() > 1)
            {
                //locate shortest path in properpaths, find index and weight
                for(int i = 0; i < properPathWeights.size(); i++)
                {
                    if(properPathWeights[i] < smallestWeight && properPathWeights[i] != 0)
                    {
                        smallestWeight = properPathWeights[i];
                        smallestWeightIndex = i;
                    }
                }
            }
            else
            {
                smallestWeight = properPathWeights[0];
                smallestWeightIndex = 0;
            }



            //at this point, we have a list of paths, which all visit locations that are outlined by the user
            //we also have the shortest path and the index to it

            //next step is to save the location and weight into two final parallel vectors

            orderedStadiumList.push_back(properPaths[smallestWeightIndex][properPaths[smallestWeightIndex].size() - 1]);
            orderedWeightsList.push_back(smallestWeight);

            //remove the stadium from the stadiumstovisit vector

            for(int h = 0; h < stadiumsToVisit.size(); h++)
            {
                QTextStream(stdout) << QString::fromStdString(orderedStadiumList.back()) << endl;
                if(orderedStadiumList.back() == stadiumsToVisit[h])
                {
                    stadiumsToVisit.erase(stadiumsToVisit.begin() + h);
                }
            }
            count++;
        }

        //OPTIMIZATION ALGORITHM COMPLETE
        // below is simply re-formatting the list widget into the correct order



        QString firstStadium = ui->selectedStadiumsListWidget->item(0)->text();
        ui->selectedStadiumsListWidget->clear();
        ui->selectedStadiumsListWidget->addItem(firstStadium);

        for(int l = 0; l < orderedStadiumList.size(); l++)
        ui->selectedStadiumsListWidget->addItem(QString::fromStdString(orderedStadiumList[l]));
    }
    ui->optimizeButton->setEnabled(false);
    ui->goButton->setEnabled(true);
    ui->selectedStadiumsListWidget->setDragEnabled(false);
}





void MainWindow::on_performdfsButton_clicked()
{
    ui->dfsPushButton->setEnabled(false);
    ui->startingStadiumLineEdit->setReadOnly(true);
    ui->dfsTotalDistanceTraveledLineEdit->setReadOnly(true);
    ui->dfsTotalDistanceTraveledLineEdit->clear();
    ui->dfsStadiumsListWidget->blockSignals(true);
    ui->dfsStadiumsListWidget->clear();
    ui->dfsStadiumsListWidget->blockSignals(false);
    ui->dfsResultsListWidget->blockSignals(true);
    ui->dfsResultsListWidget->clear();
    ui->dfsResultsListWidget->blockSignals(false);
    ui->startingStadiumLineEdit->clear();

    ui->takeTripStackedWidget->setCurrentIndex(4);
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        //fill up the availible teams stacked widget
        ui->dfsStadiumsListWidget->addItem(QString::fromStdString(thisMap.atIndex(i).value.getStadiumName()));
    }
}

void MainWindow::on_dfsPageBackButton_clicked()
{
    ui->takeTripStackedWidget->setCurrentIndex(0);
}


void MainWindow::on_dfsStadiumsListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->dfsResultsListWidget->clear();
    ui->dfsTotalDistanceTraveledLineEdit->clear();

    //place the current item into the desired starting location line edit
    ui->startingStadiumLineEdit->clear();
    ui->startingStadiumLineEdit->setText(current->text());
    ui->dfsPushButton->setEnabled(true);
}

void MainWindow::on_dfsPushButton_clicked()
{
    ui->dfsResultsListWidget->clear();
    ui->dfsTotalDistanceTraveledLineEdit->clear();
    createGraph();

    graph.reinitializeTraversalDistance();

    vector<string> vertexList;
    //RUN DFS ALGORITHM ON STARTING LOCATION
    int totalDistance;

    totalDistance = graph.DepthFirstSearch(ui->startingStadiumLineEdit->text().toStdString(), vertexList);
    ui->dfsTotalDistanceTraveledLineEdit->setText(QString::number(totalDistance));

    for(int i = 0; i < vertexList.size(); i++)
    ui->dfsResultsListWidget->addItem(QString::fromStdString(vertexList[i]));
}

void MainWindow::on_bfsPageBackButton_clicked()
{
    ui->takeTripStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_performbfsButton_clicked()
{
    ui->bfsPushButton->setEnabled(false);
    ui->bfsStartingStadiumLineEdit->setReadOnly(true);
    ui->bfsTotalDistanceTraveledLineEdit->setReadOnly(true);
    ui->bfsTotalDistanceTraveledLineEdit->clear();
    ui->bfsStadiumListWidget->blockSignals(true);
    ui->bfsStadiumListWidget->clear();
    ui->bfsStadiumListWidget->blockSignals(false);
    ui->bfsResultsListWidget->blockSignals(true);
    ui->bfsResultsListWidget->clear();
    ui->bfsResultsListWidget->blockSignals(false);
    ui->bfsStartingStadiumLineEdit->clear();
    ui->takeTripStackedWidget->setCurrentIndex(5);
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        //fill up the availible teams stacked widget
        ui->bfsStadiumListWidget->addItem(QString::fromStdString(thisMap.atIndex(i).value.getStadiumName()));
    }
}

void MainWindow::on_bfsStadiumListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->bfsResultsListWidget->clear();
    ui->bfsTotalDistanceTraveledLineEdit->clear();

    //place the current item into the desired starting location line edit
    ui->bfsStartingStadiumLineEdit->clear();
    ui->bfsStartingStadiumLineEdit->setText(current->text());
    ui->bfsPushButton->setEnabled(true);
}

void MainWindow::on_bfsPushButton_clicked()
{
    ui->bfsResultsListWidget->clear();
    ui->bfsTotalDistanceTraveledLineEdit->clear();
    createGraph();

    graph.reinitializeTraversalDistance();

    vector<string> vertexList;
    //RUN BFS ALGORITHM ON STARTING LOCATION
    int totalDistance;

    totalDistance = graph.BreadthFirstSearch(ui->bfsStartingStadiumLineEdit->text().toStdString(), vertexList);
    ui->bfsTotalDistanceTraveledLineEdit->setText(QString::number(totalDistance));

    for(int i = 0; i < vertexList.size(); i++)
    ui->bfsResultsListWidget->addItem(QString::fromStdString(vertexList[i]));
}

void MainWindow::on_performmstButton_clicked()
{
    ui->mstPushButton->setEnabled(false);
    ui->mstStartingStadiumLineEdit->setReadOnly(true);
    ui->mstTotalDistanceTraveledLineEdit->setReadOnly(true);
    ui->mstTotalDistanceTraveledLineEdit->clear();
    ui->mstStadiumListWidget->blockSignals(true);
    ui->mstStadiumListWidget->clear();
    ui->mstStadiumListWidget->blockSignals(false);
    ui->mstResultsListWidget->blockSignals(true);
    ui->mstResultsListWidget->clear();
    ui->mstResultsListWidget->blockSignals(false);
    ui->mstStartingStadiumLineEdit->clear();
    ui->takeTripStackedWidget->setCurrentIndex(6);
    for(int i = 0; i < thisMap.mapSize(); i++)
    {
        //fill up the availible teams stacked widget
        ui->mstStadiumListWidget->addItem(QString::fromStdString(thisMap.atIndex(i).value.getStadiumName()));
    }
}

void MainWindow::on_mstStadiumListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->mstResultsListWidget->clear();
    ui->mstTotalDistanceTraveledLineEdit->clear();

    //place the current item into the desired starting location line edit
    ui->mstStartingStadiumLineEdit->clear();
    ui->mstStartingStadiumLineEdit->setText(current->text());
    ui->mstPushButton->setEnabled(true);
}

void MainWindow::on_mstPushButton_clicked()
{
    ui->mstResultsListWidget->clear();
    ui->mstTotalDistanceTraveledLineEdit->clear();
    createGraph();

    graph.reinitializeTraversalDistance();

    vector<string> vertexList;
    //RUN BFS ALGORITHM ON STARTING LOCATION
    int totalDistance;

    totalDistance = graph.MinimumSpanningTree(ui->mstStartingStadiumLineEdit->text().toStdString(), vertexList);
    ui->mstTotalDistanceTraveledLineEdit->setText(QString::number(totalDistance));

    for(int i = 0; i < vertexList.size(); i++)
    ui->mstResultsListWidget->addItem(QString::fromStdString(vertexList[i]));
}

void MainWindow::on_mstBackButton_clicked()
{
    ui->takeTripStackedWidget->setCurrentIndex(0);
}

//ON A TRIP ********************************************************************************

void MainWindow::nextStadium()
{
    //set up the page with everything

    int k = 0;
    if(currentLocationIndex == -1)
    {
        //reference the selected stadiums list widget
        ui->currentStadiumNameLabel->setText(ui->selectedStadiumsListWidget->item(0)->text());

        //search for values
        bool found = false;
        k = 0;
        while(k < thisMap.mapSize() && !found)
        {
            if(ui->selectedStadiumsListWidget->item(0)->text() == QString::fromStdString(thisMap.atIndex(k).value.getStadiumName()))
            {
                found = true;
                ui->currentTeamNameLabel->setText(QString::fromStdString(thisMap.atIndex(k).key));
            }
            else {
                ++k;
            }

        }
        ui->currentTotalDistanceLabel->setText("0");
        ui->currentTotalSpendingLabel->setText("0");
    }
    else
    {
        //reference the orderedstadiums list
        ui->currentStadiumNameLabel->setText(QString::fromStdString(orderedStadiumList[currentLocationIndex]));

        bool found = false;
        k = 0;
        while(k < thisMap.mapSize() && !found)
        {
            if(orderedStadiumList[currentLocationIndex] == thisMap.atIndex(k).value.getStadiumName())
            {
                found = true;
                ui->currentTeamNameLabel->setText(QString::fromStdString(thisMap.atIndex(k).key));
            }
            else {
                ++k;
            }
        }
        ui->currentTotalDistanceLabel->setText(QString::number(ui->currentTotalDistanceLabel->text().toInt() + orderedWeightsList[currentLocationIndex]));

        //NEED TO SET CURRENT TOTAL SPENT

        QTextStream(stdout) << ui->currentTotalSpendingLabel->text() << endl;
        QTextStream(stdout) << subTotal << endl;

        ui->currentTotalSpendingLabel->setText(QString::number(ui->currentTotalSpendingLabel->text().toFloat() + subTotal));  //must add total from previous to the next
    }


    for(int j = 0; j < thisMap.atIndex(k).value.getSouvenirListSize(); j++)
    {
        ui->tripSouvenirNameListWidget->addItem(QString::fromStdString(thisMap.atIndex(k).value.getSouvenir(j).itemName));
        ui->tripSouvenirPriceListWidget->addItem(QString::number(thisMap.atIndex(k).value.getSouvenir(j).itemPrice));
        ui->tripSouvenirQuantityListWidget->addItem("0");
    }

    ui->takeTripStackedWidget->setCurrentIndex(2);

}

void MainWindow::on_goButton_clicked()
{
    ui->totalTripDistanceLineEdit->setReadOnly(true);
    nextStadium();
    //start visiting locations
}

void MainWindow::on_tripSouvenirQuantityListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->tripSouvenirQuantityListWidget->openPersistentEditor(item);
}




void MainWindow::on_tripSouvenirQuantityListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(ui->tripSouvenirQuantityListWidget->isPersistentEditorOpen(previous))
    {

        if(!isInteger(previous->text()))
        {
            previous->setText("0");
        }
        ui->tripSouvenirQuantityListWidget->closePersistentEditor(previous);

        ui->currentTotalAtLocationLabel->setText(QString::number(getSubTotal()));
        QTextStream(stdout) << ui->currentTotalAtLocationLabel->text();
    }
}

bool MainWindow::isInteger(const QString &mystring)
{
   string s = mystring.toStdString();
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

void MainWindow::on_checkoutButton_clicked()
{
    ui->totalSpentLineEdit->setReadOnly(true);
    for(int i =0; i < ui->tripSouvenirQuantityListWidget->count(); i++)
    {
        if(ui->tripSouvenirQuantityListWidget->isPersistentEditorOpen(ui->tripSouvenirQuantityListWidget->item(i)))
        {
            on_tripSouvenirQuantityListWidget_currentItemChanged(ui->tripSouvenirQuantityListWidget->item(0), ui->tripSouvenirQuantityListWidget->item(i));
        }
    }

    //collect data and move on to next stadium
    subTotal = ui->currentTotalAtLocationLabel->text().toFloat();

    //fill reciept
    if(currentLocationIndex == -1)
    {
        ui->recieptListWidget->addItem(ui->selectedStadiumsListWidget->item(0)->text());
    }
    else {
        ui->recieptListWidget->addItem(QString::fromStdString(orderedStadiumList[currentLocationIndex]));
    }


    ui->recieptListWidget->addItem("--------------------------------------------------------------------------");
    for(int i = 0; i < ui->tripSouvenirQuantityListWidget->count(); i++)
    {
        if(ui->tripSouvenirQuantityListWidget->item(i)->text().toInt() != 0)
        {
            ui->recieptListWidget->addItem(ui->tripSouvenirNameListWidget->item(i)->text() + "      -      "
                                           + ui->tripSouvenirPriceListWidget->item(i)->text() + "      -      x"
                                           + ui->tripSouvenirQuantityListWidget->item(i)->text());
        }
    }
    ui->recieptListWidget->addItem(" ");


    //clear tables
    ui->tripSouvenirNameListWidget->blockSignals(true);
    ui->tripSouvenirPriceListWidget->blockSignals(true);
    ui->tripSouvenirQuantityListWidget->blockSignals(true);
    ui->tripSouvenirNameListWidget->clear();
    ui->tripSouvenirPriceListWidget->clear();
    ui->tripSouvenirQuantityListWidget->clear();
    ui->currentTotalAtLocationLabel->clear();
    ui->tripSouvenirNameListWidget->blockSignals(false);
    ui->tripSouvenirPriceListWidget->blockSignals(false);
    ui->tripSouvenirQuantityListWidget->blockSignals(false);

    //save what was purchased

    if(currentLocationIndex + 1 < orderedStadiumList.size())
    {
        ++currentLocationIndex;
        nextStadium();
    }
    else
    {
        //go to reciept screen

        ui->totalSpentLineEdit->setText(QString::number(ui->currentTotalSpendingLabel->text().toFloat() + subTotal));
        ui->totalTripDistanceLineEdit->setText(ui->currentTotalDistanceLabel->text());
        ui->takeTripStackedWidget->setCurrentIndex(3);

    }
}

float MainWindow::getSubTotal()
{
    float sum = 0;

    for(int i = 0; i < ui->tripSouvenirQuantityListWidget->count(); i++)
    {
        sum += ui->tripSouvenirPriceListWidget->item(i)->text().toFloat() * ui->tripSouvenirQuantityListWidget->item(i)->text().toInt();
    }
    QTextStream(stdout) << sum;
    return sum;
}


void MainWindow::on_endTripButton_clicked()
{
    //clear all uis and data
    subTotal = 0;
    orderedStadiumList.clear();
    orderedWeightsList.clear();

    ui->tripSouvenirNameListWidget->blockSignals(true);
    ui->tripSouvenirPriceListWidget->blockSignals(true);
    ui->tripSouvenirQuantityListWidget->blockSignals(true);
    ui->tripSouvenirNameListWidget->clear();
    ui->tripSouvenirPriceListWidget->clear();
    ui->tripSouvenirQuantityListWidget->clear();
    ui->tripSouvenirNameListWidget->blockSignals(false);
    ui->tripSouvenirPriceListWidget->blockSignals(false);
    ui->tripSouvenirQuantityListWidget->blockSignals(false);

    ui->recieptListWidget->blockSignals(true);
    ui->recieptListWidget->clear();
    ui->recieptListWidget->blockSignals(false);

    currentLocationIndex = -1;

    ui->totalSpentLineEdit->clear();
    ui->totalTripDistanceLineEdit->clear();

    ui->currentTeamNameLabel->clear();
    ui->currentStadiumNameLabel->clear();
    ui->currentTotalDistanceLabel->clear();
    ui->currentTotalSpendingLabel->clear();
    ui->currentTotalAtLocationLabel->clear();

    ui->takeTripStackedWidget->setCurrentIndex(0);

    if(isAdmin)
    {
        ui->primaryPageStackedWidget->setCurrentIndex(1);
        ui->adminStackedWidget->setCurrentIndex(0);
    }

    ui->userStackedWidget->setCurrentIndex(0);
}
