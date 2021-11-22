// Datastructures.hh
//
// Student Emma Sjöholm:
// Student emma.sjoholm@tuni.fi:
// Student 291683:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
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

    // Estimate of performance: Θ(1)
    // Short rationale for estimate: complexity of std::size is constant
    unsigned int town_count();

    // Estimate of performance: Θ(3N) = Θ(N)
    // Short rationale for estimate: complexity of std::clear is linear in the size of container
    // and there are three data structures which has to be cleared.
    void clear_all();

    // Estimate of performance: Θ(1), O(N)
    // Short rationale for estimate: complexity of std::find is constant
    // for std::unsorted_map, other lines seems to be constant
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: Θ(1), O(N)
    // Short rationale for estimate: depends on std::find again
    Name get_town_name(TownID id);

    // Estimate of performance: Θ(1), O(N)
    // Short rationale for estimate: depends on std::find again
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: Θ(1), O(N)
    // Short rationale for estimate: depends on std::find again
    int get_town_tax(TownID id);

    // Estimate of performance: Θ(1), O(1)
    // Short rationale for estimate: since the order of TownID:s doesn't matter
    // here, we already have the datastructure to return
    std::vector<TownID> all_towns();

    // Estimate of performance: O(N)
    // Short rationale for estimate: function is linear since it has to loop trough
    // every town and check if the town name is same as parameter name
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: Θ(1), O(N)
    // Short rationale for estimate: depends on std::find again
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: best case: Ω(1), Θ(N log N), O(N log N)
    // Short rationale for estimate: in best case, we do not need sorting but
    // when not sorted, complexity of mergesort is N log N
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: best case: Ω(1), Θ(N log N), O(N log N)
    // Short rationale for estimate: in best case, we do not need sorting but
    // when not sorted, complexity of mergesort is N log N
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: best case: Ω(1), Θ(N log N), O(N log N)
    // Short rationale for estimate: in best case, we do not need sorting but
    // when not sorted, complexity of mergesort is N log N
    TownID min_distance();

    // Estimate of performance: best case: Ω(1), Θ(N log N), O(N log N)
    // Short rationale for estimate: in best case, we do not need sorting but
    // when not sorted, complexity of mergesort is N log N
    TownID max_distance();

    // Estimate of performance: Θ(1), O(N)
    // Short rationale for estimate: std::find is linear in worst case, but on
    // average this function is constant
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: Θ(1), O(N)
    // Short rationale for estimate: std::find is linear in worst case, but on
    // average this function seems to be constant
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: Ω(1), O(N)
    // Short rationale for estimate: in worst case this function is linear,
    // it's hard to evaluate complexity of average case
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: Ω(1), Θ(1), O(N)
    // Short rationale for estimate: depends again on std::find
    bool remove_town(TownID id);

    // Estimate of performance: Ω(N log N), Θ(N log N), O(N log N)
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: not implemented
    // Short rationale for estimate: not implemented
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: Ω(1), O(N)
    // Short rationale for estimate: because of for loop this function is linear
    // in worst case
    int total_net_tax(TownID id);

private:
    // Add stuff needed for your class implementation here

    void Merge(std::vector<std::string>& v, int s, int m, int e);

    void MergeSort(std::vector<std::string>& v, int s, int e);

    double distanceCalculate(Coord coord1, Coord coord2);


    struct Town
    {
        TownID id;
        Name name;
        Coord coord;
        int tax;
        TownID master;
        std::vector<TownID> vassals;
    };

    std::unordered_map<TownID, Town> towns_;
    std::vector<TownID> towns_alphabetical_;
    std::vector<TownID> towns_distance_;
    bool alphabetical_order_;
    bool distance_order_;
    bool sorting_by_distance;
    Coord coord_to_compare_;
};

#endif // DATASTRUCTURES_HH
