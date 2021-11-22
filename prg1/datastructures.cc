// Datastructures.cc
//
// Student Emma Sjöholm:
// Student emma.sjoholm@tuni.fi:
// Student 291683:

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <QDebug>

#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <string_view>

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
    Town new_town = {id, name, coord, tax, "", vassals};
    towns_[id] = new_town;
    towns_alphabetical_.push_back(id);
    towns_distance_.push_back(id);

    alphabetical_order_ = false;
    distance_order_ = false;

    return true;
}

Name Datastructures::get_town_name(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(towns_.find(id)==towns_.end()) {
        return NO_NAME;
    }
    return towns_.at(id).name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(towns_.find(id)==towns_.end()) {
        return NO_COORD;
    }
    return towns_.at(id).coord;
}

int Datastructures::get_town_tax(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(towns_.find(id)==towns_.end()) {
        return NO_VALUE;
    }
    return towns_.at(id).tax;
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
        if (it.second.name == name) {
            found_towns.push_back(it.second.id);
        }
    }
    return found_towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if(towns_.find(id)==towns_.end()) {
        return false;
    }
    towns_.at(id).name = newname;
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    if (town_count() == 1 or town_count() == 0 or alphabetical_order_ == true) {
        return towns_alphabetical_;
    } else {
        sorting_by_distance = false;
        MergeSort(towns_alphabetical_, 0, towns_alphabetical_.size() - 1);
        alphabetical_order_ = true;
        return towns_alphabetical_;
    }
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // järjestäminen puuttuu
    if (town_count() == 1 or town_count() == 0 or distance_order_ == true) {
        return towns_distance_;
    } else {
        //sorttaus
        sorting_by_distance = true;
        coord_to_compare_ = {.x=0, .y=0};
        MergeSort(towns_distance_, 0, towns_distance_.size() - 1);
        distance_order_ = true;
        return towns_distance_;
    }
}

TownID Datastructures::min_distance()
{
    // Replace the line below with your implementation
    if(towns_.empty()) {
        return NO_TOWNID;
    }
    if (distance_order_ == true) {
        return towns_distance_.front();
    } else {
        sorting_by_distance = true;
        coord_to_compare_ = {.x=0, .y=0};
        MergeSort(towns_distance_, 0, towns_distance_.size() - 1);
        distance_order_ = true;
        return towns_distance_.front();
    }
}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation
    if(towns_.empty()) {
        return NO_TOWNID;
    }
    if (distance_order_ == true) {
        return towns_distance_.back();
    } else {
        sorting_by_distance = true;
        coord_to_compare_ = {.x=0, .y=0};
        MergeSort(towns_distance_, 0, towns_distance_.size() - 1);
        distance_order_ = true;
        return towns_distance_.back();
    }
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    if(towns_.find(vassalid)==towns_.end() or towns_.find(masterid)==towns_.end()) {
        return false;
    }
    Town vassal = towns_.at(vassalid);
    Town master = towns_.at(masterid);
    //tän pitäs toimii nii et se estää kahta kaupunkia olemasta toistensa vasalleja!
    if(vassal.master != "") { //testaa onko vasallikaupungilla jo isäntäkaupunki
        return false;
    } else if (master.master != "") {
        if (towns_.at(master.master).id == vassalid) {
            return false;
        }
    }
    towns_.at(vassalid).master = masterid;
    towns_.at(masterid).vassals.push_back(vassalid);
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    if(towns_.find(id)==towns_.end()) {
        std::vector<TownID> no_townid = {NO_TOWNID};
        return no_townid;
    }
    return towns_.at(id).vassals;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxer_path;
    TownID current_id = id;
    while(true) {
        taxer_path.push_back(current_id);
        TownID master = towns_.at(current_id).master;
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

    Town removable_town = towns_.at(id);

    if(removable_town.master != "") {
        std::vector<TownID> vassals = towns_.at(removable_town.master).vassals;
        vassals.erase(std::remove(vassals.begin(),vassals.end(),id),vassals.end());
        towns_.at(removable_town.master).vassals = vassals;
        removable_town.master = "";
    } if(!removable_town.vassals.empty()) {
        for(TownID vassal : removable_town.vassals) {
            towns_.at(vassal).master = "";
        }
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

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
//    std::vector<TownID> longest_path;
//    if(towns_.find(id)==towns_.end()) {
//        std::vector<TownID> no_townid = {NO_TOWNID};
//        return no_townid;
//    }
//    for(auto town : towns_) {
//        if(town.second.master == nullptr) {
//            for()
//        }
//    }
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID id)
{
    Town town = towns_.at(id);
    int total_net_tax=town.tax;
    if(!town.vassals.empty()) {
        for(TownID vassal : town.vassals) {
            total_net_tax = total_net_tax + floor(towns_.at(vassal).tax/10);
        }
    }
    if(!town.master.empty()) {
        total_net_tax = total_net_tax - floor(total_net_tax/10);
    }
    return total_net_tax;
}

void Datastructures::Merge(std::vector<std::string> &vec, int start, int middle, int end)
{
    std::vector<std::string> temp;
    int i, j;
    i = start;
    j = middle + 1;
    while (i <= middle && j <= end) {
        Town town1 = towns_.at(vec[i]);
        Town town2 = towns_.at(vec[j]);
        if (sorting_by_distance) {
            Coord coord1 = town1.coord;
            Coord coord2 = town2.coord;
            double distance1 = distanceCalculate(coord1,coord_to_compare_);
            double distance2 = distanceCalculate(coord2,coord_to_compare_);
            if(distance1 <= distance2) {
                temp.push_back(vec[i]);
                ++i;
            } else {
                temp.push_back(vec[j]);
                ++j;
            }
        } else {
            Name name1 = town1.name;
            Name name2 = town2.name;
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

double Datastructures::distanceCalculate(Coord coord1, Coord coord2)
{
    int x = coord1.x - coord2.x;
    int y = coord1.y - coord2.y;
    double distance = sqrt(pow(x,2)+pow(y,2));
    return distance;
}
