// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

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
    // Replace the line below with your implementation
    return towns_.size();
}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    towns_.clear();
    towns_alphabetical_.clear();
    towns_distance_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (towns_.count(id)) {
        return false;
    }
    std::vector<Town*> children;
    Town new_town = {id, name, coord, tax, nullptr, children};
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
    return towns_.at(id).name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns_.at(id).coord;
}

int Datastructures::get_town_tax(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns_.at(id).tax;
}

std::vector<TownID> Datastructures::all_towns()
{
    // Replace the line below with your implementation
    return towns_alphabetical_;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    //vois ol tehokkaampi, tutki vielä
    std::vector<TownID> return_towns;
    for (TownID n : towns_alphabetical_) {
        Name n_name = towns_.at(n).name;
        if (n_name == name) {
            return_towns.push_back(n);
        }
    }
    return return_towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    //handlaa viel se jos id:llä ei löydy kaupunkii
    towns_.at(id).name = newname;
    return 1;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    // ei aakkosjärjestämistä vielä
    if (town_count() == 1 or town_count() == 0 or alphabetical_order_ == true) {
        return towns_alphabetical_;
    } else {
        //sorttaus
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
        //sorttauksen jälkeen distance_order_ = true;
        return towns_distance_;
    }
}

TownID Datastructures::min_distance()
{
    // Replace the line below with your implementation
    if (distance_order_ == true) {
        return towns_distance_.front();
    } else {
        //sorttaus
        return towns_distance_.front();
    }
}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation
    if (distance_order_ == true) {
        return towns_distance_.back();
    } else {
        //sorttaus
        return towns_distance_.back();
    }
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    Town vassal = towns_.at(vassalid);
    Town master = towns_.at(masterid);
    //tän pitäs toimii nii et se estää kahta kaupunkia olemasta toistensa vasalleja!
    if(vassal.parent) { //testaa onko vasallikaupungilla jo isäntäkaupunki
        return false;
    } else if (master.parent) {
        if (master.parent->id.compare(vassalid) == 0) {
            return false;
        }
    }
    towns_.at(vassalid).parent = &towns_.at(masterid);
    towns_.at(masterid).children.push_back(&towns_.at(vassalid));
    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    // unsorted_mapista hakeminen niin nopeeta ettei childrenien tarttis olla osottimia
    std::vector<TownID> vassals;
    for (auto vassal : towns_.at(id).children) {
        vassals.push_back(vassal->id);
    }
    return vassals;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxer_path;
    TownID current_id = id;
    while(true) {
        taxer_path.push_back(current_id);
        Town* parent = towns_.at(current_id).parent;
        if(parent) {
            current_id = parent->id;
        } else {
            return taxer_path;
        }
    }
}

bool Datastructures::remove_town(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    // poista ensin yhteydet, sitten kaupunki
    Town town_to_be_removed = towns_.at(id);
    for (auto child : town_to_be_removed.children) {
        child->parent = nullptr;
    }
    std::vector<Town*> parents_children = town_to_be_removed.parent->children;
    // poista tosta vektorista tää kaupunki
}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("towns_nearest()");
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
}
