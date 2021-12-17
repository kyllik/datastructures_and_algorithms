// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <list>
#include <memory>
#include <map>


// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int town_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_town_name(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_town_coordinates(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int get_town_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> all_towns();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Estimate of performance: O(n^2)
    // for-loopissa käydään joka n kaupunkia ja joka loopissa poistetaan
    // enintään n-1 tietä joten pahimmassa tapauksessa tehokkuus on n^2
    void clear_roads();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-silmukka on lineaarinen suhteessa
    // teiden määrään
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: Ω(1), O(n)
    // Short rationale for estimate: id:n olemassaolon tarkastaminen lineaarista
    // pahimmassa tapauksessa, vakioaikainen parhaassa tapauksessa, mappiin lisäys
    // logaritmista suhteessa teiden kokoon
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-silmukka lineaarinen
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: Θ(nlogn)
    // Short rationale for estimate: sama kuin shortest_routella, koska
    // palauttaa sen tuloksen
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-silmukka lineaarinen suhteessa teiden määrään
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: O(n+k), jossa n=solmujen määrä, k=kaarien määrä
    // Short rationale for estimate: BFS:n tehokkuus, kun kaikki solmut käydään
    // läpi eli tämän funktion huonoin tapaus
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: Θ(nlogn)
    // Short rationale for estimate: A*:n tehokkuutta vaikea arvioida,
    // nlogn perustuu perftestillä saatuun tulokseen
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:
    // Add stuff needed for your class implementation here
    void Merge(std::vector<std::string>& v, int s, int m, int e);

    void MergeSort(std::vector<std::string>& v, int s, int e);

    Distance distanceCalculate(Coord coord1, Coord coord2 = {0,0});

    int total_net_tax_recursion(TownID id);

    enum Colour { WHITE, GRAY, BLACK};


    struct Town
    {
        TownID id;
        Name name;
        Coord coord;
        int tax;
        TownID master;
        std::vector<TownID> vassals;

        Colour colour;
        int d;
        Town* pi;
        int de;

        std::unordered_map<std::shared_ptr<Town>, Distance> roads;
    };

    std::multimap<Distance,std::pair<TownID,TownID>> roads_;

    std::unordered_map<TownID, std::shared_ptr<Town>> towns_;
    std::vector<TownID> towns_alphabetical_;
    std::vector<TownID> towns_distance_;
    bool alphabetical_order_;
    bool distance_order_;
    bool sorting_by_distance;
    Coord coord_to_compare_;

    void relax_a_star(std::shared_ptr<Town> u, std::shared_ptr<Town> v, std::shared_ptr<Town> g);

};

#endif // DATASTRUCTURES_HH
