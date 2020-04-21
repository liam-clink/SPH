class Node
{
private:
    vector< Node* >     children_;
protected:
    Node() {}
public:
    void add( Node* p ) { children_.push_back( p ); }
    virtual ~Node() {}
};

template< class Cargo >
class CargoNode : public Node
{
private:
    Cargo cargo_;
public:
    CargoNode() : cargo_() {}
};

typedef CargoNode< Element >    ElementNode;
typedef CargoNode< Text >       TextNode;
typedef CargoNode< Nothing >    RootNode;

int main()
{
    RootNode*   root    = new RootNode;

    root->add( new ElementNode );
    root->add( new ElementNode );
    root->add( new TextNode );
    root->add( new ElementNode );   
}
