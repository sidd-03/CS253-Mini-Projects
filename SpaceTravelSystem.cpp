#include <iostream>
#include <vector>
#include <string>
#include <cmath>  // For sqrt, pow
#include <chrono> // For std::chrono
#include <ctime>  // For std::tm and std::localtime
using namespace std;
#define k 1000000

int Date_diff(int date)
{
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);

    // Extract year and month from current time
    std::tm *current_time_tm = std::localtime(&current_time_t);
    int year = current_time_tm->tm_year + 1900; // Add 1900 to get current year
    int month = current_time_tm->tm_mon + 1;    // Add 1 to get current month
    int MonthDay = current_time_tm->tm_mday;
    int yearDiff = date / 10000 - year;
    int monthDiff = (date % 10000) / 100 - month;
    int MonthDayDiff = date % 100 - MonthDay;
    return yearDiff * 366 + monthDiff * 31 + MonthDayDiff;
}
class ticket;
class space_travel;
class Commander;
class Astronaut;
class Passenger;

class planet
{
public:
    int x;
    int y;
    int z;
    string name;
    planet()
    {
        // cout << "defaut constr for planet\n";
    }
    planet(string name, int x, int y, int z)
    {
        this->name = name;
        this->x = x;
        this->y = y;
        this->z = z;
    }
    planet(const planet &p)
    {
        this->name = p.name;
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
    vector<int> getCoordinates()
    {
        cout << x << y << z << '\n';
        return {x, y, z};
    }
};

class traveller
{
    int traveller_ID;         // default and unique
    static int traveller_cnt; // would help us assign new id to traveller

public:
    static vector<traveller> travellers; // stores the list of travellers
    string name;
    int ID; // able to set his identifiaction num
    vector<ticket> list_of_tickets;
    traveller();
    traveller(string name, int ID);
    void getName();
    void getID();
    friend int verifyID(int ID); // returns -1 if the traveller doesn't exits
    int getUniqueId()
    {
        return traveller_ID;
    }
    void get_ticket(int date, planet source_planet, planet dest_planet, bool Return);
    // help the traveller to book the ticket
    //  Return is true if the traveller wants a return ticket as well
    void display_all_tick();
    void updateID(int ID);
};
// vector<Astronaut> Astronauts; // storing the astronauts
class Astronaut : public traveller
{
private:
    int experience;
    int LicenseID;
    string flyingLicense; //"approved or not approved"

public:
    static vector<Astronaut> astronauts; // storing the astronauts
    vector<int> all_travels_ind;
    Astronaut() : traveller() { ; }
    Astronaut(string name, int ID, int experience, int LicenseID, string flyingLicense) : traveller(name, ID)
    {
        this->experience = experience;
        this->LicenseID = LicenseID;
        this->flyingLicense = flyingLicense;
    }
};
vector<Astronaut> Astronaut::astronauts;
class Passenger : public traveller
{
    int validity = 10; // in yrs

public:
    Passenger() : traveller() { ; }
    Passenger(string name, int ID, int val) : traveller(name, ID)
    {
        validity = val;
    }
};

class Commander : public traveller
{
private:
    int authority;

public:
    int experience;
    int LicenseID;
    static vector<Commander> commanders;
    // should be able to check the travel details
    vector<int> all_travels_ind;
    Commander() : traveller() { ; }
    Commander(string name, int ID, int authority) : traveller(name, ID)
    {
        this->authority = authority;
        // commanders.push_back(*this);
    }

