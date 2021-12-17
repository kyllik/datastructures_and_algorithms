// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <QDebug>

#include <queue>

#include <stack>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    return towns_.size();
}

void Datastructures::clear_all()
{
    towns_.clear();
    towns_alphabetical_.clear();
    towns_distance_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    if (towns_.find(id) != towns_.end()) {
        return false;
    }

    std::vector<TownID> vassals;
    std::unordered_map<std::shared_ptr<Town>, Distance> roads;
    Town new_town = {id, name, coord, tax, "", vassals, WHITE, -1, nullptr, -1, roads};
    std::shared_ptr<Town> new_town_ptr = std::make_shared<Town>(new_town);
    towns_[id] = new_town_ptr;
    towns_alphabetical_.push_back(id);
    towns_distance_.push_back(id);

    alphabetical_order_ = false;
    distance_order_ = false;

    return true;
}

Name Datastructures::get_town_name(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        return NO_NAME;
    }
    return towns_.at(id)->name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(towns_.find(id)==towns_.end()) {
        return NO_COORD;
    }
    return towns_.at(id)->coord;
}

int Datastructures::get_town_tax(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(towns_.find(id)==towns_.end()) {
        return NO_VALUE;
    }
    return towns_.at(id)->tax;
}

std::vector<TownID> Datastructures::all_towns()
{
    // Replace the line below with your implementation
    return towns_alphabetical_;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> found_towns;
    for (auto& it : towns_) {
        if (it.second->name == name) {
            found_towns.push_back(it.second->id);
        }
    }
    return found_towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if(towns_.find(id)==towns_.end()) {
        return false;
    }
    towns_.at(id)->name = newname;
    alphabetical_order_ = false;
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    if (town_count() == 1 or alphabetical_order_ == true) {
        return towns_alphabetical_;
    } else {
        sorting_by_distance = false;
        MergeSort(towns_alphabetical_, 0, town_count()-1);
        alphabetical_order_ = true;
        return towns_alphabetical_;
    }
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    if (town_count() == 1 or distance_order_ == true) {
        return towns_distance_;
    } else {
        sorting_by_distance = true;
        coord_to_compare_ = {.x=0, .y=0};
        MergeSort(towns_distance_, 0, town_count()-1);
        distance_order_ = true;
        return towns_distance_;
    }
}

TownID Datastructures::min_distance()
{
    if(towns_.empty()) {
        return NO_TOWNID;
    }
    if (distance_order_ == true) {
        return towns_distance_.front();
    } else {
        TownID min_distance_town;
        int min_distance = distanceCalculate(towns_.begin()->second->coord);
        for(auto town : towns_) {
            Coord coord = town.second->coord;
            Distance distance = distanceCalculate(coord);
            if(distance==0) {
                return town.second->id;
            }
            if(distance<min_distance) {
                min_distance = distance;
                min_distance_town = town.second->id;
            }
        }
        return min_distance_town;
    }
}

TownID Datastructures::max_distance()
{
    if(towns_.empty()) {
        return NO_TOWNID;
    }
    if (distance_order_ == true) {
        return towns_distance_.back();
    } else {
        TownID max_distance_town;
        int max_distance = 0;
        for(auto town : towns_) {
            Coord coord = town.second->coord;
            Distance distance = distanceCalculate(coord);
            if(distance>max_distance) {
                max_distance = distance;
                max_distance_town = town.second->id;
            }
        }
        return max_distance_town;
    }
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    if(towns_.find(vassalid)==towns_.end() or towns_.find(masterid)==towns_.end()) {
        return false;
    }
    std::shared_ptr vassal = towns_.at(vassalid);
    std::shared_ptr master = towns_.at(masterid);
    if(vassal->master != "") {
        return false;
    } else if (master->master != "") {
        if (towns_.at(master->master)->id == vassalid) {
            return false;
        }
    }
    towns_.at(vassalid)->master = masterid;
    towns_.at(masterid)->vassals.push_back(vassalid);
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        std::vector<TownID> no_townid = {NO_TOWNID};
        return no_townid;
    }
    return towns_.at(id)->vassals;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        return {NO_TOWNID};
    }
    std::vector<TownID> taxer_path;
    TownID current_id = id;
    while(true) {
        taxer_path.push_back(current_id);
        TownID master = towns_.at(current_id)->master;
        if(!master.empty()) {
            current_id = master;
        } else {
            return taxer_path;
        }
    }
}

