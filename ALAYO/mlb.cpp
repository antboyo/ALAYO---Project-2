#include "mlb.h"

MLB::MLB()
{
    stadiumName = "";
    seatingCapacity = -1;
    location = "";
    playingSurface = "";
    league = "";
    dateOpened = -1;
    distanceToCenterField = -1;
    ballparkTypology = "";
    roofType = "";    
}

MLB::MLB(string nStadiumName, int nSeatingCapacity, string nLocation, string nPlayingSurface,
         string nLeague, int nDateOpened, int nDistanceToCenterField, string nBallParkTypology, string nRoofType)
{
    stadiumName = nStadiumName;
    seatingCapacity = nSeatingCapacity;
    location = nLocation;
    playingSurface = nPlayingSurface;
    league = nLeague;
    dateOpened = nDateOpened;
    distanceToCenterField = nDistanceToCenterField;
    ballparkTypology = nBallParkTypology;
    roofType = nRoofType;
}

MLB::~MLB()
{
}

string MLB::getStadiumName()
{
    return stadiumName;
}

int MLB::getSeatingCapacity()
{
    return seatingCapacity;
}

string MLB::getLocation()
{
    return location;
}

string MLB:: getPlayingSurface()
{
    return playingSurface;
}

string MLB::getLeague()
{
    return league;
}

int MLB::getDateOpened()
{
    return dateOpened;
}

int MLB:: getDistanceToCenterField()
{
    return distanceToCenterField;
}

string MLB::getBallParkTypology()
{
    return ballparkTypology;
}

string MLB::getRoofType()
{
    return roofType;
}

int MLB::getDistanceSize()
{
    return distances.size();
}

int MLB::getDistance(int i)
{
    return distances.at(i);
}

void MLB::setStadiumName(string newStadiumName)
{
    this->stadiumName = newStadiumName;
}

void MLB::setSeatingCapacity(int newSeatingCapacity)
{
    this->seatingCapacity = newSeatingCapacity;
}

void MLB::setLocation(string newLocation)
{
    this->location = newLocation;
}

void MLB::setPlayingSurface(string newPlayingSurface)
{
    this->playingSurface = newPlayingSurface;
}

void MLB::setLeague(string newLeague)
{
    this->league = newLeague;
}

void MLB::setDateOpened(int newDate)
{
    this->dateOpened = newDate;
}

void MLB::setDistanceToCenterField(int newDistance)
{
    this->distanceToCenterField = newDistance;
}

void MLB::setTypology(string newTypology)
{
    this->ballparkTypology = newTypology;
}

void MLB::setRoofType(string newRoofType)
{
    this->roofType = newRoofType;
}

int MLB::getSouvenirListSize() const
{
    return souvenirs.size();
}

souvenir MLB::getSouvenir(int index) const
{
    return souvenirs[index];
}

void MLB::addSouvenir(souvenir newSouvenir)
{
    souvenirs.push_back(newSouvenir);
}

void MLB::setInitialDistances(int size)
{
    for(int i = 0; i < size; i++)
    {
        this->distances.push_back(0);
    }
}

void MLB::setDistance(int i, int distance)
{
    distances.at(i) = distance;
}

void MLB::pushBackDistance()
{
    distances.push_back(0);
}

void MLB::deleteSouvenir(int index)
{
    souvenirs.erase(souvenirs.begin()+index);
}

void MLB::changeSouvenir(string name, double price, int index)
{
    souvenirs.at(index).setItemName(name);
    souvenirs.at(index).setItemPrice(price);
}
