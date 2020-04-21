// File: node.h
// Author: Liam Clink <clink.6@osu.edu>
//
// This file defines the nodes required to do dynamic memory allocation

#pragma once

#include <vector>

class Node
{
private:
    std::vector<Node*> children_;
protected:
    Node() {}
public:
    void add(Node* ptr) {children_.pushback(ptr);}
    virtual ~Node() {}
};

template <class Cargo>
class CargoNode
    : public Node
{
private:
    Cargo cargo_;
public:
    CargoNode(): cargo_() {}
};