bool Datastructures::remove_town(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        return false;
    }

    std::shared_ptr removable_town = towns_.at(id);

    if(!removable_town->vassals.empty()) {
        for(TownID vassal : removable_town->vassals) {
            towns_.at(vassal)->master = "";
            if(removable_town->master != "") {
                add_vassalship(vassal,removable_town->master);
            }
        }
    }

    if(removable_town->master != "") {
        std::vector<TownID> vassals = towns_.at(removable_town->master)->vassals;
        vassals.erase(std::remove(vassals.begin(),vassals.end(),id),vassals.end());
        towns_.at(removable_town->master)->vassals = vassals;
        removable_town->master = "";
    }


    towns_.erase(id);
    towns_alphabetical_.erase(std::remove(towns_alphabetical_.begin(),towns_alphabetical_.end(),id),towns_alphabetical_.end());
    towns_distance_.erase(std::remove(towns_distance_.begin(),towns_distance_.end(),id),towns_distance_.end());
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> all_towns = towns_distance_;
    coord_to_compare_ = coord;
    MergeSort(all_towns,0,all_towns.size()-1);
    return all_towns;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        return {NO_TOWNID};
    }
    std::list<TownID> Q;
    for(auto town : towns_) {
        towns_.at(town.first)->colour = WHITE;
        towns_.at(town.first)->d = -1;
    }
    towns_.at(id)->colour = GRAY;
    towns_.at(id)->d = 0;
    Q.push_back(id);
    TownID furthest_Town = id;
    while(!Q.empty()) {
        TownID u = Q.front();
        Q.pop_front();
        for(TownID v : towns_.at(u)->vassals) {
            if(towns_.at(v)->colour == WHITE) {
                towns_.at(v)->colour = GRAY;
                towns_.at(v)->d = towns_.at(u)->d + 1;
                if (towns_.at(v)->d > towns_.at(furthest_Town)->d) {
                    furthest_Town = v;
                }
                Q.push_back(v);
            }
        }
        towns_.at(u)->colour = BLACK;
    }
    std::vector<TownID> longest_vassal_path;
    TownID temp = furthest_Town;
    while(temp!=id) {
        longest_vassal_path.push_back(temp);
        temp = towns_.at(temp)->master;
    }
    longest_vassal_path.push_back(id);
    std::reverse(longest_vassal_path.begin(), longest_vassal_path.end());
    return longest_vassal_path;
}

int Datastructures::total_net_tax(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        return NO_VALUE;
    }
    int net_tax = total_net_tax_recursion(id);

    if(!towns_.at(id)->master.empty()) {
        net_tax = net_tax - floor(net_tax/10);
    }
    return net_tax;

}

void Datastructures::Merge(std::vector<std::string> &vec, int start, int middle, int end)
{
    std::vector<std::string> temp;
    int i, j;
    i = start;
    j = middle + 1;
    while (i <= middle && j <= end) {
        std::shared_ptr town1 = towns_.at(vec[i]);
        std::shared_ptr town2 = towns_.at(vec[j]);
        if (sorting_by_distance) {
            Coord coord1 = town1->coord;
            Coord coord2 = town2->coord;
            Distance distance1 = distanceCalculate(coord1,coord_to_compare_);
            Distance distance2 = distanceCalculate(coord2,coord_to_compare_);
            if(distance1 <= distance2) {
                temp.push_back(vec[i]);
                ++i;
            } else {
                temp.push_back(vec[j]);
                ++j;
            }
        } else {
            Name name1 = town1->name;
            Name name2 = town2->name;
            if(name1 <= name2) {
                temp.push_back(vec[i]);
                ++i;
            } else {
                temp.push_back(vec[j]);
                ++j;
            }
        }
    }
    while (i <= middle) {
        temp.push_back(vec[i]);
        ++i;
    } while (j <= end) {
        temp.push_back(vec[j]);
        ++j;
    } for (int i = start; i <= end; ++i)
        vec[i] = temp[i - start];
}

void Datastructures::MergeSort(std::vector<std::string> &vec, int start, int end)
{
    if (start < end) {
        int middle = (start + end) / 2;
        MergeSort(vec, start, middle);
        MergeSort(vec, middle + 1, end);
        Merge(vec, start, middle, end);
    }
}

Distance Datastructures::distanceCalculate(Coord coord1, Coord coord2)
{
    int x = coord1.x - coord2.x;
    int y = coord1.y - coord2.y;
    double distance = sqrt(pow(x,2)+pow(y,2));
    return distance;
}

int Datastructures::total_net_tax_recursion(TownID id)
{
    int net_tax = towns_.at(id)->tax;
    for(auto town : towns_.at(id)->vassals) {
         net_tax = net_tax + total_net_tax_recursion(town)/10;
    }
    return net_tax;
}

void Datastructures::relax_a_star(std::shared_ptr<Town> u, std::shared_ptr<Town> v, std::shared_ptr<Town> g)
{
    if(v->d==-1 or v->d > u->d + u->roads.at(v)) {
        v->d = u->d + u->roads.at(v);
        v->de = v->d + distanceCalculate(v->coord,g->coord);
        v->pi = &(*u);
    }
}