    // check upcoming travel details
};
vector<Commander> Commander::commanders;
// vector<int> Commander::all_travels_ind;
class space_travel
{

public:
    // only be created when more than 2 passengers book the ticket for the same travel from the same source station to the same destination nation on the same specific date
    // space_travel when created would be added to all_travels
    vector<traveller *> travellers; // upto 10
    static vector<space_travel> allTravels;
    // if tickets of travel class becomes less than 2, it is removed from the list of space travels
    int c;    // index of commader in commanders
    int a;    // index of astronaut in astronauts
    int date; // int date:20230423  YearMonthDate #format of date
    planet src;
    planet des;
    space_travel();
    space_travel(int c, int a, vector<traveller *> travellers, planet src, planet des, int date);
    void add_travellers(traveller *ptr);
    void list_travellers();
    void setAstroID(); // will assign the asto index such that its travel doesn't clash with other travels
    void setCommID();  // will assign the comm index such that its travel doesn't clash with other travels
    void updateAstro(int a);
    void changeComm(int c);
    // Change the commander ID, while changing the commander.
};

class ticket
{
private:
    float price;

public:
    static vector<ticket> PendingTickets; // stores tickets which have not been added in the space travel class, i.e travel is not finalised yet
    planet source_planet;
    planet dest_planet;
    traveller *travellerPtr; // stores the pointer of the traveller whose ticket is this
    int date;                // int date:20230423  YearMonthDate #format of date
    ticket();
    ticket(int date, planet source_planet, planet dest_planet, traveller *travellerPtr);
    ticket(const ticket &t);
    void cancelTicket(); // detailed implementation explained in the func
    void print_tick();
    void update_ticket(int d, planet s, planet des); // detailed implementation explained in the func
    void Price();
    float get_price();
    void printAllTick();
};

traveller::traveller()
{
    traveller_cnt++;
    traveller_ID = traveller_cnt;
}
traveller::traveller(string name, int ID)
{
    traveller_cnt++;
    traveller_ID = traveller_cnt;
    this->name = name;
    this->ID = ID;
    //  travellers.push_back(*this); // appending traveller in travellers
}
void traveller::getName()
{
    cout << "The name of the traveller is " << name << '\n';
    // return name;
}
void traveller::getID()
{
    cout << "The ID of the traveller is " << ID << '\n';
    // return ID;
}
int verifyID(int ID)
{
    for (int i = 0; i < 15; i++)
    {
        if (traveller::travellers[i].ID == ID)
            return i;
    }
    return -1;
}
void traveller::get_ticket(int date, planet source_planet, planet dest_planet, bool Return)
{
    list_of_tickets.push_back(ticket(date, source_planet, dest_planet, this));
    if (Return)
    {
        list_of_tickets.push_back(ticket(date, dest_planet, source_planet, this));
    }
}
void traveller::display_all_tick()
{
    for (auto t : list_of_tickets)
    {
        t.print_tick();
    }
}
void traveller::updateID(int ID)
{
    for (int i = 0; i < travellers.size(); i++)
    {
        if (traveller_ID == travellers[i].traveller_ID)
        {
            travellers[i].ID = ID;
        }
    }
    this->ID = ID;
}
vector<traveller> traveller ::travellers;
int traveller::traveller_cnt = 0;
void printAllTravels();

ticket::ticket() { ; }
ticket::ticket(int date, planet source_planet, planet dest_planet, traveller *travellerPtr)
{
    // cout << date << '\n';
    this->source_planet = source_planet;
    this->dest_planet = dest_planet;
    this->date = date;
    this->travellerPtr = travellerPtr;
    //  print_tick();
    int f = 0; // first of all checking whether the tciket has travel locations and date present in some space travel class ,f helps us to keep this track
    for (int j = 0; j < space_travel::allTravels.size(); j++)
    {
        auto travel = space_travel::allTravels[j];
        if (travel.date == date && travel.src.name == source_planet.name && travel.des.name == dest_planet.name)
        {
            if (space_travel::allTravels[j].travellers.size() < 10) // assurance that number of travellers doesn't exceed 10
            {
                space_travel::allTravels[j].add_travellers(travellerPtr);
            }
            else
            {
                PendingTickets.push_back(*this);
            }
            f = 1; // if so, append it in that particular spacetravel
            break;
        }
    }
    // if the space travel class doesn't exists
    int t1 = -1, t2 = -1; // index of pending ticket,if its details  matches with cuurrent tic
    if (f == 0)
    { // cheching whether a ticket with same loaction and time is present
        for (int j = 0; j < PendingTickets.size(); j++)
        {
            ticket tick = PendingTickets[j];
            if (tick.date == date && tick.source_planet.name == source_planet.name && tick.dest_planet.name == dest_planet.name)
            {
                if (t1 == -1)
                    t1 = j;
                else if (t2 == -1)
                    t2 = j;
                else
                    break;
            }
        }
        if (t1 != -1 && t2 != -1)
        { // // if that tick is present, we can remove that tick from pending ticks and create a new space travel class with two travellers
            space_travel spc(0, 0, {PendingTickets[t1].travellerPtr, PendingTickets[t2].travellerPtr, travellerPtr}, source_planet, dest_planet, date);
            PendingTickets[t1] = PendingTickets.back();
            PendingTickets.pop_back();
            PendingTickets[t2] = PendingTickets.back();
            PendingTickets.pop_back();
            space_travel::allTravels.back().setAstroID();
            space_travel::allTravels.back().setCommID();
        }
        // if there is no ticket with same specifications, then append the ticket in pending tickets
        else
        {
            PendingTickets.push_back(*this);
        }
    }
}
void ticket::cancelTicket()
{
    planet s1 = source_planet, s2 = dest_planet;
    int d = date;
    // removing ticket from pending tickets if present
    for (int i = 0; i < PendingTickets.size(); i++)
    {
        ticket t2 = PendingTickets[i];
        if (t2.travellerPtr == travellerPtr && t2.source_planet.name == source_planet.name && t2.dest_planet.name == dest_planet.name)
        {
            ticket t = PendingTickets.back();
            PendingTickets.pop_back();
            PendingTickets[i] = t;
            break;
        }
    }
    // removing ticket from the list of ticket of the traveller
    for (int i = 0; i < (travellerPtr->list_of_tickets).size(); i++)
    {
        ticket t2 = (travellerPtr->list_of_tickets)[i];
        if (t2.travellerPtr == travellerPtr && t2.source_planet.name == source_planet.name && t2.dest_planet.name == dest_planet.name)
        {
            ticket t = travellerPtr->list_of_tickets.back();
            travellerPtr->list_of_tickets.pop_back();
            travellerPtr->list_of_tickets[i] = t;
            break;
        }
    }
    // removing the traveller from space travel and if number of travellers falls less that 2,
    // then cancelling the space class i.e removing that class from alltravels
    int f = 0;
    for (int i = 0; i < space_travel::allTravels.size(); i++)
    {
        space_travel travel = space_travel::allTravels[i];
        //  cout << "loop5 " << travel.src.name << ' ' << travel.des.name << ' ' << travel.date << ' ' << '\n';
        //  cout << "loop6 " << source_planet.name << ' ' << dest_planet.name << ' ' << date << ' ' << '\n';

        if (travel.date == d && travel.src.name == s1.name && travel.des.name == s2.name)
        {
            if (travel.travellers.size() < 4)
            {
                space_travel::allTravels[i] = space_travel::allTravels.back();
                space_travel::allTravels.pop_back();
                //  appending the other ticket of that class in pending tickets
                if (travel.travellers[0] == travellerPtr)
                {
                    ticket(d, s1, s2, travel.travellers[1]);
                    ticket(d, s1, s2, travel.travellers[2]);
                }
                else if (travel.travellers[1] == travellerPtr)
                {
                    ticket(d, s1, s2, travel.travellers[0]);
                    ticket(d, s1, s2, travel.travellers[2]);
                }
                else
                {
                    ticket(d, s1, s2, travel.travellers[1]);
                    ticket(d, s1, s2, travel.travellers[0]);
                }
            }
            else
            {
                for (int j = 0; j < travel.travellers.size(); j++)
                {
                    if (travel.travellers[j] == travellerPtr)
                    {
                        space_travel::allTravels[i].travellers[j] = travel.travellers.back();
                        space_travel::allTravels[i].travellers.pop_back();
                        break;
                    }
                }
            }
            break;
        }
    }
}
void ticket::update_ticket(int date, planet source_planet, planet dest_planet)
{
    traveller *ptr = travellerPtr;
    cancelTicket();
    travellerPtr = ptr;
    int f = 0; // first of all checking whether the tciket has travel locations and date present in some space travel class ,f helps us to keep this track
    for (int j = 0; j < space_travel::allTravels.size(); j++)
    {
        auto travel = space_travel::allTravels[j];
        if (travel.date == date && travel.src.name == source_planet.name && travel.des.name == dest_planet.name)
        {
            if (space_travel::allTravels[j].travellers.size() < 10) // assurance that number of travellers doesn't exceed 10
            {
                space_travel::allTravels[j].add_travellers(travellerPtr);
            }
            else
            {
                PendingTickets.push_back(*this);
            }
            f = 1; // if so, append it in that particular spacetravel
            break;
        }
    }
    // if the space travel class doesn't exists
    int t1 = -1, t2 = -1; // index of pending ticket,if its details  matches with cuurrent tic
    if (f == 0)
    { // cheching whether a ticket with same loaction and time is present
        for (int j = 0; j < PendingTickets.size(); j++)
        {
            ticket tick = PendingTickets[j];
            if (tick.date == date && tick.source_planet.name == source_planet.name && tick.dest_planet.name == dest_planet.name)
            {
                if (t1 == -1)
                    t1 = j;
                else if (t2 == -1)
                    t2 = j;
                else
                    break;
            }
        }
        if (t1 != -1 && t2 != -1)
        { // // if that tick is present, we can remove that tick from pending ticks and create a new space travel class with two travellers
            space_travel spc(0, 0, {PendingTickets[t1].travellerPtr, PendingTickets[t2].travellerPtr, travellerPtr}, source_planet, dest_planet, date);
            PendingTickets[t1] = PendingTickets.back();
            PendingTickets.pop_back();
            PendingTickets[t2] = PendingTickets.back();
            PendingTickets.pop_back();
            space_travel::allTravels.back().setAstroID();
            space_travel::allTravels.back().setCommID();
        }
        // if there is no ticket with same specifications, then append the ticket in pending tickets
        else
        {
            PendingTickets.push_back(*this);
        }
    }
}
ticket::ticket(const ticket &t)
{
    this->source_planet = t.source_planet;
    this->dest_planet = t.dest_planet;
    this->date = t.date;
    this->travellerPtr = t.travellerPtr;
}
void ticket::print_tick()
{
    cout << "Name of the traveller: " << travellerPtr->name << '\n';
    cout << "Source station : " << source_planet.name << '\n';
    cout << "Destination station : " << dest_planet.name << '\n';
    cout << "Date of Travel : " << date << '\n';
    cout << "Price of the ticket : " << get_price() << '\n';
}
void ticket::Price()
{
    float dis = sqrt(pow(dest_planet.x - source_planet.x, 2) + pow(dest_planet.y - source_planet.y, 2) + pow(dest_planet.z - source_planet.z, 2) * 1.0);
    price = ((float)k / (Date_diff(date))) * dis;
    if (price < 0)
        cout << "Inavlid tavel date\n";
}
float ticket::get_price()
{
    Price();
    return price;
}
void ticket::printAllTick()
{
    for (auto t : PendingTickets)
    {
        t.print_tick();
    }
};
vector<ticket> ticket::PendingTickets;

vector<space_travel> space_travel::allTravels;
space_travel::space_travel(int c, int a, vector<traveller *> travellers, planet src, planet des, int date)
{
    this->c = c;
    this->a = a;
    this->travellers = travellers;
    this->src = src;
    this->des = des;
    this->date = date;
    allTravels.push_back(*this);
}
space_travel::space_travel() { ; }
void space_travel::add_travellers(traveller *ptr)
{
    travellers.push_back(ptr); // used by ticket class to add travellers
}
void space_travel::list_travellers()
{
    for (auto i : travellers)
    {
        cout << i->name << ' ';
    }
    cout << '\n';
}
void space_travel::setAstroID()
{
    // bonus point to check clash
    for (int i = 0; i < Astronaut::astronauts.size(); i++)
    {
        int f = 1;
        for (int j = 0; j < Astronaut::astronauts[i].all_travels_ind.size(); j++)
        {
            if (date == space_travel::allTravels[Astronaut::astronauts[i].all_travels_ind[j]].date)
            {
                f = 0;
            }
        }
        if (f)
        {
            a = i;
            Astronaut::astronauts[i].all_travels_ind.push_back(space_travel::allTravels.size() - 1);
            return;
        }
    }
    cout << "No astronaut is available on this date for travel,kindly add new astronauts, and then book ticket\n";
}
void space_travel::setCommID()
{
    // bonus point to check clash
    for (int i = 0; i < Commander::commanders.size(); i++)
    {
        int f = 1;
        for (int j = 0; j < Commander::commanders[i].all_travels_ind.size(); j++)
        {
            if (date == space_travel::allTravels[Commander::commanders[i].all_travels_ind[j]].date)
            {
                f = 0;
            }
        }
        if (f)
        {
            c = i;
            Commander::commanders[i].all_travels_ind.push_back(space_travel::allTravels.size() - 1);
            return;
        }
    }
    cout << "No commander is available on this date for travel,kindly add new commanders, and then book ticket\n";
}
void space_travel::updateAstro(int a)
{
    this->a = a;
}
void space_travel::changeComm(int c)
{
    this->c = c;
}
void printAllTravels()
{
    for (auto i : space_travel::allTravels)
    {
        cout << "Source station : " << i.src.name << '\n';
        cout << "Destination station : " << i.des.name << '\n';
        cout << "Date of Travel : " << i.date << '\n';
        cout << "Commander of Travel : " << Commander::commanders[i.c].name << '\n';
        cout << "Astronaut of Travel : " << Astronaut::astronauts[i.a].name << '\n';
        cout << "No. of travellers : " << i.travellers.size() << '\n'
             << '\n';
    }
    if (space_travel::allTravels.size() == 0)
        cout << "No travel exist\n";
}
void printpendingTickets()
{
    for (auto i : ticket::PendingTickets)
    {
        i.print_tick();
    }
}
class moon : public planet
{
public:
    bool atmosphere;
    bool water;
    moon() : planet() { ; }
    moon(string name, int x, int y, int z, bool atmosphere, bool water) : planet(name, x, y, z)
    {
        this->atmosphere = atmosphere;
        this->water = water;
    }
    void getAtmosphere()
    {
        if (atmosphere)
        {
            cout << "The moon has atmosphere" << endl;
        }
        else
        {
            cout << "The moon does not have atmosphere" << endl;
        }
    }

