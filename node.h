// File: node.h
// Author: Liam Clink <clink.6@osu.edu>
//
// This file defines the nodes required to do dynamic memory allocation

#pragma once

#include <vector>

struct Node
{
    std::vector<Node*> children_;
    void add(Node* ptr) {children_.push_back(ptr);}
    virtual ~Node() {}
};

template <class Cargo>
struct CargoNode
    : public Node
{
    Cargo cargo_;
    CargoNode<Cargo>(Cargo cargo): cargo_(cargo) {}
};