//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    for(std::pair<TownID, std::shared_ptr<Town>> town : towns_) {
        town.second->roads.clear();
    }
    roads_.clear();
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID, TownID>> return_vector;
    for(std::pair<Distance,std::pair<TownID,TownID>> road : roads_) {
        return_vector.push_back(road.second);
    }
    return return_vector;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    if(towns_.find(town1)==towns_.end() or towns_.find(town2)==towns_.end()) {
        return false;
    }
    Distance d = distanceCalculate(towns_.at(town1)->coord, towns_.at(town2)->coord);
    towns_.at(town2)->roads[towns_.at(town1)] = d;
    towns_.at(town1)->roads[towns_.at(town2)] = d;
    if(town1 < town2) {
        std::pair<Distance, std::pair<TownID, TownID>> pair(d,{town1,town2});
        roads_.insert(pair);
    } else {
        std::pair<Distance, std::pair<TownID, TownID>> pair(d,{town2,town1});
        roads_.insert(pair);
    }
    return true;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        return {NO_TOWNID};
    }
    std::vector<TownID> return_vector;
    for(auto town : towns_.at(id)->roads) {
        return_vector.push_back(town.first->id);
    }
    return return_vector;
}
std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    return shortest_route(fromid, toid);
}


bool Datastructures::remove_road(TownID town1, TownID town2)
{

    if(towns_.find(town1)==towns_.end() or towns_.find(town2)==towns_.end()) {
        return false;
    }

    std::pair<TownID,TownID> pair;
    if(town1 < town2) {
        pair = {town1,town2};
    } else {
        pair = {town2,town1};
    }
    for(std::map<Distance,std::pair<TownID,TownID>>::iterator it = roads_.begin(); it != roads_.end(); it++) {
        if((it->second) == pair) {
            roads_.erase(it);
            break;
        }
    }

    if (towns_.at(town1)->roads.erase(towns_.at(town2))==1 and towns_.at(town2)->roads.erase(towns_.at(town1))==1) {
        return true;
    }
    return false;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    if(towns_.find(fromid)==towns_.end() or towns_.find(toid)==towns_.end()) {
        return {NO_TOWNID};
    }
    std::vector<TownID> return_vector;
    for(std::pair<TownID, std::shared_ptr<Town>> town : towns_) {
        town.second->colour = WHITE;
        town.second->d = -1;
        town.second->pi = nullptr;
    }
    std::shared_ptr<Town> g = towns_.at(toid);
    bool goal_found = false;
    std::shared_ptr<Town> s = towns_.at(fromid);
    s->colour = GRAY;
    s->d = 0;
    std::queue<std::shared_ptr<Town>> Q;
    Q.push(s);
    while(!Q.empty()) {
        std::shared_ptr<Town> u = Q.front();
        Q.pop();
        for(auto road : u->roads) {
            std::shared_ptr<Town> v = road.first;
            if(v==g) {
                goal_found = true;
            }
            if(v->colour==WHITE) {
                v->colour = GRAY;
                v->d = u->d + 1;
                v->pi = &(*u);
                Q.push(v);
            }
        }
        u->colour = BLACK;
    }
    if(goal_found) {
        Town* temp = 0;
        temp = &(*g);
        return_vector.push_back(temp->id);
        while(temp->id != s->id) {
            temp = &(*temp->pi);
            return_vector.push_back(temp->id);
        }
        std::reverse(return_vector.begin(),return_vector.end());
    }
    return return_vector;

}

std::vector<TownID> Datastructures::road_cycle_route(TownID /*startid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("road_cycle_route()");
}


std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    if(towns_.find(fromid)==towns_.end() or towns_.find(toid)==towns_.end()) {
        return {NO_TOWNID};
    }
    std::vector<TownID> return_vector;
    for(std::pair<TownID, std::shared_ptr<Town>> town : towns_) {
        town.second->colour = WHITE;
        town.second->d = -1;
        town.second->pi = nullptr;
        town.second->de = -1;
    }
    std::shared_ptr<Town> s = towns_.at(fromid);
    std::shared_ptr<Town> g = towns_.at(toid);
    std::priority_queue<std::pair<int,std::shared_ptr<Town>>> Q;
    s->colour = GRAY;
    s->d = 0;
    std::pair<int,std::shared_ptr<Town>> pair;
    pair.first = -s->de;
    pair.second = s;
    Q.push(pair);
    while(!Q.empty()) {
        std::shared_ptr<Town> u = Q.top().second;
        Q.pop();
        if(u==g) {
            Town* temp = 0;
            temp = &(*g);
            return_vector.push_back(temp->id);
            while(temp->id != s->id) {
                temp = &(*temp->pi);
                return_vector.push_back(temp->id);
            }
            std::reverse(return_vector.begin(),return_vector.end());
            return return_vector;
        }
        for(std::pair<std::shared_ptr<Town>, Distance> road : u->roads) {
            std::shared_ptr<Town> v = road.first;
            relax_a_star(u,v,g);
            if(v->colour==WHITE) {
                v->colour = GRAY;
                std::pair<int,std::shared_ptr<Town>> pair;
                pair.first = -v->de;
                pair.second = v;
                Q.push(pair);
        }
        u->colour = BLACK;
    }
    }
    return return_vector;
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}