    void getWaterInfo()
    {
        if (water)
        {
            cout << "The moon has water" << endl;
        }
        else
        {
            cout << "The moon does not have water" << endl;
        }
    }
};

class mars : public planet
{
public:
    int inhabitants;
    bool water;
    mars() : planet() { ; }
    mars(string name, int x, int y, int z, int inhabitants, bool water) : planet(name, x, y, z)
    {
        this->inhabitants = inhabitants;
        this->water = water;
    }
    void updateWaterInfo(bool water)
    {
        this->water = water;
    }

    void getWaterInfo()
    {
        if (water)
        {
            cout << "The moon has water" << endl;
        }
        else
        {
            cout << "The moon does not have water" << endl;
        }
    }
};

void inputCommanders(int num_of_Comm)
{
    cout << "Enter the details of " << num_of_Comm << " Commanders\n";
    for (int i = 0; i < num_of_Comm; i++)
    {
        string name;
        int ID, authority;
        cout << "Enter Name(string) , Unique ID(int)(max len 8) ,authority(int)(max len 8) of commander number " << i + 1 << '\n';
        cin >> name >> ID >> authority;
        Commander::commanders.push_back(Commander(name, ID, authority));
    }
}
void inputAstronauts(int num_of_astro)
{
    cout << "Enter the details of " << num_of_astro << " Astronauts\n";
    for (int i = 0; i < num_of_astro; i++)
    {
        string name;
        int ID;
        int experience;

        int LicenseID;
        string flyingLicense; //"approved or not approved"
        cout << "Enter Name(string), Unique ID(int)(max len 8), experience in yrs(int), License ID(int)(max len 8), flyingLicense(string) of commander number " << i + 1 << '\n';
        cin >> name >> ID >> experience >> LicenseID >> flyingLicense;
        Astronaut::astronauts.push_back(Astronaut(name, ID, experience, LicenseID, flyingLicense));
    }
}
void inputPassengers(int num_of_passengers)
{
    cout << "Enter the details of " << num_of_passengers << " passengers that can book the tickets'\n";

    for (int i = 0; i < num_of_passengers; i++)
    {

        string name;
        int ID;
        cout << "Enter Name(string) and Unique ID(int)(max len 8) of traveller number " << i + 1 << '\n';
        cin >> name >> ID;
        traveller::travellers.push_back(traveller(name, ID));
    }
}
void interactive_program()
{
    planet Earth("earth", 1, 2, 3);
    mars Mars("mars", 0, 0, 0, 100, 0);
    moon Moon("moon", 2, 2, 2, 0, 0);
    cout << "Welcome to space travel\n";
    cout << "Planets that you can travel to 'mars','moon','earth'\n";
    inputAstronauts(2);
    inputCommanders(2);
    inputPassengers(15);
    int task_no;
    cout << "Enter 1 to book ticket\n2 to cancel ticket\n3 to update tick\n4 to display tickets of a particular passenger\n5 to display pending tickets\n6 to display all travels\n7 to add 1 traveller\n8 to add 1 astronaut\n9 to add 1 commander\n0 to exit\n";
    cin >> task_no;
    while (task_no != 0)
    {
        if (task_no == 1 || task_no == 2 || task_no == 3)
        {
            int ID;
            cout << "Enter Passsenger's ID\n";
            cin >> ID;
            int ind = verifyID(ID);
            if (ind == -1)
                cout << "Enter valid ID\n";
            else
            {
                int date;
                string src, dest;
                planet p_src, p_dest;
                cout << "Enter date(int (format:20230323  YearMonthDate 2023mar23)), scr planet(string), destination planet(string)\n";
                cin >> date >> src >> dest;
                if (src == "earth")
                    p_src = Earth;
                else if (src == "moon")
                    p_src = Moon;
                else
                    p_src = Mars;
                if (dest == "earth")
                    p_dest = Earth;
                else if (dest == "moon")
                    p_dest = Moon;
                else
                    p_dest = Mars;
                if (task_no == 1)
                    traveller::travellers[ind].get_ticket(date, p_src, p_dest, 0);
                else
                {
                    int ind2 = -1;
                    for (int j = 0; j < traveller::travellers.size(); j++)
                    {
                        ticket t = traveller::travellers[ind].list_of_tickets[j];
                        if (t.dest_planet.name == dest && t.source_planet.name == src && t.date == date)
                        {
                            ind2 = j;
                        }
                    }
                    if (ind2 == -1)
                        cout << "error\n";
                    else if (task_no == 2)
                        traveller::travellers[ind].list_of_tickets[ind2].cancelTicket();
                    else if (task_no == 3)
                    {
                        cout << "Enter date(int (farmat:20230323  YearMonthDate 2023mar23)), scr planet(string), destination planet(string)\n";
                        cin >> date >> src >> dest;
                        if (src == "earth")
                            p_src = Earth;
                        else if (src == "moon")
                            p_src = Moon;
                        else
                            p_src = Mars;
                        if (dest == "earth")
                            p_dest = Earth;
                        else if (dest == "moon")
                            p_dest = Moon;
                        else
                            p_dest = Mars;
                        traveller::travellers[ind].list_of_tickets[ind2].update_ticket(date, p_src, p_dest);
                    }
                }
            }
        }
        else if (task_no == 4)
        {
            int ID;
            cout << "Enter Passsenger's ID\n";
            cin >> ID;
            int ind = verifyID(ID);
            if (ind == -1)
                cout << "Enter valid ID\n";
            else
                traveller::travellers[ind].display_all_tick();
        }
        else if (task_no == 5)
        {
            printpendingTickets();
        }
        else if (task_no == 6)
        {
            printAllTravels();
        }
        else if (task_no == 7)
            inputPassengers(1);
        else if (task_no == 8)
            inputAstronauts(1);
        else if (task_no == 9)
            inputCommanders(1);
        else
        {
            cout << "enter a valid no\n";
        }
        cout << "Enter 1 to book ticket\n2 to cancel ticket\n3 to update tick\n4 to display tickets of a particular passenger\n5 to display pending tickets\n6 to display all travels\n0 to exit\n";
        cin >> task_no;
        cout << task_no << '\n';
    }
}
void main_code_with_sample_inp()
{
    planet Earth("earth", 1, 2, 3);
    mars Mars("mars", 0, 0, 0, 100, 0);
    moon Moon("moon", 2, 2, 2, 0, 0);
    Commander::commanders.push_back(Commander("c1", 1, 1));
    Commander::commanders.push_back(Commander("c2", 2, 1));
    Astronaut::astronauts.push_back(Astronaut("a1", 1, 1, 4, "approved"));
    Astronaut::astronauts.push_back(Astronaut("a2", 2, 1, 4, "approved"));
    traveller::travellers.push_back(traveller("g1", 1));
    traveller::travellers.push_back(traveller("g2", 2));
    traveller::travellers.push_back(traveller("g3", 3));
    traveller::travellers.push_back(traveller("g4", 3));
    traveller::travellers.push_back(traveller("g5", 3));
    traveller::travellers[0].get_ticket(20230601, Earth, Mars, 0);
    traveller::travellers[0].get_ticket(20230603, Moon, Mars, 0);
    traveller::travellers[1].get_ticket(20230601, Earth, Mars, 0);
    traveller::travellers[1].get_ticket(20230604, Earth, Moon, 0);
    traveller::travellers[2].get_ticket(20230604, Earth, Moon, 0);
    traveller::travellers[2].get_ticket(20230603, Moon, Mars, 0);
    traveller::travellers[3].get_ticket(20230601, Earth, Mars, 0);
    traveller::travellers[4].get_ticket(20230604, Earth, Moon, 0);

    // 0 for not booking a return ticket
    cout << "Details of all space travels\n";
    printAllTravels();
    cout << "\nPending tickets(that are not added in the space travel class)\n";
    printpendingTickets();

    cout << "\n\nCancelling ticket\n";
    traveller::travellers[3].list_of_tickets[0].cancelTicket();
    cout << "Details of all space travels\n";
    printAllTravels();
    cout << "\nPending tickets(that are not added in the space travel class)\n";
    printpendingTickets();

    cout << "\n\nUpdate ticket\n";
    traveller::travellers[1].list_of_tickets[1].update_ticket(20230603, Moon, Mars);
    cout << "Details of all space travels\n";
    printAllTravels();
    cout << "\nPending tickets(that are not added in the space travel class)\n";
    printpendingTickets();
}
int main()
{
    interactive_program();
   // main_code_with_sample_inp();
}
